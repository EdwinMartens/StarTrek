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

#ifndef _RELIANT_SHIP
#define _RELIANT_SHIP

#include "types.h"
#include "Engine.h"
#include "Ship.h"
#include "Universe.h"
#include "DialogParser.h"
#include "Dialog.h"
#include <allegro5/allegro_audio.h>
#include <vector>
#include <cmath>


class CReliant:public TShip
{
private:
    bool m_blSpockMessage;

    static CDialog * m_pDialog;
    static bool m_blConversing;

    int m_nDisposition;
    int m_nCloseIn;

protected:

double m_dFireDelay,
	   m_dFireDelay2,
	   m_dPowerload,
	   m_dAimAngle,
	   dd;


public:
int m_nFrame,
	m_nTask,
	m_nSequence;

virtual void Do_ai();
void fire_photon();
void phaser_on();
void phaser_off();

CReliant();
CReliant(ifstream & a_LoadStream, ID a_id);
void Save(ofstream & a_SaveStream);
static bool Init();
};

#endif

