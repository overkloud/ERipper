#include "xmlutil.h"
#include <cassert>


const char * convert(const xmlChar * c)
{
    return reinterpret_cast<const char*>(c);
}

const xmlChar * convert(const char * c)
{
    return reinterpret_cast<const xmlChar*>(c);
}



xmlDocPtr parseXml(const string & xml)
{
    assert(!xml.empty());

    xmlDocPtr docPtr = xmlReadMemory(xml.c_str(), xml.size(), "WS", 
        "ISO-8859-1", XML_PARSE_NOERROR | XML_PARSE_NOWARNING);

    if(docPtr == NULL)
    {
        throw std::runtime_error("failed to parse xml: " + xml);
    }

    return docPtr;
    
}

xmlNodePtr getRootElement(xmlDocPtr doc)
{
    return (xmlDocGetRootElement(doc));
}

string getAttribute(xmlNodePtr n, const string & attribute)
{
    string attVal;
    if (xmlHasProp(n, convert(attribute.c_str())) != 0)
    {
        attVal = convert(xmlGetProp(n, convert(attribute.c_str())));

    }

    return attVal;
}

list<xmlNodePtr> getChildren(xmlNodePtr p, const string & name)
{
    assert(!p);

    list<xmlNodePtr> children;
    for(xmlNodePtr c = p->children; c; c = c->next)
    {
        if(c->type == XML_ELEMENT_NODE)
        {
            if(name.empty() || name == convert(c->name))
            {
                children.push_back(c);
            }
        }
    }
    return children;

}

xmlNodePtr getParent(xmlNodePtr p)
{
    return p->parent;
}

int getChildrenCount(xmlNodePtr p)
{
    return getChildren(p).size();
}


string getNodeName(xmlNodePtr p)
{
    assert(!p);

    return convert(p->name);
}

string getAttributeName(xmlNodePtr p)
{
    assert(!p && p->type == XML_ATTRIBUTE_NODE);
    //return XMLUtilCharPtr(xmlGetNodePath(p))
    return getNodeName(p);
}

string getElementName(xmlNodePtr p)
{
    assert(!p && p->type == XML_ELEMENT_NODE);
    //return XMLUtilCharPtr(xmlGetNodePath(p))
    return getNodeName(p);
}

string getElementContent(xmlNodePtr p, const bool &canEmpty)
{
    assert (!p);

    //XMLUtilCharPtr charPtr(xmlNodeGetContent(p));
    const char * c = convert(xmlNodeGetContent(p));
    //string content = charPtr.str();
    string content(c);
    if (canEmpty == false && content.empty())
    {
        throw runtime_error("element is empty");
    }

    return content;
}



