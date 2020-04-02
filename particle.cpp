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


#include "particle.h"

TParticle::TParticle(PARTICLE_EFFECT a_Effect)
{
	switch (a_Effect)
	{
        case EFF_NORMAL:
             m_nLife=40;
             m_dSpeed=0;
             m_dX=5000;
             m_dY=5000;
             m_nZ= 100;
             m_Effect = EFF_NORMAL;
            break;

        case EFF_PROBE:
             m_nLife=10;
	         m_dSpeed=0;
	         m_dX=5000;
	         m_dY=5000;
	         m_nZ= 30;
	         m_Effect = EFF_PROBE;
	         break;

	}


	m_ID=ID_NONE;
	m_blCanCollide=false;
}

void TParticle::Do_ai()
{
	m_nLife--;
	//speed-=0.01;
	m_dX+=double((rand()%8)-4)/2;
	m_dY+=double((rand()%8)-4)/2;
	if (m_nLife<0) m_blDestroyed=true;
}

void TParticle::Draw(double a_dCamX,double a_dCamY)
{
	double DrawX=int(m_pEngine->m_nScreenMidX+(m_dX-a_dCamX));
	double DrawY=int(m_pEngine->m_nScreenMidY+(m_dY-a_dCamY));
	// only draw what we see

//	float r=1.0;
//	float g=0.2;
//	float b=0.8;
//	float a=m_nLife/50;

	if ((DrawX>=0)&&(DrawX<m_pEngine->m_nScreenWidth)&&(DrawY>=0)&&(DrawY<m_pEngine->m_nScreenHeight))
	{
		int r,g,b;
		switch (m_Effect)
        {
            case EFF_NORMAL:
                 r = (m_nLife*6)+15;
                 g = 100;
                 if (r<g) g=r;
                 b=0;
                 break;

            case EFF_PROBE:
                 r = g = b = (m_nLife*6)+15;
                 break;

            default:
                r = 255;
                g = 255;
                b = 255;
        }

		al_draw_filled_circle(DrawX, DrawY, 1, al_map_rgba(r,g,b,m_nLife*5));
	}
}

