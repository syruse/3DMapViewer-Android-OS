#include "PopularLoader.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory.h>
#include <opencv/cv.h>

PopularLoader::PopularLoader()
: ImageLoader()
, mpImg(0)
{
}

PopularLoader::~PopularLoader()
{
    if (mpImg)
    {
        cvReleaseImage(&mpImg);
    }
}

bool PopularLoader::load(const char* sFilename)
{
	bool loadStatus = true;
	int numChannels = 0;

    mpImg = cvLoadImage(sFilename, CV_LOAD_IMAGE_COLOR);
    if( mpImg == 0 )
    {
       loadStatus = false;    	
    }
    else
	{

		numChannels = mpImg->nChannels;

		if (numChannels < 3)
		{
			loadStatus = false;
		}
		else
		{
			if (numChannels == 3)
			{
				cvCvtColor(mpImg, mpImg, CV_BGR2RGB);
			}
			else
			{
				cvCvtColor(mpImg, mpImg, CV_BGRA2RGBA);
			}
		}

    }

	return loadStatus;
}

int PopularLoader::getBPP()
{
	if(mpImg)
	{
       return (mpImg->nChannels*8);
	}
	else
	{
	   return -1;
	}
}

int PopularLoader::getWidth()
{
	if(mpImg)
	{
      return (mpImg->width);
	}
	else
	{
	   return -1;
	}
}

int PopularLoader::getHeight()
{
	if(mpImg)
	{
      return (mpImg->height);
	}
	else
	{
	   return -1;
	}
}

unsigned char* PopularLoader::getData()
{
	if(mpImg)
	{
      return reinterpret_cast<unsigned char*>(mpImg->imageData);
	}
	else
	{
	   return 0;
	}
}
