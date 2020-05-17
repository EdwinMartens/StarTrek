
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

#ifndef _ANIMATIONS
#define _ANIMATIONS

#include <vector>
#include "types.h"
#include "Engine.h"

namespace ANIMATION
{
    enum TYPE
    {
      EXPLOSION = 1,
      SMOKE,
      WARP_IN,
      WARP_OUT,
      ANI_END
    };
}

    enum ANIMATIONMODE
    {
        ONCE = 0,
        LOOP,
        BACK_FORTH,
        NOFRAME,
        MODE_END
    };

    class TAnimation:public TSprite
    {
        friend class TEngine;
    private:
        bool m_blReverse;
        int m_nFrame;
        int m_nStartFrame;
        int m_nEndFrame;
        int m_nWait;
        int m_nDelay;
        bool m_blBackward;
        ANIMATION::TYPE m_Type;
        ANIMATIONMODE m_Mode;
        void SetAnimation();

    public:
        TAnimation(ANIMATION::TYPE a_Type,
                   double a_dX, double a_dY,
                   double a_dSpeed,
                   double a_dAngle,
                   int a_nZ);
        TAnimation();
        TAnimation(ifstream & a_LoadStream,ID a_id);
        void Do_ai();
        static bool Init();
        static void DeInit();
        void Save(ofstream & a_SaveStream);
    };



#endif

