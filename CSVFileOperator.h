#ifndef _CSVOPERATOR_H_
#define _CSVOPERATOR_H_

#include <vector>
#include <string>

class CSVOperator
{
private:
	CSVOperator() {}
	~CSVOperator() {};
    enum Status_splitprocess
    {
        SSP_NEW,
        SSP_NORMAL,
        SSP_INCONTENT,
        SSP_OUTCONTENT
    };

public:
	static std::vector<std::vector<std::string>> GetResult(std::string buffer);
	static bool WriteResultToString(std::vector<std::vector<std::string>> _result, std::string &_outputstr);
	
	//self tool
	static std::string to_string_nozero(float input);
};


#endif 
