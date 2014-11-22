#ifndef SSHAREDTEXTURE_H
#define SSHAREDTEXTURE_H

#include <GLES2/gl2.h>
#include <android/log.h>

/**
* class provide sharing the texture handling 
* not caring about deleting texture
*/
class SSharedTexture
{
   
private:
   GLuint handle;   
   int* pRefCount;
   
public:
   
   SSharedTexture(GLuint texHandle = 0U);
   SSharedTexture(const SSharedTexture& rhs);
   SSharedTexture& operator=(const SSharedTexture& rhs);
   
   inline GLuint getHandle() const
   {
      return handle;
   };
   
   ~SSharedTexture();
   
private:

   void processDeleting();
   
};

#endif //SSHAREDTEXTURE_H
