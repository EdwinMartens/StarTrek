#ifndef _SAX
#define _SAX

#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <cstring>

using namespace std;

/*!
@file InnoSax.h
\author E.J.M. Martens

Header for basic SAX parser (ANSI C++)

*/




enum XML_SYMBOL
{
    NONE,
    START_CONTEXT,
    END_CONTEXT,
    START_COMMENT,
    END_COMMENT,
    START_OPENING_TAG,
    CLOSE_OPENING_TAG,
    START_CLOSING_TAG,
    END_TAG
};

enum READING_MODE
{
    RM_NONE,
    RM_COMMENT,
    RM_CONTEXT,
    RM_START_TAG,
    RM_END_TAG,
    RM_TEXT
};


struct Attribute
{
    string m_Variable;
    string m_Value;
};


/*!
\author E.J.M. Martens
\section XMLParser
Class for parsing XML files FAST !
Can be used as a base for a DOM parser
*/

class XMLParser
{

public:

    //! Constructor, creates the xml parser.
    XMLParser();

    //! Virtual destructor
    virtual ~XMLParser();

    //! Read xml from stream
    //! @param a_XMLStream  A stream to read from
    bool Read(ifstream & a_XMLStream);


protected:
    //! Called when start of XML file detected
    //! @param a_strScheme  passes a scheme string
    virtual void StartFile(string & a_strScheme);

    //! Called when the end
    virtual void EndFile();

    //! Called at the start of a new node
    //! @param a_strNodeName name of the new node
    //! @param a_strAttributes Attributes string, pass to ParseAttributes to get a Variable, value list
    virtual void StartNode(string & a_strNodeName, string & a_strAttributes);

    //! Called at the end of a new node
    //! @param a_strNodeName name of the new node
    //! @param a_strAttributes Attributes string, pass to ParseAttributes to get a Variable, value list
    virtual void EndNode(string & a_strNodeName, string & a_strAttributes);

    //! Called for textual content between nodes
    //! @param a_strText plain text
    virtual void Content(string & a_strText);

    //! Parses an attribute string and builds a Variable, value list (Attribute)
    //! @param a_strAttributes attribute string
    vector<Attribute> ParseAttributes(string & a_strAttributes);

private:
    READING_MODE m_ReadingMode;
    list<string> m_lstTagStack;
    XML_SYMBOL translateSymbol(const char * a_szSymbol);
    string GetTagName(string a_strTag);
    void PushTag(string a_strTag);
    string PopTag();


};

#endif // _INNOSAX
