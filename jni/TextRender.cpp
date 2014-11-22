#include "TextRender.hpp"

TextRender::TextRender()
: miHeight(0)
, mpGlyph(0)
, mpGlslProg(0)
, mIsInit(false)
{
	GLubyte Indices[6] = 
	{
		0, 1, 2,     
		2, 3, 0
	};

	GLfloat Vertices[16] = 
	{
		0.0f, 1.0f,// vert
		0.0f, 0.0f,// tex
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	std::copy(Indices, Indices + 6, mpVertAttribs.Indices);
	std::copy(Vertices, Vertices + 16, mpVertAttribs.VertAtribs);
}

bool TextRender::initialize(const std::string& sFileName, GLSL* glslProg)
{
   if(sFileName.size()!=0 && glslProg !=0)
   {
   	  mIsInit = true;
      mpGlslProg = glslProg;
	  miHeight = 48;
	  mpGlyph = new Glyph[128];

	  //Create and initilize a freetype font library.
	  FT_Library library;
	  if (FT_Init_FreeType( &library )) 
		  throw std::runtime_error("FT_Init_FreeType failed");

	  //The object in which Freetype holds information on a given
	  //font is called a "face".
	  FT_Face face;

	  //This is where we load in the font information from the file.
	  if (FT_New_Face( library, sFileName.c_str(), 0, &face )) 
		  throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	  //Freetype measures font size
	  //in terms of 1/64ths of pixels.  Thus, to make a font
	  //h pixels high, we need to request a size of h*64.
	  //(h << 6 is just a prettier way of writting h*64)
	  FT_Set_Char_Size( face, miHeight << 6, miHeight << 6, 96, 96);

	  for(unsigned char i=0;i<128;i++)
		  make_glyphs(face, i);


	  FT_Done_Face(face);
	  FT_Done_FreeType(library);
   }
   else
   {
      mIsInit = false;  
      Tools::getLogStream()<<"Could not create TextRender object."<<std::endl;	  
   }
   return mIsInit;
}

inline int TextRender::cast_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
};

void TextRender::make_glyphs ( FT_Face face, char ch)
{
	GLuint textHandel;
	glGenTextures(1, &textHandel);
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Use  function to get the widths of
	//the bitmap data in power of two
	int width = cast_p2( bitmap.width );
	int height = cast_p2( bitmap.rows );

	//Allocate memory for the texture data.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha)
	GLubyte* data = new GLubyte[ 2 * width * height];

	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			data[2*(i+j*width)]= data[2*(i+j*width)+1] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}


	//Now we just setup some texture paramaters.
    glBindTexture( GL_TEXTURE_2D, textHandel);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height,
		  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data );

	//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//With the texture created, we don't need to expanded data anymore
    delete [] data;

	GLubyte Indices[6] = 
	{
		0, 1, 2,     
		2, 3, 0
	};

	float	fUK=(float)bitmap.width / (float)width,
			fVK=(float)bitmap.rows / (float)height;

	glm::vec4 vOffset = glm::vec4(bitmap.width, bitmap.rows, bitmap_glyph->left, bitmap_glyph->top-bitmap.rows);
	vOffset /= float(miHeight);

	float fOffsetNextGlyph = ((float(face->glyph->advance.x) / 64.0) / float(miHeight));/*(float(bitmap.width ) / float(miHeight));*/

	mpGlyph[ch].setTexCoordK(glm::vec3(fUK, fVK, 0.0f));
	mpGlyph[ch].setOffset(vOffset);
	mpGlyph[ch].setTexHandle(textHandel);
	mpGlyph[ch].setOfsetForNextGlyph(fOffsetNextGlyph);
	//std::string s="h";s[0] = 'A'+int(ch);
	//std::cout<<"ch "<<s<<" "<<bitmap_glyph->left<<" "<<bitmap.width<<" "<<bitmap_glyph->top - bitmap.rows<<" "<<bitmap.rows<<std::endl;
	//std::cout<<"ch "<<ch<<" "<<(float(face->glyph->advance.x >> 6) / float(miHeight))<<std::endl;

}

void TextRender::print(const glm::vec2& vPos, const std::string& sText, const glm::vec3& vColor, float size)
{
  if(mIsInit)
  {
	glm::vec3 vCurPos;
	glm::vec3 vInitPos;
	float ScreenW = 1000;
	float ScreenH = 700;

	vInitPos.x = glm::clamp(0.0f, ScreenW, vPos.x);
	vInitPos.y = glm::clamp(0.0f, ScreenH, vPos.y);
	vInitPos.z = 0.0f;

	vInitPos.x = (vInitPos.x - (ScreenW/2.0f)) / (ScreenW/2.0f);
	vInitPos.y = (vInitPos.y - (ScreenH/2.0f)) / (ScreenH/2.0f);
	vInitPos.z = vInitPos.z;
	vCurPos = vInitPos;
	//std::cout<<"vCurPos.x "<<vCurPos.x<<" glm::clamp(0.0f, ScreenW, vPos.x) "<<glm::clamp(0.0f, ScreenW, vPos.x)<<" (vCurPos.x - ScreenW/2.0f) "<<(vCurPos.x - ScreenW/2.0f)<<" ScreenW/2.0f "<<ScreenW/2.0f<<std::endl;
	//std::cout<<"vInitPos.x "<<vInitPos.x<<" vInitPos.y "<<vInitPos.y<<std::endl;
	float fSize = 0.001f * size;
	mpGlslProg->setFloatForStor(UID_SIZE, fSize);
	mpGlslProg->setVecForStor(UID_MATERIAL_AMBIENT, vColor);
	mpGlslProg->setVecForStor(UID_INIT_POSITION, vInitPos);

	for(int i = 0; i < sText.size(); i++)
	{
		rendGlyph(sText[i], vCurPos);
		vCurPos.x += mpGlyph[sText[i]].getOffsetForNextGlyph();
	}
  }
  else
  {
    Tools::getLogStream()<<"Error: TextRender is not initialized."<<std::endl;
  }
}



void TextRender::rendGlyph(const char ch, const glm::vec3& vCurPos)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mpGlslProg->run();
	mpGlslProg->setVecUniform(UID_CURRENT_POSITION,vCurPos);
	mpGlslProg->setVecUniform(UID_TEXCOORD_KOEFS,mpGlyph[ch].getTexCoordK());
	//std::cout<<"ch "<<ch<<" "<<mpGlyph[ch].getDir().x<<std::endl;
	mpGlslProg->setVecUniform(UID_OFFSET,mpGlyph[ch].getOffset());
	mpGlslProg->setTexUniform(UID_TEXTURE_1, mpGlyph[ch].getTexHandle());

	glEnableVertexAttribArray(AID_TexCoord);
	glEnableVertexAttribArray(AID_Position);

	glVertexAttribPointer(AID_Position, 3, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), mpVertAttribs.VertAtribs);
	glVertexAttribPointer(AID_TexCoord, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), mpVertAttribs.VertAtribs + 2);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, mpVertAttribs.Indices);

	glDisableVertexAttribArray(AID_TexCoord);
	glDisableVertexAttribArray(AID_Position);

	mpGlslProg->stop();

	glDisable(GL_BLEND);
}

TextRender::~TextRender()
{
	delete[] mpGlyph;
}

