#include "SSharedTexture.hpp"
#include "Tools.hpp"

SSharedTexture::SSharedTexture(GLuint texHandle)
: handle(texHandle)
, pRefCount(0)
{
   pRefCount = new int(1);
}

SSharedTexture::SSharedTexture(const SSharedTexture& rhs)
: handle(rhs.handle)
, pRefCount(rhs.pRefCount)
{
   (*pRefCount)++;
}

SSharedTexture& SSharedTexture::operator=(const SSharedTexture& rhs)
{
   if(handle != rhs.handle)
   {
      processDeleting();
   }
   handle = rhs.handle;
   pRefCount = rhs.pRefCount;
   (*pRefCount)++;
   return *this;   
}

SSharedTexture::~SSharedTexture()
{
   processDeleting();
}

void SSharedTexture::processDeleting()
{
   (*pRefCount)--;
   if(*pRefCount==0 && glIsTexture(handle))
   {
      LOGI("delete texture handle");
      delete pRefCount;
      glDeleteTextures(1, &handle);
   }   
} 