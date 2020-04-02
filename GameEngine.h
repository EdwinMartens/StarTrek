/*
Copyright (C) 2015  E.J.M. Martens

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


////////////////////////////////////////////////////////////////////////


#ifndef _GAME_ENGINE
#define _GAME_ENGINE

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "types.h"
#include <stdio.h>
#include <list>


class GameEngine;
class TShip;



using namespace std;


typedef struct
{
  char m_szName [10];
  int m_nID;
  int m_nPlanetType;
  int m_nHealth [20];
  int m_nTorpedoes;
  int m_nZ;
  int m_nAI;
  int m_nMember;
  int m_nRepairItem;
  int m_nCrew;
  int m_nEnergy;
  int m_nShieldEnergy;
  int m_nPhaserEnergy;
  int m_nPreferedTarget;
  int m_nPhaserPower;
  int m_nPopulation;
  int m_nInventory[20];
  int m_cAUX[10];
  int m_nTarget;


  double m_dX;
  double m_dY;
  double m_dSpeed;
  double m_dWaypointX;
  double m_dWaypointY;
  double m_dAngle;

  bool m_blDocked;
  bool m_blDocking;
  bool m_blReleasing;
  bool m_blMustBeDestroyed;
  bool m_blMustSurvive;
  bool m_blMustReachPosition;
  bool m_blDock;
  bool m_blNoRelease;
  bool m_blCanFind;


} sGameObject;





// Basic sprite object
class TSprite
{
protected:
   GameEngine * m_pEngine;
   ALLEGRO_BITMAP * m_pImage;
	int m_nZ;
	double m_dAngle,
		   m_dX,
		   m_dY,
	       m_dSpeed;
	int m_nBitmapWidth;
	int m_nBitmapHeight;
	int m_nBitmapMidX;
	int m_nBitmapMidY;

public:
bool m_blDestroyed,
	 m_blCanCollide,
	 m_blCanFind;
int  m_nID,
     m_nMember;

virtual void Draw(double a_dCamX,double a_dCamY);
virtual void Move();
virtual void Do_ai();
virtual void DoCollision(TSprite * a_pSprite);

void SetPosition(double a_dX, double a_dY, double a_dAngle);
void SetPosition(double a_dX, double a_dY);
void SetHeight(int a_nHeight);
void SetSpeed(double a_dSpeed);
void SetImage(ALLEGRO_BITMAP * a_pImage);
double GetX();
double GetY();

TSprite();
~TSprite();

friend class GameEngine;
friend bool Comp(const TSprite * a_pV1, const TSprite * a_p_V2);
};



// Game engine
class GameEngine
{
private:
	ALLEGRO_LOCKED_REGION * m_pLockRegion;
std::list <TSprite *> m_lstItems;
int m_nCount;
double m_dX,
	   m_dY,
	   m_dPosx,
	   m_dPosy,
       m_dx,
	   m_dy;


public:

bool m_blSensorStatic;
int m_nScreenWidth;
int m_nScreenHeight;
int m_nScreenMidX;
int m_nScreenMidY;

ALLEGRO_COLOR m_clBLACK;
ALLEGRO_COLOR m_clWHITE;
ALLEGRO_COLOR m_clGREEN;
ALLEGRO_COLOR m_clYELLOW;
ALLEGRO_COLOR m_clBROWN;
ALLEGRO_COLOR m_clBLUE;
ALLEGRO_COLOR m_clRED;


bool m_blKeys[6];


void Move();
void Do_AI();
void Add(TSprite * a_pSprite);
void Draw_sensor(int a_nX,int a_nY, TShip * a_pPlayer);
void DrawHud(TShip * a_pPlayer);

void SetOrigin(double a_dX, double a_dY);
void Folow(TSprite * a_pSprite);
void Draw();
void Kill();
void Clear(bool a_blKeepPlayer);


TSprite * Seek(int a_nID, double a_dViewDistance, double a_dX, double a_dY);
TSprite * Seek(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY);
TSprite * Seekstarbase(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY);

virtual bool Detect_collision(TSprite * a_pSprite1, TSprite * a_pSprite2);

GameEngine(int a_nWidth,int a_nHeight);
~GameEngine();

bool Save(FILE * a_pSaveFile);
bool Load(FILE * a_pLoadFile);

friend class Sector;

};




double Distance(double a_dX1,double a_dY1, double a_dX2,double a_dY2);
bool Comp(const TSprite * a_pV1, const TSprite * a_p_V2);


#endif
