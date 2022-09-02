#include "Mission.h"



CMission::CMission()
{


}


CMission::~CMission()
{

}



bool CMission::Save(std::ofstream & a_SaveStream)
{


    #ifdef _DEBUG
    std::cout << "Mission Saved ! \n"<< std::flush;
    #endif // _DEBUG

}

bool CMission::Load(std::ifstream & a_LoadStream)
{


    #ifdef _DEBUG
    std::cout << "Mission Loaded ! \n"<< std::flush;
    #endif // _DEBUG
}
