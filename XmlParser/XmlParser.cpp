/*
 * XmlParser.cpp
 * Main Class to manage XML operations
 * Created by: Ozgur Pekcagliyan - 2014-10-05 12:28:32 PM EEST
 * Last edited by: 
 * Notes: Ozgur Pekcagliyan - 2014-10-07
 * FIXME:This implementation always expexts a tree hierarchy, there should be always 2 head nodes but it should be flexible to accept multiple trees.
*/

#include <string>
// #include <map>
#include <stack>
#include <iostream> // TODO:This will be romeved in working copy
#include "XmlParser.h"
#include "Logger.h"

using std::cout;
using std::endl;
using std::string;
using std::stack;
using std::vector;
using namespace XmlSpace;


void XmlParser::vectorizeString (const string &str, vector<string> &returnVal) // This will tokenize a string and fill a vector
{
	string tempStr(str);
	string tempToken;
	size_t pos, posBeg, posSpace;
	stack<XmlNode *> nodeStack; //This will hold nodes and 
	if(!returnVal.empty())
	{
		returnVal.clear();
	}
	while((pos = tempStr.find(">")) != string::npos) //find a node
	{
		tempToken = tempStr.substr(0, pos);
		
		if(tempStr[pos - 1] == '/') //<br/>
		{
			//FIXME: what if it has attributes?
			string nodeName = tempToken.substr(0, tempToken.size() - 1);
			XmlNode *temp = createNode(nodeName);
			nodeStack.top() -> subNodes.push_back(temp);
		}
		else if ((posSpace = tempToken.find(" ")) != string::npos && tempToken.find("/") == string::npos) //<a href="sf">
		{
			string nodeName = tempToken.substr(1, posSpace); //0th element should be <
			size_t posTemp;
			while((posTemp = tempToken.find("\"")) != string::npos)
			{
				tempToken.erase(posTemp); //delete if any " in str
			}
			XmlNode *temp = createNode(nodeName);
			if(temp == NULL)
			{
				//TODO: be sure stack is empty!
				nodeStack.push(head); //insert firs element to stack
			}
			else
			{
				nodeStack.top() -> subNodes.push_back(temp);
				nodeStack.push(temp);
			}
			tempToken.erase(0, posSpace); //remove processed part
			string attrName, attrValue;
			while((posTemp = tempToken.find("=")) != string::npos)
			{
				size_t posComa;
				attrName = tempToken.substr(0, posTemp);
				if((posComa = tempToken.find(",")) != string::npos)
				{
					attrValue = tempToken.substr(posTemp, posComa);
				}
				else
				{
					attrValue = tempToken.substr(posTemp, tempToken.size());
				}
				//Create an XmlAttribute
				XmlAttribute *tempAttr = new XmlAttribute();
				tempAttr -> attrName = attrName;
				tempAttr -> attrVal = attrValue;
				temp -> attributes.push_back(tempAttr);
			}
		}
		else //<asd> qwe </asd>
		{
			if(tempToken.find("/") != string::npos) // asd</asd>
			{
				posBeg = tempToken.find("<");
				string nodeVal = tempToken.substr(0, posBeg);
				//FIXME: Be sure stack is not empty and this XML is valid
				nodeStack.top() -> nodeVal = nodeVal;
				nodeStack.pop();
			}
			else // <asd>
			{
				string nodeName = tempToken.substr(1, string::npos); //0th element should be <
				//FIXME: make sure that XML tree is true
				XmlNode *temp = createNode(nodeName); //create an empty node
				if(temp == NULL) //we have just modified and this mmeans stack should also be empty
				{
					//TODO: be sure stack is empty!
					nodeStack.push(head); //insert firs element to stack
				}
				else
				{
					nodeStack.top() -> subNodes.push_back(temp);
					nodeStack.push(temp);
				}
			}
		}
		tempStr.erase(0, pos + 1); //erase read part
	}
	if(nodeStack.empty() == false)
	{
		//Oh no we have an invalid XML and couldn't complete XML
	}
// 	string tempStr(str);
// 	string tempToken;
// 	size_t pos, posSpace;
// 	if(!returnVal.empty())
// 	{
// 		returnVal.clear();
// 	}
// 	bool ignore = false;
// 	while((pos = tempStr.find(">")) != string::npos)
// 	{
// 		/*if(!ignore)
// 		{*/
// 			tempToken = tempStr.substr(0, pos);
// 			//if(tempStr[pos] == '>')
// 			//{
// 			if(tempStr[pos - 1] == '/') //<b/>
// 			{
// 				//Don't do anything yet. We will consider it according to implementation of other functionalities
// 				//tempToken = tempToken.substr(0, pos - 1);
// 			}
// 			else if ((posSpace = tempToken.find(" ")) != string::npos) //<a href="sf">sad</a>
// 			{
// 				ignore = true;
// 				string tmpDataTok = tempToken.substr(posSpace, pos); //this will href="sad" part //FIXME: Need to modify this according to = and " chars
// 				tempToken = tempToken.substr(1, posSpace);
// 				returnVal.push_back(tempToken);
// 				returnVal.push_back(tmpDataTok);
// 			}
// 			else //<asd> asd </asd>
// 			{
// 				if(tempToken.find("/") != string::npos) // asd</asd>
// 				{
// 					size_t posEndStart = tempToken.find("<");
// 					if(!ignore)
// 					{
// 						string tmpDataTok = tempToken.substr(1, posEndStart); //this will get asd part
// 					}
// 					ignore = false; // make sure ignore is always false
// 					returnVal.push_back(tmpDataTok);
// 					tempToken = tempToken.substr(posEndStart + 1, string::npos); //this will get /asd part
// 				}
// 				else // <asd>
// 				{
// 					tempToken = tempToken.substr(1, string::npos); //0th element should be <
// 				}
// 			}
// 			//}
// 			tempStr.erase(0, pos);
// 			if(!ignore)
// 			{
// 				returnVal.push_back(tempToken);
// 			}
// 		/*}
// 		else
// 		{
// 			tempStr.erase(0, pos);
// 			ignore = false;
// 		}*/
// 	}
}


