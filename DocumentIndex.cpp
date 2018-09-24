#include	<fstream>
#include	<iostream>
#include	<string>
#include	<algorithm>
#include	<sstream>
#include	<vector>
#include	<map>
#include	<cctype>
#include	<set>
// #include	<unistd.h>

#include	"DocumentIndex.h"
#include	"GetLine.h"

using namespace std;

typedef	string::size_type	StringSize;

typedef std::map<int, string> MyMap;

void DocumentFile::CleanWordList() {

	string word, test;
	int excess = 0;
	for (list<string>::iterator i = copyofv.begin(); i != copyofv.end(); ++i)
	{
		word = *i;
	
		/*if (hasExt(word))
		{
			skip_words.push_front(word);
		}*/
		if (hasNum(word))
		{
			skip_words.push_front(word);
		}
		if (hasPunct(word))
		{
			skip_words.push_front(word);
		}
		for (list<string>::iterator k = copyofv.begin(); k != copyofv.end(); ++k)
		{
			test = *k;
			/*	for (list<string>::iterator p = skip_words.begin(); p != skip_words.end(); ++p)
			{*/
			// Check if word appears more than 10 times within the list. If so, add word to skip list
			if (word == test)
			{
				excess++;
				if (excess > 10)
				{
					
					skip_words.push_front(word);
					excess = 0;
				}
			}
		}
		excess = 0;
	}
	//skip_words.sort();
	skip_words.unique();

	for (list<string>::iterator p = skip_words.begin(); p != skip_words.end(); p++)
	{
		cout << "Skip Word " << *p << endl;
	}
}
// Checks if string contains extensions e.g. .txt. If true, word is added to skip list
bool DocumentFile::hasExt(const string& s)
{
	string s1 = ".txt", s2 = ".cpp", s3 = ".com", s4 = ".h";
	size_t found1 = s.find(s1);
	size_t found2 = s.find(s2);
	size_t found3 = s.find(s3);
	size_t found4 = s.find(s4);

	if (found1 != std::string::npos)
		return true;
	if (found2 != std::string::npos)
		return true;
	if (found3 != std::string::npos)
		return true;
	if (found4 != std::string::npos)
		return true;
	else
		return false;
}
// Checks if string contains a number. If true, word is added to skip list
bool DocumentFile::hasNum(const string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end())
	{
		if (isdigit(*it))
			return true;
		it++;
	}
	return false;
}
// Checks if string contains a punctuation character. If true, word is added to skip list
bool DocumentFile::hasPunct(const string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end())
	{
		if (ispunct(*it))
			return true;
		it++;
	}
	return false;
}

void	DocumentFile::Close()
{
	file_.close();
	file_.clear();

	return;
}

int	DocumentFile::GetPageNumber()
{
	return pageNumber_;
}

string	DocumentFile::GetWord()
{
	string	word;
	string word1;



	for (int iter = 0; iter <= counter; iter++)
	{
		if (grabword == V.end())
		{
			word.clear();
			return word;
		}
		word = *grabword;

		for (list<string>::const_iterator i = skip_words.begin(); i != skip_words.end(); ++i)
		{
			word1 = *i;
			if (word == word1)
			{
				if (grabword == V.end())
				{
					break;
				}
				else 
				{
					grabword++;
				}
			}
		}
	}

	if (grabword == V.end())
	{
		word.clear();
		return word;
	}
	grabword++;

	// We tried to implement a set in order to filter out duplicate words that appear on the same page.
	/*MyMap map;
	MyMap::iterator mapIt;
	std::set<std::string> values;
	int key;
	string value;
	int count;*/
	//for (count = 0; count < 3; count++)
	//{
	//	cin >> key;
	//	cin >> value;

	//	//auto found = map.find(key);

	/*	if (found != map.end())
	//		continue;*/

	/*	if (values.insert(value).second)
	//		map.insert(make_pair(key, value));*/
	//}
	/*for (mapIt = map.begin(); mapIt != map.end(); ++mapIt)
	{
		cout << " " << (*mapIt).second << endl;
	}*/

	return word;
}

