#ifndef IIMAGELOADER_H
#define IIMAGELOADER_H

class ImageLoader
{
public:
    ImageLoader(){};
    virtual ~ImageLoader() {};
    virtual bool load(const char* sFilename) = 0;
    virtual int getBPP() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual unsigned char* getData() = 0;
};

#endif // IIMAGELOADER_H
