#include "Tools.hpp"
#include "IImageLoader.hpp"
#include "TGALoader.hpp"
#include "FakeLoader.hpp"
#include "PopularLoader.hpp"
#include "SSharedTexture.hpp"
#include <boost/shared_ptr.hpp>

std::map<std::string, SSharedTexture> Tools::texCollect;

std::ofstream& Tools::getLogStream()
{
	static std::ofstream log(AplicationData::getLogFile ().c_str());
	return log;
}

void Tools::reloadTextures()
{
	std::map<std::string, SSharedTexture>::iterator it;

	for (it = texCollect.begin(); it != texCollect.end(); ++it)
	{
		GLuint textureHandle = it->second.getHandle();
		if (glIsTexture(textureHandle) == GL_FALSE)
		{
			glDeleteTextures(1, &textureHandle);
			LOGI("reload %s\n", it->first.c_str());
			Tools::getLogStream() << "texture must be reloaded: " << it->first << std::endl;
			boost::shared_ptr<ImageLoader> pImgLoader;
			bool loadStatus = false;
			std::string fileName = it->first;
			if (fileName.find(".tga") != std::string::npos)
			{
				pImgLoader.reset(new TGALoader());
			}
			else
			{
				pImgLoader.reset(new PopularLoader());
			}

			std::string szAbsoluteFileName = AplicationData::getTexturesPath() + "/" + fileName;
			loadStatus = pImgLoader->load(szAbsoluteFileName.c_str());
			if (loadStatus)
			{
				Tools::getLogStream() << "texture is reloaded: " << szAbsoluteFileName << std::endl;
				glBindTexture(GL_TEXTURE_2D, textureHandle);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
						GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0,
						pImgLoader->getBPP() == 24 ? GL_RGB : GL_RGBA,
						pImgLoader->getHeight(), pImgLoader->getWidth(), 0,
						pImgLoader->getBPP() == 24 ? GL_RGB : GL_RGBA,
						GL_UNSIGNED_BYTE, pImgLoader->getData());
				glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
				glGenerateMipmap (GL_TEXTURE_2D);
			}
		}
	}
}

GLuint Tools::get2DFakeHandle ()
{
	static GLuint fake2DHandle = Tools::loadTexture2DFake();
	return fake2DHandle;
}

GLuint Tools::getTexture2D ( const std::string& fileName, bool mipmap )
{
		GLuint textureHandle;
		std::map<std::string, SSharedTexture>::const_iterator it;
        it = texCollect.find(fileName);
		boost::shared_ptr<ImageLoader> pImgLoader;
		bool isNotFound = true;
		if(it!=texCollect.end())
		{
		  textureHandle = it->second.getHandle();
		  if (glIsTexture(textureHandle) == GL_TRUE)
		  {
			  isNotFound = false;
		  }
		}

		if(isNotFound)
		{
	      if(fileName.find(".tga")!=std::string::npos)
		  {
		    pImgLoader.reset(new TGALoader());
		  }
		  else
		  {
		    pImgLoader.reset(new PopularLoader());
		  }
		  
		  std::string szAbsoluteFileName = AplicationData::getTexturesPath() + "/" + fileName;
		  bool loadStatus = pImgLoader->load(szAbsoluteFileName.c_str());
		
		  if(loadStatus)
		  {
			Tools::getLogStream()<<" texture has loaded "<<szAbsoluteFileName<<std::endl;
            glGenTextures(1,&textureHandle);		  
            glBindTexture(GL_TEXTURE_2D, textureHandle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		  
		    glTexImage2D(GL_TEXTURE_2D,
			   0,
			   pImgLoader->getBPP() == 24 ? GL_RGB : GL_RGBA,
			   pImgLoader->getHeight(),
			   pImgLoader->getWidth(),
			   0,
			   pImgLoader->getBPP() == 24 ? GL_RGB : GL_RGBA,
			   GL_UNSIGNED_BYTE,
			   pImgLoader->getData()
			);	
		    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		    glGenerateMipmap(GL_TEXTURE_2D);			
		  
		    SSharedTexture texHandl(textureHandle);
		    texCollect[fileName] = texHandl;
		  }
		  else
		  {
			Tools::getLogStream()<<" texture has not loaded "<<szAbsoluteFileName<<std::endl;
		    textureHandle = get2DFakeHandle();
		  }
		}

        return(textureHandle);
}

GLuint Tools::loadTexture2DFake ()
{
        GLuint textureHandle;

        FakeLoader fakeLoader;
		int iRet;

        glGenTextures(1,&textureHandle);
		(void)fakeLoader.load();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, textureHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			fakeLoader.getBPP() == 24 ? GL_RGB : GL_RGBA,
			fakeLoader.getWidth(),
			fakeLoader.getHeight(),
			0,
			fakeLoader.getBPP() == 24 ? GL_RGB : GL_RGBA,
			GL_UNSIGNED_BYTE,
			fakeLoader.getData()
			);

			//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
			glGenerateMipmap(GL_TEXTURE_2D);


        return(textureHandle);
}

