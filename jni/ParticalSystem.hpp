#ifndef PARTICAL_SYSTEM_HPP
#define PARTICAL_SYSTEM_HPP
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include "GLSLLoader.hpp"
#include "Partical.hpp"
#include <list>

class ParticalSystem
{
    typedef std::list<Partical*>::iterator LiParticle;
	struct VertexAtribs
	{
		GLuint Indices[6];
		GLfloat Vertices[12];
		GLfloat Texcoords[8];
	}VertexAtribs;

private:
	int mId;
	std::list<Partical*> mnParticles;
	float mAngOffset;
	glm::mat4 mRotMat;

	GLuint mTexHandle; // common texture for seting color of a partical
	GLuint mTexMaskHandle;
	float  mSize;
	float mElepsedTime;
	glm::vec4 mvColor;
	GLSL* mpGlslProg;
public:
	~ParticalSystem();
	ParticalSystem(GLSL* glslProg, int id = 1);
	void createParticles(const glm::vec3 &vPos, const glm::vec3 &vVel, float angOffsetDegr , unsigned int quantity = 10, float fLifeTimeMs = 1000, float speedK = 1.0f);
	void setCommonTex(const std::string &texname);
	void setCommonTexMask(const std::string &texname);
	void setCommonColor(const glm::vec4 &vColor);
	void setSize(float size);
	int  getQuantity();
	void rend();
	void update(float deltaMs);
	int  getId();
private:
	void initVertAtribs();
	void GLSLDraw();
};

#endif //PARTICAL_SYSTEM_HPP