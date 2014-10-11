#include <string>
#include "XmlParser.h"

using std::string;

using namespace XmlSpace;

int main()
{
	string data = "<html><head test1=1111 test2=\"2222\" test3=\"3333 aaaa\" test4=4444><title>deneme</title></head><body>asdasd</body></html>";
	XmlParser myP;
	myP.loadXmlTree(data);
	myP.printTree();
	return 0;
}









// DB tester below
/*
#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include "DBAdapter.h"
#include "Logger.h"
#include <stdlib.h>

using namespace std;


DBAdapter dbConn (DBAdapter::MYSQL);

void sigHandler(int sig)
{
	char buffer[128];
	sprintf(buffer, "%s - %d: SIGTERM caught", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	Logger::finalize();
	int dummy;
	dbConn.disconnect(dummy);
	signal(sig, SIG_DFL);
	raise(sig);
}


int main(int argv, char **argc)
{
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	Logger::setLogConfig("./","test.log", Logger::DEBUG);
// 	DBAdapter dbConn (DBAdapter::MYSQL);
	int err;

	string ip = "192.168.1.98";
	string user = "test";
	string pass = "tester";
	dbConn.connect(ip.c_str(), 0, user.c_str(), pass.c_str(), err);
	if(err !=0 )
	{
		cout<<err<<endl;
		return 1;
	}
	dbConn.selectDB("test", err);
	dbConn.insertData("name, surname", "'yusuf', 'soyman'", "test", err);
	
	vector< list< string > >  myresult;
	dbConn.selectData("id, name, surname", "test", myresult, err);
	for(int i = 0; i < myresult.size(); i++)
	{
		for(list <string>::iterator it = myresult[i].begin(); it != myresult[i].end(); it++)
		{
			cout<<*it<<' ';
		}
		cout<<endl;
	}
	
 	dbConn.deleteData("id > 15", "test", err);
	dbConn.updateData("name, surname", "'jane', 'doe'", "test", err);
	
	dbConn.selectData("id, name, surname", "test", myresult, err);
	for(int i = 0; i < myresult.size(); i++)
	{
		for(list <string>::iterator it = myresult[i].begin(); it != myresult[i].end(); it++)
		{
			cout<<*it<<' ';
		}
		cout<<endl;
	}
	dbConn.disconnect(err);
// 	cout<<err<<endl;
	return 0;
}*/




/*

//Logger tester below

#include <unistd.h>
#include <signal.h>
#include "Logger.h"


void sigHandler(int sig)
{
	char buffer[128];
	sprintf(buffer, "%s - %d: SIGTERM caught", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	Logger::finalize();
	signal(sig, SIG_DFL);
	raise(sig);
}

int main(int argv, char **argc)
{
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	Logger::setLogConfig("./","test.log", Logger::DEBUG);
	char buffer[128];
	while(1)
	{
		sprintf(buffer, "%s - %d: This is a cool test log", __FILE__, __LINE__);
		Logger::printInfoLog(buffer);
		sleep(1);
// 		usleep(1000);
		sprintf(buffer, "%s - %d: This is a cool test log", __FILE__, __LINE__);
		Logger::printWarnLog(buffer);
		sleep(1);
// 		usleep(1000);
		sprintf(buffer, "%s - %d: This is a cool test log", __FILE__, __LINE__);
		Logger::printErrorLog(buffer);
		sleep(1);
// 		usleep(1000);
		sprintf(buffer, "%s - %d: This is a cool test log", __FILE__, __LINE__);
		Logger::printDebugLog(buffer);
		sleep(1);
		break;
// 		usleep(1000);
	}
	return 0;
}*/