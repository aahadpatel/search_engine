#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

class Config
{
public:
	void configuration(std::string filename)
	{
		std::ifstream ifile;
		ifile.open(filename); //open file
		std::string parameter;
		std::string line_parse;

		bool readVar = false;

		while(getline(ifile, line_parse))
		{
			for(unsigned int i = 0; i < line_parse.size(); i++)
			{
				if(line_parse[i] == '#') break;
				else if(line_parse[i] == ' ' || line_parse[i] == '\t' || line_parse[i] == '=') {
					if(parameter == "") continue;
					readVar = true;
				}
				else
				{
					if(!readVar) parameter += line_parse[i];
					else {
						if(parameter == "INDEX_FILE") index += line_parse[i];
						else if(parameter == "QUERY_FILE") query += line_parse[i];
						else if(parameter == "OUTPUT_FILE") output += line_parse[i];
						else if(parameter == "RESTART_PROBABILITY") restart_prob += line_parse[i];
						else if(parameter == "STEP_NUMBER") step_num += line_parse[i];
					}
				}
			}
			parameter = "";
			readVar = false;
		}
	}
	std::string getIndex()
	{
		return index;
	}
	std::string getQuery()
	{
		return query;
	}
	std::string getOutput()
	{
		return output;
	}
	std::string getRestart()
	{
		return restart_prob;
	}
	std::string getStep()
	{
		return step_num;
	}
private:
	std::string index;
	std::string query;
	std::string output;
	std::string restart_prob;
	std::string step_num;
};