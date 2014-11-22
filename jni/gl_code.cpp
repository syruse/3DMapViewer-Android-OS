// OpenGL ES 2.0 code

#include <jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <_3DS.hpp>
#include "Tools.hpp"
#include "GLSLLoader.hpp"
//#include "md5load.hpp"
#include "AplicationData.hpp"
#include "TextRender.hpp"
#include "ObjParse.hpp"
#include "Camera.hpp"
#include "Ground.hpp"
#include "Skybox.hpp"
#include "FPSCounter.hpp"
#include "PSysManager.hpp"
#include "ParticalSystem.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

glm::mat4 Projection;
glm::mat4 cameraView;
glm::mat4 Model;

GLSL _glProg;
GLSL _glProgText;
GLSL _glProgSky;
GLSL _glProgObj;
GLSL _glProgPSysSmoke;
TextRender _texRender;
//_3DS _3dsModel;
//md5load md5object;
float fXstart, fYstart;
float fXstart2, fYstart2;
float fDegY = 0;
float fDegX = 0;
float fStepZ = 0;

std::vector< boost::shared_ptr<Obj> > _vObjModels;
Camera camera;
Skybox skybox;
Ground ground;
FPSCounter fpsCounter;
int _particleSysSmoke;

bool find_files( const std::string & dir_path, std::vector<std::string>& vFiles)            // placing path here if found
{
    path p (dir_path);
    bool isFound = false;

    if (exists(p))
    {
    	  directory_iterator end_itr; // default construction yields past-the-end
    	  for ( directory_iterator itr( p );
    	        itr != end_itr;
    	        ++itr )
    	  {
    	        if (is_regular_file(itr->status()))
    	        {
    	        	std::string file = itr->path().filename().c_str();
    	        	vFiles.push_back(file);
    	        	//Tools::getLogStream() << "file found " << file << std::endl;
    	        	isFound = true;
    	        }
    	        else if (is_directory(itr->status()))
    	        {

    	        }
    	  }
    }
  return isFound;
}

void touchLeft(float x, float y, int state)
{
	//Tools::getLogStream() << "touchLeft state " << state << std::endl;
	if(state == 0) // DOWN
	{
	  fXstart = x;
	  fYstart = y;
	}
	else if(state == 1)//UP
	{

	}
	else if(state == 2)// MOVE
	{
		float stepX = x - fXstart;
		float stepY = y - fYstart;

	    if(glm::abs(stepX)<15)
	    	stepX = 0.0f;
	    if(glm::abs(stepY)<15)
	    	stepY = 0.0f;

	  fDegX = stepY/1000.0f;
	  fDegY = stepX/1000.0f;
	  camera.setDeltaShiftAngles(glm::vec2(fDegX, fDegY));

	}
	else
		;

	//Tools::getLogStream() << "touchLeft  val " << fDegY <<" x " << x <<" y " << y << " fXstart " << fXstart << " fYstart " << fYstart << std::endl;
}

void touchRight(float x, float y, int state)
{
    //Tools::getLogStream() << "touchRight state " << state << std::endl;
	if(state == 0) // DOWN
	{
	  fXstart2 = x;
	  fYstart2 = y;
	}
	else if(state == 1)//UP
	{

	}
	else if(state == 2)// MOVE
	{
		float stepX = x - fXstart2;
		float stepY = y - fYstart2;

	    if(glm::abs(stepX)<15)
	    	stepX = 0.0f;
	    if(glm::abs(stepY)<15)
	    	stepY = 0.0f;

	    fStepZ = stepY/1000.0f;
	    camera.setDeltaShiftPosition(glm::vec3(0.0f, 0.0f, fStepZ));
	}
	else
		;

	//Tools::getLogStream() << " touchRight  val " << fStepZ <<" x " << x <<" y " << y << " fXstart " << fXstart2 << " fYstart " << fYstart2 << std::endl;
}

void loadData()
{
   Tools::getLogStream() << " load data " << std::endl;
   std::vector<std::string> vFiles;
   std::string strTileDir = AplicationData::getModelsPath() + "/Tile";
   
   (void)find_files(strTileDir, vFiles);
   for(int i = 0; i < vFiles.size(); i++)
   {
      if(vFiles[i].find(".obj")!=std::string::npos)
      {
         boost::shared_ptr<Obj> pObjModel(new Obj());
         pObjModel->initialize( strTileDir + "/" + vFiles[i]
         ,strTileDir + "/osm.mtl"
         ,&_glProg
         ,50.0
         );
         _vObjModels.push_back(pObjModel);
         Tools::getLogStream() << "obj model " << (strTileDir + "/" + vFiles[i]) << std::endl;
      }
   }
}

