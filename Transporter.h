/*
   ___________________          _-_
   \==============_=_/ ____.---'---`---.____
               \_ \    \----._________.----/
                 \ \   /  /    `-_-'
             __,--`.`-'..'-_
            /____          ||
                 `--.____,-'



      Credits go to Paramount pictures for the star - trek concept and universe.


      Copyright (C) 2015 - 2016  E.J.M. Martens

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


#ifndef _TRANSPORTER_
#define _TRANSPORTER_

#include <stdio.h>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>
#include <string>
#include <vector>
#include "types.h"

using namespace std;


enum TInventory
{
    INV_NONE,
    INV_GORKON,
    INV_END
};


enum TTransState
{
    TS_EMPTY    = 0,
    TS_BEAMDOWN = 1,
    TS_BEAMUP   = 2,
    TS_OCCUPIED = 3
};



class CTransporter
{
private:
    list<TInventory> m_lstInventory;
    TTransState m_State;
    TInventory m_TransporterItem;
    int m_nBeamHeight;
    int m_nKnobOffset;
    TInventory m_Selection;
    bool m_blBeamUp;
    ALLEGRO_LOCKED_REGION * pLockedRegion;

protected:
    void addToInventory(TInventory a_Item);
    void removeFromInventory(TInventory a_Item);

public:

    CTransporter();
    void StartBeam( TInventory a_TransporterItem, bool a_blUp);
    void draw(int a_nX, int a_nY, int a_nWidth,int a_nHeight,ALLEGRO_DISPLAY * a_pDisplay);
    void ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event);
    void drawInventory();
    void drawKnob();
    void update();
    static bool Init();

};


#endif

