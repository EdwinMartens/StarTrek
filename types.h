

/*
   ___________________          _-_
   \==============_=_/ ____.---'---`---.____
               \_ \    \----._________.----/
                 \ \   /  /    `-_-'
             __,--`.`-'..'-_
            /____          ||
                 `--.____,-'


      Credits go to Paramount pictures for the star - trek concept and universe.


      Copyright (C) 2015 - 2020  E.J.M. Martens

      This program is free software; you can redistribute it and/or
      modify it under the terms of the GNU General Public License
      as published by the Free Software Foundation; either version 2
      of the License, or (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program; if not, write to the Free Software
      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <string>
#include <vector>

#ifndef _TYPES
#define _TYPES

#define TSK_STANDARD     0
#define TSK_CONTACT      1
#define TSK_STRIKE       2


#define DOCK_HEIGHT 18

#define GO_PLAYING 0
#define GO_LOOSE_CRITICAL 1
#define GO_ENTERPRISE     2
#define GO_TIME           3
#define GO_WIN_REACHED   10


#define RESERVED_SAMPLES     8
#define CLOAK_DELAY        900
#define MAX_ENERGY       10000
#define MAX_TORPEDOES       25
#define MAX_PROBES          10
#define CORE_CRITICAL       25

#define ENTERPRISE_ENERGY_DELAY      15
#define ENTERPRISE_PHASERLOAD_DELAY  40
#define ENTERPRISE_SHIELDLOAD_DELAY  40
#define ENTERPRISE_REPAIR_DELAY      80
#define ENTERPRISE_MIN_WARP_ENERGY 2000
#define ENTERPRISE_MIN_PHASER_DIST  500
#define ENTERPRISE_TRAND_DIST       300

#define ENTERPRISE_CRITICAL_WARP_HLT   40




#define BREACH_WARP_HLT     20

#define MESSAGE_READ_TIME 3

#define MAX_CREW_FEDERATION  500
#define MAX_CREW_ROMULAN     200
#define MAX_CREW_KLINGON_BC  400
#define MAX_CREW_KLINGON_BOP  50
#define MAX_STARBASE_HEALTH  200

#define ENTERPRISE_PHASER_POWER    300
#define ROMULAN_BOP_PHASER_POWER   200

#define PLASMA_DAMAGE     40
#define PHOTON_DAMAGE     30
#define DISRUPTOR_DAMAGE  10


#define DEGTORAD(x) ((x)*(ALLEGRO_PI/180.0))
#define RADTODEG(x) ((x)/(ALLEGRO_PI*180.0))

#define UNIVERSE_SIZE 200
#define SECTORSIZE 12000

#define KLINGON_EMPIRE_CENTER_X  (UNIVERSE_SIZE/2)+8
#define KLINGON_EMPIRE_CENTER_Y  (UNIVERSE_SIZE/2)+8
#define ROMULAN_EMPIRE_CENTER_X  (UNIVERSE_SIZE/2)+11
#define ROMULAN_EMPIRE_CENTER_Y  (UNIVERSE_SIZE/2)-11



enum MISSION_CRITICAL
{
    MC_NONE,
    MC_SURVIVE,
    MC_DESTROY,
    MC_REACH
};



enum GAME_STATE
{
    GS_EXIT = 0,
    GS_MENU,
    GS_LOAD,
    GS_SAVE,
    GS_GAME,
    GS_GAMEOVER,
    GS_TITLE,
    GS_CREDITS,
    GS_INTRO,
    GS_KEYMAP,
    GS_END
};


enum SCREEN_MODE
{
    MODE_MAINSCREEN,
    MODE_ENGINEERING,
    MODE_NAVIGATION,
    MODE_COMMUNICATION
};


enum CLOAK_STATE
{
    CS_UNCLOAKED,
    CS_CLOAKING,
    CS_CLOAKED,
    CS_DECLOAKING
};

enum DIALOG_RESULT
{
    DR_NONE,    // No result
    DR_WAR,     // dialog outcome results in war
    DR_PEACE,   // dialog outcome results in peace
    DR_LEAVE,   // dialog outcome results in player having to leave the sector
    DR_MISSION  // dialog outcome will start a mission
};

enum PARTICLE_EFFECT
{
    EFF_NORMAL,
    EFF_PROBE
};

enum MEMBER
{
    MEM_NONE =    0,
    MEM_NEUTRAL,
    MEM_FEDERATION,
    MEM_KLINGON,
    MEM_FERENGI,
    MEM_ROMULAN,
    MEM_NEUTRAL_ZONE,
    MEM_OTHER,
    MEM_ENEMY,
    MEM_END
};

enum NAVIGATION_MODE
{
    NAV_NONE      = 0,
    NAV_TOPO,
    NAV_TACTICAL,
    NAV_OCCUPATION,
    NAV_END
};

enum HEALTH
{
    HLT_UNDEFINED=-1,
    HLT_HULL,
    HLT_WARPCORE,
    HLT_IMPULSE,
    HLT_THRUSTER,
    HLT_COMPUTER,
    HLT_SENSOR,
    HLT_SHIELD,
    HLT_PHASER,
    HLT_PHOTON,
    HLT_CLOAK,
};

#define HLT_TRANS HLT_CLOAK

enum ID
{
    ID_NONE = 0,

    ID_ANIMATION,

    ID_PLANET_BOTTOM,
    ID_CLASS_A,
	ID_CLASS_B,
	ID_CLASS_L,
	ID_CLASS_N,
	ID_CLASS_M1,
    ID_CLASS_M2,
    ID_CLASS_M3,
    ID_PLANET_TOP,

    ID_STAR_BOTTOM,
    ID_STAR_BLUE,
	ID_STAR_WHITE,
	ID_STAR_YELLOW,
	ID_STAR_RED,
	ID_BLACK_HOLE,
	ID_STAR_TOP,

	ID_PROBE,
    ID_ASTEROID,

    ID_BULLET_BOTTOM,
    ID_DISRUPTOR,
    ID_PHOTON,
    ID_PROTON,
    ID_PLASMA,
    ID_BULLET_TOP,


    ID_SHIP_BOTTOM,
    ID_GALAXYCLASS,
    ID_PLAYER,
    ID_KLINGONBC,
    ID_KLINGONBOP,
    ID_ROMULANBOP,
    ID_SHIP_TOP,

    ID_BASE_BOTTOM,
    ID_FEDERATIONBASE,
    ID_KLINGON_BASE,
    ID_ROMULAN_BASE,
    ID_BASE_TOP,

    ID_SPECIAL_BOTTOM,
    ID_RELIANT,
    ID_SPECIAL_TOP,
};


enum AI
{
    AI_NONE,
    AI_ASTER,
    AI_CHASE,
    AI_EVADE,
    AI_MOVE,
    AI_WANDER,
    AI_DOCK,
    AI_RELEASE,
    AI_CONVERSE,
    AI_ESCORT,
    AI_HIDE,
    AI_MAX
};


struct TScanInfo
{
    bool m_Valid;
    std::vector<std::string> m_vLines;
    std::string m_vPathToImage;
};


const char  SIGNATURE[] = "File Version 0001A"; // for Load / Save



#endif
