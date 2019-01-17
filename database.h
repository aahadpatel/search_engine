#include <iostream>
#include <map>
#include <set>
#include <string>
#include "webpage.h"

class Database {
	public: 
		Database();
		~Database();
		//inserting link into map<string, Webpage*>
		void insert_into_map(std::string filepath, Webpage* new_Webpage);
		//calls intersect
		std::set<std::string> and_function(std::set<std::string> and_words);
		//calls union_set
		std::set<std::string> or_function(std::set<std::string> or_words);
		//go through all webpages and while that happens, call parse_Webpage
		void parse_Webpages();
		//parsing individual webpage 
		void parse_Webpage(std::string filepath, Webpage* new_Webpage);
		//taking care incoming and outgoing links 
		std::set<std::string> incoming(std::string page);
		std::set<std::string> outgoing(std::string page);
		//take care of candidates 
		//std::set<std::string> candidate(std::string page);
		//check if page is contained in map<string, Webpage*>
		bool containsPage(std::string page);
		std::set<Webpage*> setWebpage(std::string word);
		Webpage* getWebpage(std::string page);

	private:
		//using AND command 
		std::set<std::string> intersect(std::set<std::string> and_set);
		//using OR command 
		std::set<std::string> union_set(std::set<std::string> or_set);
		//map containing filenames and the webpages 
		std::map<std::string, Webpage*> file_name_webpage;
};
