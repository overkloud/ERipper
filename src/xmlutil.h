#ifndef XMLUTIL_CPP
#define XMLUTIL_CPP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <list>
#include <iostream>

#include <libxml/parser.h>
#include <libxml/tree.h>


using namespace std;


xmlDocPtr parseXml(const string &xml);

xmlNodePtr getRootElement(xmlDocPtr);

xmlNodePtr getParent(xmlNodePtr);

string getAttribute(xmlNodePtr, const string & attribute);
list<xmlNodePtr> getChildren(xmlNodePtr, const string & name = "");
int getChildrenCount(xmlNodePtr);

string getElementContent(xmlNodePtr, const bool & canEmpty = true);

string getNodeName(xmlNodePtr);
// string getNodeFullName(xmlNodePtr);
string getAttributeName(xmlNodePtr);
string getElementName(xmlNodePtr);


/**
HTML parsing ***/




#endif