/*
 * DBAdapter.cpp
 * Main Class to manage DB operations
 * Created by: Ozgur Pekcagliyan - 2014-09-26 09:34:38 PM EEST
 * Last edited by: Ozgur Pekcagliyan - 2014-09-28
 * Notes:
 * errorCode will be used to return specific error values back to user;
 * * Potential error codes are;
 * * * 0: success
 * * * 1: user name or password is incorrect
 * * * 2: db doesn't exist
 * * * 3: invalid sql query
 * * * 4: problem with disconnect
 * * * 5-1000: unknown (reserved)
 * * * bigger than 1000 - SQL server error system (Ex: 1045000 means 1045)
 * type will hold the database that will be connected
 * * possible values are;
 * * * 1: mysql (default value)
 * * * 2: oracle
 * * * rest of the codes will be added in time by developers
*/

#include <string>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <vector>
#include <mysql/mysql.h>


using std::string;
using std::vector;
// using std::strtok;

#include "DBAdapter.h"
#include "Logger.h"


DBAdapter::DBAdapter()
:type(MYSQL), isConnected(false)
{
	//Default constructor
	mysql_init(&myInit);
}
DBAdapter::DBAdapter(const db_types_t ty)
:type(ty), isConnected(false)
{
	//A very cool constructor :) // Ozgur
	//constructor to set db type at initialization, imlicit initialization is not allowed
	if(type == MYSQL)//Mysql
	{
		mysql_init(&myInit);
	}
	else
	{
		// FIXME: Add others here
	}
}
DBAdapter::~DBAdapter()
{
	if(isConnected)
	{
		int errorCode;
		disconnect(errorCode);
		if(errorCode != 0) //NO!!!! a Giant problem here :/
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: Can't clean connection. Raising SIGTERM immediately", __FILE__, __LINE__);
			Logger::printErrorLog(buffer);
			raise(SIGTERM);
		}
		else
		{
			isConnected = false;
		}
	}
	else
	{
		// Don't do anything yet
	}
}

bool DBAdapter::setDbType(const int ty) //function to change db type in runtime, if there is an active connection, this function will not run
{
	if(isConnected == true)
	{
		int errorCode;
		disconnect(errorCode); //we should disconnect communication if there is any
	}
	type = ty;
}

