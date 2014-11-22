#include "PSysManager.hpp"
#include "Tools.hpp"

std::list<ParticalSystem*> PSysManager::mLpPSys;
int PSysManager::_Num = 0;

int PSysManager::InstancePSys(GLSL* glslProg)
{
   _Num++;
   ParticalSystem* pPSys = new ParticalSystem(glslProg, _Num);
   mLpPSys.push_back(pPSys);
   return pPSys->getId();
}

ParticalSystem* PSysManager::getPSys(int id)
{
	bool isFound = false;
	ParticalSystem* pFoundPSys = NULL;
	for(LiPartSys iter=mLpPSys.begin(); iter != mLpPSys.end(); )
	{
		ParticalSystem* pPSys = *iter;
		if(id == pPSys->getId())
		{
			//std::cout<<"found "<<pPSys->getId()<<std::endl;
			isFound = true;
			pFoundPSys = pPSys;
			break;
		}
		++iter;
	}

	if(isFound)
		return pFoundPSys;
	else
	{
	    Tools::getLogStream()<<"getPSys "<< id <<" isn't found"<<std::endl;
		return NULL;
	}
}

void PSysManager::updateAll(float deltaMs) // need clearing dead particles
{
	for(LiPartSys iter=mLpPSys.begin(); iter != mLpPSys.end(); ++iter)
	{
		ParticalSystem* pPSys = *iter;
		pPSys->update(deltaMs);	
	}
}

void PSysManager::rendAll()
{
	for(LiPartSys iter=mLpPSys.begin(); iter != mLpPSys.end(); ++iter)
	{
		ParticalSystem* pPSys = *iter;
		pPSys->rend();
	}
}

void PSysManager::delPSys(int id)
{
	for(LiPartSys iter=mLpPSys.begin(); iter != mLpPSys.end(); )
	{
		ParticalSystem* pPSys = *iter;
		if(id == pPSys->getId())
		{
			delete (*iter);
			iter = mLpPSys.erase(iter);
			Tools::getLogStream()<<"delPSys "<< id <<std::endl;
			break;
		}
		else
		{
			++iter;
		}
	}
}

void PSysManager::clear()
{
	for(LiPartSys iter=mLpPSys.begin(); iter != mLpPSys.end(); )
	{
		delete (*iter);
		iter = mLpPSys.erase(iter);
	}
	Tools::getLogStream()<<"PSysManager::clear() "<< std::endl;
};

PSysManager:: ~PSysManager()
{
	clear();
	//Tools::getLogStream()<<"PSysManager destr\n";
};

PSysManager:: PSysManager()
{
	//Tools::getLogStream()<<"PSysManager constr\n";
};