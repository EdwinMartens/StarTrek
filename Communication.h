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


#ifndef _COMMUNICATION_
#define _COMMUNICATION_

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

enum crewmember
{
     CREW_KIRK   = 0,
     CREW_SPOCK  = 1,
     CREW_MCCOY  = 2,
     CREW_SCOTTY = 3,
     CREW_UHURA  = 4,
     CREW_SULU   = 5,
     CREW_CHECOV = 6,
     CREW_END
};

class Communication;

class Message
{
    private:
    Communication * m_pOwner;
    int m_nID;
    crewmember m_crew;
    string m_strMessage;
    ALLEGRO_BITMAP * m_pImage;

    protected:

    public:
    Message(int a_nID, string a_strMessage, crewmember a_crew);
    ~Message();

    void Draw(int a_nX, int a_nY);

    static bool Init();

    friend class Communication;
};








class Communication
{
friend class Message;

private:
    int m_nX;
    int m_nY;
    int m_nPos;
    int m_nCounter;
    list <Message *> messagequeue;

ALLEGRO_COLOR m_Color;

protected:


public:
    Communication(int a_nX, int a_nY);
    ~Communication();

    void AddMessage(int a_nID, crewmember a_CrewMember,const char * a_szMessage);
    void Draw();
    void Next();
    void Clear();
    static bool Init();
};












#endif
