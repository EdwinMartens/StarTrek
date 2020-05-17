/*
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


#include "Bullet.h"

//BOOST_CLASS_EXPORT_GUID(TBullet, "_BULLET")

static std::vector<ALLEGRO_BITMAP *> g_lstFrames;


TBullet::TBullet(double a_dX, double a_dY,double a_dSpeed, double a_dAngle, int a_nZ, ID a_nKind, MEMBER a_Member)
{
	m_dX		    =   a_dX;
	m_dY			=   a_dY;
	m_dAngle		=   a_dAngle;
	m_nZ            =   a_nZ;

	m_nLife			=	100;
	m_nWait			=	 3;
	m_dSpeed		=	 a_dSpeed;
	m_ID			=	 a_nKind;
	m_nFrame		=	  0;
	m_blCanCollide	=     false;
	m_nBitmapWidth	=    10;
	m_nBitmapHeight =    10;
	m_Member	    =    a_Member;

	switch (m_ID)
	{
	case ID_PHOTON:
		 SetImage(g_lstFrames[m_nFrame]);
		 m_nDamage  =   PHOTON_DAMAGE;
		 break;

	case ID_PLASMA:
		 SetImage(g_lstFrames[m_nFrame+4]);
		 m_nDamage	=	PLASMA_DAMAGE;
		break;

	case ID_DISRUPTOR:
         SetImage(g_lstFrames[8]);
         m_nDamage  =   DISRUPTOR_DAMAGE;
         break;

	 default:
	     throw A5Exception("Unimplemented Bullet type");

     break;



	}

}

TBullet::TBullet()
{
    m_nLife			=	100;
	m_nWait			=	 3;
	m_nFrame		=	  0;
	m_blCanCollide	=     false;
	m_nBitmapWidth	=    10;
	m_nBitmapHeight =    10;
}


TBullet::TBullet(ifstream & a_LoadStream,ID a_id)
:TSprite(a_LoadStream,a_id)
{
    a_LoadStream.read((char *)&m_nLife, sizeof (m_nLife));
    a_LoadStream.read((char *)&m_nWait, sizeof (m_nWait));

   	m_blCanCollide	=     false;
	m_nBitmapWidth	=    10;
	m_nBitmapHeight =    10;
    m_nFrame		=	  0;

    switch (m_ID)
	{
	case ID_PHOTON:
		 SetImage(g_lstFrames[m_nFrame]);
		 m_nDamage  =   PHOTON_DAMAGE;
		 break;

	case ID_PLASMA:
		 SetImage(g_lstFrames[m_nFrame+4]);
		 m_nDamage	=	PLASMA_DAMAGE;
		break;

	case ID_DISRUPTOR:
         SetImage(g_lstFrames[8]);
         m_nDamage  =   DISRUPTOR_DAMAGE;
         break;

	 default:
	     throw A5Exception("Unimplemented Bullet type");

     break;
	}
}


void TBullet::Save(ofstream & a_SaveStream)
{
    TSprite::Save(a_SaveStream);
    a_SaveStream.write((char*)& m_nLife, sizeof (m_nLife));
    a_SaveStream.write((char*)& m_nWait, sizeof (m_nWait));
}





void TBullet::Do_ai()
{
	if (m_nWait <= 0)
	{
		m_nLife--;
		m_nWait=3;

		if (m_ID!=ID_DISRUPTOR)
        {
            m_nFrame++;
            if (m_nFrame>3) m_nFrame=0;

            switch (m_ID)
            {
                case ID_PHOTON:
                    SetImage(g_lstFrames[m_nFrame]);
                break;

                case ID_PLASMA:
                    SetImage(g_lstFrames[m_nFrame+4]);
                break;


                default:
                break;

            }
        }

	}
	else
	{
		m_nWait--;
	}
	if (m_nLife<=0) m_blDestroyed=true;
}

void TBullet::Draw(double a_dCamX, double a_dCamY)
{
    int DrawX=int(m_pEngine->m_nScreenMidX+(m_dX-a_dCamX));
	int DrawY=int(m_pEngine->m_nScreenMidY+(m_dY-a_dCamY));
    if ((DrawX+m_nBitmapMidX>0)&&(DrawX-m_nBitmapMidX<m_pEngine->m_nScreenWidth)&&(DrawY+m_nBitmapMidY>0)&& (DrawY-m_nBitmapMidY<m_pEngine->m_nScreenHeight))
    {
        if (m_ID==ID_DISRUPTOR)
        {
           al_draw_rotated_bitmap(m_pImage, m_nBitmapMidX, m_nBitmapMidY, DrawX, DrawY,m_dAngle, 0);
        }
        else
        {
           al_draw_bitmap(m_pImage, DrawX, DrawY, 0);
        }
    }
}


bool TBullet::Init()
{
    ALLEGRO_PATH * pPhotonPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPhotonPath, "graphics");
    al_append_path_component(pPhotonPath, "Photon");

    ALLEGRO_PATH * pPlasmaPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPlasmaPath, "graphics");
    al_append_path_component(pPlasmaPath, "Plasma");

    ALLEGRO_BITMAP * pBitmap = NULL;
    for (int i=0;i<8;i++)
	{
		char szFileName[255];

		if (i<4)
		{
		   sprintf(szFileName,"p%02d.png",i+1);
		   al_set_path_filename(pPhotonPath,szFileName);
           pBitmap = al_load_bitmap(al_path_cstr(pPhotonPath,ALLEGRO_NATIVE_PATH_SEP));
		}
		else
		{
            sprintf(szFileName,"p%02d.png",i-3);
            al_set_path_filename(pPlasmaPath,szFileName);
            pBitmap = al_load_bitmap(al_path_cstr(pPlasmaPath,ALLEGRO_NATIVE_PATH_SEP));
		}


	    if (pBitmap!=NULL)
	    {
	        g_lstFrames.push_back(pBitmap);
	    }
	    else
	    {
            al_destroy_path(pPhotonPath);
	        al_destroy_path(pPlasmaPath);
            return false;
	    }
	}
	al_destroy_path(pPhotonPath);
	al_destroy_path(pPlasmaPath);

	ALLEGRO_PATH * pDisruptorPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pDisruptorPath, "graphics");
    al_append_path_component(pDisruptorPath, "disruptor");
    al_set_path_filename(pDisruptorPath,"disruptor.png");
    pBitmap = al_load_bitmap(al_path_cstr(pDisruptorPath,ALLEGRO_NATIVE_PATH_SEP));

    if (pBitmap!=NULL)
	{
	    g_lstFrames.push_back(pBitmap);
	}
	else
	{
        al_destroy_path(pDisruptorPath);
	    return false;
	}

    al_destroy_path(pDisruptorPath);
	return true;
}
