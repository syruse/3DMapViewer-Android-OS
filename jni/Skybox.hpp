#ifndef SKYBOX_H
#define SKYBOX_H
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>

class GLSL;

class Skybox
{
	GLuint mTextureHandle;
	GLuint mCubeIndices[36];
	GLfloat mVertices[24];
	GLfloat mTexCoords[24];
	GLSL* mpGlslProg;
	bool mIsInit;
public:
    Skybox();
	~Skybox();
    void initialize( 
	           GLSL* glsl,
               GLfloat scale, 
			   const std::string &left, 
			   const std::string &right, 
			   const std::string &top, 
			   const std::string &floor, 
			   const std::string &front, 
			   const std::string &back );
	void draw();
};

#endif //SKYBOX_H