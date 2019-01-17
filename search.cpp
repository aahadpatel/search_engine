#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include "webpage.h"
#include "database.h"
#include "config.h"

using namespace std;

//lowercase
string lowerWord(string word)
{
	string ret = "";
	for(int i=0;i<(int)word.length();i++) {
		ret += tolower(word[i]);
	}
	return ret;
}

//pagerank takes in candidate vector, restart probability, step, and database object
vector<string> page_rank(vector<string>& candidate_set, double r, int step, Database& database)
{	
	//return v
	vector<string> v;
	//creating outDegree array 
	int *degOut = new int[(int)candidate_set.size()];
	for(int n=0;n<(int)candidate_set.size();n++) {
		degOut[n] = 1;
	}
	//creating adjacency matrix
	int **matrix = new int*[(int)candidate_set.size()];
	for(int l = 0; l < (int)candidate_set.size(); l++) {
		matrix[l] = new int[(int)candidate_set.size()];
	}
	//creating 2-d array storing rank values
	double **rankResult = new double*[step+1];
	for(int m = 0; m < step+1; m++) {
		rankResult[m] = new double[(int)candidate_set.size()];
	}
	//initializing adjacency matrix diagonals and 0s
	for(int a=0;a<(int)candidate_set.size();a++) {
		for(int u=0;u<(int)candidate_set.size();u++) {
			if(a == u) matrix[a][u] = 1;
			else matrix[a][u] = 0;
		}
	}
	//initializing rankResult base cases (1/number of nodes)
	for(int o = 0; o < (int)candidate_set.size(); o++) {
		rankResult[0][o] = (1.0/(int)candidate_set.size());
	}

	//get each webpage and fill matrix
	for(int i=0;i<(int)candidate_set.size();i++) {
		Webpage *curr = database.getWebpage(candidate_set[i]);
		for(int j=i+1;j<(int)candidate_set.size();j++) {
			if(curr->inOutgoing(candidate_set[j])) {
				matrix[i][j] = 1;
				degOut[i] += 1;
			}
			if(curr->inIncoming(candidate_set[j])) {
				matrix[j][i] = 1;
				if(i < j) {
					degOut[j] += 1;
				}
			}
		}
	}

	//iterations and add to resulting set 
	for(int c = 1; c < step+1; c++)
	{
		for(int d=0;d<(int)candidate_set.size();d++) {
			double ans = 0.0;
			double sum = 0.0;
			for(int e=0;e<(int)candidate_set.size();e++) {
				if(matrix[e][d] == 1) {
					sum += (rankResult[c-1][e]*(1.0/degOut[e]));
					//sum *= (1.0/degOut[e]);
				}
			}
			ans = ((1-r)*sum) + (r*((1.0/candidate_set.size())));
			rankResult[c][d] = ans;
		}
	}

	//sorting
	for(int f=0;f<(int)candidate_set.size();f++)
	{	
		double max = 0.0;
		int ind = 0;
		for(int h=0;h<(int)candidate_set.size();h++) {
			if(rankResult[step][h] > max) {
				max = rankResult[step][h];
				ind = h;
			}
		}
		v.push_back(candidate_set[ind]);
		//reset
		rankResult[step][ind] = 0.0;
	}
	//delete degOut
	delete [] degOut;
	//delete 2-d matrix
	for(int q = 0; q < (int)candidate_set.size(); q++)
	{
		delete [] matrix[q];
	}
	delete [] matrix;
	//delete 
	for(int ab = 0; ab < step+1; ab++)
	{
		delete [] rankResult[ab];
	}
	delete [] rankResult;
	return v;
}

