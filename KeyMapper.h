
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


#ifndef _KEYMAPPER
#define _KEYMAPPER

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//TODO (Edwin#1#) Make keymapper multipage so everything fits again


#define NUMBER_OF_KEYS    14

enum KEYS{
           UP                   =  0,
           DOWN,
           LEFT,
           RIGHT,
           PROBE,
           SHIELD,
           PHOTON,
           PHOTON_BACK,
           PHASER,
           SEEK_TARGET,
           SEEK_FRIEND,
           SEEK_BASE,
           TARGET_UP,
           TARGET_DOWN,
           DOCK,
           RELEASE,
           SCREEN_MAIN,
           SCREEN_ENGINEERING,
           SCREEN_NAVIGATION,
           SCREEN_COM_TRANS,
           NAVIGATION_VIEW_TOPO,
           NAVIGATION_VIEW_TACTICAL,
           NAVIGATION_VIEW_OCCUPATION,
           WARP_1,
           WARP_2,
           WARP_3,
           WARP_4,
           WARP_5,
           WARP_0,
           KEY_MAX
         };


class TKeyMapper
{
    private:
    int m_KeyMap[KEY_MAX];
    int m_nMapping;
    ALLEGRO_COLOR m_ColorMapped;
    ALLEGRO_COLOR m_ColorMapping;
    ALLEGRO_FONT * m_pFont;

    protected:


    public:
    TKeyMapper(ALLEGRO_COLOR a_ColorMapped, ALLEGRO_COLOR a_ColorMapping,ALLEGRO_FONT * a_pFont);
    void SetDefaultMapping();
    bool MapKey(int a_nKey);
    void DrawMapping();
    bool IsKey(int a_nKeyNr,KEYS a_MappedKey);
    void Save(ofstream & a_SaveStream);
    void Load(ifstream & a_LoadStream);

};




#endif
