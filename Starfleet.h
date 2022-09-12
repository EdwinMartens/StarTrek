#ifndef _STAR_FLEET
#define _STAR_FLEET

#include "Universe.h"
#include "DialogParser.h"
#include "Dialog.h"
#include <vector>

enum GENERICMESSAGE
{
    GAMESTART = 0,
    CONGRATULATIONS,
    FORBIDDEN,
    GENERICMESSAGE_END
};


struct missionEvent
{
    bool m_blFailMission;
    //std::string m_strObject;
    int m_nSectorX;
    int m_nSectorY;
};


class Starfleet
{
public:

static bool Init();
static void SetGameoverFunc(void (* a_pGameOverFunc)());
Starfleet();
void sendMessage(GENERICMESSAGE message);
void PostEvent(missionEvent event);
void CheckMission();

protected:
void StartDialog();


private:
CDialog * m_pDialog;
std::vector<missionEvent> m_lstEvents;


};



#endif // _STAR_FLEET
