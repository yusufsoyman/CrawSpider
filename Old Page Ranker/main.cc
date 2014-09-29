#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>

using namespace std;


#include <htmlcxx/html/ParserDom.h>

using namespace htmlcxx;

// #include <dirent.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <arpa/inet.h>
#include <mysql/mysql.h>
#include <pthread.h>

#define URL_ROOT "http://localhost/wiki/tr/"
#define RANK_FREQ 3
#define RANK_TURN 3


MYSQL *connection, mysql; //mysql islemleri icin initialize edilecek olan ve connection koprusunu tutacak olan degiskenler
MYSQL_RES *result; //veritabanindan donen sonuclari tutacak olan degisken
MYSQL_ROW row; //veritabanindan donen sonuclardan uretilecek rowlari tutan degisken
int query_state; //mysql de calistirilan sorgunun basari yada basarisizlik durumunu tutacak olan degisken
string url_data;
bool done;
bool ranking;
double damping_factor;
time_t rawtime;
struct tm *timeinfo;


char *regexp (char *string, char *patrn, int *begin, int *end)
{     
	int i, w=0, len;                  
	char *word = NULL;
	regex_t rgT;
	regmatch_t match;
	regcomp(&rgT,patrn,REG_EXTENDED);
	if ((regexec(&rgT,string,1,&match,0)) == 0)
	{
		*begin = (int)match.rm_so;
		*end = (int)match.rm_eo;
		len = *end-*begin;
		word=(char*)malloc(len+1);
		for (i=*begin; i<*end; i++)
		{
			word[w] = string[i];
			w++;
		}
		word[w]=0;
	}
	regfree(&rgT);
	return word;
}

size_t handle_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{ 
	int numbytes = size*nmemb; 
	// The data is not null-terminated, so get the last character, and replace 
	// it with '\0'. 
	char lastchar = *((char *) ptr + numbytes - 1); 
	*((char *) ptr + numbytes - 1) = '\0'; 
	url_data.append((char *)ptr); 
	url_data.append(1,lastchar); 
	*((char *) ptr + numbytes - 1) = lastchar;  // Might not be necessary. 
	return size*nmemb; 
 } 

void get_html_page(const string& url)
{
	 CURL* curl = curl_easy_init(); 
	if(curl) 
	{ 
		// Tell libcurl the URL 
		curl_easy_setopt(curl,CURLOPT_URL, url.c_str()); 
		// Tell libcurl what function to call when it has data 
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data); 
		// Do it! 
		CURLcode res = curl_easy_perform(curl); 
		curl_easy_cleanup(curl); 
// 		if (res == 0) 
// 		cout << url_data << endl; 
// 		else 
// 		cerr << "Error: " << res << endl; 
	} 
}


void * spider (void *);
void * ranker (void *);

int main()
{
	char query[1024];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(query, 1024, "%H:%M:%S", timeinfo);
	cout<<"I have started working at "<<time(0)<<"(timestamp) {"<<query<<"} with "<<RANK_FREQ<<" RANK_FREQ and "<<RANK_TURN<<" RANK_TURN"<<endl;
	pthread_t spdr;
	pthread_t rnkr;
	cout<<"Please enter a value for damping factor: ";
	cin>>damping_factor;
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql,"localhost","spider","spider","spider",0,0,0);
	query_state = mysql_query(connection, "truncate table queue");
	if(query_state != 0)
		cout<<mysql_error(connection);
	query_state = mysql_query(connection, "truncate table pages");
	if(query_state != 0)
		cout<<mysql_error(connection);
	query_state = mysql_query(connection, "truncate table links");
	if(query_state != 0)
		cout<<mysql_error(connection);
	query_state = mysql_query(connection, "truncate table spider.references");
	if(query_state != 0)
		cout<<mysql_error(connection);
	done = false;
	ranking = false;
	pthread_create(&spdr, NULL, spider, NULL);
	pthread_create(&rnkr, NULL, ranker, NULL);