void ThreadFunction()
{
    int counter = 0;

    //for(;;)
    //{
    	Tools::getLogStream() << "thread run " << std::endl;

        try
        {
            // Sleep and check for interrupt.
            // To check for interrupt without sleep,
            // use boost::this_thread::interruption_point()
            // which also throws boost::thread_interrupted
        	//if(_isGLReady)
        	loadData();
        	boost::this_thread::interruption_point();
            //boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        }
        catch(boost::thread_interrupted&)
        {
        	Tools::getLogStream() << "thread stop " << std::endl;
            return;
        }
        return;
   // }
}

void drawScene()
{
   //glCullFace(GL_BACK);
   skybox.draw();
   
   //ground.draw();

   //md5object.enableSkeleton(false);
   //md5object.draw(0.0, 0.0, 0.0, 0.2);
   //_3dsModel.Rend();
   //_objModel.draw();
   bool withVBO = false;
   for(int i = 0; i < _vObjModels.size(); i++)
   {
      (*_vObjModels[i]).draw(withVBO);
   }
   
   //PSysManager::rendAll();
}

void setupGraphics()
{
	LOGI("setupGraphics()");
	Tools::getLogStream() << "setupGraphics()" << std::endl;

    Tools::printGLString("Version", GL_VERSION);
    Tools::printGLString("Vendor", GL_VENDOR);
    Tools::printGLString("Renderer", GL_RENDERER);
    Tools::printGLString("Extensions", GL_EXTENSIONS);

    camera.setLookAt(
    		glm::vec3(0,2.0,40),
    	   glm::vec3(0,0,0),
    	   glm::vec3(0,1,0)
    	);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepthf(1.0f);

    _glProg.initialize(AplicationData::getShadersPath () + "/Primitive.vsh", AplicationData::getShadersPath () + "/Primitive.fsh");
    _glProgText.initialize(AplicationData::getShadersPath () + "/Text.vsh", AplicationData::getShadersPath () + "/Text.fsh");
    _glProgSky.initialize(AplicationData::getShadersPath () + "/SkyBox.vsh", AplicationData::getShadersPath () + "/SkyBox.fsh");
    _glProgObj.initialize(AplicationData::getShadersPath () + "/Obj.vsh", AplicationData::getShadersPath () + "/Obj.fsh");
    _glProgPSysSmoke.initialize(AplicationData::getShadersPath () + "/Smoke.vsh", AplicationData::getShadersPath () + "/Smoke.fsh");
    
    _texRender.initialize(AplicationData::getDataPath () + "/arial.TTF", &_glProgText);

    std::string skyFolderName("skybox");
    skybox.initialize(
    		   &_glProgSky,
            250.0f,
            skyFolderName + "/skylt.jpg",
            skyFolderName + "/skyrt.jpg",
            skyFolderName + "/skyup.jpg",
            skyFolderName + "/skydn.jpg",
            skyFolderName + "/skyft.jpg",
            skyFolderName + "/skybk.jpg" );
    ground.initialize(
    		   &_glProgObj,
            250.0f,
            skyFolderName + "/skydn.jpg");
   
    //md5object.initialize(AplicationData::getModelsPath() + "/pinky.md5mesh" , AplicationData::getModelsPath() + "/idle1.md5anim", &_glProg);
    //_3dsModel.initialize(AplicationData::getModelsPath() + "/jeep1.3ds", &_glProg);
    /*_objModel.initialize(AplicationData::getModelsPath() + "/build1.obj"
    		                              ,AplicationData::getModelsPath() + "/osm.mtl"
    		                              ,&_glProg
    		                              ,40.0
                                  );*/

   /**_particleSysSmoke = PSysManager::InstancePSys(&_glProgPSysSmoke);
   ParticalSystem* pPSysSmoke = PSysManager::getPSys(_particleSysSmoke);
   if(pPSysSmoke)
   {
     pPSysSmoke->setCommonTex("PSys/Smoke.tga");
     pPSysSmoke->setCommonTexMask("PSys/SmokeGrad.png");
     pPSysSmoke->setSize(0.05f);
   }*/
	
   boost::thread t(&ThreadFunction);
   t.interrupt();
}

void reshape(int w, int h)
{
   LOGI("reshape(%d, %d)", w, h);
   Tools::getLogStream() << "reshape(" << w <<", " << h <<")" << std::endl;
   Projection = glm::perspective(45.0f, GLfloat(w/h), 0.01f, 500.0f);
   glViewport(0, 0, w, h);
   Tools::checkGlError("glViewport");
}

void display(long delta)
{
   glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   Tools::checkGlError("glClear");
   camera.update();

   glm::mat4 mView = camera.getViewMatrix();
   //glm::mat4 rotM = glm::rotate(glm::mat4(1.0f), fDegY, glm::vec3(0, 1, 0));
   //mView = glm::translate(rotM, -glm::vec3(0,0,40+fStepZ));
 
   glm::vec3 posLight = glm::vec3(100.0f, 100.0f, 0.0f);
   glm::mat4 viewLight = glm::lookAt( posLight,
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));
                                       
   glm::mat4 MVPLight = Projection*viewLight;
   
   _glProg.setProjectM(Projection);
   _glProg.setViewM(viewLight);
   
   _glProgObj.setProjectM(Projection);
   _glProgObj.setViewM(mView);
   _glProgObj.setVecForStor(UID_LIGHT_POSITION_1, posLight);

   _glProgSky.setProjectM(Projection);
   _glProgSky.setViewM(mView);
   _glProgSky.setVecForStor(UID_LIGHT_POSITION_1, posLight);
   
   _glProgPSysSmoke.setProjectM(Projection);
   _glProgPSysSmoke.setViewM(mView);
   
   for(int i = 0; i < _vObjModels.size(); i++)
   {
      (*_vObjModels[i]).setShader(&_glProgObj);   
   }
   ground.setShader(&_glProgObj);

   /**PSysManager::updateAll(delta);
   ParticalSystem* pPSysSmoke = PSysManager::getPSys(_particleSysSmoke);
   if(pPSysSmoke)
   {
     glm::vec3 vPosSmoke = glm::vec3(0.0f, 0.0f, 0.0f);
	 glm::vec3 vVelSmoke = glm::vec3(0.0f, 15.0f, 0.0f);
     pPSysSmoke->createParticles(vPosSmoke, vVelSmoke, 5.0f, 20, 350);
   }*/
   
   drawScene();
   
   fpsCounter.update(delta);
   float fps = fpsCounter.getLastResult();
   std::ostringstream strFPS;
   strFPS.precision(2);
   strFPS << "fps: ";
   strFPS << std::fixed << fps;
   _texRender.print(glm::vec2(40.0f, 550.0f), strFPS.str(), glm::vec3(0.2f, 1.0f, 0.0f), 45);
}

