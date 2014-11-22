#include "FrameBuffer.hpp"
#include "Tools.hpp"

FrameBuffer::FrameBuffer()
: mFBOHandle(0)
, mDepthBuffer(0)
, mvTexHandles(0)
, mDepthTexHandle(0)
, mWidth(0)
, mHeight(0)
, mLastFbo(0)
, depthFlag(depth_0)
{
}

bool FrameBuffer::initialize(int width, int height, DepthFlag dflag, 
                             GLenum internalFormat, 
                             GLenum type, int texNum )
{
   depthFlag = dflag;
   mWidth  = width;
   mHeight = height;
      
	if( mWidth <= 0|| mHeight <= 0)
		return false;

	int  depthFormat   = 0;
    if ( depthFlag & depth_16 )
        depthFormat = GL_DEPTH_COMPONENT16;
    else
	if ( depthFlag & depth_24 )
        depthFormat = GL_DEPTH_COMPONENT24_OES;
    else
    if ( depthFlag & depth_32 )
        depthFormat = GL_DEPTH_COMPONENT32_OES;
              



	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mLastFbo);
	glGenFramebuffers(1, &mFBOHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

	if(depthFormat!=0)
	{
        glGenRenderbuffers        ( 1, &mDepthBuffer );
        glBindRenderbuffer        ( GL_RENDERBUFFER, mDepthBuffer );
        glRenderbufferStorage     ( GL_RENDERBUFFER, depthFormat, mWidth, mHeight );
        glFramebufferRenderbuffer ( GL_FRAMEBUFFER,  GL_DEPTH_ATTACHMENT,
                                       GL_RENDERBUFFER, mDepthBuffer );
	}
   
/*  glGenTextures ( 1, &mDepthTexHandle );
glBindTexture(GL_TEXTURE_2D, mDepthTexHandle);
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWidth,
             mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 
             NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
*/

   //glBindTexture ( GL_TEXTURE_2D, 0 );
   
   //glDrawBuffers ( 1, &none );

	for(int i = 0; i < texNum; i++)
	{

		GLuint TexHandle = Tools::createColorTexture(mWidth, mHeight, internalFormat, type);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + i, 
			GL_TEXTURE_2D,
			TexHandle, 
			0);

		mvTexHandles.push_back(TexHandle);
	}
   
/*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                       GL_TEXTURE_2D, mDepthTexHandle, 0);   */

	bool status = isOk();
	if(mLastFbo != 0 && mLastFbo !=mFBOHandle)
		glBindFramebuffer(GL_FRAMEBUFFER, GLuint(mLastFbo));
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
	return status;
}

GLuint FrameBuffer::getTexHandle(int texId)
{
	if(texId+1 > mvTexHandles.size())
	{
		 Tools::getLogStream() << "error out of range texture index in fbo " << std::endl;
	    return mvTexHandles[0];
	}
	else
	  return mvTexHandles[texId];
}

GLuint FrameBuffer::getDepthTexHandle()
{
   return mDepthTexHandle;
}

bool FrameBuffer::isOk()
{
	bool status = glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE;
	if(!status)
	{
		Tools::getLogStream()<<"Uncoorect binding "<<glCheckFramebufferStatus(GL_FRAMEBUFFER)<<std::endl;
	}
	return status;
}

FrameBuffer::~FrameBuffer()
{
	if(mFBOHandle != 0)
	{
		glDeleteFramebuffers(1, &mFBOHandle);
		mFBOHandle = 0;
	}
	if(mDepthBuffer != 0)
	{
		glDeleteRenderbuffers(1, &mDepthBuffer);
		mDepthBuffer = 0;
	}

	for(int i = 0; i < mvTexHandles.size(); i++)
	{
		GLuint TexHandle = mvTexHandles[i];
		if(glIsTexture(TexHandle))
		{
			glDeleteTextures(1, &TexHandle);
		}
	}
}

int FrameBuffer::getWidth()
{
	return mWidth;
}
int FrameBuffer::getHeight()
{
	return mHeight;
}

bool FrameBuffer::bind()
{
	if(mFBOHandle == 0)
		return false;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mLastFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

	return isOk();
}

bool FrameBuffer::unbind()
{
	if(mFBOHandle == 0)
		return false;
	if(mLastFbo != 0 && mLastFbo !=mFBOHandle)
		glBindFramebuffer(GL_FRAMEBUFFER, GLuint(mLastFbo));
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
