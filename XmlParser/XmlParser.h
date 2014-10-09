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


namespace XmlSpace
{
	typedef struct XmlAttribute
	{
		std::string attrName;
		std::string attrVal;
	}XmlAttribute;

	typedef struct XmlNode
	{
		std::string nodeName;
		std::vector <XmlAttribute *> attributes;
		std::string nodeVal;
		std::vector <struct XmlNode *> subNodes;
	}XmlNode;

// 	template <class T>
	class XmlParser
	{
	public:
		XmlParser();
		~XmlParser();
		
		bool loadXmlTree(const std::string &data); //This will generate a tree and load it into memeory.
		const std::string getAttributeValue(const std::string &attr); //This will call one of the traverse functions and return attributes value.
		void printTree(XmlNode *node = NULL); //This will print the tree // TODO:This will be romeved in working copy
// 		T getAttributeValue(const string &attr); //This will call one of the traverse functions and return attributes value.
// 		const XmlNode & findNode (const string &date); // This is the real traverse function and every other function will call it.
// 		const XmlNode & findNode (const XmlNode &node);
	private:
		XmlNode * createNode (const std::string &nodeName, const std::string nodeVal = ""); // This function will create and return a node back to caller.
		void vectorizeString (const std::string &str, std::vector<std::string> &returnVal); // This will tokenize a string and fill a vector.
// 		map<T> tree;
		XmlNode *head;
	};

}

#endif