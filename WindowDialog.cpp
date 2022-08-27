#include "WindowDialog.h"
#include "FontManager.h"
#include <allegro5/allegro_primitives.h>


extern int g_nScreenWidth;
extern int g_nScreenHeight;



void HandleButtonOK(CWindowDialog * a_pDialog);
void HandleButtonNO(CWindowDialog * a_pDialog);
void HandleButtonYES(CWindowDialog * a_pDialog);
void HandleButtonCANCEL(CWindowDialog * a_pDialog);


CWindowDialog::CWindowDialog(int a_nX, int a_nY,SlotManager * a_pParent,std::string a_strMessage, DialogButtonOptions a_Options)
{
    m_strMessage = a_strMessage;

    m_pButtonYes    = NULL;
    m_pButtonNo     = NULL;
    m_pButtonCancel = NULL;

    m_pParent = a_pParent;

    m_ColorInner     = al_map_rgb(0,0,255);
    m_ColorOuter     = al_map_rgb(255,255,255);
    m_ColorHighlight = al_map_rgb(100,100,255);

    m_dialogResult = WDR_NONE;

    m_nX = a_nX;
    m_nY = a_nY;

    m_nWidth = 200;
    m_nHeight = 150;

    m_pFont = FontManager::GetFont(FONT::TEXT);


    int nTextWidth = al_get_text_width(m_pFont, m_strMessage.c_str());
    if (nTextWidth > m_nWidth)
    {
        m_nWidth = nTextWidth;
    }



    switch(a_Options)
    {
         case BTN_OK:
             m_pButtonYes = new CButton((m_nWidth/2)+10, m_nHeight/3 *2,"OK",this,HandleButtonOK);
         break;

         case BTN_YESNO:
             m_pButtonYes = new CButton((m_nWidth/3)-10,m_nHeight/3 *2,"Yes",this,HandleButtonYES);
             m_pButtonNo = new CButton((m_nWidth/3)*2, m_nHeight/3 *2 ,"No",this,HandleButtonNO);
         break;

         case BTN_YESNOCANCEL:
         break;
    }

}


void CWindowDialog::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event)
{

if (m_pButtonYes != NULL)
   {
       m_pButtonYes->ProcessMouseEvent(mouse_event, m_nX, m_nY);
   }


   if (m_pButtonNo != NULL)
   {
       m_pButtonNo->ProcessMouseEvent(mouse_event, m_nX, m_nY);
   }


   if (m_pButtonCancel != NULL)
   {
       m_pButtonCancel->ProcessMouseEvent(mouse_event, m_nX, m_nY);
   }


}

void CWindowDialog::Draw()
{


    al_draw_filled_rounded_rectangle((float)m_nX,
                                    (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_ColorInner);


    al_draw_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_ColorOuter,2);


   if (m_pFont != NULL)
   {
      al_draw_text(m_pFont,
                   m_ColorOuter,
                   m_nX + (m_nWidth/2), m_nY + (m_nHeight/4),
                   ALLEGRO_ALIGN_CENTRE,
                   m_strMessage.c_str());

   }

   if (m_pButtonYes != NULL)
   {
       m_pButtonYes->Draw(m_nX, m_nY);
   }


   if (m_pButtonNo != NULL)
   {
       m_pButtonNo->Draw(m_nX, m_nY);
   }


   if (m_pButtonCancel != NULL)
   {
       m_pButtonCancel->Draw(m_nX, m_nY);
   }

}


void CWindowDialog::SetResult(WDialogResult a_Result)
{
    m_dialogResult = a_Result;
    m_pParent->CloseDialog();
}

WDialogResult CWindowDialog::GetResult()
{
    return m_dialogResult;
}


void HandleButtonOK(CWindowDialog * a_pDialog)
{
    a_pDialog->SetResult(WDR_OK);
}


void HandleButtonNO(CWindowDialog * a_pDialog)
{
    a_pDialog->SetResult(WDR_NO);
}

void HandleButtonYES(CWindowDialog * a_pDialog)
{
    a_pDialog->SetResult(WDR_YES);
}

void HandleButtonCANCEL(CWindowDialog * a_pDialog)
{
    a_pDialog->SetResult(WDR_CANCEL);
}