int main(int argc, char *argv[])
{	
	//VALIDATION
	if(argc > 2)
	{
		cout << "Invalid number of arguments!" << endl;
	}

	string configure = ""; 
	if(argc == 1)
		configure = "config.txt"; //if no file given, use default- config.txt
	else
		configure = argv[1]; //take in user's file

	Config config;
	config.configuration(configure);
	string index_file_name = config.getIndex();
	int t_steps = stoi(config.getStep());
	double e_restart = stod(config.getRestart());

	ifstream index;
	index.open(index_file_name);

	//VALIDATION
	if(index.fail())
	{
		cout << "File could not be opened!" << endl;
		return 0;
	}

	//declaring database object 
	Database database;

	//file path is the actual path, not the name of the file
	string file_path;
	while(getline(index, file_path))
	{
		Webpage* new_Webpage = new Webpage;
		database.insert_into_map(file_path, new_Webpage);
	}

	database.parse_Webpages(); //parse Webpages

	//open query 
	ifstream qfile;
	string query_file = config.getQuery();
	qfile.open(query_file);

	//VALIDATION
	if(qfile.fail())
	{
		cout << "File could not be opened!" << endl;
		return 0;
	}

	//output file 
	string output_file = config.getOutput();
	ofstream ofile;
	ofile.open(output_file);

	//take in first line of query 
	string line;

	//go through query.txt  
	while(getline(qfile, line))
	{
		stringstream ss;
		ss << line;
		string command;
		ss >> command;
		set<string> candidate_set;
		//if the first word is AND 
		if(command == "AND")
		{
			string word_to_find;
			string first;
			set<string> and_words;

			int count = 0;

			while(ss >> word_to_find)
			{
				if(count > 0) break;
				first = word_to_find;
				count += 1;
			}
			//empty
			if(count == 0 || word_to_find == "") {
				and_words.insert(lowerWord(command));

				set<string> result = database.or_function(and_words);
				//ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					set<string> in = database.incoming(*it);
					set<string> out = database.outgoing(*it);
					set<string>::iterator it1;
					for(it1 = in.begin();it1 != in.end();++it1)
					{
						if(database.containsPage(*it1)) candidate_set.insert(*it1);
					}
					set<string>::iterator it2;
					for(it2 = out.begin();it2 != out.end();++it2)
					{
						if(database.containsPage(*it2)) candidate_set.insert(*it2);
					}
					candidate_set.insert(*it);
				}

			}

			else {
				//lower, push
				and_words.insert(lowerWord(first));
				and_words.insert(lowerWord(word_to_find));
				while(ss >> word_to_find)
				{
					//insert into AND set 
					and_words.insert(lowerWord(word_to_find));
				}
				set<string> result = database.and_function(and_words);
				//ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					set<string> in = database.incoming(*it);
					set<string> out = database.outgoing(*it);
					set<string>::iterator it1;
					for(it1 = in.begin();it1 != in.end();++it1)
					{
						if(database.containsPage(*it1)) candidate_set.insert(*it1);
					}
					set<string>::iterator it2;
					for(it2 = out.begin();it2 != out.end();++it2)
					{
						if(database.containsPage(*it2)) candidate_set.insert(*it2);
					}
					candidate_set.insert(*it);
				}
			}
			//PAGERANK
			vector<string> param;
			set<string>::iterator itMain;
			for(itMain = candidate_set.begin();itMain != candidate_set.end();++itMain)
			{
				param.push_back(*itMain);
			}
			vector<string> rankResult = page_rank(param, e_restart, t_steps, database);
			ofile << rankResult.size() << endl;
			for(int i=0;i<(int)rankResult.size();i++) {
				ofile << rankResult[i] << endl;
			}
		}
		//first word is OR
		else if(command == "OR")
		{
			string word_to_find;
			string first;
			set<string> or_words;

			int count = 0;

			while(ss >> word_to_find)
			{
				if(count > 0) break;
				first = word_to_find;
				count += 1;
			}
			//insert words into or_words set for future comaprison
			if(count == 0 || word_to_find == "") {
				or_words.insert(lowerWord(command));

				set<string> result = database.or_function(or_words);
				//ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					set<string> in = database.incoming(*it);
					set<string> out = database.outgoing(*it);
					set<string>::iterator it1;
					for(it1 = in.begin();it1 != in.end();++it1)
					{
						if(database.containsPage(*it1)) candidate_set.insert(*it1);
					}
					set<string>::iterator it2;
					for(it2 = out.begin();it2 != out.end();++it2)
					{
						if(database.containsPage(*it2)) candidate_set.insert(*it2);
					}
					candidate_set.insert(*it);
				}
			}

			else {
				or_words.insert(lowerWord(first));
				or_words.insert(lowerWord(word_to_find));
				while(ss >> word_to_find)
				{
					//insert into OR words set
					or_words.insert(lowerWord(word_to_find));
				}
				set<string> result = database.or_function(or_words);
				//ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					set<string> in = database.incoming(*it);
					set<string> out = database.outgoing(*it);
					set<string>::iterator it1;
					for(it1 = in.begin();it1 != in.end();++it1)
					{
						if(database.containsPage(*it1)) candidate_set.insert(*it1);
					}
					set<string>::iterator it2;
					for(it2 = out.begin();it2 != out.end();++it2)
					{
						if(database.containsPage(*it2)) candidate_set.insert(*it2);
					}
					candidate_set.insert(*it);
				}
			}
			vector<string> param;
			set<string>::iterator itMain;
			for(itMain = candidate_set.begin();itMain != candidate_set.end();++itMain)
			{
				param.push_back(*itMain);
			}
			vector<string> rankResult = page_rank(param, e_restart, t_steps, database);
			ofile << rankResult.size() << endl;
			for(int i=0;i<(int)rankResult.size();i++) {
				ofile << rankResult[i] << endl;
			}
		}
		//first word is incoming 
		else if(command == "INCOMING")
		{
			string link;
			set<string> word;
			int count = 0;

			while(ss >> link)
			{
				if(count > 0) break;
				count += 1;
			}

			if(count == 0 || link == "") {
				word.insert(lowerWord(command));

				set<string> result = database.or_function(word);
				ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					ofile << *it << endl;
				}
			}

			else {
				//check if link is present 
				if(database.containsPage(link)) {
					set<string> result = database.incoming(link);
					ofile << result.size() << endl;

					set<string>::iterator it;
					for(it = result.begin();it != result.end();++it) {
						ofile << *it << endl;
					}
				}
				else {
					ofile << "Invalid query" << endl;
				}
			}
		}
		//first word is OUTGOING
		else if(command == "OUTGOING")
		{
			string link;
			set<string> word;
			int count = 0;

			while(ss >> link)
			{
				if(count > 0) break;
				count += 1;
			}

			if(count == 0 || link == "") {
				word.insert(lowerWord(command));

				set<string> result = database.or_function(word);
				ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					ofile << *it << endl;
				}
			}

			else {
				//check if link is present 
				if(database.containsPage(link)) {
					set<string> result = database.outgoing(link);
					ofile << result.size() << endl;

					set<string>::iterator it;
					for(it = result.begin();it != result.end();++it) {
						ofile << *it << endl;
					}
				}
				else {
					ofile << "Invalid query" << endl;
				}
			}
		}
		//first word is PRINT
		else if(command == "PRINT")
		{
			string link;
			set<string> word;
			int count = 0;
			//grab the link
			while(ss >> link)
			{
				if(count > 0) break;
				count += 1;
			}

			if(count == 0 || link == "") {
				word.insert(lowerWord(command));

				set<string> result = database.or_function(word);
				ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					ofile << *it << endl;
				}
			}

			else {
				//check if link is contained 
				if(database.containsPage(link)) {
					ofile << link << endl;
					//open page 
					ifstream page;
					page.open(link);

					string line_to_parse;
					//parse all lines to get the contents of the webpage
					while(getline(page, line_to_parse)) {

						if(line_to_parse == "") {
							ofile << endl;
						}
						else {
							for(int i=0;i<(int)line_to_parse.length();i++)
							{
								if(line_to_parse[i] != '(' && line_to_parse[i] != ')') 
								{
									ofile << line_to_parse[i];
									if(i == (int)line_to_parse.length() - 1)
									{
										ofile << endl;
									}
									continue;
								}
								else if(line_to_parse[i] == '(')
								{
									i++;
									while(line_to_parse[i] != ')')
									{
										i++;
									}
								}
							}
						}
					}
					ofile << endl;
				}
				else {
					ofile << "Invalid query" << endl;
				}
			}
		}
		//if just a blank line in file
		else if(command == "") {
			ofile << "Invalid query" << endl;
		}
		else
		{
			//searching for a word
			string word_to_find;
			set<string> word;

			int count = 0;

			while(ss >> word_to_find)
			{
				if(count > 0) break;
				count += 1;
			}

			if(count == 0 || word_to_find == "") {
				word.insert(lowerWord(command));

				//use union 
				set<string> result = database.or_function(word);
				//ofile << result.size() << endl;

				set<string>::iterator it;
				for(it = result.begin();it != result.end();++it) {
					set<string> in = database.incoming(*it);
					set<string> out = database.outgoing(*it);
					set<string>::iterator it1;
					for(it1 = in.begin();it1 != in.end();++it1)
					{
						if(database.containsPage(*it1)) candidate_set.insert(*it1);
					}
					set<string>::iterator it2;
					for(it2 = out.begin();it2 != out.end();++it2)
					{
						if(database.containsPage(*it2)) candidate_set.insert(*it2);
					}
					candidate_set.insert(*it);
				}
				vector<string> param;
				set<string>::iterator itMain;
				for(itMain = candidate_set.begin();itMain != candidate_set.end();++itMain)
				{
					param.push_back(*itMain);
				}
				vector<string> rankResult = page_rank(param, e_restart, t_steps, database);
				ofile << rankResult.size() << endl;
				for(int i=0;i<(int)rankResult.size();i++) {
					ofile << rankResult[i] << endl;
				}
			}
			else {
				ofile << "Invalid query" << endl;
			}
		}
	}
}
