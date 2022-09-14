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


#ifndef _SHIP
#define _SHIP


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <vector>
#include <string>
#include "types.h"
#include "Engine.h"


using namespace std;

/**
 * \class TShip
 * TShip is the ancestor class for all ships and starbases.
 * It has a lot of handy AI support functions, like damage control, Engineering, Docking procedures...
 * Everything that is in space and has some "Smartness" in it's AI should derive from here
 */
class TShip:public TSprite
{
private:

ALLEGRO_SAMPLE_ID  * m_pPhaser_sound_id;
bool m_blPhaserSoundPlaying;

bool m_blEvading;
double m_dEvadeAngle;


protected:
    /// AI state
    AI m_AI;

/** \brief
 *  Sets the speed of a ship
 * \param double a_dSpeed
 * Speed value (0-100..200)
 */
void SetSpeed(double a_dSpeed);

/// The ID of the starbasetype to dock at. (types.h)
ID  m_PreferedBase;

       /// Turning rate
double m_dSteer,
       /// Angle turning to (steering direction)
	   m_dAngleSeek,
	   /// Distance to target if any
	   m_dTargetDistance,
	   /// Maximum distance to detect other ships etc
	   m_dViewDistance,
	   /// start of Phaser line Offset X
	   m_dPhaserX,
	   /// start of Phaser line Offset Y
	   m_dPhaserY,
	   /// Safe position for Escort missions X
	   m_dSafePosX,
	   /// Safe position for Escort missions Y
	   m_dSafePosY,
	   /// Maximum speed
	   m_dMaxSpeed,
	   /// Waypoint to go to X
	   m_dWaypointX,
	   /// Waypoint to go to Y
	   m_dWaypointY,
	   /// Angle of phaser when firing
	   m_dPhaserAngle,
	   /// Angle of bullets ( firing direction)
	   m_dAimangle;

    /// Item being repaired by crew, if any
int m_nRepairItem,
    /// Number of crewmen
	m_nCrew,
	/// Energy of the phaser
	m_nPhaserEnergy,
	/// What target subpart to fire at
	m_nPreferedTarget,
	/// How powerful is the phaser ( efficiency)
	m_nPhaserPower,
	/// Number of torpedo weapons
	m_nTorpedoes,
	/// standard cruise Z value.. used for dock and release procedures
	m_nFlyheight,
	/// Timer for Energy management ( warpcore)
	m_nEnergyTimer,
	/// Timer for repairs
	m_nRepairTimer,
	/// Timer for phaser damaging target
	m_nPhaserFireTimer,
	/// Max distance to use phaser
	m_nMaxPhaserDist,
	/// Timer for phaser recharge
	m_nPhaserTimer,
    /// Timer to recharge shield
	m_nShieldTimer,
	/// Timer to reload photon tubes
	m_nPhotonTimer,
	/// counter for cloaking device if any
	m_nCloakCounter,
	/// Energy of cloaking device
	m_nCloakCharge,
	/// Cloaking effect
	m_nTranslucency;

	/// Can this ship Cloak
	bool m_blCanCloak;

	bool m_blHadConversation;


	MISSION_CRITICAL m_MissionCritical;

public:

     /// Shields up ?
     bool m_blShieldOn,
     /// Is this ship docked
	 m_blDocked,
	 /// Has this ship entered a docking procedure
	 m_blDocking,
	 /// Has this ship entered a release procedure from a Dock
	 m_blReleasing;

    /// Tactical task number AI
int m_nTask,
    /// Maximum charge of shield
    m_nMaxShieldEnergy,
    /// current charge of shield decreases when hit
    m_nShieldEnergy,
    /// Available energy for systems.. Warpcore output
    m_nEnergy,
    /// end of phasershot
    m_nPhaserEndX,
    m_nPhaserEndY;

