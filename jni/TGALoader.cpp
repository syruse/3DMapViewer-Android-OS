#include "TGALoader.hpp"
#include <iostream>
#include <fstream>
#include <memory.h>

#define IMG_OK 0x1
#define IMG_ERR_NO_FILE 0x2
#define IMG_ERR_MEM_FAIL 0x4
#define IMG_ERR_BAD_FORMAT 0x8
#define IMG_ERR_UNSUPPORTED 0x10

TGALoader::TGALoader()
: ImageLoader()
, mpImage(0)
, mpData(0)
, mWidth(0)
, mHeight(0)
, mBPP(0)
, mbEnc(0)
, lImageSize(0)
{
}

TGALoader::~TGALoader()
{
    if (mpImage)
    {
        delete[] mpImage;
        mpImage = NULL;
    }
    if (mpData)
    {
        delete[] mpData;
        mpData = NULL;
    }
}

bool TGALoader::load(const char* sFilename)
{
    using namespace std;
    ifstream fIn;
    unsigned long ulSize;
	int iRet;
    // Clear out any existing image
    if (mpImage)
    {
        delete[] mpImage;
        mpImage = NULL;
    }
    // Open the specified file
    fIn.open(sFilename, ios::binary);

    if (fIn == NULL)
        return false;
    // Get file size
    fIn.seekg(0, ios_base::end);
    ulSize = fIn.tellg();
    fIn.seekg(0, ios_base::beg);
    // Allocate some space
    // Check and clear pDat, just in case
    if (mpData)
	{
        delete[] mpData;
		mpData = NULL;
	}
    mpData = new unsigned char[ulSize];
    if (mpData == NULL)
    {
        fIn.close();
        return false;
    }
    // Read the file into memory
    fIn.read((char*) mpData, ulSize);
    fIn.close();
    // Process the header
    iRet = readHeader();
    if (iRet != IMG_OK)
        return false;
    switch (mbEnc)
    {
    case 2: // Raw RGB
    {
        // Check filesize against header values
        if ((lImageSize + 18 + mpData[0]) > ulSize)
            return IMG_ERR_BAD_FORMAT;
        // Double check image type field
        if (mpData[1] != 0)
            return IMG_ERR_BAD_FORMAT;
        // Load image data
        iRet = loadRawData();
        if (iRet != IMG_OK)
            return false;
        BGRtoRGB(); // Convert to RGB
        break;
    }

    case 10: // RLE RGB
    {
        // Double check image type field
        if (mpData[1] != 0)
            return IMG_ERR_BAD_FORMAT;
        // Load image data
        iRet = loadTgaRLEData();
        if (iRet != IMG_OK)
            return false;
        BGRtoRGB(); // Convert to RGB
        break;
    }
    default:
        return IMG_ERR_UNSUPPORTED;
    }
    // Check flip bit
    if ((mpData[17] & 0x10)) // 5-bit - order from up to down
        flipImg();
    // Release file memory
    delete[] mpData;
    mpData = NULL;
    return true;
}

int TGALoader::readHeader() // Examine the header and populate our class attributes
{
    short ColMapStart, ColMapLen;
    short x1, y1, x2, y2;
    if (mpData == NULL)
        return IMG_ERR_NO_FILE;
    if (mpData[1] != 0)    // 0 (RGB) and 1 (Indexed) are the only types we know about
        return IMG_ERR_UNSUPPORTED;
    mbEnc = mpData[2];     // Encoding flag  1 = Raw indexed image
                         //                2 = Raw RGB
                         //                3 = Raw greyscale
                         //                9 = RLE indexed
                         //               10 = RLE RGB
                         //               11 = RLE greyscale
                         //               32 & 33 Other compression, indexed

    if (mbEnc != 2 && mbEnc != 10)       // We work only with RGB
        return IMG_ERR_UNSUPPORTED;

    // Get image window and produce width & height values
    memcpy(&x1, &mpData[8], 2);  //X Origin of Image
    memcpy(&y1, &mpData[10], 2); //Y Origin of Image
    memcpy(&x2, &mpData[12], 2); //Width of Image
    memcpy(&y2, &mpData[14], 2); //Height of Image
    mWidth = (x2 - x1);
    mHeight = (y2 - y1);
    if (mWidth < 1 || mHeight < 1)
        return IMG_ERR_BAD_FORMAT;
    // Bits per Pixel
    mBPP = mpData[16];
    // Check flip / interleave byte

// 17 byte Image Descriptor Byte.
/*
                  Bits 3-0 - number of attribute bits associated with each
                             pixel.  For the Targa 16, this would be 0 or
                             1.  For the Targa 24, it should be 0.  For the
                             Targa 32, it should be 8.
                  Bit 4    - reserved.  Must be set to 0.
                  Bit 5    - screen origin bit.
                             0 = Origin in lower left-hand corner.
                             1 = Origin in upper left-hand corner.
                             Must be 0 for Truevision images.
                  Bits 7-6 - Data storage interleaving flag.
                             00 = non-interleaved.
                             01 = two-way (even/odd) interleaving.
                             10 = four way interleaving.
                             11 = reserved.
*/

    if (mpData[17] > 32) // Interleaved data
        return IMG_ERR_UNSUPPORTED;
    // Calculate image size
    lImageSize = (mWidth * mHeight * (mBPP / 8));
    return IMG_OK;
}

