#ifndef GROUND_H
#define GROUND_H
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>

class GLSL;

class Ground
{
	GLuint mTextureHandle;
	GLSL* mpGlslProg;
	bool mIsInit;
   float mScale;
public:
    Ground();
	~Ground();
    void initialize( 
	           GLSL* glsl,
              GLfloat scale, 
			     const std::string &ground);
	void draw();
   void setShader(GLSL* glsl)
   {
      if(glsl)
      {
        mpGlslProg = glsl;
      }
   };
};

#endif //GROUND_H