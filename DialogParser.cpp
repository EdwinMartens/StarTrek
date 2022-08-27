#include "DialogParser.h"
#include "Functions.h"
#include <iostream>
#include <math.h>
#include "utils.h"


CDialogParser * CDialogParser::pInstance = NULL;


CDialogParser * CDialogParser::GetInstance()
{
    if (pInstance == NULL)
    {
        pInstance = new CDialogParser();
    }
    return pInstance;
}


CDialogParser::CDialogParser()
{
    m_blReadDialog = false;
    m_pReadingStatement = NULL;
    m_pReadingAnswer = NULL;
    m_pDialog = NULL;
}

CDialogParser::~CDialogParser()
{

}

void CDialogParser::SetOutput(CDialog * a_pDialog)
{
     m_pDialog = a_pDialog;
}

void CDialogParser::StartNode(string & a_strNodeName, string & a_strAttributes)
{
    if (m_pDialog != NULL)
    {
        if (a_strNodeName == "DIALOG")
        {
            m_blReadDialog = true;
        }
        else if (a_strNodeName == "STATEMENT")
        {
            int nId =-1; string strIMG ="";
            vector<Attribute> attribs = ParseAttributes(a_strAttributes);
            for ( size_t i=0; i< attribs.size(); i++)
            {
                Attribute a = attribs[i];
                if (a.m_Variable == "id")
                {
                    nId = StringToNumber<int>(attribs[i].m_Value);
                }
                else if (a.m_Variable == "img")
                {
                    strIMG = attribs[i].m_Value;
                }
            }

            if (nId != -1)
            {
                m_pReadingStatement = new CStatement(nId);
                m_pReadingStatement->SetImage(strIMG);
            }

        }
        else if (a_strNodeName == "ANSWER")
        {
            int nNext =-1; int nValue =0;
            vector<Attribute> attribs = ParseAttributes(a_strAttributes);
            for ( size_t i=0; i< attribs.size(); i++)
            {
                Attribute a = attribs[i];
                if (a.m_Variable == "next")
                {
                    nNext = StringToNumber<int>(attribs[i].m_Value);
                }
                else if (a.m_Variable == "value")
                {
                    nValue = StringToNumber<int>(attribs[i].m_Value);
                }
            }

            if (m_pReadingStatement != NULL)
            {
                m_pReadingAnswer = new CAnswer(nNext,nValue);
            }

        }
    }
}

void CDialogParser::EndNode(string & a_strNodeName, string & a_strAttributes)
{
    if (m_pDialog != NULL)
    {
        if (a_strNodeName == "DIALOG")
        {
            m_blReadDialog = false;
        }
        else if (a_strNodeName == "STATEMENT")
        {
            if (m_pReadingStatement != NULL)
            {
                m_pDialog->AddStatement(m_pReadingStatement);
            }
            else
            {
                // error
            }

            m_pReadingStatement = NULL;
        }
        else if (a_strNodeName == "ANSWER")
        {
            if (m_pReadingStatement != NULL)
            {
                m_pReadingStatement->AddAnswer(m_pReadingAnswer);
            }
            else
            {
                // error
            }
            m_pReadingAnswer = NULL;
        }
    }
}

void CDialogParser::StartFile(string & a_strScheme)
{

}

void CDialogParser::EndFile()
{

}

void CDialogParser::Content(string & a_strText)
{
    if (m_pDialog != NULL)
    {
    a_strText.erase(0,a_strText.find_first_not_of(" \n\r\t"));

    if (m_pReadingStatement!= NULL)
    {
        if (m_pReadingAnswer != NULL)
        {
            a_strText.erase(a_strText.find_last_not_of(" \n\r\t")+1);
            m_pReadingAnswer->SetText(a_strText);
        }
        else
        {
            if (a_strText.size()>0)
            {

                a_strText.erase(a_strText.find_last_not_of(" \r\t")+1);
                vector<string> vText = Split(a_strText);

                for ( size_t i=0; i< vText.size(); i++)
                {
                    string strText = vText[i];
                    strText = trim(strText);
                    m_pReadingStatement->AddText(strText);
                }

            }
        }
    }
    }
}



vector<string> CDialogParser::Split(string a_strText)
{
    vector<string> vResult;
    string strCurrent;
    for (size_t i = 0; i< a_strText.size(); i++)
    {
        if (a_strText[i] != '\n')
        {
            strCurrent += a_strText[i];
        }
        else
        {
            vResult.push_back(strCurrent);
            strCurrent.clear();
        }
    }
    if (!strCurrent.empty())
    {
        vResult.push_back(strCurrent);
    }

    return vResult;
}




