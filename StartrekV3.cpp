//      Copyright (C) 2015  E.J.M. Martens
//
//      This program is free software; you can redistribute it and/or
//      modify it under the terms of the GNU General Public License
//      as published by the Free Software Foundation; either version 2
//      of the License, or (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


/**  \mainpage Star - Trek   V3
*    \author    Edwin (E.J.M.) Martens 2015 - 2016
*    \section  INTRODUCTION
* @verbatim


 \++================================|                    _=_
  \_________________________________/              ___/==+++==\___
               """\__      \"""       |======================================/
                     \__    \_          / ..  . _/--===+_____+===--""
                        \__   \       _/.  .. _/         `+'
                           \__ \   __/_______/                      \ /
                          ___-\_\-'---==+____|                  ---==O=-
                    __--+" .    . .        "==_                     / \
                    /  |. .  ..     -------- | \
                    "==+_    .   .  -------- | /
                         ""\___  . ..     __=="
                               """"--=--""


      Credits go to Paramount pictures for the star - trek concept and universe.


 @endverbatim
*
* This game is actually a very advanced version of the BASIC STAR-TREK text game.
* see https://en.wikipedia.org/wiki/Star_Trek_%28text_game%29.
* Many years ago before the WebBrowser wars there was a book SpecTrek (Walther Schoonenberg).
* It tought how to write a bit more ´graphical´ version of the STAR-TREK text game.
* It was still turn based and the ´graphics´ where reprogrammed character codes in a text screen (wich is impossible with a PC).
* The book was for the ZX Spectrum computer, hence itś name. Reading the book I ported everyting to GW-Basic and improved on it.
* A second ( real time) version, still with a text command interface, but with true sprites was written in Delphi 3.0 using Delphi X.
* The third version (The second real time version) was written in C++ using Allegro 4.x and OpenLayer, and a big 50 * 50 universe.
* This version (V3)  ( third of the real time versions) has a whopping 200 * 200 sectors universe, a sector is now what a Quadrant was
* in the text version, making it more in line with the startrek universe. It uses the great Allegro 5 library wich enables a lot of cool features.
* Ships can now Cloak, and actually use it in a smart way.
*
*
* \section  BASIC WORKINGS:
*
* At the start of a new game, a random universe is created. Size is defined by UNIVERSE_SIZE in types.h
* A sector is the actual "Playing field" you see in the main view. This is where the action happens.
* Every time the Enterprise enters a new sector, the old sector is stored in the universe, saving everything that happened in that sector.
* See Universe.h and universe.cpp. And a new "playing field" is built, reading the sector at the new sector position of the enterprise.
* A sector has a size of SECTORSIZE * SECTORSIZE (10000 * 10000) pixels. So when the Enterprise passes the borders, the Sector position changes
* and the procedure described above takes place.
* A sector is built by adding TSprite objects: SpaceObjects, ships, etc. ( all derived from TSprite in Engine.h) to the Engine.
* When an object is added to the engine, the engine OWNS that object and also takes care of destruction !
*
* All ships are derived from TShip, which has a LOT of basic ship functionality. Adding a new ship class to the game is quite easy
* The Universe will be dynamic, with rules for moving ships to other sectors, and results of "Simulated battles" and other events.
*
*
* \section THE GAME
* You are placed in the role of James Tiberius Kirk, captain of the USS Enterprise NCC-1701.
* The Enterprise is a big spaceship with a lot of systems and equipment. Everything on your ship uses energy to function.
* This energy is taken from the WARPCORE energy buffer. The Warp core generates energy by a matter-antimatter reaction producing a huge amount of energy.
* Under normal circumstances more than enough to power everything on your ship and enough to make a warp jump through multiple sectors.
* However If the WARPCORE is damaged the energy output will decrease.
* Energy is also needed to maintain a containment field around the WARPCORE. If the warpcore is damaged and there is no energy left the warpcore will fail.
* Resulting in the destruction of your ship !
* Damaged systems can be repaired at the Engineering screen. The repair crew can only fix one item at a time.
* At a federation starbase repairs can be made much quicker.
* The Navigation screen shows the Known universe, revealing more when new sectors are explored or when scanned with a probe (probes not implenemted yet)
*
* The game is far from finished yet. However you can already explore a HUGE universe
* with Klingon and Romulan empires. For testing purposes A Klingon Task force will invade your home sector at the start of a game.
* To disable this.. go to Universe.cpp and comment out:
*
*    a_Sector.m_nKlingonBC  = 2;
*    a_Sector.m_nKlingonBOP = 2;
*    a_Sector.m_nFederation1 = 3;
*
* In Universe::GenerateHomeSector(Sector & a_Sector).
*
*
* \section COMBAT
* The Enterprise has 2 weapons, Photon torpedoes and Phasers.
* PHOTON TORPEDOES torpedoes explode when they hit something resulting in massive damage over all systems.
* The damage is not dependent on the distance, there is only a maximum range.
* you only have a certain amount of torpedoes, they can be replenished at a starbase only !
* When COMPUTER and SENSORS are in good shape, they will precalculate a firing solution for a selected target.
*
* PHASERS only consume energy. Over time, the Phaserbanks are charged with energy from the warpcore buffer.
* The damage a phaser does is dependend on the distance to the target. The further away, the less damage.
* PHASERS are much more precise and can take out one single system.
*
* The Enterprise is also equipped with shields. When hit, the shield energy will drop using energy to block the incoming weapon.
* The shieldsystem will immediately begin to recharge the shield, taking energy from the WARPCORE buffer.
* If the shieldsystem is damaged,it will become less efficient in charging the shield.
* (Converting Warpcore energy into shield energy will be less efficient)
*
*
* \section CONTROLS
*
* The standard controls are:
*
*       Arrow UP     --->  Speed up
*       Arrow DOWN   --->  Slow Down
*       Arrow LEFT   --->  Turn Left
*       Arrow RIGHT  --->  Turn Right
*       T            --->  Select nearest Enemy Target
*       F            --->  Select nearest Friend
*       TAB          --->  Fire front photon tube
*       L SHIFT      --->  Fire aft photon tube
*       P            --->  Launch a probe
*       SPACE        --->  Fire Phaser
*       1..5         --->  Warp 1 tm Warp 5
*       T            --->  target nearest enemy
*       F            --->  target nearest friend
*       D            --->  Dock at starbase (when in range)
*       R            --->  Release from dock.
*       F1           --->  Main screen
*       F2           --->  Engineering
*       F3           --->  Astro Navigation
*
*       F5           --->  Navigation Exploration
*       F6           --->  Navigation Tactical mode
*       F7           --->  Navigation Political mode
*
*       F12          --->  Take a screenshot saved in screenshot folder
*
**/

/** @file StartrekV3.cpp
    Main source file, containing the game loop and setup and initialization functions.
    Also basic game rules like sector bounds checking and game state switching is implemented here.
*/



//TODO (Edwin#2#) NPC-Player communication and correct behaviour
//TODO (Edwin#3#) Spock analysis, click object to let spock tell about it


#include <stdio.h>
#include <iostream>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <map>
#include <vector>

#include "StarTrekV3.h"
#include "version.h"
//AutoVersion::MAJOR //<< version Text
#include "utils.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "DialogParser.h"
#include "WindowDialog.h"
#include "Paralax.h"
#include "Engine.h"
#include "Bullet.h"

