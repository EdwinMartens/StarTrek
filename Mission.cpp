#include "Mission.h"
#include "Enterprise.h"


extern TEnterprise * g_pEnterprise;


CMission::CMission()
{
    m_nID =GetID();
    m_Type = MT_NONE;
    m_State = MS_CONTINUE;
    m_strDescription="<UNDEFINED>";
    m_nPoints = 0;
}


CMission::CMission(MISSION_TYPE a_Type, int a_nPoints,const char * a_szDescription, const char * a_szSuccess, const char * a_szFail)
{
    m_nID =GetID();
    m_Type = a_Type;
    m_State = MS_CONTINUE;
    m_strDescription = a_szDescription;
    m_strSuccess = a_szSuccess;
    m_strFail = a_szFail;
    m_nPoints = a_nPoints;
}


CMission::~CMission()
{

}


int CMission::GetID()
{
    static int NextBase =0;
    return NextBase++;
}


bool CMission::Save(std::ofstream & a_SaveStream)
{
    #ifdef _DEBUG
    std::cout << "Mission Saved ! \n"<< std::flush;
    #endif // _DEBUG
    return true;
}

bool CMission::Load(std::ifstream & a_LoadStream)
{


    #ifdef _DEBUG
    std::cout << "Mission Loaded ! \n"<< std::flush;
    #endif // _DEBUG
    return true;
}


std::string CMission::getSuccessMessage()
{
    return m_strSuccess;
}


MISSIONSTATE CMission::check_Mission(MissionEvent event)
{
    switch(event.m_Type)
    {
        case ET_FAILED:
            m_State = MS_FAIL;
        break;

        case ET_LOCATION_REACHED:
        break;

        case ET_OBJECT_ABOARD:
        if (m_Type == MT_CREW_COMPLETE)
        {
            if (g_pEnterprise->CrewIsComplete())
            {
                m_State = MS_SUCCESS;
            }
        }
        break;

        case ET_OBJECT_DELIVERED:
        break;

        case ET_TARGET_DESTROYED:
        break;

    }

    return m_State;
}

MISSIONSTATE CMission::GetMissionState()
{
    return m_State;
}
