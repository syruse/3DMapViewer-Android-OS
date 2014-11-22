#include "AplicationData.hpp"

const std::string AplicationData::msDataPath = "/sdcard/MyProg";
const std::string AplicationData::msTexturesPath = "/Textures";
const std::string AplicationData::msModelsPath = "/Models";
const std::string AplicationData::msShadersPath = "/Shaders";
const std::string AplicationData::msDBPath = "/db.bin";
const std::string AplicationData::msLogFile = "/log.txt";

std::string AplicationData::getDataPath ()
{
    return AplicationData::msDataPath;
}

std::string AplicationData::getDBPath ()
{
    return AplicationData::msDataPath + AplicationData::msDBPath;
}

std::string AplicationData::getShadersPath ()
{
	return AplicationData::msDataPath + AplicationData::msShadersPath;
}

std::string AplicationData::getTexturesPath ()
{
    return AplicationData::msDataPath + AplicationData::msTexturesPath;
}

std::string AplicationData::getModelsPath ()
{
    return AplicationData::msDataPath + AplicationData::msModelsPath;
}

std::string AplicationData::getLogFile ()
{
    return AplicationData::msDataPath + AplicationData::msLogFile;
}
