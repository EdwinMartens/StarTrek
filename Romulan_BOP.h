/*

        _______________
       [_|___________||)
            /      |
           /       |
          / _______|_
   __.---'-'         `.
  |           ___      \
  |        ,-'__ \      |
  |-======|==[__) |     |
  |        `-.___/      |
  |__                  /
     `-.---._________.'
          \        |
           \       |
      ______\______|___
     [_|_____________||)



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

#ifndef _ROMULANWB
#define _ROMULANWB


#include "types.h"
#include "Engine.h"
#include "Ship.h"
#include "Universe.h"
#include "DialogParser.h"
#include "Dialog.h"
#include <allegro5/allegro_audio.h>
#include <vector>
#include <cmath>


class TRomulanBop:public TShip
{
private:
//ALLEGRO_SAMPLE_ID  m_beam_id;
bool m_blBeamSound;
int m_nDisposition;

static CDialog * m_pDialog;
static bool m_blConversing;

protected:
double m_dFireDelay,
	   m_dFireDelay2,
	   m_dPowerload,
	   m_dAimAngle,
	   dd;


public:
int m_nFrame,
	m_nTask,
	m_nSequence,
	m_nEscortTimer;

virtual void DoCollision(TSprite * a_pSprite);

virtual void Do_ai();
void fire_photon();
TRomulanBop();
TRomulanBop(ifstream & a_LoadStream,ID a_id);
void Save(ofstream & a_SaveStream);
 static bool Init();
};

#endif
