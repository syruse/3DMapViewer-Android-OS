
#ifndef APLICATION_DATA_H
#define APLICATION_DATA_H
#include <string>

class AplicationData
{
private:
    static const std::string msDataPath;
	static const std::string msTexturesPath;
	static const std::string msModelsPath;
	static const std::string msShadersPath;
	static const std::string msDBPath;
	static const std::string msLogFile;
public:
	~AplicationData(){};
	static std::string getDataPath ();
	static std::string getDBPath ();
	static std::string getShadersPath ();
	static std::string getTexturesPath ();
    static std::string getModelsPath ();
    static std::string getLogFile ();
private:
    AplicationData();
    AplicationData(const AplicationData&);
    AplicationData& operator=(const AplicationData&);
};

#endif //APLICATION_DATA_H
