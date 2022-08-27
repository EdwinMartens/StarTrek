
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






#ifndef _COOL_MATH
#define _COOL_MATH

#include <math.h>

#define PI               3.14159265358


double Distance(double a_dX1,double a_dY1, double a_dX2,double a_dY2);
int Distance(int a_nX1,int a_nY1, int a_nX2,int a_nY2);

bool PointInTriangle(int a_nPx, int a_nPy,
                     int a_nTriPo1x, int a_nTripo1y,
                     int a_nTriPo2x, int a_nTripo2y,
                     int a_nTriPo3x, int a_nTripo3y);





#endif // _COOL_MATH