//#include "Smoke.h"
//#include "Explosion.h"
#include "Animation.h"

#include "Universe.h"

#include "Enterprise.h"
#include "Romulan_BOP.h"
#include "Klingon_BC.h"
#include "Klingon_BOP.h"
#include "SpaceObject.h"
#include "Reliant.h"
#include "Starbase.h"
#include "Federation_Ship.h"
#include "KeyMapper.h"
#include "Communication.h"
#include "Probe.h"
#include "Menu.h"
#include "SaveSlots.h"
#include "Transporter.h"
#include "Starfleet.h"
#include "Mission.h"


//FILE  * g_pFile;
char   g_szFileName[] = "MyGame.sav"; // temporary hardcoded savefile


#ifdef _DEBUG
char   g_szSector[20];  // sector position storage for debug version
char   g_szMouse[20];
//char   g_szStates[20];
#endif // DEBUG

bool g_blGodMode = false;  // GOD cheat mode for testing the game


///General save path
ALLEGRO_PATH * g_pSavePath = NULL;

/// Path for screenshots
ALLEGRO_PATH * g_pScreenshotPath = NULL;

/// The display for graphics...
ALLEGRO_DISPLAY * g_pDisplay = NULL;

ALLEGRO_BITMAP * g_pIcon = NULL;

/// The event queue
ALLEGRO_EVENT_QUEUE *	g_pEventQueue = NULL;

// Other Globals
bool g_blQuit = false;          // if true quit program
bool g_blEscape = false;
bool g_blFullScreen = false;

int g_nUniverseCycleInterval = 6000;
int g_nUniverseCycle = g_nUniverseCycleInterval;

int g_nMissionCycleInterval = 500;
int g_nMissionCycle = g_nMissionCycleInterval;


/// game logic cycles per second
double g_nCyclesPerSecond = 50;

/// last polled system time to calculate deltatime
double g_dLastTime;

/// screenmode.. what screen should be visible [Main] [Engineering] [Navigation] etc
SCREEN_MODE g_nScreenMode = MODE_MAINSCREEN;

/// Gamestate stack [Menu] [Intro] [Game]
vector<int> g_vGameState;

// screen properties
int g_nScreenWidth;
int g_nScreenHeight;
int g_nWindowWidth;
int g_nWindowHeight;

// Global pointers
ALLEGRO_TIMER * g_pTimer =0;
/// Pointer to universe Object.
Universe      * g_pUniverse         = NULL;
/// Pointer to Keymapper.
TKeyMapper    * g_pKeyMapper        = NULL;
/// Pointer to Game Engine.
TEngine       * g_pEngine           = NULL;
/// Pointer Paralax system.
Paralax       * g_pParalax          = NULL;
/// Pointer to The enterprise ( The player !)
TEnterprise   * g_pEnterprise       = NULL;
/// Pointer to The Communication System.
Communication * g_pCommunication    = NULL;
/// Pointer to the menu system
TMenu * g_pMenu = NULL;
/// Pointer to starfleet
Starfleet * g_pStarfleet = NULL;


SlotManager * g_pSlotManager = NULL;

//Forward declarations
/// Get the desktop resolution
void get_desktop_resolution(int a_nAdapter, int *a_pWidth, int * a_pHeighth);
/// This function creates a new universe
void NewUniverse();
/// When the Enterprise enters a new sector This function builds a new playing field
/// from the sector you just entered.
void NewSector(int a_nX, int a_nY);
/// Checks if the enterprise leaves this sector
void CheckSectorBounds();
///  Invoke a keymapping functionality so you can customize the controls. still a Rough implementation
void DoKeymap();
///  invoke menu ( not implemented yet )
void DoMenu();
/// The actual game loop
void DoGame();
/// Intro screen
void DoIntro();
/// screen for loading a game
void DoLoadScreen();
/// screen for saving a game
void DoSaveScreen();
/// start a new game
void NewGame();
/// save current game
void SaveGame(string a_strSaveName);
/// setup load screen
void SetupLoadScreen();
/// setup save screen
void SetupSaveScreen();
/// quit program
void Quit();
/// return to running game
void ReturnToGame();
/// Save key mapping
void SaveMapping();
/// Load key Mapping
void LoadMapping();
/// get the date and time
const string currentDateTime();
/// gameover function
void DoGameOver();

// Menu event handlers

void NewGame()
{
     g_pEngine->Clear(false);
     // create the Enterprise
	 g_pEnterprise = new TEnterprise();
	 // add the enterprise to the game engine
	 g_pEngine->Add(g_pEnterprise);
	 Log("Enterprise created");

	 delete g_pStarfleet;
	 g_pStarfleet = new Starfleet();


	 // Create the communication system
     delete g_pCommunication;
     g_pCommunication = new Communication((g_nScreenWidth/2) - 400 ,5);
     Log("Communication created");

	 // create a new universe, this should move to a menu function in the future
	 NewUniverse();
	 Log("New universe generated");

	 // Build a new sector
	 NewSector(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);
	 Log("New Sector");

     // set enterprise position and move camera to it
     g_pEnterprise->SetPosition(g_pUniverse->m_nSectorCenter + 1100, g_pUniverse->m_nSectorCenter);
     g_pEngine->SetOrigin(g_pUniverse->m_nSectorCenter, g_pUniverse->m_nSectorCenter);

     // push gamestate GS_GAME on the gamestate stack
     g_vGameState.push_back(GS_GAME);
     Log("GAME STATE TO GAME");

     #ifdef _DEBUG
            sprintf(g_szSector,"SECTOR: %d,%d",g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);
     #endif // DEBUG
     al_hide_mouse_cursor(g_pDisplay);
     g_dLastTime = al_get_time();

	 if (g_pMenu != NULL)
     {
         g_pMenu->ClearItems();
         g_pMenu->AddMenuItem(g_nScreenWidth/2,"Return to game",ReturnToGame );
         g_pMenu->AddMenuItem(g_nScreenWidth/2,"New Game",NewGame);
         g_pMenu->AddMenuItem(g_nScreenWidth/2,"Load",SetupLoadScreen);
         g_pMenu->AddMenuItem(g_nScreenWidth/2,"Save",SetupSaveScreen);
	     g_pMenu->AddMenuItem(g_nScreenWidth/2,"Map Keys",DoKeymap);
         g_pMenu->AddMenuItem(g_nScreenWidth/2,"Quit",Quit);
     }

     if (g_pCommunication!=NULL)
    {
        g_pCommunication->Clear();
    }

    if (g_pStarfleet!= NULL)
    {
        g_pStarfleet->sendMessage(GAMESTART);
        CMission * pMission = new CMission(MT_CREW_COMPLETE,50,"Beam Spock onboard from earth","Spock is aboard, we have a full crewstaff !","You failed to get Mr Spock");
        g_pStarfleet->NewMission(pMission);
    }
}


void DoGameOver(MissionEvent event)
{
    if (g_pMenu != NULL)
    {
        g_pMenu->ClearItems();
        g_pMenu->AddMenuItem(g_nScreenWidth/2,"New Game",NewGame);
        g_pMenu->AddMenuItem(g_nScreenWidth/2,"Load",SetupLoadScreen);
        g_pMenu->AddMenuItem(g_nScreenWidth/2,"Map Keys",DoKeymap);
        g_pMenu->AddMenuItem(g_nScreenWidth/2,"Quit",Quit);
    }

    while (g_vGameState.back()!= GS_MENU)
    {
        g_vGameState.pop_back();
    }
    Log("GAME STATE TO MENU");
    al_show_mouse_cursor(g_pDisplay);

    std::string strReason = event.m_strMessage;
}


