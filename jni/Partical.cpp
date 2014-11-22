#include "Partical.hpp"
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

int Partical::quantity = -1;

glm::vec3 Partical::getInitPos()
{
	return mPos;
}

glm::vec3 Partical::getVelocity()
{
	return mvVelocity;
}

float Partical::getLifeK_0to1()
{
	return (1.0f - mLifeTms/mLifeTmsInit);
}

float Partical::getSpeed()
{
	return mSpeed;
}

void Partical::update(float deltaMS)
{
	mLifeTms -= deltaMS;
	if(mLifeTms<=0)
		isLive = false;

	if(!isLive)
	{
       mLifeTms=0;
	}
}

bool Partical::isAlive()
{
   return isLive;
}

Partical::Partical(const glm::vec3 &vPos, const glm::vec3 &vVel, float angOffsetDegr, float fLifeTimeMs, float speedK)
{
	quantity++;
	mLifeTms = mLifeTmsInit = fLifeTimeMs;
	isLive = true;
	mPos = vPos;;
	mSpeed = glm::linearRand(0.0f, 10.0f);
	mSpeed *= speedK;
	mvVelocity = glm::normalize(vVel);
	glm::mat4 matRotZ = glm::rotate(glm::mat4(1.0f), glm::linearRand(-angOffsetDegr, angOffsetDegr), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matRotX = glm::rotate(glm::mat4(1.0f), glm::linearRand(-angOffsetDegr, angOffsetDegr), glm::vec3(1.0f, 0.0f, 0.0f));
	mvVelocity = glm::vec3(glm::vec4(mvVelocity,1.0f)*matRotZ*matRotX);
}

Partical::Partical()
{
	quantity++;
	mLifeTmsInit=mLifeTms = 0;
	isLive = false;
	mPos = glm::vec3(0);
	mSpeed = 0;
	mvVelocity = glm::vec3(0);
}

Partical::Partical(const Partical& p)
{
	quantity++;
	mLifeTms = p.mLifeTms;
	mLifeTmsInit=p.mLifeTmsInit;
	isLive = p.isLive;
    mPos = p.mPos;
	mSpeed = p.mSpeed;
	mvVelocity = p.mvVelocity;
}

Partical::~Partical()
{
	quantity--;
}
