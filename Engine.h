
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


#ifndef _Engine
#define _Engine

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <exception>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>
#include <math.h>
#include "types.h"
#include "CoolMath.h"
#include "KeyMapper.h"

using namespace std;

class TEngine;
class TShip;
class TEnterprise;

struct SPoint
{
    int m_nX;
    int m_nY;
};

/**
 * \class TSprite
 * TSprite is the basic sprite class where all sprite classes should be derived from.
 * a TSprite can be handled and drawn by the Engine
 */
class TSprite
{
private:

protected:
   /// Pointer to the engine
   TEngine * m_pEngine;
   /// pointer to a bitmap representing this sprite
   ALLEGRO_BITMAP * m_pImage;
   /// Z drawing order ( Depth)
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

/// A sprite can be associated with a certain group (Federation, Klingon etc.)
MEMBER m_Member;

/// an ID providig some runtime type information
ID     m_ID;

/** \brief
 *  ** VIRTUAL **
 *  Draw the sprite. is sprite and camera are in the same position,
 *  the sprite will be in the middle of the screen !
 *  Called by Engine
 * \param double a_dCamX
 *  X position of the CENTER of the camera
 * \param double a_dCamY
 *  X position of the CENTER of the camera
 */
virtual void Draw(double a_dCamX,double a_dCamY);

/** \brief
 *  ** VIRTUAL **
 *  Move the sprite according to its speed.
 *  Called by Engine
 *  \param double a_dLagCount
 *  The lagtime since the last cycle
 */
virtual void Move(double a_dLagCount);

/** \brief
 *  ** VIRTUAL **
 *  Enter AI routine
 *  Called by Engine
 */
virtual void Do_ai();

/** \brief
 *  ** VIRTUAL **
 *  Handle collision if m_blCanCollide is true
 *  Called by Engine
 *  \param TSprite * const a_pSprite
 *  a sprite that collided with this one.
 */
virtual void DoCollision(TSprite * const a_pSprite);

/** \brief
 * Sets the position of this sprite
 * \param double a_dX
 *  X position
 * \param double a_dY
 * Y position
 * \param double a_dAngle
 *  Rotation angle
 */
void SetPosition(double a_dX, double a_dY, double a_dAngle);

/** \brief
 * Sets the position of this sprite
 * \param double a_dX
 *  X position
 * \param double a_dY
 * Y position
 */
void SetPosition(double a_dX, double a_dY);

/** \brief
 * Sets the Height Z position ( drawing order)
 * \param int a_nHeight
 *  Z position
 */
void SetHeight(int a_nHeight);

/** \brief
 * Sets the Speed
 * \param double a_dSpeed
 *  Velocity
 */
void SetSpeed(double a_dSpeed);

/** \brief
 * Sets the Image of the sprite.
 * \param ALLEGRO_BITMAP * a_pImage
 *  Bitmap representing the sprite
 */
void SetImage(ALLEGRO_BITMAP * a_pImage);

/** \brief
 * Get the X position.
 * \return
 * X Position of the sprite
 */
double GetX();

/** \brief
 * Get the Y position.
 * \return
 * Y Position of the sprite
 */
double GetY();

/** \brief
 * Basic sprite constructor.
 */
TSprite();
// load constructor
TSprite(ifstream & a_LoadStream,ID a_id);

// copy constructor
TSprite(TSprite & a_rSprite);

// destructor
virtual ~TSprite();
// save function
virtual void Save(ofstream & a_SaveStream);

friend class TEngine;
friend bool Comp(const TSprite * a_pV1, const TSprite * a_p_V2);
};



/**
 * \class TEngine
 * This is the container class for all sprites.
 * It handles AI calling, Moving, Collision detection and drawing.
 * The engine position is a camera position, everything is drawn with these values as an offset.
 * Sprites beyond the screen boundaries will not be drawn, but will have their AI code called, moved etc.
 */