GLuint Tools::loadTextureCube(
		const std::string &left, 
		const std::string &right, 
		const std::string &top, 
		const std::string &floor, 
		const std::string &front, 
		const std::string &back )
{
    std::string textureNames[6];
	textureNames[0]=left;
	textureNames[1]=right;
	textureNames[2]=top;
	textureNames[3]=floor;
	textureNames[4]=front;
	textureNames[5]=back;
	
	GLenum targets[6];
	targets[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	targets[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	targets[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	targets[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	targets[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	targets[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;	
	
	int numChannel=0;
	GLuint textureHandle;
	glGenTextures(1, &textureHandle);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);
	/**
	* error: 'GL_TEXTURE_WRAP_R' was not declared in this scope
	* android opengl es has not GL_TEXTURE_WRAP_R
	*/
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for(int i = 0;i<6;i++)
	{
	    boost::shared_ptr<ImageLoader> pImgLoader;
		if(textureNames[i].find(".tga")!=std::string::npos)
		{
		  pImgLoader.reset(new TGALoader());
		}
		else
		{
		  pImgLoader.reset(new PopularLoader());
		}
		  
        std::string szAbsoluteFileName = AplicationData::getTexturesPath() + "/" + textureNames[i];
  		bool loadStatus = pImgLoader->load(szAbsoluteFileName.c_str());
		if(!loadStatus)
		{
		   pImgLoader.reset(new FakeLoader());
		}
		
		if(loadStatus)
		{	  
		    glTexImage2D(targets[i],
			   0,
			   pImgLoader->getBPP() == 24 ? GL_RGB : GL_RGBA,
			   pImgLoader->getHeight(),
			   pImgLoader->getWidth(),
			   0,
			   pImgLoader->getBPP() == 24 ? GL_RGB : GL_RGBA,
			   GL_UNSIGNED_BYTE,
			   pImgLoader->getData()
			);				
		}
        else
        {	

        }		
		
	}

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	return(textureHandle);
}

GLuint Tools::createColorTexture(GLuint w, GLuint h, GLenum internalFormat, GLenum type, bool withMipMap)
{
	GLuint texHandle;
	glGenTextures   ( 1, &texHandle );
   glBindTexture   ( GL_TEXTURE_2D, texHandle );
   glTexImage2D    ( GL_TEXTURE_2D, 0, internalFormat, w, h, 0,
                      internalFormat, type, NULL );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR   );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, withMipMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR   );

   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

   if(withMipMap) 
   {   
      glGenerateMipmap(GL_TEXTURE_2D);
   }

	return texHandle;
}

void Tools::printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    Tools::getLogStream()<<" GL "<<name<<" "<<v<<std::endl;
    LOGI("GL %s = %s\n", name, v);
}

void Tools::checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
        Tools::getLogStream()<<"after "<<op<<"() glError (0x"<<error<<")"<<std::endl;
    }
}

Tools::~Tools()
{
	std::cout<<"~Tools()"<<" "<<std::endl;
}
