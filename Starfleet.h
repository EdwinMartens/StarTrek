#ifndef _STAR_FLEET
#define _STAR_FLEET

#include "Universe.h"
#include "DialogParser.h"
#include "Dialog.h"


enum GENERICMESSAGE
{
    GAMESTART = 0,
    CONGRATULATIONS,
    FORBIDDEN,
    GENERICMESSAGE_END
};



class Starfleet
{
public:

static bool Init();
Starfleet();

void sendMessage(GENERICMESSAGE message);


protected:
void StartDialog();


private:

CDialog * m_pDialog;

};



#endif // _STAR_FLEET
