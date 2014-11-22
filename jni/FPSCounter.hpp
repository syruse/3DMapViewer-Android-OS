#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

/**
* class for count number of fps
* you must call update every frame
* pass delta time in ms from last frame
* when mTimeCounter is bigger then 1000ms
* count fps and write it in mFPS when safe
* this result untill mTimeCounter become overflowed again 
*/

class FPSCounter
{
public:

	FPSCounter();

	void update(long timebyframe);

	inline float getLastResult()
	{
       return mFPS;
	};
	
private:
	long mTimeCounter;
	int mFrameCounter;
	float mFPS;

};

#endif //FPSCOUNTER_H
