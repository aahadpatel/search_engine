#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <iostream>
#include <string>
#include <set>

class Webpage {
	public: 
		//constructor
		Webpage();
		//destructor
		~Webpage();
		//inserting words
		void insert_word(std::string output);
		//insert filepaths to incoming links
		void insert_incoming_links(std::string incoming_link);
		//inserting filepaths to outgoing_links
		void insert_outgoing_links(std::string outgoing_link);
		//void insert_candidate_set(std::string candidate);
		//check is a word is in the words set 
		bool containsWord(std::string word);
		std::set<std::string> incomingLinks();
		std::set<std::string> outgoingLinks();
		//std::set<std::string> get_candidate_set();
		//std::string get_name();
		//void set_name(std::string name);
		bool inIncoming(std::string page);
		bool inOutgoing(std::string page);
		bool inEmpty;
		bool outEmpty;

	private:
		//making a word lowercase 
		std::string lowerWord(std::string word);
		//storing words
		std::set<std::string> words;
		//store all incoming links
		std::set<std::string> incoming_links;
		//store all outgoing links
		std::set<std::string> outgoing_links;
		//candidate set storing incoming and outgoing links
		//std::set<std::string> candidate_set;
		//std::string name_Webpage;
};

#endif