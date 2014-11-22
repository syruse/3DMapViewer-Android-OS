#ifndef FAKELOADER_H
#define FAKELOADER_H
#include "IImageLoader.hpp"

class FakeLoader : public ImageLoader
{
public:
    FakeLoader();
    ~FakeLoader();
	/**
	* sFilename don not pass to method
	* it not use
	*/
    bool load(const char* sFilename = 0);
    int getBPP();
    int getWidth();
    int getHeight();
    unsigned char* getData();
private:
    short int mWidth, mHeight, mBPP;
    unsigned char *mpImage;
};

#endif // FAKELOADER_H
