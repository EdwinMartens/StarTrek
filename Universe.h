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


#ifndef _UNIVERSE
#define _UNIVERSE


#include "types.h"
#include "Engine.h"
#include "SpaceObject.h"
#include "Starbase.h"
#include "Transporter.h"
#include <vector>
#include <math.h>

// sector specials
#define SPEC_NONE       0
#define SPEC_RELIANT    1




class TStarbase;

class GamePlanetRec
{
    friend class TEnterprise;

public:
	MEMBER m_Member;
	ID  m_PlanetType;
	int  m_nX;
	int  m_nY;
	int  m_nPopulation;
	TInventoryImage m_Item;
  GamePlanetRec();
};

class GameSpecialRec
{
    friend class TEnterprise;

public:
	MEMBER m_Member;
	ID  m_SpecialType;
	int m_nX;
	int m_nY;
	GameSpecialRec();
};

struct TDestination
{
    int m_nX;
    int m_nY;
};


class GameStarbaseRec
{
    friend class TEnterprise;

public:
    MEMBER m_Member;
    ID m_StarbaseType;
	int m_nX;
	int m_nY;
	int m_nHealth[11];
	GameStarbaseRec(ID a_nStarbaseType);
};




class Sector
{
    friend class TEnterprise;

private:
bool m_blKnown;
bool m_blHasAsteroidField;
char m_nMember;
char m_nKlingonBC;
char m_nKlingonBOP;
char m_nRomulanBOP;
char m_nFederation1;
char m_nFederation2;
char m_nFederation3;
char m_nTimer1;
char m_nTimer2;

vector<GamePlanetRec>  m_Planet;
vector<GameSpecialRec> m_Special;
vector<GameStarbaseRec> m_Starbase;

void StoreStarbase(TStarbase    * a_pStarbase);
void StorePlanet  (TSpaceObject * a_pPlanet);
void StoreSpecial (TSprite      * a_pSprite);

public:
GamePlanetRec * GetPlanetForStarbase();
Sector();

void Build(TEngine * a_pEngine);
void Store(TEngine * a_pEngine);

void Save(ofstream & a_SaveStream);
void Load(ifstream & a_LoadStream);

void Clear();

bool HasSpaceObjectType(ID id);
bool HasBase(MEMBER a_Member);

friend class Universe;
};




class Universe
{
    friend class TEnterprise;

private:
  Sector m_nSector[UNIVERSE_SIZE][UNIVERSE_SIZE];
  int m_nHomeX;
  int m_nHomeY;

  bool m_blWarWithRomulan;
  bool m_blWarWithKlingon;

protected:
  void BuildFederationSpace();
  void BuildKlingonSpace();
  void BuildRomulanSpace();

  void GenerateStarSystems();
  void GenerateBlueStarSystem(Sector & a_Sector);
  void GenerateWhiteStarSystem(Sector & a_Sector);
  void GenerateYellowStarSystem(Sector & a_Sector);
  void GenerateRedStarSystem(Sector & a_Sector);
  void GenerateBlackHoleSystem(Sector & a_Sector);

  void GenerateHomeSector(Sector & a_Sector);

  void CreateKahnQuest(Sector & a_Sector);

  TDestination GetDestination(int a_nX, int a_nY);

  void DoFederationActions(int a_nX, int a_nY,int a_nEnterpriseSectorX, int a_nEnterpriseSectorY, TEngine * a_pEngine);
  void DoKlingonActions(int a_nX, int a_nY,int a_nEnterpriseSectorX, int a_nEnterpriseSectorY, TEngine * a_pEngine);
  void DoRomulanActions(int a_nX, int a_nY,int a_nEnterpriseSectorX, int a_nEnterpriseSectorY, TEngine * a_pEngine);

public:

  int GetSectorState(int a_nX,int a_nY);
  bool HasSingularity(int a_nX,int a_nY);
  int m_nSectorCenter;
  int GetHomeX();
  int GetHomeY();

  Universe();

  void Generate();
  void BuildSector(int a_nX, int a_nY,TEngine * a_pEngine);
  void StoreSector(int a_nX, int a_nY,TEngine * a_pEngine);
  void DoNPCActions(int a_nEnterpriseSectorX, int a_nEnterpriseSectorY,TEngine * a_pEngine);
  void StartKlingonWar();
  void StartRomulanWar();
  void EndKlingonWar();
  void EndRomulanWar();
  bool WarWithKlingon();
  bool WarWithRomulan();

  void Save(ofstream & a_SaveStream);
  void Load(ifstream & a_LoadStream);

};


#endif
