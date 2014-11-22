#ifndef OBJPARSE_H
#define OBJPARSE_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <string>

class GLSL;

class Obj
{

    enum EOffset {
	               VERT = 0
	              ,TEX =  3 * sizeof(GLfloat)
				  ,NORM =  3 * sizeof(GLfloat) + 2 * sizeof(GLfloat)
				  };
				  
    struct Material
    {
    public:
      float mAmbient[3];
      float mDiffuse[3];
      float mSpecular[3];
      float mTrasparent;
	  float mShininess;
	  std::string mTexName;
      //GLuint mTexID;
    };
	
	struct MeshIdent
	{
	  int indexBegin;
	  std::string matName;
	};

    struct Mesh
    {
      unsigned int indxsNum;
	  std::vector<GLushort> nVertIndexes;
      GLuint vertIndxsVBO;
      std::string mMatName;
    };
	
    struct Geometry
    {
      std::vector<float> nVertAttribs;// normal vertex texcoord
	  GLuint atribsVBO;// normal vertex texcoord
    };

public:
private:
   std::map<std::string, Material> mMaterialDic;
   std::deque<Mesh> mnMeshes;
   float mRadius;
   float mScale;
   GLSL* mpGlslProg;
   /**
   * @member
   * common vertex attributes
   * for all meshes
   */
   Geometry mGeometry;
   std::deque<MeshIdent> mnMeshIdent;
	
   bool mIsInit;
public:
   Obj();
   void initialize(std::string objName, std::string materialName, GLSL* glsl, float scale);
   ~Obj();
   void draw(bool isViaVBO = true);
   void setShader(GLSL* glsl)
   {
      if(glsl)
      {
        mpGlslProg = glsl;
      }
   }  
   GLSL* getShader()
   {
      return mpGlslProg;
   }     
   float getRadius()
   {
      return  mScale;
   }
private:
   void drawModel(bool isViaVBO);	
   void loadMtl(std::string materialName);
   void loadObj(std::string objName);
   void assignData(std::deque<float> &verts,
                   std::deque<float> &texcoords,
                   std::deque<float> &normals,
                   std::deque<GLushort> &indexes,
                   std::set<int> &uniqIndxs);
   Obj(const Obj& );
};

#endif // OBJPARSE_H
