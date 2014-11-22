#include "GLSLLoader.hpp"
#include "Tools.hpp"

GLSL::GLSL()
: mProjMat(1.0f)
, mModelMat(1.0f)
, mViewMat(1.0f)
, mIsInit(false)
{
}

bool GLSL::initialize(const std::string& vertName, const std::string& fragName)
{
  std::ifstream fileVert(vertName.c_str());
  std::ifstream fileFrag(fragName.c_str());
  std::string sline;

  std::string sVertSource;
  std::string sFragSource;

  while(getline(fileVert, sline))
  {
	  sVertSource += sline+"\n";
  }

  while(getline(fileFrag, sline))
  {
	  sFragSource += sline+"\n";
  }

	mProgram = createProgram(sVertSource.c_str(), sFragSource.c_str());
	if (!mProgram)
    {
		Tools::getLogStream()<<"Could not create shader program."<<std::endl;
        mIsInit = false;
    }
	else
	{   
		initUniforms();
		mIsInit = true;
	}
	
	return mIsInit;
}

void GLSL::initAttributs(GLuint progHandle)
{
   glBindAttribLocation(progHandle, AID_Position, "a_vPosition");
   glBindAttribLocation(progHandle, AID_TexCoord, "a_vTexCoord");
   glBindAttribLocation(progHandle, AID_Normal, "a_vNormal");
}

void GLSL::initUniforms()
{
	mUniformHandles[UID_LIGHT_POSITION_1                ] = glGetUniformLocation(mProgram, "u_vLightPos_1");
	mUniformHandles[UID_LIGHT_RADIUS_1                  ] = glGetUniformLocation(mProgram, "u_fLightR_1");
	mUniformHandles[UID_LIFE                            ] = glGetUniformLocation(mProgram, "u_fLife");
	mUniformHandles[UID_TIME_0_X                        ] = glGetUniformLocation(mProgram, "u_fTime_0_X");
	mUniformHandles[UID_LIGHT_AMBIENT_1                 ] = glGetUniformLocation(mProgram, "u_vLightAmb_1");
	mUniformHandles[UID_LIGHT_DIFFUSE_1                 ] = glGetUniformLocation(mProgram, "u_vLightDiff_1");
	mUniformHandles[UID_EYE_POSITION                    ] = glGetUniformLocation(mProgram, "u_vEyePos");
	mUniformHandles[UID_NORMAL_MATRIX                   ] = glGetUniformLocation(mProgram, "u_matNormal");
	mUniformHandles[UID_MODEL_MATRIX                    ] = glGetUniformLocation(mProgram, "u_matModel");
	mUniformHandles[UID_MODELVIEW_MATRIX                ] = glGetUniformLocation(mProgram, "u_matModelView");
	mUniformHandles[UID_MODELVIEWPROJECTION_MATRIX      ] = glGetUniformLocation(mProgram, "u_matModelViewProjection");
	mUniformHandles[UID_MODELVIEWPROJECTION_LIGHT_MATRIX] = glGetUniformLocation(mProgram, "u_matModelViewProjectionLight");
	mUniformHandles[UID_TEXTURE_1                       ] = glGetUniformLocation(mProgram, "s_2DTexture");
	mUniformHandles[UID_TEXTURE_2                       ] = glGetUniformLocation(mProgram, "s_2DTexture_2");
	mUniformHandles[UID_TEXTURE_3                       ] = glGetUniformLocation(mProgram, "s_2DTexture_3");
	mUniformHandles[UID_MATERIAL_AMBIENT                ] = glGetUniformLocation(mProgram, "u_vMaterAmb");
	mUniformHandles[UID_USERFLAG                        ] = glGetUniformLocation(mProgram, "u_fUserFlag");
	mUniformHandles[UID_SIZE                            ] = glGetUniformLocation(mProgram, "u_fSize");
	mUniformHandles[UID_VELOCITY                        ] = glGetUniformLocation(mProgram, "u_vVeloc");
	mUniformHandles[UID_SPEED                           ] = glGetUniformLocation(mProgram, "u_fSpeed");
	mUniformHandles[UID_TEXTURE_ENV                     ] = glGetUniformLocation(mProgram, "s_cubeTexture");
	mUniformHandles[UID_OFFSET                          ] = glGetUniformLocation(mProgram, "u_vOffset");
	mUniformHandles[UID_TEXCOORD_KOEFS                  ] = glGetUniformLocation(mProgram, "u_vTexCoordK");
	mUniformHandles[UID_INIT_POSITION                   ] = glGetUniformLocation(mProgram, "u_vInitPos");
	mUniformHandles[UID_CURRENT_POSITION                ] = glGetUniformLocation(mProgram, "u_vCurPos");
}

void GLSL::setVecForStor(Uniform_Vec_ID id, const glm::vec4& vVar)
{
	mVecDic.erase (id); 
	mVecDic.insert(std::make_pair(id, glm::vec4(vVar)));
}

void GLSL::setFloatForStor(Uniform_Float_ID id, float f)
{
	mFloatDic.erase(id);
	mFloatDic.insert(std::make_pair(id, f));
}

void GLSL::setVecForStor(Uniform_Vec_ID id, const glm::vec3& vVar)
{
	mVecDic.erase (id);
	mVecDic.insert(std::make_pair(id, glm::vec4(vVar, 1.0f)));
}

