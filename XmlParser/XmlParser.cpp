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


void XmlParser::vectorizeString (const string &str, vector<string> &returnVal) // This will tokenize a string and fill a vector
{
	string tempStr(str);
	string tempToken;
	size_t pos, posSpace;
	bool ignore = false;
	if(!returnVal.empty())
	{
		returnVal.clear();
	}
	while((pos = tempStr.find(">")) != string::npos)
	{
		/*if(!ignore)
		{*/
			tempToken = tempStr.substr(0, pos);
			//if(tempStr[pos] == '>')
			//{
			if(tempStr[pos - 1] == '/') //<b/>
			{
				//Don't do anything yet. We will consider it according to implementation of other functionalities
				//tempToken = tempToken.substr(0, pos - 1);
			}
			else if ((posSpace = tempToken.find(" ")) != string::npos) //<a href="sf">sad</a>
			{
				ignore = true;
				string tmpDataTok = tempToken.substr(posSpace, pos); //this will get asd part
				tempToken = tempToken.substr(1, posSpace);
				returnVal.push_back(tempToken);
				returnVal.push_back(tmpDataTok);
			}
			else //<asd> asd </asd>
			{
				if(tempToken.find("/") != string::npos) // asd</asd>
				{
					size_t posEndStart = tempToken.find("<");
					if(!ignore)
					{
						string tmpDataTok = tempToken.substr(1, posEndStart); //this will get asd part
					}
					ignore = false; // make sure ignore is always false
					returnVal.push_back(tmpDataTok);
					tempToken = tempToken.substr(posEndStart + 1, string::npos); //this will get /asd part
				}
				else // <asd>
				{
					tempToken = tempToken.substr(1, string::npos); //0th element should be <
				}
			}
			//}
			tempStr.erase(0, pos);
			if(!ignore)
			{
				returnVal.push_back(tempToken);
			}
		/*}
		else
		{
			tempStr.erase(0, pos);
			ignore = false;
		}*/
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
	vectorizeString(data, tokenizedData, "<");
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
