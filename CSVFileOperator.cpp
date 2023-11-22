#include "CSVFileOperator.h"
#include <map>
#include <sstream>
#include <fstream>

using namespace std;

vector<vector<string>> CSVOperator::GetResult(std::string buffer)
{
	vector<vector<string>> m_result;
	auto& m_content = buffer;

	vector<string> line;
	string current_buf;
	line.clear();
	current_buf.clear();

	Status_splitprocess status = SSP_NEW;

	for (int i = 0; i < m_content.length(); i++)
	{
		switch (status)
		{
		case CSVOperator::SSP_NEW:
			if (buffer[i] == '"') status = SSP_INCONTENT;
			else if (buffer[i] == ',')
			{
				status = SSP_NEW;
				line.push_back(current_buf);
				current_buf.clear();
			}
			else if (buffer[i] == '\r'|| buffer[i] == '\n')
			{
				if (buffer[i] == '\r'&&buffer[i + 1] == '\n')i++;
				status = SSP_NEW;
				line.push_back(current_buf);
				current_buf.clear();
				m_result.push_back(line);
				line.clear();
			}
			else
			{
				status = SSP_NORMAL;
				current_buf.push_back(buffer[i]);
			}
			break;
		case CSVOperator::SSP_NORMAL:
			if (buffer[i] == ',')
			{
				status = SSP_NEW;
				line.push_back(current_buf);
				current_buf.clear();
			}
			else if (buffer[i] == '\r' || buffer[i] == '\n')
			{
				if (buffer[i] == '\r'&&buffer[i + 1] == '\n')i++;
				status = SSP_NEW;
				line.push_back(current_buf);
				current_buf.clear();
				m_result.push_back(line);
				line.clear();
			}
			else
			{
				status = SSP_NORMAL;
				current_buf.push_back(buffer[i]);
			}
			break;
		case CSVOperator::SSP_INCONTENT:
			if (buffer[i] == '"') status = SSP_OUTCONTENT;
			else
			{
				status = SSP_INCONTENT;
				current_buf.push_back(buffer[i]);
			}
			break;
		case CSVOperator::SSP_OUTCONTENT:
			if (buffer[i] == '"')
			{
				status = SSP_INCONTENT;
				current_buf.push_back(buffer[i]);
			}
			else if (buffer[i] == ',')
			{
				status = SSP_NEW;
				line.push_back(current_buf);
				current_buf.clear();
			}
			else if (buffer[i] == '\r' || buffer[i] == '\n')
			{
				if (buffer[i] == '\r'&&buffer[i + 1] == '\n')i++;
				status = SSP_NEW;
				line.push_back(current_buf);
				current_buf.clear();
				m_result.push_back(line);
				line.clear();
			}
			else
			{
				status = SSP_NORMAL;
				current_buf.push_back(buffer[i]);
			}
			break;
		default:
			break;
		}

	}
	//防止没有换行符
	if (current_buf != "")
	{
		line.push_back(current_buf);
		current_buf.clear();
	}
	if (line.size() != 0)
	{
		m_result.push_back(line);
		line.clear();
	}
	

	return m_result;
}

bool CSVOperator::WriteResultToString(vector<vector<string>> _result, string &_outputstr)
{
	string word_tmp;
	bool special = false;
	for (vector<vector<string>>::iterator line = _result.begin(); line != _result.end(); line++)
	{
		for (vector<string>::iterator word = line->begin(); word != line->end(); word++)
		{
			special = false;
			word_tmp.clear();
			word_tmp = *word;

			if (word_tmp.find(',') != -1|| word_tmp.find('\n') != -1|| word_tmp.find('\r') != -1)
			{
				special = true;
				_outputstr.push_back('"');
			}
			for (int i = 0; i < word_tmp.size(); i++)
			{
				_outputstr.push_back(word_tmp[i]);
				if (word_tmp[i] == '"'&& special == true)
				{
					_outputstr.push_back(word_tmp[i]);
				}
			}
			if (special == true) _outputstr.push_back('"');
			_outputstr.push_back(',');
		}
		_outputstr.pop_back();
		_outputstr.append("\n");	
	}

	return true;
}

std::string CSVOperator::to_string_nozero(float input)
{
	char _output[20];
	memset(_output, 0, 20);

	sprintf_s(_output, "%g", input);

	return _output;
}

			