std::string ConvertJString(JNIEnv* env, jstring str)
{
   const jsize len = env->GetStringUTFLength(str);
   const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

   std::string Result(strChars, len);

   env->ReleaseStringUTFChars(str, strChars);

   return Result;
}

void debug(std::string str)
{
   Tools::getLogStream()<< str << std::endl;
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setupGraphics(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_reshape(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_display(JNIEnv * env, jobject obj, jlong delta);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_touchLeft(JNIEnv * env, jobject obj,  jfloat x, jfloat y, jint state);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_touchRight(JNIEnv * env, jobject obj,  jfloat x, jfloat y, jint state);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_debug(JNIEnv * env, jobject obj,  jstring str);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setupGraphics(JNIEnv * env, jobject obj)
{
    setupGraphics();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_reshape(JNIEnv * env, jobject obj,  jint width, jint height)
{
    reshape(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_display(JNIEnv * env, jobject obj, jlong delta)
{
    display(delta);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_touchLeft(JNIEnv * env, jobject obj,  jfloat x, jfloat y, jint state)
{
    touchLeft(x, y, state);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_touchRight(JNIEnv * env, jobject obj,  jfloat x, jfloat y, jint state)
{
    touchRight(x, y, state);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_debug(JNIEnv * env, jobject obj, jstring str)
{
	std::string res = ConvertJString( env, str );
	debug(res);
}
