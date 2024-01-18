
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

void (* g_pGameOverFunc)(MissionEvent event) = NULL;


Starfleet::Starfleet()
{
    m_pDialog = NULL;
    m_lstEvents.clear();
    m_pCurrentMission = NULL;
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


void Starfleet::PostEvent(MissionEvent event)
{
     m_lstEvents.push_back(event);
}


void Starfleet::CheckMission()
{
     bool blGameOver = false;
     MissionEvent GameOverEvent;
     if (!m_lstEvents.empty())
     {
        for (std::size_t i = 0; i < m_lstEvents.size(); i++)
        {
            MissionEvent event = m_lstEvents.at(i);

            switch(event.m_Type)
            {
                case ET_GAME_OVER:
                    GameOverEvent = event;
                    blGameOver = true;
                break;
            }

            if ((m_pCurrentMission != NULL) && ((m_pCurrentMission->GetMissionState() == MS_CONTINUE)))
            {
                MISSIONSTATE state = m_pCurrentMission->check_Mission(event);

                switch (state)
                {
                    case MS_CONTINUE:

                    break;

                    case MS_SUCCESS:
                        g_pCommunication->AddMessage(0,CREW_KIRK,m_pCurrentMission->getSuccessMessage().c_str());

                    break;

                    case MS_FAIL:

                    break;
                }

            }
        }
     }

     if (blGameOver)
     {
         m_lstEvents.clear();
         if (g_pGameOverFunc != NULL)
         {
             g_pGameOverFunc(GameOverEvent);
         }
     }
}


void Starfleet::NewMission(CMission * a_pMission)
{
    if (m_pCurrentMission != NULL)
    {
        delete m_pCurrentMission;
    }
    m_pCurrentMission = a_pMission;
}

void Starfleet::SetGameoverFunc(void (* a_pGameOverFunc)(MissionEvent event))
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



