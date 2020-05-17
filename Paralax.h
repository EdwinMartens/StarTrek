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

#ifndef _PARALAX
#define _PARALAX
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <cmath>


using namespace std;



struct TStar
{
	double m_dX,m_dY;
	int m_nZ;
};


class Paralax
{
    public:
    Paralax(int a_nWidth, int a_nHeight, int a_nNumberOfStars, int a_nLayers);
    ~Paralax();

    void Move(double a_dX, double a_dY);
    void Draw(ALLEGRO_DISPLAY * a_pDisplay, int a_nWarp = 0, double a_dAngle = 0);


    protected:


    private:
       double m_dOldX;
       double m_dOldY;
       int m_nWidth;
       int m_nHeight;
       int m_nLayers;
       int m_nNumberOfStars;

       ALLEGRO_LOCKED_REGION * pLockedRegion;

       vector<ALLEGRO_COLOR> m_vStarColor;
       vector<TStar> m_vStar;


};


#endif // _PARALAX
