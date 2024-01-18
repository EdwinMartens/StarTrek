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

#ifndef _USSENTERPRISE
#define _USSENTERPRISE

#include <vector>
#include <cmath>
#include "types.h"
#include "Engine.h"
#include "Ship.h"
#include "Universe.h"
#include "Transporter.h"
#include "Dialog.h"
#include "Communication.h"

enum PLAYED
{
    WARP_DMG,
    TARGET_DESTROYED,
    PLAYED_END
};


class Universe;

class TEnterprise:public TShip
{
private:
//    Universe * m_pUniverse;
    CTransporter * m_pTransporter;
    CDialog * m_pDialog;
    TSprite * m_pTransportTarget;

    /// true if loading Photon torpedoes
    bool m_blTorploading;
    bool m_blProbeloading;
    bool m_blCrewLoading;
    bool m_blRepaired;
    bool m_blWasDocked;
    bool m_blWasHinted;
    bool m_blToggleEnable;
    bool m_blEnergyWarning;
    bool m_blPhaserKeyDown;

//	int m_nEngineeringStartX;
//  int m_nEngineeringStartY;

	int m_nNavigationXOffset;
    int m_nNavigationYOffset;

    int m_nDestructTimer;
    bool m_blSelfDestruct;
    bool m_blPlayed[PLAYED_END];
    bool m_blHasCrewMember[CREW_END];
    bool m_blTransportAnnounced = false;
    TScanInfo m_ScanInfo;

	NAVIGATION_MODE m_NavigationMode;

	void DrawBaseInfo(const Sector & a_sector,int a_nX, int a_nY);
	void DrawPlanetInfo(const Sector & a_sector,int a_nX, int a_nY);
    void DrawOccupationInfo(const Sector & a_sector,int a_nX, int a_nY, int a_nSizeX, int a_nSizeY);
    void DrawTacticalInfo(const Sector & a_sector,int a_nX, int a_nY, int a_nSizeX, int a_nSizeY);
    void DrawNavigationLegenda(int a_nX, int a_nY);
    void draw_probeScan();
    bool AnnounceTransportTarget();


public:

    int m_nSectorPositionX;
    int m_nSectorPositionY;
    int m_nWarpFactor;
    int m_nProbes;

	virtual void DoEngineering();
	virtual void Do_ai();

	double GetX();
	double GetY();
	CTransporter * GetTransporter();
	inline int GetWarp() {return m_nWarpFactor;}
	inline double GetAngle() {return m_dAngle;}
	inline bool IsShieldOn() {return m_blShieldOn;}

	void SetX(double a_dX);
	void SetY(double a_dY);
	void LockUp();
	void LockDown();
    void SetNavigationMode(NAVIGATION_MODE a_Mode);
    void SetDefaultNavigationOrigin();
    void DNavOriginX(int a_nOffset);
    void DNavOriginY(int a_nOffset);
    void UpdateStarmap(int a_dXOffset, int a_dYOffset);
    void SignalTargetDesytoyed();
    void GetTransportTarget();
    void SetScanInfo(TScanInfo scaninfo);
    bool HasCrewMember(crewmember member);
    void SetCrewMember(crewmember member);
    void ResetCrewMember(crewmember member);
    void ResetTransportAnnouncement();
    bool CrewIsComplete();

    void   IncNavX();
    void   DecNavX();
    void   IncNavY();
    void   DecNavY();

    void SetPhaserTarget(HEALTH a_Subsystem);

    void SetSelfDestruct();

    void ToggleShield();
    void FirePhoton(bool bf);
	void LaunchProbe();
	void draw_engineering();
	void draw_communication();
	void DrawTargetInfo(int a_nSelection,int a_nXLocation,int a_nYLocation);
	void MouseMove(const ALLEGRO_MOUSE_EVENT & mouse_event);
	void MouseButtonDown(const ALLEGRO_MOUSE_EVENT & mouse_event);
	void MouseButtonUp(const ALLEGRO_MOUSE_EVENT & mouse_event);


	void Draw_Navigation(Universe * a_pUniverse);
	void Hail(MEMBER a_Member);
	void StartWarp(int a_nWarpFactor);
	void DoWarp();
	void StopWarp();
	TEnterprise();
	TEnterprise(ifstream & a_LoadStream, ID a_id);
	void Save(ofstream & a_SaveStream);

	CDialog * GetDialog();

	~TEnterprise();

	void CheatRandomDamage();

	static bool Init();
};


#endif
