#include "Skybox.hpp"
#include "GLSLLoader.hpp"
#include <iostream>	


Skybox::Skybox()
: mTextureHandle(0)
, mpGlslProg(0)
, mIsInit(false)
{
}

void Skybox::draw()
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

	(void)mpGlslProg->run();
	mpGlslProg->setTexUniform(UID_TEXTURE_ENV,mTextureHandle,GL_TEXTURE_CUBE_MAP);

	glEnableVertexAttribArray(AID_TexCoord);
	glEnableVertexAttribArray(AID_Position);

	glVertexAttribPointer(AID_TexCoord, 3, GL_FLOAT, GL_FALSE, 0, mTexCoords);
	glVertexAttribPointer(AID_Position, 3, GL_FLOAT, GL_FALSE, 0, mVertices);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, mCubeIndices);
	glDisableVertexAttribArray(AID_TexCoord);
	glDisableVertexAttribArray(AID_Position);

	mpGlslProg->stop();

	glEnable(GL_DEPTH_TEST);
}


void Skybox::initialize( GLSL* glsl,
                         GLfloat scale, 
			   const std::string &left, 
			   const std::string &right, 
			   const std::string &top, 
			   const std::string &floor, 
			   const std::string &front, 
			   const std::string &back )
{
	mpGlslProg = glsl;
    mIsInit = true;
	mTextureHandle = Tools::loadTextureCube(
	    	                  	           left, 
		                      			   right, 
	 	                     			   top, 
		                      			   floor, 
	           	           			       front, 
	           	           			       back );

										   
GLfloat vertices[24] =
{
	1.0f,   0.0f,   1.0f,
	1.0f,   0.0f,  -1.0f,
	1.0f,   1.0f,  -1.0f,
	1.0f,   1.0f,   1.0f,
	-1.0f,  0.0f,   1.0f,
	-1.0f,  0.0f,  -1.0f,
	-1.0f,  1.0f,  -1.0f,
	-1.0f,  1.0f,   1.0f
};

GLfloat texCoords[24] =
{
	1.0f,  -1.0f,   1.0f,
	1.0f,  -1.0f,  -1.0f,
	1.0f,   1.0f,  -1.0f,
	1.0f,   1.0f,   1.0f,
	-1.0f, -1.0f,   1.0f,
	-1.0f, -1.0f,  -1.0f,
	-1.0f,  1.0f,  -1.0f,
	-1.0f,  1.0f,   1.0f
};

GLuint cubeIndices[36] = 
{
	0, 1, 3,     
	2, 3, 1,   

	0, 3, 4, 
	3, 4, 7,

	2, 1, 6,
	1, 6, 5,

	5, 6, 7, 
	5, 4, 7,

	7, 6, 3, 
	6, 3, 2,

	4, 0, 5,
	1, 0, 5, 
};

	std::copy(cubeIndices, cubeIndices + 36, mCubeIndices);
	std::copy(texCoords,   texCoords + 24,   mTexCoords);
	std::copy(vertices,    vertices + 24,    mVertices);
	
	for(int i = 0;i<8;i++)
	{
		mVertices[i*3  ] *= scale;
		mVertices[i*3+1] *= scale;
		mVertices[i*3+2] *= scale;
	}
}
Skybox::~Skybox()
{
	glDeleteTextures(1, &mTextureHandle);
	std::cout<<"~Skybox()"<<" "<<std::endl;
}