void SaveMapping()
{
    ALLEGRO_PATH * pSavePath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(pSavePath,"keys.ini");
    ofstream SaveFile(al_path_cstr(pSavePath,ALLEGRO_NATIVE_PATH_SEP),ios::out);
    if (SaveFile)
    {
       g_pKeyMapper->Save(SaveFile);
       SaveFile.close();
    }
}

void LoadMapping()
{
   ALLEGRO_PATH * pLoadPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_set_path_filename(pLoadPath,"keys.ini");
   ifstream LoadFile(al_path_cstr(pLoadPath,ALLEGRO_NATIVE_PATH_SEP),ios::in);
   if (LoadFile)
   {
      g_pKeyMapper->Load(LoadFile);
      LoadFile.close();
   }
   else
   {
      // Set default
      g_pKeyMapper->SetDefaultMapping();
   }
   al_destroy_path(pLoadPath);
}



void LoadGame(string a_strLoadName)
{
    al_stop_timer(g_pTimer);
	delete g_pSlotManager;
	g_pSlotManager = NULL;

    a_strLoadName+=".UNI";
    al_set_path_filename(g_pSavePath, a_strLoadName.c_str());
    std::ifstream LoadFile( al_path_cstr(g_pSavePath,ALLEGRO_NATIVE_PATH_SEP),
                       ios::in | ios::binary);

    if (LoadFile)
       {
          g_pEngine->Load(LoadFile);
          g_pUniverse = new Universe();
          g_pUniverse->Load(LoadFile);
          LoadFile.close();
          g_pEnterprise->SetDefaultNavigationOrigin();
          // Create the communication system
          delete g_pCommunication;
          g_pCommunication = new Communication((g_nScreenWidth/2) - 400 ,5);
          Log("Communication created");
          g_vGameState.pop_back();
          g_vGameState.push_back(GS_GAME);
          Log("GAME STATE TO GAME");
          al_hide_mouse_cursor(g_pDisplay);
          g_dLastTime = al_get_time();


          delete g_pStarfleet;
	      g_pStarfleet = new Starfleet();

#ifdef _DEBUG
          sprintf(g_szSector,"SECTOR: %d,%d",g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);
#endif // DEBUG
          string strInfo = "Game "+ a_strLoadName +" loaded...";
          g_pCommunication->AddMessage(9, CREW_KIRK,strInfo.c_str());

       }
       else
       {
          delete g_pCommunication;
          g_pCommunication = new Communication((g_nScreenWidth/2) - 400 ,5);
          Log("Communication created");
          g_pCommunication->AddMessage(9, CREW_KIRK,"There was an error loading your game !");
          g_vGameState.pop_back();
       }


    if (g_pMenu != NULL)
    {
        g_pMenu->ClearItems();
        g_pMenu->AddMenuItem(g_nScreenWidth/2, "Return to game",ReturnToGame );
        g_pMenu->AddMenuItem(g_nScreenWidth/2, "New Game",NewGame);
        g_pMenu->AddMenuItem(g_nScreenWidth/2, "Load",SetupLoadScreen);
        g_pMenu->AddMenuItem(g_nScreenWidth/2, "Save",SetupSaveScreen);
	    g_pMenu->AddMenuItem(g_nScreenWidth/2, "Map Keys",DoKeymap);
        g_pMenu->AddMenuItem(g_nScreenWidth/2, "Quit",Quit);
    }

    al_destroy_path(g_pSavePath);
    g_pSavePath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(g_pSavePath, "Save");
	al_start_timer(g_pTimer);
}


void SaveGame(string a_strSaveName)
{
    delete g_pSlotManager;
    g_pSlotManager = NULL;
    if (g_pCommunication!=NULL)
    {
        g_pCommunication->Clear();
    }
    al_stop_timer(g_pTimer);
    string strFileName = a_strSaveName;
    strFileName+=".UNI";
    al_set_path_filename(g_pSavePath, strFileName.c_str());
    std::ofstream SaveFile(al_path_cstr(g_pSavePath,ALLEGRO_NATIVE_PATH_SEP),
                      ios::out | ios::binary);
    if (SaveFile)
    {

        g_pUniverse->StoreSector(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY,g_pEngine);
        g_pEngine->Save(SaveFile);
        g_pUniverse->Save(SaveFile);
        SaveFile.close();
        g_pCommunication->AddMessage(9, CREW_KIRK,"Game saved !");
        //cerr << "OK: " << al_path_cstr(g_pSavePath,ALLEGRO_NATIVE_PATH_SEP);
    }
    else
    {
        g_pCommunication->AddMessage(9, CREW_KIRK,"There was an error saving your game !");
        //g_pCommunication->AddMessage(10, CREW_KIRK,al_path_cstr(g_pSavePath,ALLEGRO_NATIVE_PATH_SEP));
        cerr << "Error: " << al_path_cstr(g_pSavePath,ALLEGRO_NATIVE_PATH_SEP) <<"    " << strerror(errno);
    }

    al_destroy_path(g_pSavePath);
    g_pSavePath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(g_pSavePath, "Save");
	g_vGameState.pop_back();
	ReturnToGame();
}


void SetupLoadScreen()
{
    g_pSlotManager = new SlotManager(true,g_pSavePath,LoadGame);
    Log("Gamestate to GS_LOAD");
    g_vGameState.push_back(GS_LOAD);
}

void SetupSaveScreen()
{
    g_pSlotManager = new SlotManager(false,g_pSavePath,SaveGame);
    Log("Gamestate to GS_SAVE");
    g_vGameState.push_back(GS_SAVE);
}


void Quit()
{
    g_vGameState.clear();
    g_blQuit = true;
}

void ReturnToGame()
{
     g_vGameState.push_back(GS_GAME);
     Log("GAME STATE TO GAME");
     al_hide_mouse_cursor(g_pDisplay);
     g_dLastTime = al_get_time();
     al_start_timer(g_pTimer);
}



void DoKeymap()
{
    if (g_vGameState.back() != GS_KEYMAP)
    {
        g_vGameState.push_back(GS_KEYMAP);
        Log("Gamestate to GS_KEYMAP");
    }
}




/** \brief
 *  Initializes all Classes using resources for multiple instances.
 *  Sprite classes for example
 * \return
 *  returns true on success , false on failure
 */