void GLSL::setVecUniform(Uniform_Vec_ID id, const glm::vec3& vVar)
{
	GLint uHandle = mUniformHandles[id];
	if(uHandle >= 0)
	{
		glUniform4f(uHandle, vVar.x, vVar.y, vVar.z, 1.0f);
		//glUniform3f(uHandle, vVar.x, vVar.y, vVar.z);
	}
}

void GLSL::setVecUniform(Uniform_Vec_ID id, const glm::vec4& vVar)
{
	GLint uHandle = mUniformHandles[id];
	if(uHandle >= 0)
		glUniform4f(uHandle, vVar.x, vVar.y, vVar.z, vVar.w);
}

void GLSL::setMatUniform(Uniform_Mat_ID id, float* pmatVar)
{
	GLint uHandle = mUniformHandles[id];
	if(uHandle >= 0)
		glUniformMatrix4fv(uHandle, 1, GL_FALSE, pmatVar);
}

void GLSL::setFloatUniform(Uniform_Float_ID id, float fVar)
{
	GLint uHandle = mUniformHandles[id];
	if(uHandle >= 0)
		glUniform1f(uHandle, fVar);
}

void GLSL::setBoolUniform(Uniform_BOOL_ID id, bool bVar)
{
	GLint uHandle = mUniformHandles[id];
	if(uHandle >= 0)
	{
		int _flag;
		if(bVar)
			_flag = 1;
		else
			_flag = 0;
		glUniform1i(uHandle, _flag);
	}
}

void GLSL::setTexUniform(Uniform_Tex_ID id, GLuint texHandle, GLenum texType)
{
	int activeUnit = (id - UID_TEXTURE_1);
	glActiveTexture(GL_TEXTURE0 + activeUnit);
	glBindTexture(texType, texHandle);
	GLint uHandle = mUniformHandles[id];
	if(uHandle >= 0)
	{
		glUniform1i(uHandle, activeUnit);
	}
}

GLint GLSL::getProgHandle()
{
	return mProgram;
}

void GLSL::applyMatrixMVP()
{
    glm::mat4 MVmatrix = mViewMat * mModelMat;
	glm::mat4 MVPmatrix = mProjMat * MVmatrix;
	glm::mat3 MRotMatx = glm::mat3(mModelMat);

	GLint MVPHandle    = mUniformHandles[UID_MODELVIEWPROJECTION_MATRIX];
	GLint MVHandle     = mUniformHandles[UID_MODELVIEW_MATRIX];
	GLint MHandle      = mUniformHandles[UID_MODEL_MATRIX];
	GLint NHandle      = mUniformHandles[UID_NORMAL_MATRIX];

	if(MVPHandle >= 0)
		glUniformMatrix4fv(MVPHandle, 1, GL_FALSE, glm::value_ptr(MVPmatrix));
	if(MVHandle >= 0)
		glUniformMatrix4fv(MVHandle, 1, GL_FALSE, glm::value_ptr(MVmatrix));
	if(MHandle >= 0)
		glUniformMatrix4fv(MHandle, 1, GL_FALSE, glm::value_ptr(mModelMat));
	if(NHandle >= 0)
		glUniformMatrix3fv(NHandle, 1, GL_FALSE, glm::value_ptr(MRotMatx));

}

void GLSL::applyStorage()
{
    std::map<Uniform_Vec_ID, glm::vec4>::iterator itVec;
	for(itVec = mVecDic.begin(); itVec != mVecDic.end(); itVec++)
	{
		setVecUniform((*itVec).first, (*itVec).second);
	    //std::cout<<"r "<<(*itVec).second.x<<" "<<(*itVec).second.y<<" "<<(*itVec).second.z<<std::endl;
	}

    std::map<Uniform_Float_ID, float>::iterator itFloat;
	for(itFloat = mFloatDic.begin(); itFloat != mFloatDic.end(); itFloat++)
	{
		setFloatUniform((*itFloat).first, (*itFloat).second);
		//std::cout<<"r "<<(*itFloat).second<<std::endl;
	}
}

bool GLSL::run()
{
   if(mIsInit)
	{
	   glUseProgram(mProgram);
	   applyMatrixMVP();
	   applyStorage();
	}
	else
	{
	   Tools::getLogStream()<<"Run error: GLSL object is not initialized."<<std::endl;
	}
	
	return mIsInit;
}

void GLSL::stop()
{
	glUseProgram(0);
}

void GLSL::setProjectM(const glm::mat4& projM)
{
	mProjMat = projM;
}

void GLSL::setViewM(const glm::mat4& viewM)
{
	mViewMat = viewM;
}

void GLSL::setModelM(const glm::mat4& modelM)
{
	mModelMat = modelM;
}

GLuint GLSL::loadShader(GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char* buf = new char[infoLen];
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    Tools::getLogStream()<<"Could not compile shader "<<shaderType<<":\n"<<buf<<std::endl;
                    delete buf;
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint GLSL::createProgram(const char* pVertexSource, const char* pFragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader)
    {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader)
    {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program)
    {
		initAttributs(program);
        glAttachShader(program, vertexShader);
        Tools::checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        Tools::checkGlError("glAttachShader");

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
            {
                char* buf = new char[bufLength];
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    Tools::getLogStream()<<"Could not link program:\n"<<buf<<std::endl;
                    delete buf;
                }
            }
            glDeleteProgram(program);
            program = 0;
		}
		else
		{
			Tools::getLogStream()<<"Shader success"<<std::endl;
		};
    }
    return program;
}

GLSL::~GLSL()
{
	Tools::getLogStream()<<"GLSL program was deleted"<<std::endl;
	glDeleteProgram(mProgram);
    mProgram = 0;
}
