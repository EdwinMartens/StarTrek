#include "SaveSlots.h"
#include "Engine.h"
#include "WindowDialog.h"
#include <allegro5/allegro_primitives.h>


extern CWindowDialog * g_pWindowDialog;

Slot::Slot(SlotManager * a_pParent)
{
    m_pParent = a_pParent;
    m_blActive = false;
    m_blNewSave = false;
    m_blHighLight = false;
    m_nHeight = 40;
    m_nWidth  = 600;
    m_strCaption = "--------------------------------";
}

Slot::Slot(int a_nX, int a_nY,const string a_strCaption, SlotManager * a_pParent)
{
    m_nX = a_nX;
    m_nY = a_nY;
    m_pParent = a_pParent;
    m_blActive = false;
    m_blNewSave = false;
    m_blHighLight = false;
    m_nHeight = 40;
    m_nWidth  = 600;
    m_strCaption = a_strCaption;
}


Slot::~Slot()
{

}

void Slot::Draw(ALLEGRO_FONT * a_pFont)
{
     if (m_blHighLight)
    {
       al_draw_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorHighlight,2);

    }
    else
    {
        al_draw_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                    m_pParent->m_ColorOuter,2);


    }

     al_draw_filled_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorInner);

   if (a_pFont != NULL)
   {
      al_draw_text(a_pFont,
                   m_pParent->m_ColorOuter,
                   m_nX + (m_nWidth/2), m_nY + (m_nHeight/4),
                   ALLEGRO_ALIGN_CENTRE,
                   m_strCaption.c_str());

      if ((!m_pParent->m_blLoading) && (m_blActive) && (m_blNewSave))
      {
          int nWidth = al_get_text_width(a_pFont, m_strCaption.c_str())/2;
          al_draw_line(m_nX + (m_nWidth/2)+ nWidth, m_nY + (m_nHeight/4)+20,m_nX + (m_nWidth/2)+nWidth+10, m_nY + (m_nHeight/4)+20,m_pParent->m_ColorOuter,2);
      }

   }
}


void Slot::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    if ((mouse_event.x > m_nX) &&
        (mouse_event.x < m_nX + m_nWidth) &&
        (mouse_event.y > m_nY) &&
        (mouse_event.y < m_nY + m_nHeight))
    {
        m_blHighLight = true;
        if (mouse_event.button == 1)
        {
            m_pParent->DeactivateSlots();


            if ((m_strCaption == "NEW SAVE...")||(m_pParent->m_blLoading))
            {
                m_blActive = true;
                m_pParent->m_pActiveSlot = this;
                m_pParent->m_pPendingActive = NULL;
                if (m_strCaption == "NEW SAVE...")
                {
                    m_strCaption="";
                }

            }
            else
            {
                m_pParent->m_pPendingActive = this;
                m_pParent->m_pWindowDialog = new CWindowDialog(m_nX+(m_nWidth/2),m_nY + m_nHeight,m_pParent,"Overwrite this game ?",BTN_YESNO);
            }
        }
    }
    else
    {
        m_blHighLight = false;
    }
}

void Slot::ProcessKeyEvent(int a_nUniChar)
{

    switch (a_nUniChar)
    {
        case 8:
        if (!m_strCaption.empty())
        {
           m_strCaption.erase(m_strCaption.end() -1);
        }

        break;

        case 127:
        if (!m_strCaption.empty())
        {
           m_strCaption.erase(m_strCaption.end() -1);
        }
        break;


        case 13:
            m_pParent->m_pCallBack(m_strCaption);
        break;


    default: // add Numbers !!
    if ((a_nUniChar>47)&&(a_nUniChar<123)&&(m_strCaption.size()<60))
        {
            m_strCaption+= char(a_nUniChar);
        }

    }
}

string Slot::GetCaption()
{
    return m_strCaption;
}


SlotManager::SlotManager(bool a_blLoading,  ALLEGRO_PATH * a_pSearchpath,void (* a_pCallBack)(string))