bool DBAdapter::connect (const char *ip, const int port, const char *user, const char *pass, int &errorCode) //connects database and resturns the result
{
	char buffer[1024];
	sprintf(buffer, "%s - %d: Connection request recieved", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	if(type == MYSQL)//Mysql
	{
		myConnection = mysql_real_connect(&myInit, ip, user, pass, NULL , port, NULL, 0);
		int err = mysql_errno(&myInit);
		if(err != 0)
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: Can't connect to mysql DB", __FILE__, __LINE__);
			Logger::printDebugLog(buffer);
			if(err == 1045)
			{
				errorCode = 1;
			}
			else
			{
				errorCode = err * 1000; //FIXME: Add every possible error code
			}
			return false;
		}
		else
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: MySQL connection established", __FILE__, __LINE__);
			Logger::printInfoLog(buffer);
			isConnected = true;
			errorCode = 0;
			return true;
		}
	}
	else
	{
		//Do other DB connections
	}
}
bool DBAdapter::disconnect (int &errorCode) //disconnect from database and return the result
{
	char buffer[1024];
	sprintf(buffer, "%s - %d: Disconnect request recieved", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	if(type == MYSQL) //Mysql
	{
		mysql_close(myConnection); //will be freed bt mysql_close
					   //This function doesn't return any error and always succeeds.
		isConnected = false;
		errorCode = 0;
		char buffer[1024];
		sprintf(buffer, "%s - %d: MySQL connection terminated", __FILE__, __LINE__);
		Logger::printInfoLog(buffer);
		return true;
	}
	else
	{
		isConnected = false;
		errorCode = 0;
		return true;
		//Do other DB disconnections
	}
}
bool DBAdapter::selectDB(const string &dbName, int &errorCode) //selecting DB to connect
{
	char buffer[1024];
	sprintf(buffer, "%s - %d: DB selection procedure initiated", __FILE__, __LINE__);
	Logger::printInfoLog(buffer);
	if(type == MYSQL) //Mysql
	{
		if(mysql_select_db(myConnection, dbName.c_str()) != 0)
		{
			int err = mysql_errno(&myInit);
			char buffer[1024];
			sprintf(buffer, "%s - %d: Can't select DB", __FILE__, __LINE__);
			Logger::printErrorLog(buffer);
			errorCode = err * 1000; //FIXME: Add every possible error code
			return false;
		}
		else
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: DB %s successfuly selected", __FILE__, __LINE__, dbName.c_str());
			Logger::printInfoLog(buffer);
			errorCode = 0;
			return true;
		}
	}
	else
	{
		errorCode = 0;
		return true;
	}
}
/*bool DBAdapter::execQuery(const string &sql, int &errorCode, char * reason) //execute a specific user written query, also returns a failure reason as set of chars
{
	if(type == MYSQL) //Mysql
	{
		
	}
	else
	{
		errorCode = 0;
		return true;
	}
}*/
bool DBAdapter::execQuery(const string &sql, int &errorCode) //overloaded version
{
	if(type == MYSQL) //Mysql
	{
		mysql_query(myConnection, sql.c_str());
		int err = mysql_errno(&myInit);
		if(err != 0)
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: Can't execute the sql query", __FILE__, __LINE__);
			Logger::printErrorLog(buffer);
			sprintf(buffer, "%s - %d: Problem with query \"%s\"", __FILE__, __LINE__, sql.c_str());
			Logger::printDebugLog(buffer);
			errorCode = err * 1000; //FIXME: Add every possible error code
			return false;
		}
		else
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: SQL Query successfuly executed on MySQL DB\n\"%s\"", __FILE__, __LINE__, sql.c_str());
			Logger::printDebugLog(buffer);
			errorCode = 0;
			return true;
		}
	}
	else
	{
		errorCode = 0;
		return true;
	}
}
bool DBAdapter::execQuery(const void *sql, const int& querySize, int &errorCode) //overloaded version which accepts binary data
{
	if(type == MYSQL) //Mysql
	{
		mysql_real_query(myConnection, static_cast<const char*>(sql), querySize);
		int err = mysql_errno(&myInit);
		if(err != 0)
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: Can't execute the sql query", __FILE__, __LINE__);
			Logger::printErrorLog(buffer);
			sprintf(buffer, "%s - %d: Problem with query \"%s\"", __FILE__, __LINE__, static_cast<const char*>(sql));
			Logger::printDebugLog(buffer);
			errorCode = err * 1000; //FIXME: Add every possible error code
			return false;
		}
		else
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: SQL Query successfuly executed on MySQL DB\n\"%s\"", __FILE__, __LINE__, static_cast<const char*>(sql));
			Logger::printDebugLog(buffer);
			errorCode = 0;
			return true;
		}
	}
	else
	{
		errorCode = 0;
		return true;
	}
}
bool DBAdapter::insertData(const string &fields, const string& values, const string &table, int &errorCode) /*inserts data into specific tables
							Example usage:
							insertData("name, surname, age", "'john', 'smith', 52", "users", errorCodeVar);
							*/
{
// 	int queryLength = fields.size() + values.size() + table.size();
	string sql = "insert into " + table + " ( " + fields + " ) " + " values ( " + values + " )";
	Logger::printDebugLog(sql.c_str());
	return execQuery(static_cast<const void*>(sql.c_str()), sql.size() + 1, errorCode);
	/*if(type == MYSQL) //Mysql
	{
		mysql_real_query(myConnection, sql.c_str(), sql.size());
		int err = mysql_errno(&myInit);
		if(err != 0)
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: Can't insert data into database", __FILE__, __LINE__);
			Logger::printErrorLog(buffer);
			sprintf(buffer, "%s - %d: Problem with query \"%s\"", __FILE__, __LINE__, sql.c_str());
			Logger::printDebugLog(buffer);
			errorCode = err * 1000; //FIXME: Add every possible error code
			return false;
		}
		else
		{
			char buffer[1024];
			sprintf(buffer, "%s - %d: SQL Query successfuly executed on MySQL DB\n\"%s\"", __FILE__, __LINE__, sql.c_str());
			Logger::printDebugLog(buffer);
			errorCode = 0;
			return true;
		}
	}
	else
	{
		errorCode = 0;
		return true;
	}*/
}
bool DBAdapter::selectData(const string &fields, const string& condition, const string &table, vector<string> &returnVal, int &errorCode, const char seperator) /*select data from specific tables
							Example usage:
							selecttData("name, surname, age", "name='john' or age < 52", "users", errorCodeVar);
							*/
{
	string sql = "select " + fields + " from " + table + " where " + condition;
	bool rValue = execQuery(static_cast<const void*>(sql.c_str()), sql.size(), errorCode);
	//FIXME: Find a logical way to install fix this part
	if(rValue == false)
	{
		return false;
	}
	if(type == MYSQL) //Mysql
	{
		MYSQL_RES *result = mysql_store_result(myConnection);
		MYSQL_ROW row;
		/*int rowNum = mysql_num_rows(result);
		returnVal = new unsigned char [rowNum]; //This should be deleted by the caller
		*/
		string temp;
		int fieldNum = mysql_num_fields(result);
		while(row = mysql_fetch_row(result))
		{
			temp.clear();
			for(int i = 0; i < fieldNum; i++)
			{
				if( i != fieldNum - 1)
				{
					if(temp.empty() == true)
					{
						temp = row[i];
						temp += seperator;
					}
					else
					{
						temp += row[i] + seperator;
					}
				}
				else
				{
					temp += row[i];
				}
			}
			returnVal.push_back(temp);
		}
	}
	else
	{
		errorCode = 0;
		return true;
	}
}
bool DBAdapter::selectData(const string &fields, const string &table, vector<string> &returnVal, int &errorCode, const char seperator)
{
	return selectData(fields, "1 = 1", table, returnVal, errorCode, seperator);
}
bool DBAdapter::deleteData(const string& condition, const string &table, int &errorCode) /*delete data from specific tables
							Example usage:
							deleteData("name=john and age >= 63", "users", errorCodeVar);
							*/
{
	string sql = "delete from " + table + " where " + condition;
	return execQuery(sql, errorCode);
	/*if(type == MYSQL) //Mysql
	{
	}
	else
	{
		errorCode = 0;
		return true;
	}*/
}
bool DBAdapter::updateData(const string &fields, const string& values, const string &condition, const string &table, int &errorCode) /*inserts data to specific tables
							Example usage
							updatetData("name, surname, age", "'john', 'smith', 52", "name=john and age >= 63", "users", errorCodeVar);
							*/
{
	string sql = "update " + table + " set ";
	char *fieldTok, *valueTok;
	char *saveptr1, *saveptr2;
	char *fieldsBuf = new char[fields.size() + 1];
	char *valuesBuf = new char[values.size() + 1];
	int c = 0;
	strcpy(fieldsBuf, fields.c_str());
	strcpy(valuesBuf, values.c_str());
	fieldTok = strtok_r(fieldsBuf, ", ", &saveptr1);
	valueTok = strtok_r(valuesBuf, ", ", &saveptr2);
	while(fieldTok != NULL && valueTok != NULL)
	{
		if(c > 0)
		{
			sql += " , ";
		}
		sql += fieldTok;
		sql += " = ";
		sql += valueTok;
		fieldTok = strtok_r(NULL, ", ", &saveptr1);
		valueTok = strtok_r(NULL, ", ", &saveptr2);
		c++;
	}
	sql += " where " + condition;
	delete []fieldsBuf;
	delete []valuesBuf;
	return execQuery(sql, errorCode);
	/*if(type == MYSQL) //Mysql
	{
	}
	else
	{
		errorCode = 0;
		return true;
	}*/
}
bool DBAdapter::updateData(const string &fields, const string& values, const string &table, int &errorCode) // overloaded version
{
	return updateData(fields, values, "1 = 1", table, errorCode);
}