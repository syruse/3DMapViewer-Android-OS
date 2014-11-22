#include "FPSCounter.hpp"


FPSCounter::FPSCounter()
: mTimeCounter(0)
, mFrameCounter(0)
, mFPS(0.0f)
{
};
	
void FPSCounter::update(long timebyframe)
{
	mTimeCounter += timebyframe;
	mFrameCounter++;
	if(mTimeCounter>1000)
	{
		mFPS = (float)(1000*mFrameCounter)/(float)(mTimeCounter);
		mFrameCounter = 0;
		mTimeCounter = 0;
	}
};