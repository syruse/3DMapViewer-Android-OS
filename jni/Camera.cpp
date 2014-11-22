#include "Camera.hpp"
#include <glm/gtx/euler_angles.hpp>


glm::mat4 Camera::createLookAtMatx(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	glm::vec3 f = glm::normalize(center - eye);
	glm::vec3 u = glm::normalize(up);
	glm::vec3 s = glm::normalize(glm::cross(f, u));
	u = glm::cross(s, f);

	glm::mat4 LookAtMatrx(1);
	LookAtMatrx[0][0] = s.x;
	LookAtMatrx[1][0] = s.y;
	LookAtMatrx[2][0] = s.z;
	LookAtMatrx[0][1] = u.x;
	LookAtMatrx[1][1] = u.y;
	LookAtMatrx[2][1] = u.z;
	LookAtMatrx[0][2] =-f.x;
	LookAtMatrx[1][2] =-f.y;
	LookAtMatrx[2][2] =-f.z;
	LookAtMatrx[3][0] =-glm::dot(s, eye);
	LookAtMatrx[3][1] =-glm::dot(u, eye);
	LookAtMatrx[3][2] = glm::dot(f, eye);

	return LookAtMatrx;
}

Camera::Camera()
: mLookAt()
, mViewMatrix(1.0f)
, mCamQuat(1.0f, glm::vec3(0.0f))
, mShiftPos(0.0f)
, mModifiedQuat(1.0f, glm::vec3(0.0f))
{
	mViewMatrix = createLookAtMatx(
		mLookAt.mPos,
		mLookAt.mTar,
		mLookAt.mUp
		);
	mCamQuat = glm::quat_cast(mViewMatrix);
	mModifiedQuat = mCamQuat;
}

void Camera::setLookAt(const glm::vec3 &pos, const glm::vec3 &tar, const glm::vec3 &up)
{
	mLookAt.mPos = pos;
	mLookAt.mTar = tar;
	mLookAt.mUp  = up;
	mViewMatrix = createLookAtMatx(
		mLookAt.mPos,
		mLookAt.mTar,
		mLookAt.mUp
		);
	mCamQuat = glm::quat_cast(mViewMatrix);
}

glm::quat Camera::getCameraQuat()
{
	return glm::quat(mViewMatrix);
}

glm::mat4 Camera::getViewMatrix()
{
	return mViewMatrix;
}

Camera::LookAtStruct Camera::getLookAt()
{
    mLookAt.mTar = mLookAt.mTar * glm::mat3(mViewMatrix);
    mLookAt.mUp  = mLookAt.mUp  * glm::mat3(mViewMatrix);	
	return mLookAt;
}

void Camera::update()
{
	if(mShiftPos!=glm::vec3(0.0f))
	{
      mLookAt.mPos += (mShiftPos * mCamQuat);
	  mShiftPos = glm::vec3(0.0f);
	}
	mCamQuat = mModifiedQuat;
	mViewMatrix = glm::mat4_cast(mCamQuat);
	mViewMatrix = glm::translate(mViewMatrix, -mLookAt.mPos);
}

void Camera::setDeltaShiftAngles(const glm::vec2& angles)
{
	if(glm::vec2(0.0f)!=angles)
	{
		glm::quat tmp = glm::angleAxis(angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat tmp2 = glm::angleAxis(angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mModifiedQuat = (tmp*mCamQuat)*tmp2;
	}
}

void Camera::setDeltaShiftPosition(const glm::vec3& shift)
{
	mShiftPos = shift;
}