class TEngine
{
friend class TShip;

private:
// a list of sprites
std::list <TSprite *> m_lstItems;

int m_nCount;
int m_nGameOver;
double m_dPosx,
	   m_dPosy,
       m_dX,
	   m_dY;

/// count how many objects should be saved when serializing.
int CountSaveObjects();

public:

double CalcVolume(double a_dX, double a_dY);

bool m_blSensorStatic;
bool m_blHasBlackHole;
int m_nScreenWidth;
int m_nScreenHeight;
int m_nScreenMidX;
int m_nScreenMidY;


// standard colors
ALLEGRO_COLOR m_clBLACK;
ALLEGRO_COLOR m_clWHITE;
ALLEGRO_COLOR m_clGREEN;
ALLEGRO_COLOR m_clYELLOW;
ALLEGRO_COLOR m_clBROWN;
ALLEGRO_COLOR m_clBLUE;
ALLEGRO_COLOR m_clRED;
ALLEGRO_COLOR m_clMAGENTA;
ALLEGRO_COLOR m_clAQUA;
ALLEGRO_COLOR m_clATHM;

// key toggle list
bool m_blKeys[KEY_MAX];


/** \brief
 *  Move all sprite objects according to speed and lagtime
 * \param double a_dLagCount
 * Time since last cycle
 */
void Move(double a_dLagCount);

/** \brief
 *  Call AI routines of the sprites
 */
void Do_ai();

/** \brief
 *  Add a sprite to the Engine.
 * The engine OWNS the sprite from this moment and handles destruction
 * \param TSprite * const a_pSprite
 * Pointer to a sprite
 */
void Add(TSprite * const a_pSprite);


/** \brief
 *  Draws a small sensor screen
 * \param int a_nX
 * screen X position
 * \param int a_nY
 * screen Y position
 * \param TShip * const a_pPlayer
 * Pointer to the player
 */
void Draw_sensor(int a_nX,int a_nY, TEnterprise * const a_pPlayer);

/** \brief
 *  Draws a HUD Targetline pointing to a locked ship
 * \param TShip * const a_pPlayer
 * Pointer to the player
 */
void DrawHud(TShip * const a_pPlayer);

/** \brief
 *  Sets the camera center
 * \param int a_dX
 * camera X position
 * \param int a_dY
 * camera Y position
 */
void SetOrigin(double a_dX, double a_dY);


void Pan(double a_dX, double a_dY);

/** \brief
 *  Move camera to this sprite
 * \param TSprite * const a_pSprite
 * A sprite
 */
void Folow(TSprite * const a_pSprite);

/** \brief
 *  Draw all spritese
 */
void Draw();

/** \brief
 *  Free all destroyed sprites
 */
void Kill();

/** \brief
 *  Z sort the sprites
 */
void Sort();

/** \brief
 *  Clear the engine, remove all sprites
 *  \param bool a_blKeepPlayer
 *  Keep the player (Enterprise)
 */
void Clear(bool a_blKeepPlayer);

/** \brief
 *  Get Camera X position
 */
double GetDx();

/** \brief
 *  Get Camera Y position
 */
double GetDy();


/** \brief
 *  Seek for a certain sprite, returns the closest sprite with given parameters
 * \param int a_nID
 * the ID of a sprite ( sprite Type) see types.h
 * \param double a_dViewDistance
 * distance to X,Y to search
 * \param  double a_dX
 *  X position of search center
  * \param  double a_dY
 *  Y position of search center
 * \return A pointer to a sprite or NULL if nothing was found
 *
 */
TSprite * Seek(int a_nID, double a_dViewDistance, double a_dX, double a_dY);



/** \brief
 *  Seek for a certain sprite, returns the closest sprite with given parameters
 * \param int a_nID
 * the ID of a sprite ( sprite Type) see types.h
 * \param double a_dViewDistance
 * distance to X,Y to search
 * \param  double a_dX
 *  X position of search center
 * \param  double a_dY
 *  Y position of search center
 * \param  double a_dAngle
 * viewAngle
 * \param  double a_dFOV
 * Field of view
 * \return A pointer to a sprite or NULL if nothing was found
 *
 */
TSprite * Seek(int a_nID, double a_dViewDistance, double a_dX, double a_dY,double a_dAngle, double a_dRadius);


/** \brief
 *  Seek for a certain sprite, returns the closest sprite with given parameters
 * \param int a_nID
 * the ID of a sprite ( sprite Type) see types.h
 * \param bool a_blEnemy
 * Look for friend or enemy
 * \param double a_dViewDistance
 * distance to X,Y to search
 * \param  double a_dX
 *  X position of search center
  * \param  double a_dY
 *  Y position of search center
 * \return A pointer to a sprite or NULL if nothing was found
 *
 */
TSprite * Seek(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY);

/** \brief
 *  Seek for a certain starbase with given parameters
 * \param int a_nMember
 * the group the starbase should belong to types.h
 * \param bool a_blEnemy
 *  Look for friend or enemy, if enemy the function looks for a starbase belonging to an enemy of the given group
 * \param double a_dViewDistance
 * distance to X,Y to search
 * \param  double a_dX
 *  X position of search center
  * \param  double a_dY
 *  Y position of search center
 * \return A pointer to a sprite or NULL if nothing was found
 *
 */
TSprite * Seekstarbase(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY);


/** \brief
 *  Gets the closest target for transporting
 * \return A pointer to the closest transporter target
 */
TSprite * SeekClosestTransportTarget(double a_dX, double a_dY);

/** \brief
 *  Get the black hole if any
 * \return A pointer to a black hole sprite
 */
TSprite * GetBlackHole();



/** \brief
 *  Collision detection function
 * Circulair collision, distance based
 * \param TSprite * a_pSprite1
 * first sprite
 * \param TSprite * a_pSprite2
 * Second sprite
 * \return
 * true if sprites collide
 */
bool Detect_collision(TSprite * a_pSprite1, TSprite * a_pSprite2);

SPoint Detect_collision_Phaser(TSprite * const sprite, double a_dStartX, double a_dStartY,double a_dEndX, double a_dEndY);

/** \brief
 *  Engine constructor
 * \param int a_nWidth
 * Screen width
 * \param int a_nHeight
 * Screenheight
 */
TEngine(int a_nWidth,int a_nHeight);

/** \brief
 *  Virtual Destructor
 */
~TEngine();

/** \brief
 * Saves the Engine contents to an output stream
 * \param ofstream & a_SaveStream
 * The stream to serialize to
 * \return
 * true on succes
 *
 */
bool Save(ofstream & a_SaveStream);

/** \brief
 * Loads contents from an input stream into the Engine
 * \param ifstream & a_LoadStream
 * The stream to deserialize from
 * \return
 * true on succes
 */
bool Load(ifstream & a_LoadStream);

friend class Sector;

};


/**
 * \class A5Exception
 * Exception handler class.
 */
class A5Exception: public exception
{
  private:
  string m_strError;

  public:

  /** \brief
   * Exception Constructor
   * \param string a_strError
   * Error message
   */
  A5Exception(string a_strError)
  {
     m_strError = a_strError;
  }

  /** \brief
   * Exception descriptor
   * \return
   * Error message
   */
  virtual const char* what() const throw()
  {
    return m_strError.c_str();
  }

  virtual ~A5Exception() throw() { }
};


/** \brief
 * Compare function for Z sorting.
 * Compares the Z value of the sprites passed through the pointers.
 * \param const TSprite * a_pV1
 * Pointer to first sprite.
 * \param const TSprite * a_p_V2
 * Pointer to secons sprite.
 * \return
 * true if the first sprite has a greater Z value than the second.
 *
 */
bool Comp(const TSprite * a_pV1, const TSprite * a_p_V2);




#endif
