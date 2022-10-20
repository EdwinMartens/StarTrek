#include "Sax.h"
#include <strings.h>
#include <iostream>
#include <sstream>
#define MAX_SYMBOL_SIZE 10

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                          Constructor/destructor
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


XMLParser::XMLParser()
{


}


XMLParser::~XMLParser()
{


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                          Functions
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool XMLParser::Read(ifstream & a_XMLStream)
{
    char sSymbol[MAX_SYMBOL_SIZE];
    memset(sSymbol,'\0',sizeof(sSymbol));
    int nIndex =0;
    char chRead =' ';
    XML_SYMBOL symbol = NONE;
    bool blReadingSymbol = false;
    m_ReadingMode = RM_NONE;
    READING_MODE oldMode = RM_NONE;
    string strStartTag = "";
    string strEndTag = "" ;
    string strCurrentAttributes = "";
    string strScheme = "";
    string strContent= "";
    int nErrorlevel = 0;
    while( (!a_XMLStream.eof() ) && (nErrorlevel == 0) )
    {
       a_XMLStream.get(chRead);

       if (blReadingSymbol)
       {
           sSymbol[nIndex] = chRead;
           nIndex++;

           if ((chRead == '?') || (chRead == '/') || (chRead == '!') || (chRead == '-') || (chRead == '>'))
           {
               symbol = translateSymbol(sSymbol);
               if (symbol != NONE)
               {
                   blReadingSymbol = false;
                   nIndex = 0;
                   memset(sSymbol,'\0',sizeof(sSymbol));
               }
           }
           else
           {
               if (sSymbol[0]=='<')
               {
                   symbol = START_OPENING_TAG;
               }
               blReadingSymbol = false;
               nIndex = 0;
               memset(sSymbol,'\0',sizeof(sSymbol));
           }
       }
       else
       {
           if (chRead == '>')
           {
               symbol = END_TAG;
               nIndex = 0;
               memset(sSymbol,'\0',sizeof(sSymbol));
           }
           else
           {


           }

           if ((chRead == '<') || (chRead == '/') || (chRead == '?'))
           {
               blReadingSymbol = true;
               sSymbol[nIndex] = chRead;
               nIndex++;
           }
       }

       switch (symbol)
       {
            case START_CONTEXT:
                 strScheme.clear();
                 m_ReadingMode = RM_CONTEXT;
            break;

            case END_CONTEXT:
                 m_ReadingMode = RM_NONE;
                 StartFile(strScheme);
            break;

            case START_COMMENT:
                 oldMode = m_ReadingMode;
                 m_ReadingMode = RM_COMMENT;
            break;

            case END_COMMENT:
                 m_ReadingMode = oldMode;
                 oldMode = RM_NONE;
            break;

            case START_OPENING_TAG:
                 if (m_ReadingMode == RM_TEXT)
                 {
                     Content(strContent);
                 }
                 strStartTag.clear();
                 m_ReadingMode = RM_START_TAG;
            break;

            case START_CLOSING_TAG:
                 if (m_ReadingMode == RM_TEXT)
                 {
                     Content(strContent);
                 }
                 strEndTag.clear();
                 m_ReadingMode = RM_END_TAG;
            break;

            case END_TAG:
                 if (m_ReadingMode == RM_START_TAG)
                 {
                     string strTagName = GetTagName(strStartTag);
                     PushTag(strTagName);

                     size_t s = strTagName.size();
                     strCurrentAttributes = strStartTag.substr(s);
                     StartNode(strTagName,strCurrentAttributes);
                 }
                 else if (m_ReadingMode == RM_END_TAG)
                 {
                     string strReadTagName = GetTagName(strEndTag);
                     string strExpectedTagName = PopTag();

                     if (strReadTagName == strExpectedTagName)
                     {
                        EndNode(strReadTagName, strCurrentAttributes);
                     }
                     else
                     {
                         // error
                         nErrorlevel = -1;


                     }
                 }

                 m_ReadingMode = RM_TEXT;
                 strContent.clear();
            break;

            case CLOSE_OPENING_TAG:
                 if (m_ReadingMode == RM_START_TAG)
                 {
                     string strTagName = GetTagName(strStartTag);
                     size_t s = strTagName.size();
                     string strAttributes = strStartTag.substr(s);
                     StartNode(strTagName,strCurrentAttributes);
                     EndNode(strTagName,strCurrentAttributes);
                 }
                 m_ReadingMode = RM_TEXT;
                 strContent.clear();
            break;

            default:

            break;
        }

       symbol = NONE;


        if ((!blReadingSymbol) && (chRead != '?') && (chRead != '/') && (chRead != '!')  && (chRead != '>'))
        {
            char appendText[2];
            appendText[0] = chRead;
            appendText[1] = '\0';
            switch (m_ReadingMode)
            {
              case RM_TEXT:
                strContent.append(appendText);
              break;

              case RM_CONTEXT:
                   strScheme.append(appendText);
              break;

              case RM_START_TAG:
                   strStartTag.append(appendText);
              break;

              case RM_END_TAG:
                   strEndTag.append(appendText);
              break;

              default:

              break;
            }
        }

      if (nIndex > MAX_SYMBOL_SIZE)
       {
           memset(sSymbol,'\0',sizeof(sSymbol));
           nIndex =0;
           nErrorlevel = -3;
       }
    }

    if (m_lstTagStack.size() == 0)
    {
        // succes
        EndFile();
    }
    else
    {
        nErrorlevel = -2;
    }

    if (nErrorlevel == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


XML_SYMBOL XMLParser::translateSymbol(const char * a_szSymbol)
{
     XML_SYMBOL result = NONE;
     if (strncmp(a_szSymbol,"<",1) == 0)
     {
         result = START_OPENING_TAG;
     }

     if (strncmp(a_szSymbol,"</",2) == 0)
     {
         result = START_CLOSING_TAG;
     }
     else if (strncmp(a_szSymbol,"<?",2) == 0)
     {
          result = START_CONTEXT;
     }
     else if (strncmp(a_szSymbol,"?>",2) == 0)
     {
          result = END_CONTEXT;
     }
     else if (strncmp(a_szSymbol,"/>",2) == 0)
     {
          result = CLOSE_OPENING_TAG;
     }
     else if (strncmp(a_szSymbol,"<!--",4) == 0)
     {
          result = START_COMMENT;
     }
     else if (strncmp(a_szSymbol,"-->",3) == 0)
     {
          result = END_COMMENT;
     }

     if (strncmp(a_szSymbol,">",1) == 0)
     {
         result = END_TAG;
     }

    return result;
}


string XMLParser::GetTagName(string a_strTag)
{
     size_t index1 = a_strTag.find_first_not_of("</ ");
     size_t index2 = a_strTag.find_first_of(" />", index1);
     return a_strTag.substr(index1,index2);
}


void XMLParser::PushTag(string a_strTag)
{
    m_lstTagStack.push_back(a_strTag);
}


string XMLParser::PopTag()
{
    string strReturn;
    strReturn = m_lstTagStack.back();
    m_lstTagStack.pop_back();
    return strReturn;
}


vector<Attribute> XMLParser::ParseAttributes(string & a_strAttributes)
{
    vector<Attribute> vResult;

    string strVariable;
    string strValue;
    bool blReadVariable = true;
    int quotes = 0;
    int doublequotes =0;
    char chRead=' ';
    for ( size_t i=0; i< a_strAttributes.size(); i++)
    {
       chRead = a_strAttributes[i];
       switch (chRead)
       {
           case '=':
           blReadVariable = false;
           break;

           case '\'':
           quotes ++;
           break;

           case '\"':
           doublequotes++;
           break;

           case ' ':
           case '\t':
           case '\n':
           if (!blReadVariable && ((quotes %2) == 0) && ((doublequotes %2) == 0) && (strValue.size() >0))
           {
                Attribute a;
                a.m_Value = strValue;
                a.m_Variable = strVariable;
                vResult.push_back(a);
                strVariable.clear();
                strValue.clear();
                blReadVariable = true;
                quotes = 0;
                doublequotes =0;
                chRead=' ';
           }
           break;

           default:
            if (blReadVariable)
            {
               strVariable.push_back(chRead);
            }
            else
            {
               strValue.push_back(chRead);
            }

       }

    }

    if (!blReadVariable && ((quotes %2) == 0) && ((doublequotes %2) == 0) && (strValue.size() >0))
    {
        Attribute a;
        a.m_Value = strValue;
        a.m_Variable = strVariable;
        vResult.push_back(a);
    }

    return vResult;
}



void XMLParser::StartNode(string & a_strNodeName, string & a_strAttributes)
{
    cout << "Start: " << a_strNodeName.c_str() << "\n";

    vector<Attribute> attribs = ParseAttributes(a_strAttributes);

    for ( size_t i=0; i< attribs.size(); i++)
    {
        cout << attribs[i].m_Variable << " --> " << attribs[i].m_Value << "\n";
    }

}

void XMLParser::EndNode(string & a_strNodeName, string & a_strAttributes)
{
     cout << "End " << a_strNodeName.c_str() << "\n";
}

void XMLParser::StartFile(string & a_strScheme)
{
    cout << "Start of XML file   scheme: " << a_strScheme  << "\n";
}

void XMLParser::EndFile()
{
    cout << "End of XML file \n";
}

void XMLParser::Content(string & a_strText)
{
    a_strText.erase(a_strText.find_last_not_of(" \n\r\t")+1);
    cout << "[ " << a_strText << " ]\n";
}
