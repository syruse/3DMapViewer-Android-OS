#ifndef PARTICAL_H
#define PARTICAL_H
#include <glm/glm.hpp>

class Partical
{
private:
	static int quantity;
	float mLifeTms;
	float mLifeTmsInit;
	bool isLive;
	float mSpeed;
	glm::vec3 mPos;
	glm::vec3 mvVelocity;
public:
	~Partical();
	Partical(const glm::vec3 &vPos, const glm::vec3 &vVel, float angOffsetDegr = 0.0f, float fLifeTimeMs = 1000, float speedK = 1.0f);
	Partical();
	Partical(const Partical& p);
	glm::vec3 getInitPos();
	glm::vec3 getVelocity();
	float getLifeK_0to1();
	float getSpeed();
	bool isAlive();
	void update(float deltaMS);
};

#endif //PARTICAL_H