int TGALoader::loadRawData() // Load uncompressed image data
{
    short iOffset;

    if (mpImage) // Clear old data if present
        delete[] mpImage;
    mpImage = new unsigned char[lImageSize];
    if (mpImage == NULL)
        return IMG_ERR_MEM_FAIL;
    iOffset = mpData[0] + 18; // Add header to ident field size
    memcpy(mpImage, &mpData[iOffset], lImageSize);
    return IMG_OK;
}

int TGALoader::loadTgaRLEData() // Load RLE compressed image data
{
    short iOffset, iPixelSize;
    unsigned char *pCur;
    unsigned long Index = 0;
    unsigned char bLength, bLoop;
    // Calculate offset to image data
    iOffset = mpData[0] + 18;
    // Get pixel size in bytes
    iPixelSize = mBPP / 8;
    // Set our pointer to the beginning of the image data
    pCur = &mpData[iOffset];
    // Allocate space for the image data
    if (mpImage != NULL)
        delete[] mpImage;
    mpImage = new unsigned char[lImageSize];
    if (mpImage == NULL)
        return IMG_ERR_MEM_FAIL;
    // Decode
    while (Index < lImageSize)
    {
        if (*pCur & 0x80/*Dec 128*/) // Run length chunk (High bit = 1) ( value of (*pCur - 127) = quantity of repeats pixel )
        {
            bLength = *pCur - 127; // Get run length
            pCur++;            // Move to pixel data
            // Repeat the next pixel bLength times
            for (bLoop = 0; bLoop != bLength; ++bLoop, Index += iPixelSize)
                memcpy(&mpImage[Index], pCur, iPixelSize);

            pCur += iPixelSize; // Move to the next descriptor chunk
        }
        else // Raw chunk ( value of *pCur = quantity of colors - 1 ,therefour we increment for read pixels )
        {
            bLength = *pCur + 1; // Get run length
            pCur++;          // Move to pixel data
            // Write the next bLength pixels directly
            for (bLoop = 0; bLoop != bLength; ++bLoop, Index += iPixelSize, pCur += iPixelSize)
                memcpy(&mpImage[Index], pCur, iPixelSize);
        }
    }

    return IMG_OK;
}

void TGALoader::BGRtoRGB() // Convert BGR to RGB (or back again)
{
    unsigned long Index, nPixels;
    unsigned char *bCur;
    unsigned char bTemp;
    short iPixelSize;
    // Set ptr to start of image
    bCur = mpImage;
    // Calc number of pixels
    nPixels = mWidth * mHeight;
    // Get pixel size in bytes
    iPixelSize = mBPP / 8;
    for (Index = 0; Index != nPixels; Index++)  // For each pixel
    {
        bTemp = *bCur;      // Get Blue value
        *bCur = *(bCur + 2);  // Swap red value into first position
        *(bCur + 2) = bTemp;  // Write back blue to last position
        bCur += iPixelSize; // Jump to next pixel
    }
}

void TGALoader::flipImg() // Flips the image vertically (Why store images upside down?)
{
    unsigned char bTemp;
    unsigned char *pLine1, *pLine2;
    int iLineLen, iIndex;

    iLineLen = mWidth * (mBPP / 8);
    pLine1 = mpImage;
    pLine2 = &mpImage[iLineLen * (mHeight - 1)];
    for (; pLine1 < pLine2; pLine2 -= (iLineLen * 2))
    {
        for (iIndex = 0; iIndex != iLineLen; pLine1++, pLine2++, iIndex++)
        {
            bTemp = *pLine1;
            *pLine1 = *pLine2;
            *pLine2 = bTemp;
        }
    }
}

int TGALoader::getBPP()
{
    return mBPP;
}

int TGALoader::getWidth()
{
    return mWidth;
}

int TGALoader::getHeight()
{
    return mHeight;
}

unsigned char* TGALoader::getData()
{
    return mpImage;
}
