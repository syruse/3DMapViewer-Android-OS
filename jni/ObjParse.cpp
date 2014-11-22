#include "ObjParse.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <glm/glm.hpp>
#include "Tools.hpp"
#include "GLSLLoader.hpp"

Obj::Obj()
: mMaterialDic()
, mnMeshes()
, mRadius(1.0f)
, mScale(1.0f)
, mpGlslProg(0)
, mGeometry()
, mnMeshIdent()
, mIsInit(false)
{
}

void Obj::initialize(std::string objName, std::string materialName, GLSL* glsl, float scale)
{
   if(materialName.size()!=0 && objName.size()!=0 && glsl !=0)
   {
      mpGlslProg = glsl;
      mScale = scale;
      loadMtl(materialName);
      loadObj(objName);
	  mIsInit = true;
   }
   else
   {
      mIsInit = false;
      Tools::getLogStream()<<"Could not create Obj object."<<std::endl;	  
   }
}

Obj::~Obj()
{
  //!do not delete texture, texture common for all models
  //deleting occur in Tools class
  /*
  std::map<std::string,Material>::iterator it =  mMaterialDic.begin();
  for(;it!=mMaterialDic.end();it++)
  {
    glDeleteTextures(1, &it->second.mTexID);
  }
  */

  for(int i=0;i<mnMeshes.size();i++)
  {
     glDeleteBuffers(1,  &mnMeshes[i].vertIndxsVBO);
  }
  glDeleteBuffers(1,  &mGeometry.atribsVBO);

  LOGI("Obj model was deleted\n");
  std::cout<<"\n---- <Destructor has cleared all resources> ----"<<std::endl;
  Tools::getLogStream()<<"Obj model was deleted"<<std::endl;
}

void Obj::loadMtl(std::string materialName)
{

  std::ifstream in(materialName.c_str());
  assert(in.is_open());

  std::string line;
  std::string curMatName;

  while(getline(in, line))
  {
      std::istringstream is(line);
      std::string word;

      while(is>>word)
      {

             if(word=="newmtl")
             {
                 is>>word;
                 curMatName=word;
             }
             else if(word=="Ka")
             {
                 is>>mMaterialDic[curMatName].mAmbient[0];
                 is>>mMaterialDic[curMatName].mAmbient[1];
                 is>>mMaterialDic[curMatName].mAmbient[2];
             }
             else if(word=="Kd")
             {
                 is>>mMaterialDic[curMatName].mDiffuse[0];
                 is>>mMaterialDic[curMatName].mDiffuse[1];
                 is>>mMaterialDic[curMatName].mDiffuse[2];
             }
             else if(word=="Ks")
             {
                 is>>mMaterialDic[curMatName].mSpecular[0];
                 is>>mMaterialDic[curMatName].mSpecular[1];
                 is>>mMaterialDic[curMatName].mSpecular[2];
             }
			 /*else if(word=="Ke")
             {
                 is>>mMaterialDic[curMatName].mEmis[0];
                 is>>mMaterialDic[curMatName].mEmis[1];
                 is>>mMaterialDic[curMatName].mEmis[2];
             }*/
             else if(word=="d" /* || word=="Tr"*/)
             {
                 is>>mMaterialDic[curMatName].mTrasparent;
             }
			 else if(word=="Ns")
             {
                 is>>mMaterialDic[curMatName].mShininess;
             }
             else if(word=="map_Kd")
             {
                 std::string texName;
                 is>>texName;
                 //mMaterialDic[curMatName].mTexID = Tools::loadTexture2D(texName);
				 mMaterialDic[curMatName].mTexName = texName;
                 //std::cout<<"+"<<texName<<"+"<<std::endl;
             }
             else
             {
                 //std::cout<<"-else-"<<word<<"+"<<std::endl;
                 break;
             }

      }

      //while(is>>word)std::cout<<"+"<<word<<"+"<<std::endl;
  }

  //std::map<std::string,Material>::iterator it =  mMaterialDic.begin();
  //for(;it!=mMaterialDic.end();it++)std::cout<<"+"<<it->first<<"+"<<std::endl;

}

