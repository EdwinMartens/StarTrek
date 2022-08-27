#include <iostream>
#include "types.h"
#include "Statement.h"

ALLEGRO_COLOR CStatement::clStatement;
ALLEGRO_COLOR CStatement::clAnswer;
ALLEGRO_COLOR CStatement::clAnswerHighlight;

ALLEGRO_FONT * CStatement::pTextFont;
int            CStatement::nFontHeight;


void CStatement::INIT(ALLEGRO_FONT * a_pFont,
                      ALLEGRO_COLOR a_StatementColor,
                      ALLEGRO_COLOR a_AnswerColor,
                      ALLEGRO_COLOR a_AnswerColorHighlight)
{
    clStatement       = a_StatementColor;
    clAnswer          = a_AnswerColor;
    clAnswerHighlight = a_AnswerColorHighlight;
    pTextFont         = a_pFont;
}


CStatement::CStatement(int a_nId)
{
    m_nId = a_nId;
    m_nX = 20;
    m_nY = 30;
    m_nAnswerY = 200;
}


void CStatement::SetImage(string a_strImagePath)
{
    ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "conversation");
    al_append_path_component(pPath, "img");
    al_set_path_filename(pPath,a_strImagePath.c_str());
    m_pImage = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(pPath);
}


CStatement::~CStatement()
{
    vector<CAnswer *>::iterator p = m_vAnswer.begin();
    while (p!= m_vAnswer.end())
    {
        CAnswer * pAnswer = *p;
        delete pAnswer;
        p = m_vAnswer.erase(p);
    }

    al_destroy_bitmap(m_pImage);
}


void CStatement::AddText(string a_strText)
{
     m_vStatement.push_back(a_strText);
}

void CStatement::AddText(const char * a_szText)
{
    string strText = a_szText;
    m_vStatement.push_back(strText);
}

void CStatement::AddAnswer(CAnswer * a_pAnswer)
{
    a_pAnswer->m_nY = m_nAnswerY;
    m_nAnswerY += a_pAnswer->m_nHeight + 6;
    a_pAnswer->m_nX = 10;

    int nTextX,nTextY,nTextWidth,nTextHeight;
    al_get_text_dimensions( CStatement::pTextFont,
                             a_pAnswer->m_strText.c_str(),
                             &nTextX,&nTextY,&nTextWidth,&nTextHeight);

    a_pAnswer->m_nWidth = nTextWidth;
    m_vAnswer.push_back(a_pAnswer);
}


void CStatement::Draw()
{
    if (m_pImage != NULL)
    {
        al_draw_bitmap(m_pImage,1000 - al_get_bitmap_width(m_pImage),25,0);
    }

    for (size_t i =0; i< m_vStatement.size(); i++)
    {
         al_draw_text(pTextFont, clStatement, m_nX+10, m_nY+(20*i), 0, m_vStatement[i].c_str());
    }

    vector<CAnswer *>::const_iterator p = m_vAnswer.begin();
    while (p!= m_vAnswer.end())
    {
        (*p)->Draw(m_nX, m_nY,pTextFont,clAnswer, clAnswerHighlight);
        p++;
    }
}


void CStatement::OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{
     if (m_blMouseWasUp)
     {
        bool blFound = false;
        vector<CAnswer *>::const_iterator p = m_vAnswer.begin();
        while ((p!= m_vAnswer.end()) && (!blFound))
        {
            if ((*p)->MouseOver(m_nX, m_nY,a_MouseEvent.x,a_MouseEvent.y))
            {
                if (m_pParent != NULL)
                {
                    m_blMouseWasUp = false;
                    if ((*p)->m_nTo >= 0)
                    {
                        size_t sTo = (size_t)(*p)->m_nTo;
                        m_pParent->SelectStatement(sTo);
                    }
                    else
                    {
                        m_pParent->SetResult((*p)->m_nValue);
                        m_pParent->EndDialog();
                    }
                    blFound = true;
                }
            }
            p++;
        }
     }
}


void CStatement::OnMouseButtonUp(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{
    m_blMouseWasUp = true;
}

void CStatement::OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{
    vector<CAnswer *>::const_iterator p = m_vAnswer.begin();
    while (p!= m_vAnswer.end())
    {
        if ((*p)->MouseOver(m_nX, m_nY,a_MouseEvent.x,a_MouseEvent.y))
        {
            (*p)->m_blHighLight = true;
        }
        else
        {
            (*p)->m_blHighLight = false;
        }
        p++;
    }

}

CAnswer::CAnswer(int a_nTo, int a_nValue)
{
    m_strText = "[UNKNOWN]";
    m_nTo     = a_nTo;
    m_nValue  = a_nValue;
    m_nX      = 0;
    m_nY      = 0;
    m_nHeight = 24;
    m_nWidth  = 550;
    m_blHighLight = false;
}

void CAnswer::SetText(string a_strText)
{
    m_strText = a_strText;
}

void CAnswer::SetText(const char * a_szText)
{
    m_strText = a_szText;
    //ToDo: Set width according to text
}


void CAnswer::Draw(int a_nOffsetX, int a_noffsetY, ALLEGRO_FONT * a_pFont, ALLEGRO_COLOR a_AnswerColor, ALLEGRO_COLOR a_AnswerHighLight)
{
    int nLeft = m_nX + a_nOffsetX;
    int nTop  = m_nY + a_noffsetY;
    int nBottom = nTop + m_nHeight;

    int w = al_get_text_width(a_pFont,m_strText.c_str());

    if (m_blHighLight)
    {
        al_draw_rectangle(nLeft,nTop,nLeft+w+10,nBottom,a_AnswerHighLight,2);
        al_draw_text(a_pFont,a_AnswerHighLight,nLeft+2, nTop+1,0,m_strText.c_str());
    }
    else
    {
        al_draw_rectangle(nLeft,nTop,nLeft+w+10,nBottom,a_AnswerColor,2);
        al_draw_text(a_pFont,a_AnswerColor,nLeft+2, nTop+1,0,m_strText.c_str());
    }

}


bool CAnswer::MouseOver(int a_nOffsetX, int a_noffsetY,int a_nMouseX, int a_nMouseY)
{
    int nLeft = m_nX + a_nOffsetX;
    int nTop  = m_nY + a_noffsetY;
    int nRight = nLeft + m_nWidth;
    int nBottom = nTop + m_nHeight;

    if ((a_nMouseX > nLeft) &&(a_nMouseX < nRight) && (a_nMouseY > nTop) && (a_nMouseY < nBottom))
    {
        return true;
    }
    else
    {
        return false;
    }

}