    /// state of cloaking device if any
    CLOAK_STATE m_CloakState;


/// The color of the phaser
ALLEGRO_COLOR m_PhaserColor;
/// Pointer to target
TShip * m_pTarget;
/// Pointer to starbase
TShip * m_pBaseTarget;
/// Name of a ship
string m_strName;

/// Health of subsystems
vector<int> m_lstHealth;
/// Maximum health of any system
int m_nMaxHealth;

// game conditions
     /// if true this ship must be destroyed to complete a task
bool m_blMustBeDestroyed,
     /// if true this ship must survive to complete a task
	 m_blMustSurvive,
	 /// if true this ship must reach a certain position (m_dSafePosX, m_dSafePosY)
     m_blMustReachPosition,
     /// True if ship must also dock
     m_blDock,
     /// if true ship will stay docked until false
     m_blNoRelease;


/// constructor
TShip();
/// virtual destructor
virtual ~TShip();

TShip(ifstream & a_LoadStream,ID a_id);
virtual void Save(ofstream & a_SaveStream);

/** \brief
 *  Calculate angle to reach waypoint
 * \param double a_dX
 *  Waypoint X
 * \param double a_dY
 *  Waypoint Y
 * \return
 *  The angle to turn to, should be passed to m_dAngleSeek
 */
double WayPoint(double a_dX,double a_dY);

/// Get the X position
double GetX();
/// Get the Y position
double GetY();
/// Get the speed
double GetSpeed();
/// Get the Angle
double GetAngle();
/// Get the cloak state
int GetCloackState();


void StopPhaser();
void StartPhaser(ID a_ID);

bool IsPhaserActive();

/** \brief
 * *VIRTUAL*
 * Do Engineering AI, damage control, energy management etc.
 * Should be called in Do_ai(), inherited from TSprite
 */
virtual void DoEngineering();

/** \brief
 * *VIRTUAL*
 * Calculates the shield energy reduction and damage done by incoming phaser fire.
 * \param double a_dEnergy
 * Ammount of energy hitting target
 * \param int a_nTarget
 * What subsystem was fired at
 */
virtual void CalcPhaserDamage(double a_dEnergy, int a_nTarget);

/** \brief
 * *VIRTUAL*
 * Handle collision for this ship
 * \param TSprite * a_pSprite
 * The sprite this ship collided with
 * Called By Detect_collision(TSprite * a_pSprite1, TSprite * a_pSprite2)
 * in TEngine.
 */
virtual void DoCollision(TSprite * a_pSprite);


/** \brief
 * *VIRTUAL*
 *  Handle Cloaking
 *  Called by DoEngineering if m_blCanCloak is true
 */
virtual void DoCloak();

/** \brief
 * *VIRTUAL*
 *  Draw the ship and some effects (phaser) relative to camera position
 * \param double a_dCamX
 * Camera X position
 * \param  double a_dCamY
 * Camera Y position
 * Only draws what is within the screen boundaries
 * Called by TEngine::Draw();
 */
virtual void Draw(double a_dCamX, double a_dCamY);

/** \brief
 * *VIRTUAL*
 * Destroy this ship with an explosion
 * should be called in Do_AI() when damage is critical
 */
virtual void Explode();


/** \brief
 * *VIRTUAL*
 *  Do the actual steering of the ship
 *  should be called in Do_AI() when m_dAngleSeek is calculated
 */
virtual void Control();



/** \brief
 * Get angle to steer away from closest collision object (Asteroid)
 * \param double a_dDefaultAngle
 * default steering angle;
 * \return
 * preferred steering angle
 */
double GetEvasiveAngle(double a_dDefaultAngle);


void StartPlayingPhaser(ID a_ID);

void StopPlayingPhaser();


/** \brief
 * Handle Docking procedure
 * \param TShip * a_pBaseTarget
 * Where to dock
 * \return
 * true when still in docking procedure, false when unable to dock or when already docked.
 */
bool Dock(TShip * a_pBaseTarget);

/** \brief
 * Try to enter the docking procedure
 * \return
 * true if successful, the ship will switch AI to docking
 */
bool TryEnterDocking();

/** \brief
 *  Start releasing procedure from Docked
 * \param TShip * a_pBaseTarget
 * The object docked at
 */
void Release(TShip * a_pBaseTarget);

/** \brief
 *  Calculate the best firing angles for photon torpedoes
 *  Only works if sensor and computer are in good shape
 * \param int a_nLimit
 * Number of iterations, more iterations is more precise
 * sets m_dAimangle
 */
void PrecalcFireAngles(int a_nLimit);

/** \brief
 * Make this ship loose it's target
 */
void LooseTarget();

/** \brief
 * Calculate the Volume for events happening on/near this ship
 * \return
 * The volume of any sound according to the distance to the camera center
 */
double CalcVolume();

/** \brief
 * pended for destruction
 * \return
 * true is pended for destruction
 */
bool IsDestroyed();

/// set health of systems to zero so the ship will be destroyed
void Die();

friend class TEngine;
};




#endif