void Obj::assignData(std::deque<float> &verts,
                    std::deque<float> &texcoords,
                    std::deque<float> &normals,
                    std::deque<GLushort> &indexes,
                    std::set<int> &uniqIndxs)
  {
      int indxsPerFace = indexes.size()/3;

      int tempMax=verts.size()/3>texcoords.size()/2?verts.size()/3:texcoords.size()/2;
      int indexMax = tempMax>normals.size()/3?tempMax:normals.size()/3;
      int totalIndxs = indxsPerFace+indexMax;

	  std::vector<float> nVertexes;
      std::vector<float> nTexCoords;
      std::vector<float> nNormals;
		
      nVertexes.reserve(3*totalIndxs);
      nVertexes.assign(3*totalIndxs,0);
      nTexCoords.reserve(2*totalIndxs);
      nTexCoords.assign(2*totalIndxs,0);
      nNormals.reserve(3*totalIndxs);
      nNormals.assign(3*totalIndxs,0);

      for(int k=0;k<verts.size();k+=3){
         glm::vec3 vtmp = glm::vec3(verts[k],verts[k+1],verts[k+2]);
         float curR = glm::length(vtmp);
         if(curR>mRadius)mRadius=curR;
      }

      for(int k=0;k<verts.size();k++){
         nVertexes[k] = verts[k] = ( verts[k] / mRadius ) * mScale;
      }
      for(int k=0;k<texcoords.size();k++){
         nTexCoords[k] = texcoords[k];
      }
      for(int k=0;k<normals.size();k++){
         nNormals[k] = normals[k];
      }

      float fval;
      int inc =0;
	  int indexIdent = 1;
	  std::vector<GLushort> nVertIndexes;
	  /*if(mnMeshIdent.size()==1)
	  {
	    nVertIndexes.reserve(indxsPerFace);
        nVertIndexes.assign(indxsPerFace,0);
	  }
	  else if(mnMeshIdent.size()>1)
	  {
	    nVertIndexes.reserve(mnMeshIdent[indexIdent].indexBegin/3);
        nVertIndexes.assign(mnMeshIdent[indexIdent].indexBegin/3,0);	  
	  }*/
      for(int k=0;k<indexes.size();k+=3)
	  {
	     if(mnMeshIdent.size()-1 >= indexIdent)
		 {
		    if(k == mnMeshIdent[indexIdent].indexBegin)
			{		  
			  Mesh mesh;
              mesh.mMatName=mnMeshIdent[(indexIdent-1)].matName;
			  mesh.nVertIndexes.reserve(nVertIndexes.size());
			  mesh.nVertIndexes.assign(nVertIndexes.begin(),nVertIndexes.end());
              mesh.indxsNum = nVertIndexes.size();
			  
			  glGenBuffers(1,&mesh.vertIndxsVBO);
              glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh.vertIndxsVBO);
              glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*(mesh.indxsNum), &nVertIndexes[0], GL_STATIC_DRAW);
              glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	          mnMeshes.push_back(mesh);
			  
			  indexIdent++;
              nVertIndexes.clear();
			}
		 }

         GLushort &curIndx=indexes[k];
         if(!uniqIndxs.count(curIndx))uniqIndxs.insert(curIndx);
         else
         {
             inc++;
             int temp = curIndx;
             curIndx = indexMax+inc;
             fval = verts[temp*3];
             nVertexes[curIndx*3] = fval;
             fval = verts[temp*3+1];
             nVertexes[curIndx*3+1] = fval;
             fval = verts[temp*3+2];
             nVertexes[curIndx*3+2] = fval;

             //std::cout<<"index new "<<indexMax<<" "<<std::endl;
             uniqIndxs.insert(curIndx);
         }
         nVertIndexes.push_back(curIndx);
         //std::cout<<"f "<<indexes[k]<<" "<<mesh.nVertexes[curIndx*3]<<std::endl;
         fval = texcoords[indexes[k+1]*2];
         nTexCoords[curIndx*2] = fval;
         fval = texcoords[indexes[k+1]*2+1];
         nTexCoords[curIndx*2+1] = fval;
         //std::cout<<"f "<<indexes[k]<<" "<<fval<<std::endl;
         fval = normals[indexes[k+2]*3];
         nNormals[curIndx*3] = fval;
         fval = normals[indexes[k+2]*3+1];
         nNormals[curIndx*3+1] = fval;
         fval = normals[indexes[k+2]*3+2];
         nNormals[curIndx*3+2] = fval;
      }
	  
	  {		  
		Mesh mesh;
        mesh.mMatName=mnMeshIdent[(indexIdent-1)].matName;
		mesh.nVertIndexes.reserve(nVertIndexes.size());
		mesh.nVertIndexes.assign(nVertIndexes.begin(),nVertIndexes.end());
        mesh.indxsNum = nVertIndexes.size();
			  
		glGenBuffers(1,&mesh.vertIndxsVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh.vertIndxsVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*(mesh.indxsNum), &nVertIndexes[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	    mnMeshes.push_back(mesh);
			  
		indexIdent++;
        nVertIndexes.clear();
	  }
	  
      /*for(int k=0;k<mesh.nVertIndexes.size();k+=3){

			 float vert1 = mesh.nVertexes[mesh.nVertIndexes[k]*3];
			 float vert2 = mesh.nVertexes[mesh.nVertIndexes[k]*3+1];
			 float vert3 = mesh.nVertexes[mesh.nVertIndexes[k]*3+2];

			 glm::vec3 v1 = glm::vec3(vert1, vert2, vert3);

			 vert1 = mesh.nVertexes[mesh.nVertIndexes[(k+1)]*3];
			 vert2 = mesh.nVertexes[mesh.nVertIndexes[(k+1)]*3+1];
			 vert3 = mesh.nVertexes[mesh.nVertIndexes[(k+1)]*3+2];

			 glm::vec3 v2 = glm::vec3(vert1, vert2, vert3);

			 vert1 = mesh.nVertexes[mesh.nVertIndexes[(k+2)]*3];
			 vert2 = mesh.nVertexes[mesh.nVertIndexes[(k+2)]*3+1];
			 vert3 = mesh.nVertexes[mesh.nVertIndexes[(k+2)]*3+2];

			 float n1 = mesh.nNormals[mesh.nVertIndexes[k]*3];
			 float n2 = mesh.nNormals[mesh.nVertIndexes[k]*3+1];
			 float n3 = mesh.nNormals[mesh.nVertIndexes[k]*3+2];
			 glm::vec3 normal = glm::vec3(n1,n2,n3);

			 glm::vec3 v3 = glm::vec3(vert1, vert2, vert3);

			 glm::vec3 vec1 = glm::vec3(v2 - v1);
			 glm::vec3 vec2 = glm::vec3(v3 - v1);
			 glm::vec3 n = glm::cross(vec1,vec2);

			 float alpha = glm::acos(glm::dot(glm::normalize(n), glm::normalize(normal)));
	         std::cout<<alpha<<" ang "<<glm::degrees(alpha)<<std::endl;
			 //float sign = n.x+n.y+n.z;

			 int indx1 = mesh.nVertIndexes[(k)];
			 int indx2 = mesh.nVertIndexes[(k+1)];
			 int indx3 = mesh.nVertIndexes[(k+2)];

			 glm::vec3 vec = glm::vec3(v1 - glm::vec3(0.0f));
			 float sign = glm::dot( glm::normalize(vec), glm::normalize(n));

			 //std::cout<<" sign \n\n\n"<<sign<<std::endl;
			 if(sign>0)
			 {
               mesh.nVertIndexes[(k)] = indx3;
			   mesh.nVertIndexes[(k+2)] = indx1;
			   std::cout<<" sign "<<std::endl;
			 }
			 else
			 {

				 //std::cout<<" sign<0 "<<std::endl;
			 }

         //std::cout<<mesh.nTexCoords[k]<<" "<<mesh.nTexCoords[k+1]<<std::endl;
      }*/

	 /*std::cout<<" indc " <<mesh.nVertIndexes.size()<<std::endl;
     for(int k=0;k<mesh.nVertIndexes.size();k++){
     std::cout<<mesh.nVertIndexes[k]<<std::endl;
			 float vert1 = mesh.nVertexes[mesh.nVertIndexes[k]*3];
			 float vert2 = mesh.nVertexes[mesh.nVertIndexes[k]*3+1];
			 float vert3 = mesh.nVertexes[mesh.nVertIndexes[k]*3+2];
 std::cout<<vert1<<" "<<vert2<<" "<<vert3<<std::endl;
	 }*/
	  totalIndxs=indexMax+inc+1;
	  mGeometry.nVertAttribs.reserve(totalIndxs*8);


	  for(int k=0;k<totalIndxs;k++){

		  float vert1 = nVertexes[k*3];
		  float vert2 = nVertexes[k*3+1];
		  float vert3 = nVertexes[k*3+2];

		  float norm1 = nNormals[k*3];
		  float norm2 = nNormals[k*3+1];
		  float norm3 = nNormals[k*3+2];

		  float tex1 = nTexCoords[k*2];
		  float tex2 = nTexCoords[k*2+1];

		  mGeometry.nVertAttribs.push_back(vert1);
		  mGeometry.nVertAttribs.push_back(vert2);
		  mGeometry.nVertAttribs.push_back(vert3);

		  mGeometry.nVertAttribs.push_back(tex1);
		  mGeometry.nVertAttribs.push_back(tex2);

		  mGeometry.nVertAttribs.push_back(norm1);
		  mGeometry.nVertAttribs.push_back(norm2);
		  mGeometry.nVertAttribs.push_back(norm3);

	  }

      //std::cout<<totalIndxs<<std::endl;

      glGenBuffers(1,&mGeometry.atribsVBO);
      glBindBuffer(GL_ARRAY_BUFFER,mGeometry.atribsVBO);
      glBufferData(GL_ARRAY_BUFFER,8*sizeof(float)*totalIndxs, &mGeometry.nVertAttribs[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER,0);

      ///** We not required in this data*/
      nVertexes.clear();
      nTexCoords.clear();
      nNormals.clear();

      texcoords.clear();
      verts.clear();
      normals.clear();
      indexes.clear();
      uniqIndxs.clear();
  }

void Obj::loadObj(std::string objName)
{

  std::ifstream in(objName.c_str());
  assert(in.is_open());

  std::string line;
  std::string curMatName;
  bool ismesh = false;

  std::deque<float> verts;
  std::deque<float> texcoords;
  std::deque<float> normals;
  std::deque<GLushort> indexes;
  std::set<int> uniqIndxs;

  while(getline(in, line))
  {
      std::istringstream is(line);
      std::string word;
      float fval;
      GLushort usval;

      while(is>>word)
      {
             if(word=="usemtl")
             {
                 is>>word;
                 curMatName=word;
				 ismesh = true;
             }
             else if(word=="v")
             {
                 is>>fval;
                 verts.push_back(fval);
                 is>>fval;
                 verts.push_back(fval);
                 is>>fval;
                 verts.push_back(fval);

             }
             else if(word=="vn")
             {
                 is>>fval;
                 normals.push_back(fval);
                 is>>fval;
                 normals.push_back(fval);
                 is>>fval;
                 normals.push_back(fval);
             }
             else if(word=="vt")
             {
                 is>>fval;
                 texcoords.push_back(fval);
                 is>>fval;
                 texcoords.push_back(fval);
             }
             else if(word=="f")
             {
                if(ismesh)
			    {
			      MeshIdent meshIdent;
				  meshIdent.indexBegin = indexes.size();
				  meshIdent.matName = curMatName;
				  mnMeshIdent.push_back(meshIdent);
				
			      ismesh = false;
			    }
				
                 for(int i=0;i<3;i++)
                 {
                    is>>word;
                    std::istringstream iss(word);
                    std::string token;
                    while ( getline(iss, token, '/') )
                    {
                       std::istringstream sindex(token);
                       //std::cout<<i<<"+"<<token<<"+"<<std::endl;
                       sindex>>usval;
                       indexes.push_back(--usval);
                       //std::cout<<i<<"+"<<usval<<"+"<<std::endl;
                       //break;
                    }
                }
             }
             else
             {
                 //std::cout<<"-else-"<<word<<"+"<<std::endl;
                 //break;
             }

      }

  }
  
  assignData(verts,texcoords,normals,indexes,uniqIndxs);

  /*for(int i=0;i<mnMeshes.size();i++){
     std::cout<<"N"<<i<<"+"<<std::endl;
     std::cout<<mnMeshes[i].mMatName<<std::endl;
      //for(int j=0;j<mnMeshes[i].faceLen*3;j++){
         //std::cout<<"+"<<mnMeshes[i].pIndexes[j]<<"+"<<std::endl;
      }
  }*/

}

void Obj::draw(bool isViaVBO)
{
	if(mIsInit)
	{
	   drawModel(isViaVBO);
	}
	else
	{
	   Tools::getLogStream()<<"Error: Obj is not initialized."<<std::endl;
	}   
}

void Obj::drawModel(bool isViaVBO)
{
	glEnableVertexAttribArray(AID_Position);
	glEnableVertexAttribArray(AID_TexCoord);
	glEnableVertexAttribArray(AID_Normal);

	(void)mpGlslProg->run();
	
	if(isViaVBO)
	{
      glBindBuffer(GL_ARRAY_BUFFER, mGeometry.atribsVBO);
      glVertexAttribPointer(AID_Position, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (GLvoid*)static_cast<unsigned int>(VERT));
      glVertexAttribPointer(AID_TexCoord, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (GLvoid*)static_cast<unsigned int>(TEX));
      glVertexAttribPointer(AID_Normal,   3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (GLvoid*)static_cast<unsigned int>(NORM));
	}
	else
	{
      glVertexAttribPointer(AID_Position, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (&mGeometry.nVertAttribs[0] + 0));
      glVertexAttribPointer(AID_TexCoord, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (&mGeometry.nVertAttribs[0] + 3));
      glVertexAttribPointer(AID_Normal,   3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (&mGeometry.nVertAttribs[0] + 5));
	}

    for(int i=0;i<mnMeshes.size();i++)
	{
		

		glm::vec4 vAmbLight;
		vAmbLight.r = mMaterialDic[mnMeshes[i].mMatName].mAmbient[0];
		vAmbLight.g = mMaterialDic[mnMeshes[i].mMatName].mAmbient[1];
		vAmbLight.b = mMaterialDic[mnMeshes[i].mMatName].mAmbient[2];
		vAmbLight.a = 1.0f;

		mpGlslProg->setVecUniform(UID_MATERIAL_AMBIENT,vAmbLight);
		mpGlslProg->setFloatUniform(UID_SIZE, 1.0f);
		mpGlslProg->setBoolUniform(UID_USERFLAG,false);//at the beginning enable texturing 

		GLuint texId = 0;
		std::string texName = mMaterialDic[mnMeshes[i].mMatName].mTexName;
		if(texName.size())
		{
		   texId = Tools::getTexture2D(texName);
		}
		
		if(glIsTexture(texId))
			mpGlslProg->setTexUniform(UID_TEXTURE_1,texId);
		else
			mpGlslProg->setBoolUniform(UID_USERFLAG,true);//disable texturing

		if(isViaVBO)
		{
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mnMeshes[i].vertIndxsVBO);
          glDrawElements(GL_TRIANGLES, mnMeshes[i].indxsNum, GL_UNSIGNED_SHORT,0);
		}
		else
		{
		  glDrawElements(GL_TRIANGLES, mnMeshes[i].indxsNum, GL_UNSIGNED_SHORT, &mnMeshes[i].nVertIndexes[0]);
		}
    }

	mpGlslProg->stop();

	glDisableVertexAttribArray(AID_Position);
	glDisableVertexAttribArray(AID_TexCoord);
	glDisableVertexAttribArray(AID_Normal);

	if(isViaVBO)
	{
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	  glBindBuffer(GL_ARRAY_BUFFER,0);
	}

}

   /* for(int i=0;i<mnMeshes.size();i++){
            glBegin(GL_TRIANGLES);
            for(int j=0;j<mnMeshes[i].faceLen;j++){
                int indx = mnMeshes[i].nVertIndexes[j]*3;
                std::cout<<"indx"<<indx/3<<"+"<<std::endl;
                std::cout<<mnMeshes[i].nVertexes[indx]<<"+"<<mnMeshes[i].nVertexes[indx+1]<<"+"<<mnMeshes[i].nVertexes[indx+2]<<std::endl;
                glVertex3f(mnMeshes[i].nVertexes[indx],mnMeshes[i].nVertexes[indx+1],mnMeshes[i].nVertexes[indx+2]);
            }
            glEnd;
    }*/
