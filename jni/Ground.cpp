#include "Ground.hpp"
#include "GLSLLoader.hpp"
#include <iostream>	


Ground::Ground()
: mTextureHandle(0)
, mpGlslProg(0)
, mIsInit(false)
, mScale(1.0f)
{
}

void Ground::draw()
{

   static GLfloat vertices[12] =
   {
     -1.0f,  0.0f,   1.0f,
      1.0f,  0.0f,   1.0f,
      1.0f,  0.0f,  -1.0f,
     -1.0f,  0.0f,  -1.0f,
   };

   static GLfloat texCoords[8] =
   {
      0.0f,   0.0f,
      1.0f,   0.0f,
      1.0f,   1.0f,
      0.0f,   1.0f
   };

   static GLuint indices[6] =
   {
      0, 1, 2,     
      0, 2, 3
   };

	(void)mpGlslProg->run();
   
	mpGlslProg->setTexUniform(UID_TEXTURE_1,mTextureHandle);
	mpGlslProg->setFloatUniform(UID_SIZE, mScale);
	mpGlslProg->setBoolUniform(UID_USERFLAG,false);//at the beginning enable texturing 
	glm::vec4 vAmbLight(1.0);
   mpGlslProg->setVecUniform(UID_MATERIAL_AMBIENT,vAmbLight);
      
	glEnableVertexAttribArray(AID_TexCoord);
	glEnableVertexAttribArray(AID_Position);

	glVertexAttribPointer(AID_TexCoord, 3, GL_FLOAT, GL_FALSE, 0, texCoords);
	glVertexAttribPointer(AID_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
	glDisableVertexAttribArray(AID_TexCoord);
	glDisableVertexAttribArray(AID_Position);

	mpGlslProg->stop();
}


void Ground::initialize( 
	           GLSL* glsl,
              GLfloat scale, 
			     const std::string &ground)
{
	mpGlslProg = glsl;
   mIsInit = true;
	mTextureHandle = Tools::getTexture2D(ground);
   mScale = scale;
}
Ground::~Ground()
{
	glDeleteTextures(1, &mTextureHandle);
	std::cout<<"~Ground()"<<" "<<std::endl;
}