bool InitObjects()
{
	if (!CTransporter::Init())
    {
       Log("Could not initialize Transporter");
       return false;
    }


	if (!TBullet::Init())
	{
       Log("Could not initialize Bullets");
       return false;
	}

	if (!TAnimation::Init())
    {
        Log("Could not initialize Animations");
        return false;
    }

	if (!TEnterprise::Init())
	{
	    Log("Could not initialize Enterprise");
	    return false;
	}

	if (!TRomulanBop::Init())
    {
        Log("Could not initialize Romulan BOP");
        return false;
    }
	if (!TKlingonBC::Init())
	{
	    Log("Could not initialize KlingonBC");
	    return false;
	}

	if (!TKlingonBOP::Init())
	{
	    Log("Could not initialize KlingonBOP");
	    return false;
	}

    if (!TSpaceObject::Init())
    {
        Log("Could not initialize Space Object");
        return false;
    }
	if (!TStarbase::Init())
	{
	    Log("Could not initialize Starbase");
	    return false;
	}

	if (!TFederation_Ship::Init())
    {
        Log("Could not initialize Federation ship");
        return false;
    }

	if (!Communication::Init())
    {
        Log("Could not initialize Communication");
        return false;
    }

    if (!TProbe::Init())
    {
        Log("Could not initialize Probes");
        return false;
    }

    if (!CReliant::Init())
    {
        Log("Could not initialize Reliant");
        return false;
    }

    if (!Starfleet::Init())
    {
        Log("Could not initialize Starfleet");
        return false;
    }


    Log("Objects initialized succesfully");
	return true;
}


/** \brief
 *  Some classes better be DeInited when using multiple resources.
 *  Allegro frees them anyway but Valgrind doesn't like it that way.
 */
void DeInitObjects()
{
    delete g_pIcon;
    TStarbase::DeInit();
    TSpaceObject::DeInit();
    TAnimation::DeInit();
}





/** \brief
 *  Setup and initialize Allegro, the game Engine, Sprite Classes, Sound manager etc.
 * \return
 * returns true on success, false on failure
 */
bool Setup()
{

	if(al_init())
	{
        InitLog(); // Rude log system for debugging
        Log("Allegro Initialized");
        g_pSavePath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        al_append_path_component(g_pSavePath, "Save");
        g_pScreenshotPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);;
        al_append_path_component(g_pScreenshotPath, "screenshot");

#ifdef _DEBUG
        strcpy(g_szSector,"");
#endif // DEBUG

		g_nScreenWidth  = 1024;
	    g_nScreenHeight =  768;
		//al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
#ifdef _DEBUG
        al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
        al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
        al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
		al_set_new_display_flags(ALLEGRO_WINDOWED);
		g_pDisplay = al_create_display(g_nScreenWidth, g_nScreenHeight);
#else
		if ( al_show_native_message_box(g_pDisplay,
                                        "SETUP",
                                        "Screen Mode",
                                        "Go Fullscreen ?",
                                        NULL,
                                        ALLEGRO_MESSAGEBOX_YES_NO) == 1)
        {
            al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
            al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
            al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
            al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
            g_pDisplay = al_create_display(g_nScreenWidth, g_nScreenHeight);

            g_nScreenWidth = al_get_display_width(g_pDisplay);
            g_nScreenHeight= al_get_display_height(g_pDisplay);

            /*
            al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
            g_pDisplay = al_create_display(g_nWindowWidth, g_nWindowHeight);

            g_nWindowWidth = al_get_display_width(g_pDisplay);
            g_nWindowHeight = al_get_display_height(g_pDisplay);

            float sx = g_nWindowWidth / (float)g_nScreenWidth;
            float sy = g_nWindowHeight / (float)g_nScreenHeight;

            ALLEGRO_TRANSFORM trans;
            al_identity_transform(&trans);
            al_scale_transform(&trans, sx, sy);
            al_use_transform(&trans);
            */
        }
        else
        {
            al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
            al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
            al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
            al_set_new_display_flags(ALLEGRO_WINDOWED);
            g_pDisplay = al_create_display(g_nScreenWidth, g_nScreenHeight);
        }
#endif

		if (g_pDisplay != NULL)
		{
			Log("Display created");

			// Install allegro modules
			al_init_primitives_addon();
			al_init_image_addon();
			al_install_keyboard();
			al_install_mouse();
			Log("Addons installed");

			ALLEGRO_PATH * pIconPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
            al_set_path_filename(pIconPath,"startrek.png");
            string strIconPath = "Loading Icon from : ";
            strIconPath.append(al_path_cstr(pIconPath,ALLEGRO_NATIVE_PATH_SEP));
            Log(strIconPath);
            g_pIcon = al_load_bitmap(al_path_cstr(pIconPath,ALLEGRO_NATIVE_PATH_SEP));
            al_set_display_icon(g_pDisplay,g_pIcon);

			// Init Sound manager
			SoundManager::Init_SoundManager(RESERVED_SAMPLES);
			Log("SoundManager Initializes");
            // Load the sounds
			SoundManager::LoadSounds();
            Log("Sounds loaded");


			// install event queue
			g_pEventQueue = al_create_event_queue();
			al_register_event_source(g_pEventQueue, al_get_keyboard_event_source());
			al_register_event_source(g_pEventQueue, al_get_mouse_event_source());
			al_register_event_source(g_pEventQueue, al_get_display_event_source(g_pDisplay));

			Log("Event sources registered");


			// Init Fontmanager
            FontManager::Init_FontManager();
			Log("Fontmanager Initializes");
			FontManager::LoadFonts();


			CStatement::INIT(  FontManager::GetFont(FONT::TEXT),
                               al_map_rgb(61,134,200),
                               al_map_rgb(1,150,10),
                               al_map_rgb(55,255,100) );

           // Initialize the Sprite Object classes
			if (!InitObjects())
			{
				Log("Could not init Objects");
				return false;
			}


			// set quit to false
			g_blQuit = false;

			// create the menu
			g_pMenu = new TMenu();
			if (g_pMenu != NULL)
            {
               g_pMenu->AddMenuItem(g_nScreenWidth/2,"New Game",NewGame);
               g_pMenu->AddMenuItem(g_nScreenWidth/2, "Load",SetupLoadScreen);
               g_pMenu->AddMenuItem(g_nScreenWidth/2, "Map Keys",DoKeymap);
			   g_pMenu->AddMenuItem(g_nScreenWidth/2, "Quit",Quit);
            }
            else
            {
                Log("Could not create menu");
				return false;
            }





            // Create game engine
            g_pEngine = new TEngine(g_nScreenWidth,g_nScreenHeight);
            Log("Game engine created");

			// create Paralax system
			g_pParalax = new Paralax(g_nScreenWidth,g_nScreenHeight,200,5);
			Log("Paralax created");


            // create the keymapper
            g_pKeyMapper = new TKeyMapper(g_pEngine->m_clGREEN, g_pEngine->m_clYELLOW,FontManager::GetFont(FONT::KEYS));
            Log("Key Mapper created");

            // push gamestate GS_GAME on the gamestate stack
            g_vGameState.push_back(GS_MENU);
            Log("GAME STATE TO MENU");



			// Get rid of the mouse
			al_show_mouse_cursor(g_pDisplay);

			// Initialize DeltaTime, and create the timer
			g_dLastTime = al_get_time();
			g_pTimer=al_create_timer(1.000/g_nCyclesPerSecond);
            al_register_event_source(g_pEventQueue, al_get_timer_event_source(g_pTimer));

            Starfleet::SetGameoverFunc(DoGameOver);

            LoadMapping();
			// start the timer
			al_start_timer(g_pTimer);
		}
		else
		{
			// Display creation failed
			return false;
		}
	}
	else
	{
		// Allegro could not be initialized
		return false;
	}

    // all went well
	return true;
}


/** \brief
 *  Gets the desktop resolution
 *  Currently not in use !
 *
 * \param int a_nAdapter
 * Number of the screen adapter in use
 * \param int *a_pWidth
 * OUT ScreenWidth
 * \param int *a_pHeighth
 * OUT ScreenHeight
 *
 */
