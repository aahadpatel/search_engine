#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include "config.h"

using namespace std;

//check if a page is in the vector
bool found(string page, vector<string>& v) {
	bool found = false;
	for(int i=0;i<(int)v.size();i++) {
		if(page == v[i]) {
			found = true;
			break;
		}
	}
	return found;
}

//remove page from vector
void remove(string page, vector<string>& v) {
	for(int i=0;i<(int)v.size();i++) {
		if(page == v[i]) {
			v.erase(v.begin()+i);
		}
	}
}

//parse a filepath and use dfs to explore new nodes
void parse(string filepath, vector<string>& v)
{
	ifstream page;
	page.open(filepath);

	if(page.fail())
	{
		remove(filepath, v);
	}

	else {
		string line_to_parse;
		//go through the contents of the webpage 
		while(getline(page, line_to_parse)) {
			for(int i = 0; i < (int)line_to_parse.length(); i++)
			{
				if(line_to_parse[i] == '(')
				{
					string link_webpage = "";
					i++;
					while(line_to_parse[i] != ')')
					{
						link_webpage += line_to_parse[i];
						i++;
					}
					//accounting for outgoing and incoming links as we parse
					//new_Webpage->insert_outgoing_links(link_webpage);
					bool f = found(link_webpage, v);
					//dfs
					if(!f) {
						v.push_back(link_webpage);
						parse(link_webpage, v);
					}
					else {
						continue;
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{	
	string configure; 
	if(argc == 1)
		configure = "config.txt"; //if no file given, use default- config.txt
	else
		configure = argv[1]; //take in user's file

	Config config;
	config.configuration(configure);
	//get index and output
	string index = config.getIndex();
	string output = config.getOutput();

	ifstream index_contents;
	index_contents.open(index);

	if(index_contents.fail())
	{
		cout << "File could not be opened!" << endl;
	}

	vector<string> v;
	string line_to_parse;
	while(getline(index_contents, line_to_parse)) {
		if(!found(line_to_parse, v)) v.push_back(line_to_parse);
		parse(line_to_parse, v);
	}
	//output
	ofstream of;
	of.open(output);
	//print final contents
	for(unsigned int i = 0; i < v.size(); i++)
	{
		of << v[i] << endl;
	}
	of.close();
}