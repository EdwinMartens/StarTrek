
#include <iostream>
#include "StarTrekV3.h"
#include "Enterprise.h"
#include "Communication.h"
#include "Starfleet.h"
#include "utils.h"


extern Communication * g_pCommunication;
extern TEnterprise   * g_pEnterprise;
extern Universe      * g_pUniverse;


ALLEGRO_PATH   * pStarfleetConversationPath = NULL;
ALLEGRO_FS_ENTRY * pStarfleetDialogs = NULL;

void (* g_pGameOverFunc)() = NULL;


Starfleet::Starfleet()
{
    m_pDialog = NULL;
    m_lstEvents.clear();
}


void Starfleet::sendMessage(GENERICMESSAGE message)
{
    g_pCommunication->AddMessage(0,CREW_UHURA,"Message from starfleet sir !");

    if (g_pEnterprise!= NULL)
    {
        if (m_pDialog == NULL)
        {
            m_pDialog = g_pEnterprise->GetDialog();

            if (m_pDialog->IsEmpty())
            {
                switch(message)
                {
                    case GAMESTART:
                    al_set_path_filename(pStarfleetConversationPath,"gamestart.xml");
                    break;

                    case CONGRATULATIONS:
                    break;

                    case FORBIDDEN:
                    break;

                    default:
                    break;
                }
                StartDialog();
            }
        }
    }
 }


void Starfleet::StartDialog()
 {
     ifstream xmlStream;
     xmlStream.open(al_path_cstr(pStarfleetConversationPath,ALLEGRO_NATIVE_PATH_SEP));
     if (xmlStream.is_open())
     {
        CDialogParser * pParser = CDialogParser::GetInstance();
        pParser->SetOutput(m_pDialog);
        pParser->Read(xmlStream);
        xmlStream.close();
        Log("Dialog opened");
     }
     else
     {
        Log("Could not open dialog file");
        m_pDialog = NULL;
     }
 }


void Starfleet::PostEvent(missionEvent event)
{
     m_lstEvents.push_back(event);
}


void Starfleet::CheckMission()
{
     bool blMissionFail = false;
     bool blGameOver = true;
     if (!m_lstEvents.empty())
     {
        for (std::size_t i = 0; i < m_lstEvents.size(); i++)
        {
            missionEvent event = m_lstEvents.at(i);

            if (event.m_blFailMission)
            {


                blMissionFail = true;
            }
         else
            {

            }
        }
     }

     if (blMissionFail)
     {
         m_lstEvents.clear();
         if (g_pGameOverFunc != NULL)
         {
             g_pGameOverFunc();
         }
     }
}


void Starfleet::SetGameoverFunc(void (* a_pGameOverFunc)())
{
     g_pGameOverFunc= a_pGameOverFunc;
}


bool Starfleet::Init()
{
    pStarfleetConversationPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pStarfleetConversationPath, "conversation");
    al_append_path_component(pStarfleetConversationPath, "default");
	al_append_path_component(pStarfleetConversationPath, "starfleet");

	pStarfleetDialogs = al_create_fs_entry(al_path_cstr(pStarfleetConversationPath,ALLEGRO_NATIVE_PATH_SEP));

	if ((pStarfleetConversationPath != NULL) &&
        (pStarfleetDialogs != NULL))
    {
        return true;
    }
    else
    {
        return false;
    }
}