void get_desktop_resolution(int a_nAdapter, int *a_pWidth, int * a_pHeighth)
{
  ALLEGRO_MONITOR_INFO info;
  al_get_monitor_info(a_nAdapter, &info);

  *a_pWidth = info.x2 - info.x1;
  *a_pHeighth = info.y2 - info.y1;
}




/** \brief
 *  Builds a new universe and sets Enterprise home position
 *  This creates a new game !
 */
void NewUniverse()
{
    Log("Delete old universe");
    delete g_pUniverse;
    Log("Create new universe Object");
    g_pUniverse = new Universe();
    Log("Generate random universe");
    g_pUniverse->Generate();
    Log("Set enterprise position");
    g_pEnterprise->m_nSectorPositionX = g_pUniverse->GetHomeX();
    g_pEnterprise->m_nSectorPositionY = g_pUniverse->GetHomeY();
    g_pEnterprise->SetDefaultNavigationOrigin();
    g_dLastTime = al_get_time();
}


/** \brief
 *  Clears the Engine, keeping the Enterprise and
 *  builds a new Playingfield from a sector in the universe.
 * \param int a_nX
 * X position of new sector
 * \param int a_nY
 * Y position of new sector
 */

void NewSector(int a_nX, int a_nY)
{
    g_pEngine->Clear(true);
    g_pUniverse->BuildSector(a_nX,a_nY,g_pEngine);
    g_pEnterprise->SetDefaultNavigationOrigin();
}




/** \brief
 *  Shutdown and free, Engine, Paralax sprite Objects, allegro etc.
 */
void ShutDown()
{
    delete g_pEngine;
    g_pEngine = NULL;
    delete g_pParalax;
    g_pParalax = NULL;

    DeInitObjects();
    SoundManager::DeInit_SoundManager();
    al_uninstall_keyboard();
    al_destroy_path(g_pSavePath);
    al_destroy_path(g_pScreenshotPath);
	al_destroy_event_queue(g_pEventQueue);
	al_destroy_timer(g_pTimer);
	al_destroy_display(g_pDisplay);
	al_uninstall_system();
}



/** \brief
 *  Render the correct screen according to the gamestate
 */

void Render()
{
    al_clear_to_color(g_pEngine->m_clBLACK);
    if (!g_vGameState.empty())
    {
        switch(g_vGameState.back())
        {
            case GS_INTRO:

            break;

            case GS_MENU:
                 DoMenu();
            break;

            case GS_LOAD:
                DoLoadScreen();
            break;

            case GS_SAVE:
                DoSaveScreen();
            break;

            case GS_KEYMAP:
                g_pKeyMapper->DrawMapping();
            break;

            case GS_GAME:
                 DoGame();
            break;
        }

#ifdef _DEBUG
        //sprintf(g_szStates,"States: %d",g_vGameState.size());
        //al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 30, 0,g_szStates);

        char szGameState[20];
        for (size_t i=0; i < g_vGameState.size(); i++)
        {
            switch(g_vGameState[i])
            {
                case GS_EXIT:
                    sprintf(szGameState,"GS_EXIT");
                break;

                case GS_MENU:
                    sprintf(szGameState,"GS_MENU");
                break;

                case GS_LOAD:
                     sprintf(szGameState,"GS_LOAD");
                break;

                case GS_SAVE:
                    sprintf(szGameState,"GS_SAVE");
                break;

                case GS_GAME:
                    sprintf(szGameState,"GS_GAME");
                break;

                case GS_GAMEOVER:
                    sprintf(szGameState,"GS_GAME_OVER");
                break;

                case GS_TITLE:
                    sprintf(szGameState,"GS_TITLE");
                break;

                case GS_CREDITS:
                    sprintf(szGameState,"GS_CREDITS");
                break;

                case GS_INTRO:
                sprintf(szGameState,"GS_INTRO");
                break;

                case GS_KEYMAP:
                    sprintf(szGameState,"GS_KEYMAP");
                break;

                case GS_END:
                    sprintf(szGameState,"GS_END");
                break;

            }
            al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 30 + 15 * i, 0, szGameState);
        }

#endif


        al_flip_display();
	}
	else
    {
        // exit game
    }
}

/** \brief
 *  Process a game cycle
 *  Calls the AI function of the game objects
 *  Invokes the next Communication message.
 */
void GameCycle()
{
	g_pEngine->Do_ai();
	if (g_pEnterprise!=NULL)
    {
       CTransporter * pTransporter = g_pEnterprise->GetTransporter();
       if ((pTransporter != NULL) && (g_nScreenMode == MODE_COMMUNICATION))
       {
           pTransporter->update();
       }
    }

    g_pCommunication->Next();

     if (g_nMissionCycle > 0)
       {
           g_nMissionCycle--;
       }
       else
       {
           if (g_pStarfleet!= NULL)
           {
                g_pStarfleet->CheckMission();
           }

           g_nMissionCycle = g_nMissionCycleInterval;
       }


       if (g_nUniverseCycle > 0)
       {
           g_nUniverseCycle--;
       }
       else
       {
           g_pUniverse->DoNPCActions(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY,g_pEngine);

           if (g_pUniverse->WarWithKlingon())
           {
               int kns = rand() % 100;
               if(kns > 80)
               {
                   g_pUniverse->EndKlingonWar();
               }
           }
           else
           {
               int kns = rand() % 100;
               if(kns > 90)
               {
                   g_pUniverse->StartKlingonWar();
               }
           }

           if (g_pUniverse->WarWithRomulan())
           {
               int kns = rand() % 100;
               if(kns > 70)
               {
                   g_pUniverse->EndRomulanWar();
               }
           }

           g_nUniverseCycle = g_nUniverseCycleInterval;
       }
}


int GetCurrentSectorState()
{
     int nSectorState = MEM_NEUTRAL;
     if (g_pEnterprise != NULL)
     {
         nSectorState = g_pUniverse->GetSectorState(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);
     }
     return nSectorState;
}


/** \brief
 *  Checks if the Enterprise crossed the borders of a sector
 *  If so it stores the old sector in the universe
 *  and calls void NewSector(int a_nX, int a_nY)
 */