XmlNode * XmlParser::createNode (const string &nodeName, const string nodeVal)
{
	if ( head == NULL) //if head is null then, we are just starting a new tree
	{
		head = new XmlNode();
		head -> nodeName = nodeName;
		head -> nodeVal = nodeVal;
		return NULL; //indicates that we have initialized new tree and modified head.
	}
	XmlNode *newNode = new XmlNode(); // TODO: This should be deleted by destructor/
	newNode -> nodeName = nodeName;
	newNode -> nodeVal = nodeVal;
	return newNode;
}

void XmlParser::printTree(XmlNode *node)
{
	if(head != NULL)
	{
		if(node == NULL)
		{
			node = head;
		}
		int attrSize = node -> attributes.size();
		int subNodesize = node -> subNodes.size();
		int i;
		cout<<"Node name: "<<node -> nodeName<<endl;
		for( i = 0; i < attrSize; ++i)
		{
			cout<<node -> attributes[i] -> attrName<<" = "<<node -> attributes[i] -> attrVal<<endl;
		}
		cout<<"Node Value: "<<node -> nodeVal<<endl<<"-------------------"<<endl;
		for(i = 0; i < subNodesize; ++i)
		{
			printTree(node -> subNodes[i]);
		}
		
	}
	else
	{
		cout<<"XML Not loaded"<<endl;
	}
}

XmlParser::XmlParser()
{
	head = NULL;
}

XmlParser::~XmlParser()
{
}

bool XmlParser::loadXmlTree(const string &data) //This will generate a tree and load it into memeory
{
	vector<string> tokenizedData;
	vectorizeString(data, tokenizedData);
	return true;
}

// template <class T>
const string XmlParser::getAttributeValue(const string &attr) //This will call one of the traverse functions and return attributes value.
{
}

// const XmlNode & XmlParser::findNode (const string &date) // This is the real traverse function and every other function will call it
// {
// }
// 
// const XmlNode & XmlParser::findNode (const XmlNode &node)
// {
// }
