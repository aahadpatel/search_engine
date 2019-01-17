#include <iostream>
#include <string>
#include <cctype>
#include <set>
#include "webpage.h"

using namespace std;

Webpage::Webpage()
{
	
}

Webpage::~Webpage()
{

}
//making the string lowercase 
string Webpage::lowerWord(string word)
{
	string ret = "";
	for(int i=0;i<(int)word.length();i++) {
		ret += tolower(word[i]);
	}
	return ret;
}

//insert words in words set
void Webpage::insert_word(string output)
{
	words.insert(lowerWord(output));
}

//inserting filepath into set incoming_links
void Webpage::insert_incoming_links(string filepath)
{
	incoming_links.insert(filepath);
}

//inserting filepath into set outgoing_links
void Webpage::insert_outgoing_links(string filepath)
{
	outgoing_links.insert(filepath);
}

//check if a word is contained in set words
bool Webpage::containsWord(string word)
{
	if(words.find(word) == words.end()) {
		return false;
	}
	return true;
}

//get incoming links set
set<string> Webpage::incomingLinks()
{
	return incoming_links;
}

//get outgoing links set
set<string> Webpage::outgoingLinks()
{
	return outgoing_links;
}

//check incoming 
bool Webpage::inIncoming(string page){
	if(!incoming_links.empty()) {
		set<string>::iterator it = incoming_links.find(page);
		if(it == incoming_links.end()) return false;
		return true;
	}
	return false;
}
//check if outgoing
bool Webpage::inOutgoing(string page){
	if(!outgoing_links.empty()) {
		set<string>::iterator it = outgoing_links.find(page);
		if(it == outgoing_links.end()) return false;
		return true;
	}
	return false;
}
