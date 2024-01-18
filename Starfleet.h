#ifndef _STAR_FLEET
#define _STAR_FLEET

#include "types.h"
#include "Universe.h"
#include "DialogParser.h"
#include "Dialog.h"
#include "Mission.h"
#include <vector>


class CMission;
class Starfleet
{
public:

static bool Init();
static void SetGameoverFunc(void (* a_pGameOverFunc)(MissionEvent event));
Starfleet();
void sendMessage(GENERICMESSAGE message);
void PostEvent(MissionEvent event);
void CheckMission();
void NewMission(CMission * a_pMission);

protected:
void StartDialog();


private:
CDialog * m_pDialog;
std::vector<MissionEvent> m_lstEvents;
CMission * m_pCurrentMission;


};



#endif // _STAR_FLEET
