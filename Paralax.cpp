/*
Copyright (C) 2015  E.J.M. Martens

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

#include "Paralax.h"

Paralax::Paralax(int a_nWidth, int a_nHeight,int a_nNumberOfStars,int a_nLayers)
{
    m_nWidth         = a_nWidth;
    m_nHeight        = a_nHeight;
    m_nLayers        = a_nLayers;
    m_nNumberOfStars = a_nNumberOfStars;

    // initialize colors
    for (int i=0; i< m_nLayers;i++)
    {
        ALLEGRO_COLOR col = al_map_rgb(((200/(i*2+1))+55),((200/(i*2+1))+55),((200/(i*2+1))+55));
        m_vStarColor.push_back(col);
    }

    // initialize stars
    for (int i=0;i<m_nNumberOfStars; i++)
    {
        TStar star;
        star.m_dX = rand() % a_nWidth;
        star.m_dY = rand() % a_nHeight;
        star.m_nZ = rand() % a_nLayers;
        m_vStar.push_back(star);
    }

    m_dOldX = m_nWidth/2;
    m_dOldY = m_nHeight/2;

    pLockedRegion = NULL;
}


Paralax::~Paralax()
{
   m_vStarColor.clear();
   m_vStar.clear();

}

void Paralax::Move(double a_dX, double a_dY)
{
     double DeltaX = m_dOldX -a_dX;
     double DeltaY = m_dOldY -a_dY;
     m_dOldX = a_dX;
     m_dOldY = a_dY;

    for (size_t i=0; i< m_vStar.size();i++)
    {
        m_vStar[i].m_dX += (DeltaX / (m_vStar[i].m_nZ + 1) );
		m_vStar[i].m_dY += (DeltaY / (m_vStar[i].m_nZ + 1) );

        if (m_vStar[i].m_dX < 0)
		{
			m_vStar[i].m_dX = m_nWidth+m_vStar[i].m_dX;
			m_vStar[i].m_dY = rand()% m_nHeight;
			m_vStar[i].m_nZ = rand()% m_nLayers;
		}

		if (m_vStar[i].m_dX > m_nWidth)
		{
			m_vStar[i].m_dX = m_vStar[i].m_dX-m_nWidth;
			m_vStar[i].m_dY = rand()% m_nHeight;
			m_vStar[i].m_nZ = rand()% m_nLayers;
		}

		if (m_vStar[i].m_dY < 0)
		{
			m_vStar[i].m_dX = rand()% m_nWidth;
			m_vStar[i].m_dY = m_nHeight+m_vStar[i].m_dY;
			m_vStar[i].m_nZ = rand()% m_nLayers;
		}

		if (m_vStar[i].m_dY > m_nHeight)
		{
			m_vStar[i].m_dX = rand()% m_nWidth;
			m_vStar[i].m_dY = m_vStar[i].m_dY-m_nHeight;
			m_vStar[i].m_nZ = rand()% m_nLayers;
		}
    }
}

void Paralax::Draw(ALLEGRO_DISPLAY * a_pDisplay, int a_nWarp, double a_dAngle)
{
    if (a_nWarp>0)
    {
       int dX = a_nWarp* 5 * cos(a_dAngle);
       int dY = a_nWarp* 5 * sin(a_dAngle);

       for (size_t i=0;i< m_vStar.size();i++)
       {
            al_draw_line(m_vStar[i].m_dX,
                         m_vStar[i].m_dY,
                         m_vStar[i].m_dX+dX,
                         m_vStar[i].m_dY+dY,
                         m_vStarColor[m_vStar[i].m_nZ], 2);

       }
    }
    else
    {
       for (size_t i=0;i< m_vStar.size();i++)
       {
            int nRadius = 4/(m_vStar[i].m_nZ+1) > 0 ? 4/(m_vStar[i].m_nZ+1):1;
            nRadius = nRadius > 2 ? 2 : nRadius;
            al_draw_filled_circle(m_vStar[i].m_dX, m_vStar[i].m_dY,nRadius, m_vStarColor[m_vStar[i].m_nZ]);
            //al_put_pixel(m_vStar[i].m_dX, m_vStar[i].m_dY, m_vStarColor[m_vStar[i].m_nZ]);
       }
    }

}


