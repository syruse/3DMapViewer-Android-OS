#ifndef GLSL_H
#define GLSL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <map>
#include "Tools.hpp"

enum Attrib_ID { AID_Position = 0, AID_TexCoord, AID_Normal };
enum Uniform_Mat_ID 
{ 
	UID_NORMAL_MATRIX = 0,
	UID_MODEL_MATRIX,
	UID_MODELVIEW_MATRIX,
	UID_MODELVIEWPROJECTION_MATRIX,
 	UID_MODELVIEWPROJECTION_LIGHT_MATRIX,  
	UID_MAT_NUM,
};
enum Uniform_Vec_ID 
{ 
	UID_LIGHT_POSITION_1 = UID_MAT_NUM,
	UID_LIGHT_AMBIENT_1,
	UID_LIGHT_DIFFUSE_1,
	UID_EYE_POSITION,
	UID_MATERIAL_AMBIENT,
	UID_INIT_POSITION,
	UID_CURRENT_POSITION,
	UID_OFFSET,
	UID_TEXCOORD_KOEFS,
    UID_VELOCITY,
	UID_VEC_NUM,
};
enum Uniform_Float_ID 
{ 
	UID_SIZE = UID_VEC_NUM,
	UID_LIGHT_RADIUS_1,
	UID_LIFE,
	UID_TIME_0_X,
	UID_SPEED,
	UID_F_NUM,
};
enum Uniform_Tex_ID 
{ 
	UID_TEXTURE_1 = UID_F_NUM,
	UID_TEXTURE_2,
	UID_TEXTURE_3,
	UID_TEXTURE_ENV,
	UID_TEX_NUM,
};
enum Uniform_BOOL_ID 
{ 
	UID_USERFLAG = UID_TEX_NUM,
	UID_B_NUM,
};
class GLSL
{
public:
	GLint mProgram;

	glm::mat4 mProjMat;
	glm::mat4 mModelMat;
	glm::mat4 mViewMat;

	std::map<Uniform_Vec_ID, glm::vec4> mVecDic;
	std::map<Uniform_Float_ID, float> mFloatDic;

	GLint mUniformHandles[UID_B_NUM];
	
	bool mIsInit;
public:
	GLSL();
	bool initialize(const std::string& vertName, const std::string& fragName);
	void setFloatForStor(Uniform_Float_ID id, float f);
	void setVecForStor(Uniform_Vec_ID id, const glm::vec4& vVar);
	void setVecForStor(Uniform_Vec_ID id, const glm::vec3& vVar);
	void setVecUniform(Uniform_Vec_ID id, const glm::vec3& vVar);
	void setVecUniform(Uniform_Vec_ID id, const glm::vec4& vVar);
	void setMatUniform(Uniform_Mat_ID id, float* pmatVar);
	void setFloatUniform(Uniform_Float_ID id, float fVar);
	void setBoolUniform(Uniform_BOOL_ID id, bool bVar);
	void setTexUniform(Uniform_Tex_ID id, GLuint texHandle, GLenum texType = GL_TEXTURE_2D);
    void setProjectM(const glm::mat4& projM);
    void setViewM(const glm::mat4& viewM);
    void setModelM(const glm::mat4& modelM);
	GLint getProgHandle();
	bool run();
	void stop();
	~GLSL();
private:
	GLSL(const GLSL&);
	GLSL& operator=(const GLSL&);
	GLuint loadShader(GLenum shaderType, const char* pSource);
	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
	void applyMatrixMVP();
	void applyStorage();
	void initAttributs(GLuint progHandle);
	void initUniforms();
};

#endif // GLSL_H
