#ifndef CAMERA_H
#define CAMERA_H
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	struct LookAtStruct
	{
	    LookAtStruct()
		: mPos(0.0f,0.0f, 0.0f)
		, mTar(0.0f,0.0f,-1.0f)
		, mUp (0.0f,1.0f, 0.0f)
		{
		};
		glm::vec3 mPos;
		glm::vec3 mTar;
		glm::vec3 mUp;
	};
private:
	LookAtStruct mLookAt;
	glm::mat4 mViewMatrix;
    glm::quat mCamQuat;
	/**
	* members mShiftPos, mModifiedQuat
	* for deffered assigment to mLookAt
	* and mCamQuat in update() method
	*/
	glm::vec3 mShiftPos;
	glm::quat mModifiedQuat;
public:
	Camera();
	void setLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	glm::quat getCameraQuat();
	glm::mat4 getViewMatrix();
	LookAtStruct getLookAt();
    static glm::mat4 createLookAtMatx(const glm::vec3& pos, const glm::vec3& tar, const glm::vec3& up);
	void update();
	/**
	* @params angles are pitch, yaw value
	* modify current quaternion by way
	* multiplying on new quaternion
	*/
	void setDeltaShiftAngles(const glm::vec2& angles);
	void setDeltaShiftPosition(const glm::vec3& shift);
};

#endif // CAMERA_H
