#include "Communication.h"

#include <stdio.h>
#include <iostream>
#include "utils.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "Enterprise.h"

extern ofstream logfile;
extern TEnterprise * g_pEnterprise;


static std::vector<ALLEGRO_BITMAP *> g_lstAvatar;

Message::Message(int a_nID, string a_strMessage, crewmember a_Crew)
{
    m_nID = a_nID;
    m_strMessage = a_strMessage;
    m_crew = a_Crew;
    m_pImage = g_lstAvatar[m_crew];
}

Message::~Message()
{


}

void Message::Draw(int a_nX, int a_nY)
{
    al_draw_filled_rectangle(a_nX, a_nY, a_nX + 800, a_nY + 60,m_pOwner->m_Color);
    al_draw_bitmap(m_pImage, a_nX+4, a_nY + 4, 0);
    al_draw_text(FontManager::GetFont(FONT::TEXT),al_map_rgb(255,255,255),a_nX + 80, a_nY + 10, 0, m_strMessage.c_str());
    al_draw_rectangle(a_nX, a_nY, a_nX + 800, a_nY + 60, al_map_rgb(255,255,255), 2);
}

bool Message::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Avatar");

    for (int i=0;i<7;i++)
	{
		char szFileName[255];
		sprintf(szFileName,"av%03d.png",i);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstAvatar.push_back(pBitmap);
	    }
	    else
	    {
            al_destroy_path(pPath);
            return false;
	    }
	}

	al_destroy_path(pPath);
	return true;
}


Communication::Communication(int a_nX, int a_nY)
{
    Log("Create communications ***");

    m_Color = al_map_rgba(50, 50,255, 40);
    m_nX = a_nX;
    m_nY = a_nY;
    m_nCounter = 60 * MESSAGE_READ_TIME;
    m_nPos = 80;
}

Communication::~Communication()
{
    Clear();
}

void Communication::Clear()
{
    if (!messagequeue.empty())
    {
        for (size_t i=0; i < messagequeue.size(); i++)
        {
            Message * pMessage = messagequeue.front();
            delete pMessage;
            messagequeue.pop_front();
        }
    }
}



void Communication::AddMessage(int a_nID, crewmember a_CrewMember,const char * a_szMessage)
{
    if ((g_pEnterprise != NULL) && (g_pEnterprise->HasCrewMember(a_CrewMember)))
    {
        list<Message *>::const_iterator p = messagequeue.begin();
        bool blFound = false;

        while ((!blFound) && (p != messagequeue.end()))
        {
            if ((*p)->m_nID == a_nID )
            {
                blFound = true;
            }
            p++;
        }

        if (!blFound)
        {
            if (messagequeue.empty())
            {
                m_nPos = 80;
                m_nCounter = 60 * MESSAGE_READ_TIME;
                SoundManager::PlaySound(SOUND::INTERCOM,1.0);
            }

            Message * pMessage = new Message(a_nID,a_szMessage, a_CrewMember);
            pMessage->m_pOwner = this;
            messagequeue.push_back(pMessage);
        }
    }
}

void Communication::Draw()
{
    if ((!messagequeue.empty()) && ( messagequeue.front()!= NULL))
    {
       messagequeue.front()->Draw(m_nX, m_nY - m_nPos );
    }
}

bool Communication::Init()
{
     return Message::Init();
}

void Communication::Next()
{
    if ((!messagequeue.empty()) && (m_nCounter <= 0))
    {

        Message * pMessage = messagequeue.front();
        delete pMessage;
        messagequeue.pop_front();
        m_nCounter = 60 * MESSAGE_READ_TIME;
        if (!messagequeue.empty())
        {
            m_nPos = 70;
            SoundManager::PlaySound(SOUND::INTERCOM,1.0);
        }
    }
    else
    {
        m_nCounter--;
    }

    if (m_nPos > 0)
    {
        m_nPos--;
    }
}





