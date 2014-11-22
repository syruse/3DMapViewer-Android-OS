#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <GLES2/gl2.h>
#include <iostream>
#include <string>
#include <vector>
#include "ft2build.h"
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <glm/glm.hpp>
#include "GLSLLoader.hpp"
#include "Tools.hpp"
#include <stdexcept>

class Glyph
{
private:
	glm::vec4 mvOffset;
	glm::vec3 mvTexCoordK;
	GLuint mTextHandel;
	float mfOffsetNext;
public:
	Glyph()
    : mvOffset(0.0f)
    , mvTexCoordK(1.0f)
    , mTextHandel(0)
    , mfOffsetNext(0.0f)
    {};
	void setTexCoordK(const glm::vec3& vK)
	{
		mvTexCoordK = vK;
	};
	glm::vec3 getTexCoordK()
	{
		return mvTexCoordK;
	};
	void setOffset(const glm::vec4& vOffset)
	{
		mvOffset= vOffset;
	};
	glm::vec4 getOffset(){
		return mvOffset;
	};
	void setTexHandle(GLuint texHandle)
	{
		mTextHandel = texHandle;
	};
	GLuint getTexHandle()
	{
		return mTextHandel;
	};
	void setOfsetForNextGlyph(float fOffset)
	{
		mfOffsetNext = fOffset;
	};
	float getOffsetForNextGlyph()
	{
		return mfOffsetNext;
	};
	~Glyph()
	{
		glDeleteTextures(1, &mTextHandel);
	}
};

class TextRender
{
public:
	int miHeight;
	struct VertexAtribs
	{
		GLubyte Indices[6];
		GLfloat VertAtribs[16];
	}mpVertAttribs;
	Glyph* mpGlyph;
	GLSL* mpGlslProg;
	bool mIsInit;
public:
	TextRender();
	bool initialize(const std::string& sFileName, GLSL* glslProg);
	void print(const glm::vec2& vPos, const std::string& sText, const glm::vec3& vColor, float size); //text coordinates must be in range : for X [0...1000] for Y [0...700]
	~TextRender();
private:
	TextRender(const TextRender&);
	TextRender& operator=(const TextRender&);
	void make_glyphs ( FT_Face face, char ch);
	inline int cast_p2 ( int a ); // casting of size to power of two
	void rendGlyph(const char ch, const glm::vec3& vCurPos);
};

#endif // TEXT_RENDER_H
