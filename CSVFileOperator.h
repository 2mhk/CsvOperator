#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;

class CSVOperator
{
protected:
	string m_content;
	vector<vector<string>> m_result;
	static int Error_line;		//not used at all
	void clearresult();

public:
	CSVOperator(std::string buffer);
	CSVOperator() { m_content = ""; }
	~CSVOperator();

	enum Status_splitprocess
	{
		SSP_NEW,
		SSP_NORMAL,
		SSP_INCONTENT,
		SSP_OUTCONTENT
	};

	int GetErrorLine() { return Error_line; };
	vector<vector<string>> GetResult() { return m_result; }
	vector<vector<string>> GetResult(std::string buffer);
	static bool WriteResultToString(vector<vector<string>> _result, string &_outputstr);
	
	//self tool
	static std::string to_string_nozero(float input);
};
