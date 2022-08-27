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


#include "SpaceObject.h"


static std::vector<ALLEGRO_BITMAP *> g_lstSpaceObjectImages;

TSpaceObject::TSpaceObject(ID a_Type)
{
   	m_nZ	 = 10;
	m_blDestroyed=false;
	m_blCanCollide=false;
	m_blHasInventoryItem = false;
	m_ID = a_Type;


	switch (m_ID)
	{
		case ID_STAR_BLUE:
			SetImage(g_lstSpaceObjectImages[0]);
		break;

		case ID_STAR_WHITE:
			SetImage(g_lstSpaceObjectImages[1]);
		break;

		case ID_STAR_YELLOW:
			SetImage(g_lstSpaceObjectImages[2]);
		break;

		case ID_STAR_RED:
			SetImage(g_lstSpaceObjectImages[3]);
		break;

		case ID_ASTEROID:
			{
				int nOffset = rand() % 4;
			    SetImage(g_lstSpaceObjectImages[11 + nOffset]);
			    m_blCanCollide = true;
			}

		break;

		case ID_CLASS_A:
			SetImage(g_lstSpaceObjectImages[4]);
		break;

		case ID_CLASS_B:
			SetImage(g_lstSpaceObjectImages[5]);
		break;

		case ID_CLASS_L:
			SetImage(g_lstSpaceObjectImages[6]);
		break;

		case ID_CLASS_M1:
			SetImage(g_lstSpaceObjectImages[7]);
		break;

		case ID_CLASS_M2:
			SetImage(g_lstSpaceObjectImages[8]);
		break;

		case ID_CLASS_M3:
			SetImage(g_lstSpaceObjectImages[9]);
		break;

		case ID_CLASS_N:
			//SetImage(g_lstSpaceObjectImages[3]);
		break;

		case ID_BLACK_HOLE:
             SetImage(g_lstSpaceObjectImages[10]);
        break;

		default:
		break;


	}
}

bool TSpaceObject::HasInventoryItem()
{
    return m_blHasInventoryItem;
}

TInventoryItem TSpaceObject::GetInventoryItem()
{
    return m_InventoryItem;
}

void TSpaceObject::RemoveInventoryItem()
{
    m_blHasInventoryItem = false;
    m_InventoryItem.m_blValid = false;
}

void TSpaceObject::SetInventoryItem(TInventoryItem item)
{
    if (item.m_blValid)
    {
        m_InventoryItem = item;
        m_blHasInventoryItem = true;
    }
}


TSpaceObject::TSpaceObject(ifstream & a_LoadStream,ID a_id)
:TSprite(a_LoadStream, a_id)
{
	a_LoadStream.read((char*)& m_blHasInventoryItem, sizeof (m_blHasInventoryItem));
    a_LoadStream.read((char*)& m_InventoryItem, sizeof (TInventoryItem));

	m_nZ	 = 10;
	m_blDestroyed=false;
	m_blCanCollide=false;
	m_ID = a_id;


	switch (m_ID)
	{
		case ID_STAR_BLUE:
			SetImage(g_lstSpaceObjectImages[0]);
		break;

		case ID_STAR_WHITE:
			SetImage(g_lstSpaceObjectImages[1]);
		break;

		case ID_STAR_YELLOW:
			SetImage(g_lstSpaceObjectImages[2]);
		break;

		case ID_STAR_RED:
			SetImage(g_lstSpaceObjectImages[3]);
		break;

		case ID_ASTEROID:
			{
				int nOffset = rand() % 4;
			    SetImage(g_lstSpaceObjectImages[11 + nOffset]);
			    m_blCanCollide = true;
			}

		break;

		case ID_CLASS_A:
			SetImage(g_lstSpaceObjectImages[4]);
		break;

		case ID_CLASS_B:
			SetImage(g_lstSpaceObjectImages[5]);
		break;

		case ID_CLASS_L:
			SetImage(g_lstSpaceObjectImages[6]);
		break;

		case ID_CLASS_M1:
			SetImage(g_lstSpaceObjectImages[7]);
		break;

		case ID_CLASS_M2:
			SetImage(g_lstSpaceObjectImages[8]);
		break;

		case ID_CLASS_M3:
			SetImage(g_lstSpaceObjectImages[9]);
		break;

		case ID_CLASS_N:
			//SetImage(g_lstSpaceObjectImages[3]);
		break;

		case ID_BLACK_HOLE:
             SetImage(g_lstSpaceObjectImages[10]);
        break;

		default:
		break;

	}
}

void TSpaceObject::Save(ofstream & a_SaveStream)
{
	TSprite::Save(a_SaveStream);
    a_SaveStream.write((char*)& m_blHasInventoryItem, sizeof (m_blHasInventoryItem));
    a_SaveStream.write((char*)& m_InventoryItem, sizeof (TInventoryItem));
}


void TSpaceObject::Do_ai()
{
	if (m_ID == ID_ASTEROID)
	{
		if (m_dSpeed>0) m_dSpeed-=0.5;




	}

}

double TSpaceObject::GetX()
{
    return m_dX;
}

double TSpaceObject::GetY()
{
    return m_dY;
}



bool TSpaceObject::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "planets");

    for (int i=0;i<15;i++)
	{
		char szFileName[255];
		sprintf(szFileName,"so%04d.png",i);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstSpaceObjectImages.push_back(pBitmap);
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

void TSpaceObject::DeInit()
{
    ALLEGRO_BITMAP * pBitmap = NULL;
    for (size_t i =0; i < g_lstSpaceObjectImages.size();i++)
    {
        pBitmap = g_lstSpaceObjectImages[i];
        al_destroy_bitmap(pBitmap);
    }
    g_lstSpaceObjectImages.clear();
}

