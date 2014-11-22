# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libfreetype    
LOCAL_SRC_FILES := C:\android\Other\FreeType\lib\libfreetype.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libboost_filesystem    
LOCAL_SRC_FILES := C:\Android\Other\boost\lib\libboost_filesystem.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libboost_system   
LOCAL_SRC_FILES := C:\Android\Other\boost\lib\libboost_system.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libboost_thread   
LOCAL_SRC_FILES := C:\Android\Other\boost\lib\libboost_thread.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
OPENCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off
# OPENCV_LIB_TYPE:=STATIC
include C:\android\OpenCV-2.4.5-android-sdk\sdk\native\jni\OpenCV.mk
LOCAL_C_INCLUDES+= C:\android\OpenCV-2.4.5-android-sdk\sdk\native\jni\include
LOCAL_MODULE    := libgl2jni
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := gl_code.cpp        \
                   TGALoader.cpp      \
                   Tools.cpp          \
                   TextRender.cpp     \
                   GLSLLoader.cpp     \
                   FakeLoader.cpp     \
                   AplicationData.cpp \
                   ObjParse.cpp       \
                   Camera.cpp         \
                   FPSCounter.cpp     \
                   SSharedTexture.cpp \
                   FrameBuffer.cpp    \
                   ParticalSystem.cpp \
                   Partical.cpp       \
                   PopularLoader.cpp  \
                   Skybox.cpp         \
                   Ground.cpp         \
                   PSysManager.cpp
APP_CPPFLAGS := -frtti -fexceptions
LOCAL_LDLIBS    := -llog -lstdc++ -lGLESv2 -lz
APP_STL := gnustl_static
LOCAL_STATIC_LIBRARIES := libfreetype libboost_filesystem libboost_system libboost_thread
LOCAL_C_INCLUDES += C:\Android\Other\GLM \
                    C:\android\Other\FreeType \
                    C:\Android\Other\boost\include\boost-1_49
                    
include $(BUILD_SHARED_LIBRARY)
