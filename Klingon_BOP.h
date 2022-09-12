/*
                    .----------.___
               / |        |||\  ~~~--_               ____
            __|-------------| |~~~~~~~~----______--~~    ~~-_
           | _|-_           |_|                  =============
           |_|   ~~---------'__\_____------~~~~~~--.______.-~
            |__|~~ ~-__/   ~-_~-_
               ~--__   ~-__/  ~-_~-_
                    ~--__  ~-__/ ~-_~-_
                         ~--__ ~-___/  ~-_
                               ~--__       ~-_
                                  [__________]=====[[

                        _------_        _------_
                       / /~~~~~~~\----/~~~~~~~\ \
                    __|_|    /~ _-~~~~-_ ~\    |_|__
              __--~~____|   |  /________\  |   |____~~--__
        __--~~__--~~     ~---__\   ()   /__---~     ~~--__~~--__
     /~~__--~~                  ~--__--~                  ~~--__~~\
   / /~~                                                        ~~\ \
 / /                                                                \ \
(0)                                                                  (0)



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

#ifndef _KLINGON_BOP
#define _KLINGON_BOP


#include "types.h"
#include "Engine.h"
#include "Ship.h"
#include "Universe.h"
#include <allegro5/allegro_audio.h>
#include <vector>
#include <cmath>


class TKlingonBOP:public TShip
{
private:

protected:
double m_dFireDelay,
	   m_dFireDelay2,
	   m_dPowerload,
	   m_dAimAngle,
	   dd;


public:
int m_nFrame,
		m_nSequence;

virtual void Do_ai();
void fire_photon();
void fire_disruptors();
TKlingonBOP();
TKlingonBOP(ifstream & a_LoadStream, ID a_id);
void Save(ofstream & a_SaveStream);

static bool Init();
};













#endif // _KLINGON_BOP
