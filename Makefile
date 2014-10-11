
CXX=g++
CFLAGS=
LDFLAGS=-I./Database\ Connections/ -I./Logger/ -I./XmlParser -lmysqlclient
SOURCES=Logger/Logger.cpp Database\ Connections/DBAdapter.cpp XmlParser/XmlParser.cpp main.cpp
EXECUTABLES=main

all: 
	$(CXX) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $(EXECUTABLES)