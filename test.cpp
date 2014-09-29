// DB tester below

#include <iostream>
#include <vector>
#include <string>
#include "DBAdapter.h"
#include "Logger.h"

using namespace std;

int main(int argv, char **argc)
{
	Logger::setLogConfig("./","test.log", Logger::DEBUG);
	DBAdapter dbConn (DBAdapter::MYSQL);
	int err;
	string ip = "127.0.0.1";
	string user = "test";
	string pass = "tester";
	dbConn.connect(ip.c_str(), 0, user.c_str(), pass.c_str(), err);
	if(err !=0 )
	{
		cout<<err<<endl;
		return 1;
	}
	dbConn.selectDB("test", err);
	dbConn.insertData("name, surname", "'john', 'smith'", "test", err);
	cout<<err<<endl;
// 	dbConn.deleteData("id = 3", "test", err);
// 	dbConn.updateData("name, surname", "'jane', 'doe'", "test", err);
	vector< list< string > >  myresult;
	dbConn.selectData("name, surname", "test", myresult, err);
	for(int i = 0; i < myresult.size(); i++)
	{
		for(list <string>::iterator it = myresult[i].begin(); it != myresult[i].end(); it++)
		{
			cout<<*it<<' ';
		}
		cout<<endl;
	}
	dbConn.disconnect(err);
	cout<<err<<endl;
	return 0;
}













//Logger tester below

/*#include <unistd.h>
#include <signal.h>
#include "Logger.h"


void sigHandler(int sig)
{
	char buffer[128];
	sprintf(buffer, "%s - %d: SIGTERM caught", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	Logger::finalize();
	signal(SIGTERM, SIG_DFL);
	raise(SIGTERM);
}

int main(int argv, char **argc)
{
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	Logger::setLogConfig("./","test.log", Logger::ERROR);
	char buffer[128];
	sprintf(buffer, "%s - %d: This is a cool test log", __FILE__, __LINE__);
	while(1)
	{
		Logger::printInfoLog(buffer);
		sleep(1);
// 		usleep(1000);
		Logger::printWarnLog(buffer);
		sleep(1);
// 		usleep(1000);
		Logger::printErrorLog(buffer);
		sleep(1);
// 		usleep(1000);
		Logger::printDebugLog(buffer);
		sleep(1);
// 		usleep(1000);
	}
	return 0;
}*/