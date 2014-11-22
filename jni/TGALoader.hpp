#ifndef TGALOADER_H
#define TGALOADER_H
#include "IImageLoader.hpp"

class TGALoader : public ImageLoader
{
public:
    TGALoader();
    ~TGALoader();
    bool load(const char* sFilename = 0);
    int getBPP();
    int getWidth();
    int getHeight();
    unsigned char* getData();
private:
    short int mWidth, mHeight, mBPP;
    unsigned long lImageSize;
    char mbEnc;
    unsigned char *mpImage, *mpData;

    // Internal workers
    int readHeader();
    int loadRawData();
    int loadTgaRLEData();
    void BGRtoRGB();
	void flipImg();
};

#endif // TGALOADER_H
