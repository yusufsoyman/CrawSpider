/*
 * XmlParser.h
 * Main Class to manage XML operations
 * Created by: Ozgur Pekcagliyan - 2014-10-04 01:17:32 PM EEST
 * Last edited by: 
 * Notes: Ozgur Pekcagliyan - 2014-10-04
 * 
*/

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <vector>
#include <map> //this might be used to build a tree

namespace XmlParser
{

	template <class T>
	typedef struct XmlNode //FIXME: This can be a tree class an can be defined in another header file.
	{
	}XmlNode;


	template <class T>
	class XmlParser
	{
	public:
		XmlParser();
		~XmlParser();
		
		bool loadXmlTree(const string &data); //This will generate a tree and load it into memeory
		T getAttributeValue(const string &attr); //This will call one of the traverse functions and return attributes value.
		const XmlNode & findNode (const string &date); // This is the real traverse function and every other function will call it
		const XmlNode & findNode (const XmlNode &node);
	private:
		void vectorizeString (const string &str, vector<string> &returnVal); // This will tokenize a string and fill a vector
		map<T> tree;
	};

}

#endif