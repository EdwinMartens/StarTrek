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


#include "Smoke.h"
#include "types.h"

static std::vector<ALLEGRO_BITMAP *> g_lstSmokeFrames;



TSmoke::TSmoke()
{
	m_nZ = 100;
	m_nWait=2;
	m_nFrame=0;
	m_blDestroyed=false;
	m_blCanCollide=false;
	m_ID=ID_ANIMATION;
	switch (m_ID)
	{
	case ID_SMOKE:
		m_pImage=g_lstSmokeFrames[m_nFrame];
		break;
	default:m_pImage=g_lstSmokeFrames[m_nFrame];
	}

}

void TSmoke::Do_ai()
{

	if (m_nWait <=0)
	{
		m_nFrame++;

		switch (m_ID)
		{
		case ID_SMOKE:
			if (m_nFrame<11) SetImage(g_lstSmokeFrames[m_nFrame]);
			else m_blDestroyed=true;
			break;

		default:
			if (m_nFrame<11) SetImage(g_lstSmokeFrames[m_nFrame]);
			else m_blDestroyed=true;
		}

		m_nWait=2;
	}
	else
	{
		m_nWait--;
	}
}




bool TSmoke::Init()
{

	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "smoke");

    for (int i=0;i<12;i++)
	{
		char szFileName[255];
		sprintf(szFileName,"S%02d.png",i);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstSmokeFrames.push_back(pBitmap);
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


void TSmoke::DeInit()
{
    ALLEGRO_BITMAP * pBitmap = NULL;
    for (size_t i=0; i< g_lstSmokeFrames.size();i++)
    {
        pBitmap = g_lstSmokeFrames[i];
        al_destroy_bitmap(pBitmap);
    }
    g_lstSmokeFrames.clear();
}