void CheckSectorBounds()
{
    if ((g_pEnterprise!=NULL)&&(!g_pEnterprise->m_blDestroyed))
    {
        bool blNewSector = false;
        int XX = 0;
        int YY = 0;
        double secsize = double(SECTORSIZE);

        // check sector boundaries
        if (g_pEnterprise->GetX() < 0)
        {
            XX = -1;
            g_pEnterprise->SetX(secsize + g_pEnterprise->GetX());
            blNewSector = true;
        }
        else if (g_pEnterprise->GetX() > secsize)
        {
            XX = 1;
            g_pEnterprise->SetX(g_pEnterprise->GetX()-secsize );
            blNewSector = true;
        }
        if (g_pEnterprise->GetY() < 0)
        {
            YY= -1;
            g_pEnterprise->SetY(secsize + g_pEnterprise->GetY());
            blNewSector = true;
        }
        else if (g_pEnterprise->GetY() > secsize)
        {
            YY = 1;
            g_pEnterprise->SetY(g_pEnterprise->GetY()-secsize);
            blNewSector = true;
        }

        // in a new sector, store the old one and build a new...
        if (blNewSector)
        {
            g_pEnterprise->ResetTransportAnnouncement();
            int nOldSectorState = g_pUniverse->GetSectorState(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);

            g_pUniverse->StoreSector(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY,g_pEngine);
            g_pEnterprise->m_nSectorPositionX += XX;
            g_pEnterprise->m_nSectorPositionY += YY;

            if (g_pEnterprise->m_nSectorPositionX==UNIVERSE_SIZE)
            {
                g_pEnterprise->m_nSectorPositionX =0;
            }
            else if (g_pEnterprise->m_nSectorPositionX<0)
            {
                g_pEnterprise->m_nSectorPositionX=UNIVERSE_SIZE-1;
            }

            if (g_pEnterprise->m_nSectorPositionY==UNIVERSE_SIZE)
            {
                g_pEnterprise->m_nSectorPositionY =0;
            }
            else if (g_pEnterprise->m_nSectorPositionY<0)
            {
                g_pEnterprise->m_nSectorPositionY =UNIVERSE_SIZE-1;
            }
            NewSector(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);

            int nNewSectorState = g_pUniverse->GetSectorState(g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);


            if ((nOldSectorState != MEM_NEUTRAL_ZONE)&&((nNewSectorState == MEM_NEUTRAL_ZONE)))
            {
                g_pCommunication->AddMessage(1, CREW_SULU,"We entered the NEUTRAL ZONE captain !");
            }

            if ((nOldSectorState != MEM_FEDERATION)&&((nNewSectorState == MEM_FEDERATION)))
            {
                g_pCommunication->AddMessage(1, CREW_SULU,"We are now in federation space !");
            }

            if ( g_pUniverse->HasSingularity(g_pEnterprise->m_nSectorPositionX ,g_pEnterprise->m_nSectorPositionY))
            {
                int z = 1+ rand()% 4;
                switch(z)
                {
                    case 1:
                        g_pCommunication->AddMessage(1, CREW_SPOCK,"Jim, There's a very heavy object in this sector");
                    break;

                    case 2:
                         g_pCommunication->AddMessage(1, CREW_SCOTTY,"Captain !, The Enterprise is pulled to something");
                    break;

                    case 3:
                         g_pCommunication->AddMessage(1, CREW_SPOCK,"Jim, Sensors detect a singularity");
                    break;

                    case 4:
                         g_pCommunication->AddMessage(1, CREW_SCOTTY,"The pull on the necelles is enormous");
                    break;

                    case 5:
                         g_pCommunication->AddMessage(1, CREW_SPOCK,"Sensors detect the remains of a dead star...");
                    break;
                }


            }

            if (g_pStarfleet != NULL)
            {
                MissionEvent event;
                event.m_Type = ET_SECTOR_ENTERED;
                event.m_Goal_Member = MEM_NONE;
                switch (nNewSectorState)
                {
                    case MEM_NEUTRAL_ZONE:
                         event.m_Sector_Member = MEM_NEUTRAL_ZONE;
                    break;

                    case MEM_FEDERATION:
                         event.m_Sector_Member = MEM_FEDERATION;
                    break;

                    case MEM_ROMULAN:
                         event.m_Sector_Member = MEM_ROMULAN;
                    break;

                    case MEM_KLINGON:
                         event.m_Sector_Member = MEM_KLINGON;

                    break;

                    case MEM_NEUTRAL:
                         event.m_Sector_Member = MEM_NONE;
                    break;
                }


                if (g_pEnterprise != NULL)
                {
                    event.m_nSectorX = g_pEnterprise->GetX();
                    event.m_nSectorY = g_pEnterprise->GetY();
                }

                g_pStarfleet->PostEvent(event);
            }

    //

#ifdef _DEBUG
            sprintf(g_szSector,"SECTOR: %d,%d",g_pEnterprise->m_nSectorPositionX,g_pEnterprise->m_nSectorPositionY);
#endif // DEBUG

            g_pEngine->SetOrigin(g_pEnterprise->GetX(),g_pEnterprise->GetY());
            g_pEnterprise->LooseTarget();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//     EVENT HANDLERS
///////////////////////////////////////////////////////////////////////////////////////


/** \brief
 *  KeyDown event handler
 * \param int a_nKey
 * The key code
 */
void KeyDown(int a_nKey)
{
    if (g_vGameState.back() == GS_MENU)
    {



    }
    else if (g_vGameState.back() == GS_KEYMAP)
    {
        if (!g_pKeyMapper->MapKey(a_nKey))
        {
            SaveMapping();
        }
    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_0))
    {
        g_pEngine->m_blKeys[WARP_0] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_1))
    {
        g_pEngine->m_blKeys[WARP_1] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_2))
    {
        g_pEngine->m_blKeys[WARP_2] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_3))
    {
        g_pEngine->m_blKeys[WARP_3] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_4))
    {
        g_pEngine->m_blKeys[WARP_4] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_5))
    {
        g_pEngine->m_blKeys[WARP_5] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,UP))
	{
	    g_pEngine->m_blKeys[UP] = true;

	    if ((g_pEnterprise!=NULL)&&(g_nScreenMode == MODE_NAVIGATION))
        {
            g_pEnterprise->DecNavY();
        }
	}
	else if (g_pKeyMapper->IsKey(a_nKey,DOWN))
	{
	    g_pEngine->m_blKeys[DOWN] = true;

	    if ((g_pEnterprise!=NULL)&&(g_nScreenMode == MODE_NAVIGATION))
        {
            g_pEnterprise->IncNavY();
        }
	}
	else if (g_pKeyMapper->IsKey(a_nKey,LEFT))
	{
	    g_pEngine->m_blKeys[LEFT] = true;

	    if ((g_pEnterprise!=NULL)&&(g_nScreenMode == MODE_NAVIGATION))
        {
            g_pEnterprise->DecNavX();
        }
	}
	else if (g_pKeyMapper->IsKey(a_nKey,RIGHT))
	{
	    g_pEngine->m_blKeys[RIGHT] = true;

	    if ((g_pEnterprise!=NULL)&&(g_nScreenMode == MODE_NAVIGATION))
        {
            g_pEnterprise->IncNavX();
        }
	}
    else if (g_pKeyMapper->IsKey(a_nKey,PHASER))
	{
	    g_pEngine->m_blKeys[PHASER] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,TARGET_UP))
	{
	    if ((g_pEnterprise!=NULL)&&(!g_pEnterprise->m_blDestroyed))
	    {
	        g_pEnterprise->LockUp();
	    }
	}
	else if (g_pKeyMapper->IsKey(a_nKey,TARGET_DOWN))
	{
	    if ((g_pEnterprise!=NULL)&&(!g_pEnterprise->m_blDestroyed))
	    {
            g_pEnterprise->LockDown();
	    }
	}
	else if (g_pKeyMapper->IsKey(a_nKey,PHOTON))
	{
	    g_pEngine->m_blKeys[PHOTON] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,PHOTON_BACK))
	{
	    g_pEngine->m_blKeys[PHOTON_BACK] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,PROBE))
    {
        g_pEngine->m_blKeys[PROBE] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,SHIELD))
    {
        g_pEngine->m_blKeys[SHIELD] = true;
    }
    else if (g_pKeyMapper->IsKey(a_nKey,DOCK))
	{
	    g_pEngine->m_blKeys[DOCK] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,RELEASE))
	{
	    g_pEngine->m_blKeys[RELEASE] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,SEEK_TARGET))
	{
	    g_pEngine->m_blKeys[SEEK_TARGET] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,SEEK_FRIEND))
	{
	    g_pEngine->m_blKeys[SEEK_FRIEND] = true;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,SCREEN_MAIN))
	{
	    al_hide_mouse_cursor(g_pDisplay);
		g_nScreenMode = MODE_MAINSCREEN;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,SCREEN_ENGINEERING))
	{
	    al_show_mouse_cursor(g_pDisplay);
		g_nScreenMode = MODE_ENGINEERING;
    }
	else if (g_pKeyMapper->IsKey(a_nKey,SCREEN_NAVIGATION))
	{
	    al_show_mouse_cursor(g_pDisplay);
		g_nScreenMode = MODE_NAVIGATION;
	}
	else if (g_pKeyMapper->IsKey(a_nKey,SCREEN_COM_TRANS))
	{
	    al_show_mouse_cursor(g_pDisplay);
		g_nScreenMode = MODE_COMMUNICATION;

		if (g_pEnterprise!= NULL)
        {
            g_pEnterprise->GetTransportTarget();
		    g_pEnterprise->GetDialog()->SetBusy();
        }
	}
	else if ((g_pKeyMapper->IsKey(a_nKey,NAVIGATION_VIEW_TOPO))&&(g_pEnterprise!= NULL)&& (! g_pEnterprise->m_blDestroyed))
	{
        g_pEnterprise->SetNavigationMode(NAV_TOPO);
	}
	else if ((g_pKeyMapper->IsKey(a_nKey,NAVIGATION_VIEW_TACTICAL))&&(g_pEnterprise!= NULL)&& (! g_pEnterprise->m_blDestroyed))
	{
        g_pEnterprise->SetNavigationMode(NAV_TACTICAL);
	}
	else if ((g_pKeyMapper->IsKey(a_nKey,NAVIGATION_VIEW_OCCUPATION))&&(g_pEnterprise!= NULL)&& (! g_pEnterprise->m_blDestroyed))
	{
        g_pEnterprise->SetNavigationMode(NAV_OCCUPATION);
	}
    else
    {
        // handle fixed keys
        switch(a_nKey)
        {
            case ALLEGRO_KEY_F12:
                screenshot(al_path_cstr(g_pScreenshotPath,ALLEGRO_NATIVE_PATH_SEP),NULL);
            break;
        }
    }
}



