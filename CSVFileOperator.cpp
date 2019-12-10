#include "CSVFileOperator.h"

int CSVOperator::Error_line = 0;

CSVOperator::CSVOperator(std::string buffer)
{
	m_content = buffer;
	Error_line = 0;
}

CSVOperator::~CSVOperator() 
{
	clearresult();
}

void CSVOperator::clearresult()
{
	for (vector<vector<string>>::iterator it = m_result.begin(); it != m_result.end(); it++)
	{
		it->clear();
	}
}

vector<vector<string>> CSVOperator::GetResult(std::string buffer)
{
	if (!strcmp(m_content.c_str(), buffer.c_str())) return m_result;

	m_content = buffer;
	clearresult();

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
	//��ֹû�н�����
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

bool CSVOperator::ChangeStrResultToOutput(map<int,LanguageOutput> &_LanguageOutput, vector<vector<string>> _result)
{
	bool check = true;
	LanguageOutput LanguageOutput_tmp;
	Error_line = 0;
	for (vector<vector<string>>::iterator line = _result.begin(); line != _result.end(); line++)
	{
		LanguageOutput_tmp.Chinese = "";
		LanguageOutput_tmp.English = "";
		LanguageOutput_tmp.Janpanese = "";
		int index = -1;

		for (vector<string>::iterator word = line->begin(); word != line->end(); word++)
		{
			switch ( word - line->begin())
			{
			case 0:
				if (*word == "0") index = 0;
				else
				{
					index = atoi(word->c_str());
					if (index == 0)
					{
						index = -1;
						Error_line++;
						check = false;
					}
				}
				break;
			case 1:
				LanguageOutput_tmp.Chinese = *word;
				break;
			case 2:
				LanguageOutput_tmp.English = *word;
				break;
			case 3:
				LanguageOutput_tmp.Janpanese = *word;
				break;
			default:
				break;
			}
		}
		_LanguageOutput[index] = LanguageOutput_tmp;
	}

	return check;
}

string CSVOperator::GetString(std::map<int, LanguageOutput> _LanguageOutput, CLanguageType LanguageType, int LanguageIndex)
{
	map<int, LanguageOutput>::iterator itor;
	switch (LanguageType)
	{
	case CLanguageType::LNG_ENGLISH:
		itor = _LanguageOutput.find(LanguageIndex);
		if (itor == _LanguageOutput.end()) return "";
		return itor->second.English;
		break;
	case CLanguageType::LNG_CHINESE:
		itor = _LanguageOutput.find(LanguageIndex);
		if (itor == _LanguageOutput.end()) return "";
		if (itor->second.Chinese != "") return itor->second.Chinese;
		return itor->second.English;
		break;
	case CLanguageType::LNG_JAPANESE:
		itor = _LanguageOutput.find(LanguageIndex);
		if (itor == _LanguageOutput.end()) return "";
		if (itor->second.Janpanese != "") return itor->second.Janpanese;
		return itor->second.English;
		break;
	default:
		itor = _LanguageOutput.find(LanguageIndex);
		if (itor == _LanguageOutput.end()) return "";
		return itor->second.English;
		break;
	}
}


std::string CSVOperator::to_string_nozero(float input)
{
	char _output[20];
	memset(_output, 0, 20);

	sprintf_s(_output, "%g", input);

	return _output;
}

			