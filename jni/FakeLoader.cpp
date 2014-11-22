#include "FakeLoader.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory.h>

FakeLoader::FakeLoader()
: ImageLoader()
, mpImage(0)
, mWidth(16)
, mHeight(16)
, mBPP(24)
{
}

FakeLoader::~FakeLoader()
{
    if (mpImage)
    {
        delete[] mpImage;
        mpImage = NULL;
    }
}

bool FakeLoader::load(const char* sFilename)
{
    unsigned long Index, nPixels;
    unsigned char *bCur;
    short iPixelSize;

    // Calc number of pixels
    nPixels = mWidth * mHeight;
    // Get pixel size in bytes
    iPixelSize = mBPP / 8;

    mpImage = new unsigned char[nPixels*iPixelSize];

    // Set ptr to start of image
    bCur = mpImage;
	glm::ivec3 vRGBeven = glm::ivec3(255, 0, 255);
	glm::ivec3 vRGBodd  = glm::ivec3(255, 255, 255);
	bool isColEven = false;
	for(int i = 1; i <= mHeight; i++)
	{
		for(int j = 1; j <= mWidth; j++)
		{

			if(j % 4 == 1 || j % 4 == 2)
				isColEven = false;
			else
				isColEven = true;

			if(i % 4 == 1 || i % 4 == 2)
				isColEven = !isColEven;

			if(isColEven)
			{
				*bCur       = vRGBeven.r;
			    *(bCur + 1) = vRGBeven.g;
			    *(bCur + 2) = vRGBeven.b;
			}
			else
			{
				*bCur       = vRGBodd.r;
			    *(bCur + 1) = vRGBodd.g;
			    *(bCur + 2) = vRGBodd.b;
			}
			bCur += iPixelSize;
		}
	}

	return true;
}

int FakeLoader::getBPP()
{
    return mBPP;
}

int FakeLoader::getWidth()
{
    return mWidth;
}

int FakeLoader::getHeight()
{
    return mHeight;
}

unsigned char* FakeLoader::getData()
{
    return mpImage;
}
