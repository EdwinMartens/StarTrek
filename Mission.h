#ifndef _MISSION
#define _MISSION

#include <stdio.h>
#include <iostream>
#include <ostream>

class CMission
{
public:

 CMission();
 ~CMission();


bool Save(std::ofstream & a_SaveStream);
bool Load(std::ifstream & a_LoadStream);


protected:

private:

    int m_nID;
    std::string m_strName;
    int m_nTargetSectorX;
    int m_nTargetSectorY;
    int m_nTargetPositionX;
    int m_nTargetPositionY;


};


#endif // _MISSION

