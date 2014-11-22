#ifndef FBO_HPP
#define FBO_HPP

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>

enum DepthFlag { depth_0 = 0, depth_16, depth_24, depth_32 };
class FrameBuffer
{
public:
	GLuint mFBOHandle;
	GLuint mDepthBuffer;
	std::vector<GLuint> mvTexHandles;
   GLuint mDepthTexHandle;
	int mWidth;
	int mHeight;
	GLint mLastFbo;
	DepthFlag depthFlag;
public:
	FrameBuffer();
	bool initialize(int width, int height, DepthFlag dflag = depth_0,
                   GLenum internalFormat = GL_RGBA, 
                   GLenum type = GL_UNSIGNED_BYTE,
                   int texNum = 1);
	int getWidth();
	int getHeight();
	GLuint getTexHandle(int texId = 0);
	GLuint getDepthTexHandle();
	bool bind();
	bool unbind();
	~FrameBuffer();
private:
   bool isOk();
	FrameBuffer(const FrameBuffer&);
	FrameBuffer& operator=(const FrameBuffer&);
};

#endif // FBO_HPP
