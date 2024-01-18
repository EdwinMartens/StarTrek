#ifndef _MISSION
#define _MISSION

#include "types.h"
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <vector>


enum MISSIONSTATE
{
    MS_CONTINUE,
    MS_SUCCESS,
    MS_FAIL
};


enum MISSION_TYPE
{
    MT_NONE,
    MT_PATROL,
    MT_DESTROY,
    MT_RESCUE,
    MT_DELIVER,
    MT_CREW_COMPLETE
};




class CMission
{
public:

 CMission();
 CMission(MISSION_TYPE a_Type, int a_nPoints,const char * a_szDescription, const char * a_szSuccess, const char * a_szFail);
 ~CMission();


bool Save(std::ofstream & a_SaveStream);
bool Load(std::ifstream & a_LoadStream);

MISSIONSTATE check_Mission(MissionEvent event);
MISSIONSTATE GetMissionState();
std::string getSuccessMessage();



protected:

private:

    int m_nID;
    int m_nPoints;
    MISSION_TYPE m_Type;
    MISSIONSTATE m_State;
    std::string m_strDescription;
    std::string m_strSuccess;
    std::string m_strFail;
    static int GetID();
};


#endif // _MISSION

