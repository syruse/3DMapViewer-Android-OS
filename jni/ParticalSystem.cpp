#include "ParticalSystem.hpp"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Tools.hpp"

std::list<Partical*>::iterator LiParticle;

void ParticalSystem::update(float deltaMs)
{
    mElepsedTime += deltaMs;
	
	for(LiParticle iter = mnParticles.begin(); iter != mnParticles.end(); )
	{
		(*iter)->update(deltaMs);
		
		if(!(*iter)->isAlive())
		{
			delete (*iter);
			iter = mnParticles.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

int ParticalSystem::getQuantity()
{
	return mnParticles.size();
}

void ParticalSystem::rend()
{
   GLSLDraw();
}

void ParticalSystem::GLSLDraw()
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	mpGlslProg->setModelM(glm::mat4(1.0));
	(void)mpGlslProg->run();

	mpGlslProg->setTexUniform(UID_TEXTURE_1,mTexHandle);
	mpGlslProg->setTexUniform(UID_TEXTURE_2,mTexMaskHandle);
	mpGlslProg->setFloatUniform(UID_TIME_0_X,mElepsedTime);
	mpGlslProg->setFloatUniform(UID_SIZE,mSize);
	mpGlslProg->setVecUniform(UID_MATERIAL_AMBIENT, mvColor);

	for(LiParticle iter = mnParticles.begin(); iter != mnParticles.end(); ++iter)
	{
		mpGlslProg->setFloatUniform(UID_LIFE, (*iter)->getLifeK_0to1());
		mpGlslProg->setFloatUniform(UID_SPEED, (*iter)->getSpeed());
		mpGlslProg->setVecUniform(UID_VELOCITY, (*iter)->getVelocity());
		mpGlslProg->setVecUniform(UID_INIT_POSITION, (*iter)->getInitPos());

		glEnableVertexAttribArray(AID_TexCoord);
		glEnableVertexAttribArray(AID_Position);

		glVertexAttribPointer(AID_Position, 3, GL_FLOAT, GL_FALSE, 0, VertexAtribs.Vertices);
		glVertexAttribPointer(AID_TexCoord, 2, GL_FLOAT, GL_FALSE, 0, VertexAtribs.Texcoords);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,VertexAtribs.Indices);

		glDisableVertexAttribArray(AID_TexCoord);
		glDisableVertexAttribArray(AID_Position);
	}

	mpGlslProg->stop();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ParticalSystem::setCommonTex(const std::string &texname)
{
	mTexHandle = Tools::getTexture2D(texname);
}

void ParticalSystem::setCommonColor(const glm::vec4 &vColor)
{
	mvColor = vColor;
}

void ParticalSystem::setSize(float size)
{
	mSize = size;
}

ParticalSystem::ParticalSystem(GLSL* glslProg, int id)
{
	mId = id;
	initVertAtribs();
	mpGlslProg = glslProg;
	mTexHandle = -1;
	mSize = 1.0f;
	mRotMat = glm::mat4(1.0f);
	mAngOffset = 0.0f;
	mTexMaskHandle = -1;
	mElepsedTime = 0;
	mvColor = glm::vec4(1.0);
}

void ParticalSystem::setCommonTexMask(const std::string &texname)
{
	mTexMaskHandle = Tools::getTexture2D(texname);
}

void ParticalSystem::initVertAtribs()
{
	GLuint Indices[6] = 
	{
		0, 1, 2,     
		2, 3, 0,   
	};

	GLfloat Texcoords[8] = 
	{
		1.0f, 1.0f,
		1.0f, 0.0f,	
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	GLfloat Vertices[12] = 
	{
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};


	std::copy(Indices, Indices + 6, VertexAtribs.Indices);
	std::copy(Texcoords, Texcoords + 8, VertexAtribs.Texcoords);
	std::copy(Vertices, Vertices + 12, VertexAtribs.Vertices);
}

void ParticalSystem::createParticles(const glm::vec3 &vPos, const glm::vec3 &vVel, float angOffsetDegr , unsigned int quantity, float fLifeTimeMs, float speedK)
{
	mAngOffset = angOffsetDegr;
	for(int i = 0; i < quantity; i++)
	{
       Partical* p = new Partical(vPos, vVel, mAngOffset, fLifeTimeMs, speedK);
	   mnParticles.push_back(p);
	}
}

int ParticalSystem::getId()
{
	return mId;
}

ParticalSystem::~ParticalSystem()
{
	for(LiParticle iter = mnParticles.begin(); iter != mnParticles.end(); )
	{
		delete (*iter);
		iter = mnParticles.erase(iter);
	}
	//std::cout<<"~ParticalSystem()"<<" "<<std::endl;
}
