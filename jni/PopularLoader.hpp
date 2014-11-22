#ifndef POPULARLOADER_H
#define POPULARLOADER_H
#include "IImageLoader.hpp"
#include <opencv/highgui.h>

/**
* class for loading image format such as :
* Windows bitmaps - BMP,DIB
* JPEG files - JPEG,JPG,JPE
* Portable Network Graphics - PNG
* Portable image format - PBM,PGM,PPM
* Sun rasters - SR,RAS
* TIFF files - TIFF,TIF
*/


class PopularLoader : public ImageLoader
{
public:
    PopularLoader();
    ~PopularLoader();
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
    IplImage* mpImg;
};

#endif // POPULARLOADER_H