/** \brief
 *  KeyUp event handler
 * \param int a_nKey
 * The key code
 */
void KeyUp(int a_nKey)
{
    if (g_vGameState.back() == GS_MENU)
    {



    }
    else if (g_pKeyMapper->IsKey(a_nKey,WARP_0))
        {
            g_pEngine->m_blKeys[WARP_0] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,WARP_1))
        {
            g_pEngine->m_blKeys[WARP_1] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,WARP_2))
        {
            g_pEngine->m_blKeys[WARP_2] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,WARP_3))
        {
            g_pEngine->m_blKeys[WARP_3] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,WARP_4))
        {
            g_pEngine->m_blKeys[WARP_4] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,WARP_5))
        {
            g_pEngine->m_blKeys[WARP_5] = false;
        }
        if (g_pKeyMapper->IsKey(a_nKey,UP))
        {
            g_pEngine->m_blKeys[UP] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,DOWN))
        {
            g_pEngine->m_blKeys[DOWN] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,LEFT))
        {
            g_pEngine->m_blKeys[LEFT] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,RIGHT))
        {
            g_pEngine->m_blKeys[RIGHT] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,PHASER))
        {
            g_pEngine->m_blKeys[PHASER] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,PHOTON))
        {
            g_pEngine->m_blKeys[PHOTON] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,PHOTON_BACK))
        {
            g_pEngine->m_blKeys[PHOTON_BACK] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,PROBE))
        {
            g_pEngine->m_blKeys[PROBE] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,SHIELD))
        {
            g_pEngine->m_blKeys[SHIELD] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,DOCK))
        {
            g_pEngine->m_blKeys[DOCK] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,RELEASE))
        {
            g_pEngine->m_blKeys[RELEASE] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,SEEK_TARGET))
        {
            g_pEngine->m_blKeys[SEEK_TARGET] = false;
        }
        else if (g_pKeyMapper->IsKey(a_nKey,SEEK_FRIEND))
        {
            g_pEngine->m_blKeys[SEEK_FRIEND] = false;
        }
        else
        {
            // handle fixed keys
            switch(a_nKey)
            {
                case ALLEGRO_KEY_ESCAPE:
                g_vGameState.pop_back();
                if (g_vGameState.back()==GS_MENU)
                {
                    al_show_mouse_cursor(g_pDisplay);
                }
                break;
            }
        }
}




void KeyPressed(int a_nCharacter)
{
    switch (g_vGameState.back())
    {
        case GS_SAVE:
           if (g_pSlotManager!= NULL)
           {
               g_pSlotManager->ProcessKeyEvent(a_nCharacter);
           }
        break;

        default:
        break;

    }


}




/** \brief
 *  mouse event handler
 *  \param ALLEGRO_MOUSE_EVENT * mouse_event
 *
 */
void Mouse(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    #ifdef _DEBUG
    sprintf(g_szMouse,"Mouse: %d,%d",mouse_event.x,mouse_event.y);
    #endif // _DEBUG

    switch(g_vGameState.back())
    {

            case GS_MENU:
            if (g_pMenu!=NULL)
            {
                g_pMenu->ProcessMouseEvent(mouse_event);
            }
            break;

            case GS_LOAD:
            if (g_pSlotManager!=NULL)
            {
                g_pSlotManager->ProcessMouseEvent(mouse_event);
            }

            break;

            case GS_SAVE:
            if (g_pSlotManager!=NULL)
            {
                g_pSlotManager->ProcessMouseEvent(mouse_event);
            }
            break;

            case GS_GAME:
            if ((g_pEnterprise!= NULL)&&(!g_pEnterprise->m_blDestroyed))
            {
                g_pEnterprise->MouseMove(mouse_event);
            }
            break;

    }

}

/** \brief
 *  MouseButtonDown event handler
 *  \param ALLEGRO_MOUSE_EVENT * mouse_event
 *
 */
void MouseButtonDown(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    switch(g_vGameState.back())
    {

            case GS_MENU:
            if (g_pMenu!=NULL)
            {
                g_pMenu->ProcessMouseEvent(mouse_event);
            }
            break;

            case GS_LOAD:
            if (g_pSlotManager!=NULL)
            {
                g_pSlotManager->ProcessMouseEvent(mouse_event);
            }
            break;

            case GS_SAVE:
            if (g_pSlotManager!=NULL)
            {
                g_pSlotManager->ProcessMouseEvent(mouse_event);
            }
            break;

            case GS_GAME:
            if ((g_pEnterprise!= NULL)&&(!g_pEnterprise->m_blDestroyed))
            {
                g_pEnterprise->MouseButtonDown(mouse_event);
            }
            break;

    }
}

/** \brief
 *  MouseButtonUp event handler
 *  \param ALLEGRO_MOUSE_EVENT * mouse_event
 *
 */
