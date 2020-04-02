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


#include "Explosion.h"


static std::vector<ALLEGRO_BITMAP *> g_lstExplosionFrames;

TExplosion::TExplosion(double a_dX, double a_dY,double a_dSpeed = 0, double a_dAngle = 0, int a_nZ = 100)
{
    m_dX = a_dX;
    m_dY = a_dY;
    m_nZ = a_nZ;

    m_dSpeed = a_dSpeed;
    m_dAngle = a_dAngle;
}

TExplosion::TExplosion()
{
   	m_nZ	 = 100;
	m_nFrame =  0;
	m_nWait  = 6;
	m_blDestroyed=false;
	m_blCanCollide=false;
	m_ID=ID_ANIMATION;
	switch (m_ID)
	{
	case ID_EXPLOSION:
		m_pImage=g_lstExplosionFrames[m_nFrame];
		break;
	default:m_pImage=g_lstExplosionFrames[m_nFrame];
	}

}

void TExplosion::Do_ai()
{
	if (m_nWait <=0)
	{
		m_nFrame++;

		switch (m_ID)
		{
		case ID_EXPLOSION:
			if (m_nFrame<11) SetImage(g_lstExplosionFrames[m_nFrame]);
			else m_blDestroyed=true;
			break;

		default:
			if (m_nFrame<11) SetImage(g_lstExplosionFrames[m_nFrame]);
			else m_blDestroyed=true;
		}

		m_nWait=3;
	}
	else
	{
		m_nWait--;
	}
}


bool TExplosion::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Explosion");

    for (int i=0;i<16;i++)
	{
		char szFileName[255];
		sprintf(szFileName,"Expl%02d.png",i+1);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstExplosionFrames.push_back(pBitmap);
	    }
	    else
	    {
            al_destroy_path(pPath);
            return false;
	    }
	}

	al_destroy_path(pPath);
	return true;
}

void TExplosion::DeInit()
{
    ALLEGRO_BITMAP * pBitmap = NULL;
    for (size_t i=0; i< g_lstExplosionFrames.size();i++)
    {
        pBitmap = g_lstExplosionFrames[i];
        al_destroy_bitmap(pBitmap);
    }
    g_lstExplosionFrames.clear();
}