// 	spider(NULL);
	while(!done);
	mysql_close(connection);
	return 0;
}

void * ranker (void *)
{
	int uid;
	int rlink;
	double urank;
	double rrank;
	char query[1024];
	MYSQL_RES *refres; //veritabanindan donen sonuclari tutacak olan degisken
	MYSQL_ROW refrow; //veritabanindan donen sonuclardan uretilecek rowlari tutan degisken
	double newrank;
	while(!done)
	{
		while(!ranking);
		for(int i = 0; i < RANK_TURN; i++)
		{
			query_state = mysql_query(connection, "select id, rank from pages");
			if(query_state != 0)
				cout<<mysql_error(connection);
			result = mysql_store_result(connection);
			while(row = mysql_fetch_row(result))
			{
				newrank = 0;
				uid = atoi(row[0]);
				urank = atof(row[1]);
				sprintf(query, "select l.links, p.rank from links l, pages p, spider.references r where r.uid = %d and r.rid = p.id and r.rid = l.uid", uid);
				query_state = mysql_query(connection, query);
				if(query_state != 0)
					cout<<mysql_error(connection);
				refres = mysql_store_result(connection);
				while(refrow = mysql_fetch_row(refres))
				{
					rlink = atoi(row[0]);
					rrank = atof(row[1]);
					newrank += rrank / rlink;
				}
				newrank = (1 - damping_factor) + (damping_factor * newrank);
				sprintf(query, "update pages set rank = %f where id = %d", newrank, uid);
				query_state = mysql_query(connection, query);
				if(query_state != 0)
					cout<<mysql_error(connection);
				
			}
		}
		timeinfo = localtime(&rawtime);
		strftime(query, 1024, "%H:%M:%S", timeinfo);
		cout<<"Ranking of saved URLs conmpleted at "<<time(0)<<"(timestamp) {"<<query<<"} within "<<RANK_TURN<<" turns"<<endl;
		ranking = false;
	}
	return NULL;
}