void MouseButtonUp(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    switch(g_vGameState.back())
    {
        case GS_GAME:
        if ((g_pEnterprise!= NULL)&&(!g_pEnterprise->m_blDestroyed))
        {
            g_pEnterprise->MouseButtonUp(mouse_event);
        }
        break;
    }
}

/** \brief
 *  Timer event handler
 *  \param ALLEGRO_TIMER_EVENT * timer_event
 *
 */
void Timer(ALLEGRO_TIMER_EVENT & timer_event)
{
	if (!g_vGameState.empty())
    {
        switch(g_vGameState.back())
        {
            case GS_INTRO:
            DoIntro();
            break;

            case GS_MENU:

            break;

            case GS_LOAD:

            break;

            case GS_SAVE:
            break;

            case GS_GAME:
            GameCycle();
            break;
        }
    }
    else
    {
        g_blQuit = true;
    }

}




/** \brief
 *  Handle the Introduction screen,
 *  Not implemented yet
 */
void DoIntro()
{

	//	g_blGameRunning =true;
	//	g_nGameState = GS_GAME;
	//	g_nScreenMode = MODE_MAINSCREEN;

}

/** \brief
 *  Handle the Menu,
 *  Not implemented yet
 *    [New Game]
 *    [Load]
 *    [Save]
 *    [Help]
 */
void DoMenu()
{
    if (g_pMenu!=NULL)
    {
        g_pMenu->Draw(FontManager::GetFont(FONT::MENU));
    }
}


void DoLoadScreen()
{
    al_draw_text(FontManager::GetFont(FONT::TEXT),g_pEngine->m_clBLUE, 10, 30, 0,"Load game...");
    if (g_pSlotManager!=NULL)
    {
        g_pSlotManager->Draw(FontManager::GetFont(FONT::TEXT));
    }


}

void DoSaveScreen()
{
    al_draw_text(FontManager::GetFont(FONT::TEXT),g_pEngine->m_clBLUE, 10, 30, 0,"Save game...");
    if (g_pSlotManager!=NULL)
    {
        g_pSlotManager->Draw(FontManager::GetFont(FONT::TEXT));
    }
}



/** \brief
 *  Move everything in the engine.
 *  Free all destroyed objects.
 *  Check the sector bounds
 *  Move the paralax
 *  Draw everything
 */
void DoGame()
{
     double dDeltaTime = al_get_time() - g_dLastTime;
     g_dLastTime = al_get_time();
     g_pEngine->Move(dDeltaTime);
     g_pParalax->Move(g_pEngine->GetDx(), g_pEngine->GetDy());

     if ((g_pEnterprise!=NULL) && (g_pEnterprise->m_blDestroyed))
     {
        g_pEnterprise=NULL;
     }
     g_pEngine->Kill();

     if(g_pEnterprise!=NULL)
     {
        CheckSectorBounds();
        g_pEngine->Folow(g_pEnterprise);


        switch (g_nScreenMode)
        {
            case MODE_MAINSCREEN:
            g_pParalax->Draw(g_pDisplay,g_pEnterprise->GetWarp(),g_pEnterprise->GetAngle());
            g_pEngine->Draw();
            g_pEngine->DrawHud(g_pEnterprise);
            g_pEngine->Draw_sensor(60,g_nScreenHeight-60,g_pEnterprise);
            g_pEnterprise->DrawTargetInfo(0,g_nScreenWidth-140, g_nScreenHeight-120);
#ifdef _DEBUG
            al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 10, 0,g_szSector);
            if (g_blGodMode)
            {
                al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 30, 0,"GOD MODE !");
            }
#endif // DEBUG

         break;

         case MODE_ENGINEERING:
              g_pEnterprise->draw_engineering();
              #ifdef _DEBUG
              al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 10, 0,g_szMouse);
              #endif // DEBUG
         break;

         case MODE_NAVIGATION:
              g_pEnterprise->Draw_Navigation(g_pUniverse);
              #ifdef _DEBUG
              al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 10, 0,g_szMouse);
              #endif // DEBUG
         break;

         case MODE_COMMUNICATION:
              g_pEnterprise->draw_communication();
              g_pEnterprise->GetTransporter()->draw(g_pEngine->m_nScreenMidX - 260,480,250,g_pDisplay);
              #ifdef _DEBUG
              al_draw_text(FontManager::GetFont(FONT::DBG),al_map_rgb(255,255,255), 10, 10, 0,g_szMouse);
              #endif // DEBUG

         break;

       }

       g_pCommunication->Draw();

     }
     else
     {
        g_nScreenMode = MODE_MAINSCREEN;

        if (g_pEngine->m_blKeys[UP])
        {
            g_pEngine->Pan(0, -4);
        }
        else if (g_pEngine->m_blKeys[DOWN])
        {
            g_pEngine->Pan(0, 4);
        }

        if (g_pEngine->m_blKeys[LEFT])
        {
            g_pEngine->Pan(-4,0);
        }
        else if (g_pEngine->m_blKeys[RIGHT])
        {
            g_pEngine->Pan(4,0);
        }
        g_pParalax->Draw(g_pDisplay);
        g_pEngine->Draw();
     }
}


/** \brief
 *  Program entry
 * \param int argc
 * Number of program arguments ( not used yet)
 * \param char **argv
 *  Argument list ( not used yet)
 */


int main(int argc, char **argv)
{
	try
	{

	if (Setup())
	{
		// Event loop
		while (!g_blQuit)
		{
			ALLEGRO_EVENT ev;
			if (al_get_next_event(g_pEventQueue, &ev))
			{
               switch (ev.type)
                    {

                    case ALLEGRO_EVENT_KEY_CHAR:
					 KeyPressed(ev.keyboard.unichar);
     				break;

                    case ALLEGRO_EVENT_KEY_DOWN:
					 KeyDown(ev.keyboard.keycode);
     				break;

                    case ALLEGRO_EVENT_KEY_UP:
					 KeyUp(ev.keyboard.keycode);
                     break;

                    case ALLEGRO_EVENT_DISPLAY_CLOSE:
					 g_blQuit = true;
					break;

                    case ALLEGRO_EVENT_MOUSE_AXES:
					 Mouse(ev.mouse);
					 break;

                    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
					 MouseButtonDown(ev.mouse);
					 break;

                    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
					 MouseButtonUp(ev.mouse);
					 break;

                    case ALLEGRO_EVENT_TIMER:
					 Timer(ev.timer);
					 break;
                    }

           	}
			else
			{
				Render();
			}
    	}
	}
	else
	{
		// show error message
		al_show_native_message_box(
        g_pDisplay,
        "ERROR",
        "Setup Failed",
        "Setup failure",
        NULL,
        ALLEGRO_MESSAGEBOX_ERROR);

	}

	}

	catch (A5Exception & except)
	{
        // show error message
        al_show_native_message_box(
        g_pDisplay,
        "ERROR",
        "Game engine exception",
        except.what(),
        NULL,
        ALLEGRO_MESSAGEBOX_ERROR);
    }

    catch (exception & e)
    {
        // show error message
        al_show_native_message_box(
        g_pDisplay,
        "ERROR",
        "Standard exception",
        e.what(),
        NULL,
        ALLEGRO_MESSAGEBOX_ERROR);
    }

	ShutDown();
	return 0;
}

const string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%A-%Y-%m-%d-%H-%M-%S", &tstruct);
    return buf;
}

