#ifndef PSYSMANAGER_H
#define PSYSMANAGER_H
#include "ParticalSystem.hpp"
#include <list>

typedef std::list<ParticalSystem*>::iterator LiPartSys;
class PSysManager
{
private:
	static int _Num;
	static std::list<ParticalSystem*> mLpPSys;
public:
	static int InstancePSys(GLSL* glslProg);
	static ParticalSystem* getPSys(int id);
	static void updateAll(float deltaMs);
    static void rendAll();
	static void delPSys(int id);
	static void clear();
	~PSysManager();
private:
	PSysManager();
	PSysManager(const PSysManager&);
	PSysManager& operator=(const PSysManager&);
};

#endif // PSYSMANAGER_H
