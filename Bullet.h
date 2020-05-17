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





#ifndef _BULLETS
#define _BULLETS


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <vector>
#include "types.h"
#include "Engine.h"

class TBullet:public TSprite
{
    friend class TEngine;
private:

 int m_nFrame,
	 m_nLife;
 int m_nWait;

public:
	int	m_nDamage;


	TBullet(double a_dX, double a_dY,double a_dSpeed, double a_dAngle, int a_nZ,ID a_nKind, MEMBER a_Member);
	TBullet();
	TBullet(ifstream & a_LoadStream, ID a_id);
	void Do_ai();
	static bool Init();
	void Draw(double a_dCamX, double a_dCamY);
    void Save(ofstream & a_SaveStream);
};


#endif