void * spider (void *dummy)
{
	int size;
// 	string buffer;
// 	ifstream in;
	int id;
	char path[1024];
	int count;
	int queueCount = 0;
	int c;
	char query[1024];
	char url[255];
	sprintf(query, "insert into queue (url) values('index.html')");
// 	cout<<query<<endl;
	query_state = mysql_query(connection, query);
	if(query_state != 0)
		cout<<mysql_error(connection);
	while(!done)
	{
head:		query_state = mysql_query(connection, "select id, url from queue order by id asc");
		if(query_state != 0)
			cout<<mysql_error(connection);
		result = mysql_store_result(connection);
		row = mysql_fetch_row(result);
		strcpy(url, row[1]);
		id = atoi(row[0]);
		//FIXME: What if queue is empty
		sprintf(path, "%s%s", URL_ROOT, url);
		sprintf(query, "delete from queue where id = %d", id);
		query_state = mysql_query(connection, query);
		if(query_state != 0)
			cout<<mysql_error(connection);
		sprintf(query, "select id from pages where url = '%s'", url);
		query_state = mysql_query(connection, query);
		if(query_state != 0)
			cout<<mysql_error(connection);
		result = mysql_store_result(connection);
		count = mysql_num_rows(result);
		if(count != 0)
		{
			row = mysql_fetch_row(result);
			id = atoi(row[0]);
		}
		else
		{
			sprintf(query, "insert into pages (url, rank) values ('%s', %f)", url, 1.0 - damping_factor);
			query_state = mysql_query(connection, query);
			if(query_state != 0)
				cout<<mysql_error(connection);
			sprintf(query, "select id from pages where url = '%s'", url);
			query_state = mysql_query(connection, query);
			if(query_state != 0)
				cout<<mysql_error(connection);
			result = mysql_store_result(connection);
			row = mysql_fetch_row(result);
			id = atoi(row[0]);
		}
		if(strcmp(path, "") == 0)
		{
			goto head;
		}
		if(queueCount >= RANK_FREQ)
		{
			queueCount = 0;
			ranking = true;
		}
		while(ranking);
// 		in.open(path, ios::in);
// 		in.seekg(0, ios::end);
// 		size = in.tellg();
// 		in.seekg(0, ios::beg);
// 		if(size == -1)
// 		{
// 			in.close();
// 			goto head;
// 		}
// 		buffer = new char[size];
// 		in.read(buffer, size);
// 		in.close();
		get_html_page(path);
		//Parse some html code
		HTML::ParserDom parser;
		tree<HTML::Node> dom = parser.parseTree(url_data);

		//Print whole DOM tree
		//cout <<dom <<endl;

		//Dump all links in the tree
		tree<HTML::Node>::iterator it = dom.begin();
		tree<HTML::Node>::iterator end = dom.end();
		c = 0;
// 		cout<<url<<endl<<"------------------"<<endl<<endl;
		for (; it !=end; ++it)
		{
			if (strcasecmp(it->tagName().c_str(), "A") == 0)
			{
				it->parseAttributes();
	// 			cout << it->attribute("href").second << endl;
// 				strcpy(query, it->attribute("href").second.c_str());
				char temp[1000];
				strcpy(temp, it->attribute("href").second.c_str());
// 				char *link = strstr(temp, "articles");
				int beg = 0;
				int end = strlen(temp);
				if(end != 0)
				{
					char *link = regexp(temp, "\\w.*html", &beg, &end);
// 					strcpy(link, regexp(temp, "\\w.*html", &beg, &end));
	// 				cout<<temp<<endl;
					
					if(link != NULL && strncmp(link, "http:", 5) != 0)
					{
						c++;
// 						if(c == 2070)
// 							cout<<"hi";
						sprintf(query, "select id from pages where url = '%s'", link);
						query_state = mysql_query(connection, query);
						if(query_state != 0)
							cout<<mysql_error(connection);
						result = mysql_store_result(connection);
						count = mysql_num_rows(result);
						if(count == 0)
						{
							sprintf(query, "insert into pages (url, rank) values('%s', %f)", link, 1.0 - damping_factor);
							query_state = mysql_query(connection, query);
							if(query_state != 0)
								cout<<mysql_error(connection);
							sprintf(query, "insert into queue (url) values('%s')", link);
							query_state = mysql_query(connection, query);
							if(query_state != 0)
								cout<<mysql_error(connection);
							sprintf(query, "select id from pages where url = '%s'", link);
							query_state = mysql_query(connection, query);
							if(query_state != 0)
								cout<<mysql_error(connection);
							result = mysql_store_result(connection);
							row = mysql_fetch_row(result);
							int uid = atoi(row[0]);
							sprintf(query, "insert into spider.references (uid, rid) values (%d, %d)", uid, id);
						}
						else
						{
							row = mysql_fetch_row(result);
							int uid = atoi(row[0]);
							sprintf(query, "insert into spider.references (uid, rid) values (%d, %d)", uid, id);
						}
						query_state = mysql_query(connection, query);
						if(query_state != 0)
							;//cout<<mysql_error(connection);
						
					}
				}
			}
		}
		sprintf(query, "select id from links where uid = %d", id);
		query_state = mysql_query(connection, query);
		if(query_state != 0)
			cout<<mysql_error(connection);
		result = mysql_store_result(connection);
		count = mysql_num_rows(result);
		if(count == 0)
		{
			sprintf(query, "insert into links (uid, links) values (%d, %d)", id, c);
			query_state = mysql_query(connection, query);
			if(query_state != 0)
				cout<<mysql_error(connection);
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			strftime(query, 1024, "%H:%M:%S", timeinfo);
			cout<<"I have completed processing of "<<url<<" at "<<time(0)<<"(timestamp) {"<<query<<"}"<<endl;
			c = 0;
		}
		queueCount++;
	}
	return NULL;
}