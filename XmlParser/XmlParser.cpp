/*
 * XmlParser.cpp
 * Main Class to manage XML operations
 * Created by: Ozgur Pekcagliyan - 2014-10-05 12:28:32 PM EEST
 * Last edited by: 
 * Notes: Ozgur Pekcagliyan - 2014-10-04
 * 
*/

#include <string>
#include <map>
#include "XmlParser.h>
#include "Logger.h"

using std::string;
using namespace XmlParser;


void vectorizeString (const string &str, vector<string> &returnVal, const char *seperator) // This will tokenize a string and fill a vector
{
	string tempStr(str);
	string tempToken;
	size_t pos;
	if(!returnVal.empty())
	{
		returnVal.clear();
	}
	while((pos = tempStr.find(seperator)) != string::npos)
	{
		tempToken = tempStr.substr(0, pos);
		tempStr.erase(0, pos);
		returnVal.push_back(tempToken);
	}
}

XmlParser::XmlParser()
{
}

XmlParser::~XmlParser()
{
}

bool XmlParser::loadXmlTree(const string &data) //This will generate a tree and load it into memeory
{
	vector<string> tokenizedData;
	vectorizeString(data, tokenizedData, "<
}

template <class T>
T XmlParser::getAttributeValue(const string &attr) //This will call one of the traverse functions and return attributes value.
{
}

const XmlNode & XmlParser::findNode (const string &date) // This is the real traverse function and every other function will call it
{
}

const XmlNode & XmlParser::findNode (const XmlNode &node)
{
}
