/*
 * XmlParser.h
 * Main Class to manage XML operations
 * Created by: Ozgur Pekcagliyan - 2014-10-04 01:17:32 PM EEST
 * Last edited by: 
 * Notes: Ozgur Pekcagliyan - 2014-10-07
 * FIXME:This implementation always expexts a tree hierarchy, there should be always 2 head nodes but it should be flexible to accept multiple trees.
*/

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <vector>
// #include <map> //this might be used to build a tree


namespace XmlParser
{
	typedef struct XmlAttribute
	{
		string attrName;
		string attrVal;
	}XmlAttribute;

	typedef struct XmlNode
	{
		string nodeName;
		vector <XmlAttribute *> attributes;
		string nodeVal;
		vector <struct XmlNode *> subNodes;
	}XmlNode;

// 	template <class T>
	class XmlParser
	{
	public:
		XmlParser();
		~XmlParser();
		
		bool loadXmlTree(const string &data); //This will generate a tree and load it into memeory.
		const string getAttributeValue(const string &attr); //This will call one of the traverse functions and return attributes value.
// 		T getAttributeValue(const string &attr); //This will call one of the traverse functions and return attributes value.
// 		const XmlNode & findNode (const string &date); // This is the real traverse function and every other function will call it.
// 		const XmlNode & findNode (const XmlNode &node);
	private:
		XmlNode * createNode (const string &nodeName, const string nodeVal = ""); // This function will create and return a node back to caller.
		void vectorizeString (const string &str, vector<string> &returnVal); // This will tokenize a string and fill a vector.
// 		map<T> tree;
		XmlNode *head;
	};

}

#endif