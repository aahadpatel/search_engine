#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <string>
#include <cctype>
#include "database.h"
#include "webpage.h"

using namespace std;

Database::Database()
{

}
//deletes allocated memory (Webpage*)
Database::~Database()
{
	map<string, Webpage*>::iterator it;
	for(it = file_name_webpage.begin();it != file_name_webpage.end();it++) 
	{
		delete it->second;
	}
}

//set Webpage, return word_set of webpage pointers
set<Webpage*> Database::setWebpage(string word)
{
	set<Webpage*> word_set;
	map<std::string, Webpage*>::iterator it;
	for(it = file_name_webpage.begin(); it != file_name_webpage.end(); ++it)
	{
		if(it->second->containsWord(word))
			word_set.insert(it->second);
	}
	return word_set;
}

//inserting filepath into the map
void Database::insert_into_map(string filepath, Webpage* new_Webpage)
{
	file_name_webpage.insert(make_pair(filepath, new_Webpage));
}

//intersection using and_words set, move into intersect function NEW
set<string> Database::and_function(set<string> and_words)
{
	return intersect(and_words);
}

//move into union_set function NEW
set<string> Database::or_function(set<string> or_words)
{
	return union_set(or_words);
}

//go through the webpages and parse each one, calling parse_Webpage 
void Database::parse_Webpages()
{
	map<std::string, Webpage*>::iterator it;
	for(it = file_name_webpage.begin(); it != file_name_webpage.end(); ++it)
	{
		parse_Webpage(it->first, it->second);
	}
}
//parse the individual webpage
void Database::parse_Webpage(string filepath, Webpage* new_Webpage)
{
	ifstream index_contents;
	index_contents.open(filepath);

	string line_to_parse;
	string output;
	string output_link;

	//go through the contents of the webpage 
	while(getline(index_contents, line_to_parse)) {
		for(int i = 0; i < (int)line_to_parse.length(); i++)
		{
			if(isalnum(line_to_parse[i]))
			{
				//add each valid character to output string 
				output += line_to_parse[i];
				if(i == (int)line_to_parse.length() - 1) {
					new_Webpage->insert_word(output);
					output = "";
				}
				continue;
			}
			//if output is not a string 
			else if(output != "")
			{
				//put word
				new_Webpage->insert_word(output);
				output = "";
			}
			if(line_to_parse[i] == '(')
			{
				string link_webpage = "";
				i++;
				while(line_to_parse[i] != ')')
				{
					link_webpage += line_to_parse[i];
					i++;
				}
				new_Webpage->insert_outgoing_links(link_webpage);
				map<string, Webpage*>::iterator it = file_name_webpage.find(link_webpage);
				if(it != file_name_webpage.end()) {
					it->second->insert_incoming_links(filepath);
				}
			}
		}
	}
}

//for access to incoming links
set<string> Database::incoming(string page)
{
	return file_name_webpage[page]->incomingLinks();
}

set<string> Database::outgoing(string page)
{
	return file_name_webpage[page]->outgoingLinks();
}

bool Database::containsPage(string page)
{
	//if page is contained in the map<string, Webpage*>
	if(file_name_webpage.find(page) == file_name_webpage.end()) 
	{
		return false;
	}
	return true;
}

//find intersection for AND command 
set<string> Database::intersect(set<string> and_set)  
{
	//return set is the final set that will be returned containing intersection
	set<string> return_set;
	//iterate through map
	map<string, Webpage*>::iterator it1;
	//interate through set 
	set<string>::iterator it2;
	//interate through all webpages first (stored in a map)
	for(it1 = file_name_webpage.begin();it1 != file_name_webpage.end();++it1) 
	{
		bool hasAllWords = true;
		//iterate through the and_set, which contains all words after 
		//AND command
		for(it2 = and_set.begin();it2 != and_set.end();++it2) 
		{
			//make comaprison 
			if(!(it1->second->containsWord(*it2))) 
			{
				hasAllWords = false;
			}
		}
		//if valid, insert into return_set 
		if(hasAllWords) return_set.insert(it1->first);
	}
	return return_set;
}

//find union for OR command 
set<string> Database::union_set(set<string> or_set)  
{
	//return set is the final set that will be returned containing union
	set<string> return_set;
	//iterator through map 
	map<string, Webpage*>::iterator it1;
	set<string>::iterator it2;

	//iterate through webpages (stored in map)
	for(it1 = file_name_webpage.begin();it1 != file_name_webpage.end();++it1) 
	{
		bool hasAllWords = false;
		//iterate through or_set, which contains all words after
		//OR command 
		for(it2 = or_set.begin();it2 != or_set.end();++it2) 
		{
			//make comparison 
			if(it1->second->containsWord(*it2)) 
			{
				hasAllWords = true;
				break;
			}
		}
		//insert into return_set if valid 
		if(hasAllWords) return_set.insert(it1->first);
	}
	return return_set;
}

Webpage* Database::getWebpage(string page) {
	return file_name_webpage[page];
}