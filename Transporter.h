/*
   ___________________          _-_
   \==============_=_/ ____.---'---`---.____
               \_ \    \----._________.----/
                 \ \   /  /    `-_-'
             __,--`.`-'..'-_
            /____          ||
                 `--.____,-'



      Credits go to Paramount pictures for the star - trek concept and universe.


      Copyright (C) 2015 - 2020 E.J.M. Martens

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
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>
#include "types.h"

using namespace std;


enum TInventoryImage
{
    INV_NONE = -1,
    INV_GORKON,
    INV_SPOCK,
    INV_SAREK,
    INV_END
};

struct TInventoryItem
{
    TInventoryImage imageID;
    int m_nWidth;
    int m_nHeight;
    int m_nX;
    int m_nY;
    bool b_blUnique;
    bool m_blValid;
};

enum TResponse
{
    RE_NOTHING,
    RE_BEAM_UP,
    RE_BEAM_DOWN,
    RE_DAMAGED,
    RE_NOTARGET,
    RE_NOTINRANGE,
    RE_SHIELD
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
    vector<TInventoryItem> m_lstInventory;
    TTransState m_State;
    TInventoryItem m_TransporterItem;
    int m_nBeamHeight;
    int m_nKnobOffset;
    TInventoryItem * m_pSelection;
    bool m_blBeamUp;
    ALLEGRO_LOCKED_REGION * pLockedRegion;

protected:
    void addToInventory(TInventoryItem a_Item);
    void removeFromInventory(int a_nIndex);

public:

    CTransporter();
    CTransporter(ifstream & a_LoadStream);
    void Save(ofstream & a_SaveStream);
    bool IsEmpty();
    void StartBeam(bool a_blUp);
    void Enter(TInventoryItem a_Item);
    void Target(TInventoryItem a_Item);
    void draw(int a_nX, int a_nY,int a_nHeight,ALLEGRO_DISPLAY * a_pDisplay);
    TResponse ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event,int a_nHealth, int a_nDistance, bool a_blShield);
    void drawInventory();
    void drawKnob();
    void update();
    TInventoryItem GetBeamedDownItem();
    static bool Init();

};


#endif

