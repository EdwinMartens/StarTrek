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

#ifndef _SPACEOBJECT
#define _SPACEOBJECT

#include "types.h"
#include "Engine.h"
#include "Transporter.h"
#include <vector>

class Sector;

class TSpaceObject:public TSprite
{

private:
    bool m_blHasInventoryItem;
	TInventoryItem m_InventoryItem;

public:
    TSpaceObject(ID a_Type);
    TSpaceObject(ifstream & a_LoadStream,ID a_id);
    void Save(ofstream & a_SaveStream);
	void Do_ai();

	double GetX();
    double GetY();
    bool HasInventoryItem();
    TInventoryItem GetInventoryItem();
    void RemoveInventoryItem();
    void SetInventoryItem(TInventoryItem item);

	static bool Init();
	static void DeInit();

friend class Sector;

};


#endif