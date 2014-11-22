
#ifndef TOOLS_H
#define TOOLS_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <map>
#include <android/log.h>
#include "AplicationData.hpp"
#include <iostream>
#include <fstream>

#define  LOG_TAG    "DEBUG"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class SSharedTexture;

class Tools
{
public:
	static void reloadTextures();
	static GLuint get2DFakeHandle ();
	static GLuint getTexture2D ( const std::string& fileName, bool mipmap = true );
	static GLuint loadTextureCube(
	const std::string &left, 
	const std::string &right, 
	const std::string &top, 
	const std::string &floor, 
	const std::string &front, 
	const std::string &back );
	static GLuint createColorTexture(GLuint w, GLuint h, GLenum internalFormat, GLenum type, bool withMipMap = false);
	static void printGLString(const char *name, GLenum s);
	static void checkGlError(const char* op);
	static std::ofstream& getLogStream();
	~Tools();
private:
	Tools();
	Tools(const Tools&);
	Tools& operator=(const Tools&);
	static GLuint loadTexture2DFake ();
	static GLuint getTGA2DHandle ( const std::string& fileName, bool mipmap = true );
	static GLuint getPNGJPG2DHandle ( const std::string& fileName, bool mipmap = true );
private:
	static std::map<std::string, SSharedTexture> texCollect;
};

#endif //TOOLS_H