{
    m_ColorInner     = al_map_rgb(0,0,0);
    m_ColorOuter     = al_map_rgb(100,100,255);
    m_ColorHighlight = al_map_rgb(0,0,255);
    m_pActiveSlot    = NULL;
    m_pPendingActive = NULL;
    m_blLoading      = a_blLoading;
    m_pCallBack      = a_pCallBack;
    m_nIndex         = 1;

    if (a_pSearchpath!=NULL)
    {
        ALLEGRO_FS_ENTRY * pDir = al_create_fs_entry(al_path_cstr(a_pSearchpath,ALLEGRO_NATIVE_PATH_SEP));
        if (al_open_directory(pDir))
        {
            ALLEGRO_FS_ENTRY * pFile;
            while ((pFile = al_read_directory(pDir)))
            {
                ALLEGRO_PATH * pPath = al_create_path(al_get_fs_entry_name(pFile));

                AddSlot(al_get_path_basename(pPath));

                al_destroy_path(pPath);
                al_destroy_fs_entry(pFile);
            }
        }
        al_destroy_fs_entry(pDir);
    }

    if (!a_blLoading)
    {
        AddSlot("NEW SAVE...");
    }
}

SlotManager::~SlotManager()
{
     if (!m_vSlots.empty())
     {
        m_vSlots.clear();;
     }
     delete m_pWindowDialog;
}


void SlotManager::DeactivateSlots()
{
    for (size_t i=0; i< m_vSlots.size(); i++ )
    {
       m_vSlots[i].m_blActive=false;
    }
    m_pActiveSlot = NULL;
}

void SlotManager::ProcessKeyEvent(int a_nUniChar)
{
    if ((m_pActiveSlot!=NULL) && (m_pActiveSlot->m_blNewSave))
    {
        m_pActiveSlot->ProcessKeyEvent(a_nUniChar);
    }

}

void SlotManager::AddSlot(string a_strCaption)
{
    Slot s(100,20+(48*m_nIndex),a_strCaption,this);
    if (a_strCaption == "NEW SAVE...")
    {
        s.m_blNewSave = true;
    }

    m_vSlots.push_back(s);
    m_nIndex++;
}

void SlotManager::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    if (m_pWindowDialog != NULL)
    {
        m_pWindowDialog->ProcessMouseEvent(mouse_event);
    }
    else
    {
        for (size_t i=0; i< m_vSlots.size(); i++ )
        {
            m_vSlots[i].ProcessMouseEvent(mouse_event);
        }

        if ((m_blLoading)&&(m_pActiveSlot!=NULL)&&(m_pCallBack!=NULL))
        {
            m_pCallBack(m_pActiveSlot->GetCaption());
        }
    }
}

void SlotManager::Draw(ALLEGRO_FONT * a_pFont)
{

    for (size_t i=0; i< m_vSlots.size(); i++ )
    {
       m_vSlots[i].Draw(a_pFont);
    }

    if (m_pWindowDialog != NULL)
    {
        m_pWindowDialog->Draw();
    }

}


void SlotManager::DeleteSlot(int a_nIndex)
{
    m_vSlots.erase(m_vSlots.begin()+a_nIndex);
}

int SlotManager::Count()
{
    return m_nIndex -1;
}



void SlotManager::CloseDialog()
{
    switch (m_pWindowDialog->GetResult())
    {
       case WDR_YES:
           if (m_pPendingActive != NULL)
           {
               DeactivateSlots();
               m_pActiveSlot = m_pPendingActive;
               m_pActiveSlot->m_blActive = true;
               m_pPendingActive = NULL;
           }

           delete m_pWindowDialog;
           m_pWindowDialog = NULL;

           m_pCallBack(m_pActiveSlot->GetCaption());
       break;

       case WDR_NO:
           delete m_pWindowDialog;
           m_pWindowDialog = NULL;
           if (m_pActiveSlot != NULL)
           {
               m_pActiveSlot->m_blActive = false;
               m_pActiveSlot = NULL;
           }
       break;

       default:
           delete m_pWindowDialog;
           m_pWindowDialog = NULL;
       break;
    }


}