bool	DocumentFile::LoadExclusions(const string& name)
{
	DocumentFile exclusion;
	cout << name << endl;
	exclusion.Open(name);

	bool	success;
	string ex_word;

	if (!exclusion.file_.fail())
	{
		//	You may add any useful initialization here.
	}
	else
	{
		cout << "failed" << endl;
		return(false);
	}

	while (!exclusion.file_.eof())
	{
		GetLine(exclusion.file_, ex_word);
		if (!ex_word.empty())
			skip_words.push_back(ex_word);
	}
	success = true;

	return(success);
}

bool	DocumentFile::Open(const string& name)
{
	file_.open(name, ios::in);
	if (!file_.fail())
	{
		//	You may add any useful initialization here.
		return(true);
	}
	else
	{
		return(false);
	}
}

bool	DocumentFile::Read()
{
	bool success;
	string word;

	int new_page = 0;

	GetLine(file_, text_);

	if (text_.empty())
	{
		for (int i = 0; i <= 2; i++)
		{
			if (new_page == 2)
			{
				pageNumber_++;
				counter = 0;
				break;
			}
			new_page++;
			GetLine(file_, text_);
		}
	}

	success = true;
	V.clear();
	counter = 0;
	if (!text_.empty())
	{
		//cout << "printing from read function added: " << text_ << endl;
		istringstream getword(text_);
		for (string word; getword >> word;)
		{		
			if (hasExt(word))
			{
				removeStuff(word, '.');
				skip_words.push_front(word);
			}
			// Removes undesired characters from strings
			removeStuff(word, '.');
			removeStuff(word, ';');
			removeStuff(word, ':');
			removeStuff(word, '"');
			removeStuff(word, '(');
			removeStuff(word, ')');
			removeStuff(word, ',');
			removeStuff(word, '\'');
			removeStuff(word, '!');
			removeStuff(word, '?');
			removeStuff(word, ' ');
			//map.insert(*)
			for (auto c : word)
				if (ispunct(c))
					getword >> word;
			for (auto c : word)
				if (isdigit(c))
					getword >> word;
			copyofv.push_back(word);
			V.push_back(word);
			std::vector<int> vec;
			vec.push_back(pageNumber_);
			main.insert(std::pair<string, vector<int>>(word, vec));
			counter++;
		}
		grabword = V.begin();
	}

	
	if (file_.eof())
		success = false;

	return(success);
}
void DocumentFile::removeStuff(string& str, char character)
{
	size_t pos;
	while ((pos = str.find(character)) != string::npos)
		str.erase(pos, 1);
}
void DocumentFile::NoExcessive()
{
	
	string word;
	copyofv = V;
	for (list<string>::iterator i = skip_words.begin(); i != skip_words.end(); ++i)
	{
		copyofv.remove(*i);
	}
	for (list<string>::iterator i = copyofv.begin(); i != copyofv.end(); ++i)
	{
		word = *i;
	}

	for (list<string>::iterator i = skip_words.begin(); i != skip_words.end(); i++)
	{
		main.erase(*i);
	}

	for (itt = main.begin(); itt != main.end(); ++itt)
	{
			for (size_t i = 0; i < (itt->second).size(); i++)
		{
			int num = itt->second[i];
			string word = itt->first;
			cout << "Map Key: " << itt->first << " Value: " << itt->second[i] << endl;	
		}
			
	}
	while (main.size() > 0)
	{
		auto element = *(main.begin());
		new_map.insert(make_pair(element.first, element.second));
		main.erase(element.first);
	}
}

void	DocumentIndex::Create(DocumentFile& documentFile)
{
	//Start the map 
	string key;

	int pages;
	int vector_counter = 0;

	vector_counter = 0;
	pages = 0;
	while (documentFile.Read())
	{
		//mmaps.insert(pair<string, int>(documentFile.GetWord(), documentFile.GetPageNumber()));	
	}
	documentFile.CleanWordList();
	documentFile.NoExcessive();
	documentFile.initMap(indexmap);

	return;
}


void	DocumentIndex::Write(ostream& indexStream)
{	
	int excess = 0,
		page = 0;
	string key;
	string test;

	for (index = indexmap.begin(); index != indexmap.end(); ++index)
	{
		for (size_t i = 0; i < (index->second).size(); ++i)
			indexStream << index->first << " " << index->second[i] << '\n';
	}
	return;
}

void DocumentFile::initMap(multimap<string, vector<int>>& theMap)
{
	theMap = new_map;
	//new_map.insert(theMap.begin(), theMap.end());
	/* do stuff in theMap */
}
