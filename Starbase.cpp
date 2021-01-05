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



#include "Starbase.h"
#include "Bullet.h"
#include "SoundManager.h"

static std::vector<ALLEGRO_BITMAP *> g_lstStarBases;

TStarbase::TStarbase(ID a_nStarbaseType)
{
	m_dSteer     	=       0;
	m_nEnergy		=	10000;
	m_nRepairItem	=	   -1;
	m_nCrew			=	 5000;
	m_nShieldTimer	=       0;
	m_dMaxSpeed     =       0;
	m_nZ			=      20;
	m_nMaxHealth    =   MAX_STARBASE_HEALTH;
	m_nPhaserPower	=    1000;
	m_dViewDistance	=    2000;
	m_nPhaserPower	=	 2000;
	m_nPhotonTimer =      40;

	m_nTask         =  TSK_STANDARD;
	m_PreferedBase	=  ID_NONE;
	m_AI			=  AI_NONE;

	m_nFlyheight	= m_nZ;
	m_ID            = a_nStarbaseType;

	for (int i=0;i<9;i++)
    {
		m_lstHealth.push_back(m_nMaxHealth);
    }


	switch (a_nStarbaseType)
	{
        case ID_FEDERATIONBASE:
	         SetImage(g_lstStarBases[0]);
	         m_Member			=  MEM_FEDERATION;

	         m_nMaxShieldEnergy =  500;
             m_nShieldEnergy    =  m_nMaxShieldEnergy;
             m_nPhaserEnergy	=  1000;
             m_nTorpedoes	    =  1000;
             m_strName           = "Federation Base";

	    break;

	    case ID_ROMULAN_BASE:
	         SetImage(g_lstStarBases[1]);
	         m_Member			=  MEM_ROMULAN;

	         m_nMaxShieldEnergy =  800;
             m_nShieldEnergy    =  m_nMaxShieldEnergy;
             m_nPhaserEnergy	=  1000;
             m_nTorpedoes	    =  1000;
             m_strName           = "Romulan Base";

	    break;

	    case ID_KLINGON_BASE:
	         SetImage(g_lstStarBases[2]);
	         m_Member			=  MEM_KLINGON;

	         m_nMaxShieldEnergy =  400;
             m_nShieldEnergy    =  m_nMaxShieldEnergy;
             m_nPhaserEnergy	=  1000;
             m_nTorpedoes	    =  1000;
             m_strName           = "Klingon Base";

	    break;


	    default:
	        throw A5Exception("Unimplemented Starbase type");

        break;

	}

}


TStarbase::TStarbase(ifstream & a_LoadStream,ID a_id)
:TShip(a_LoadStream, a_id)
{
    	switch (a_id)
	{
        case ID_FEDERATIONBASE:
	         SetImage(g_lstStarBases[0]);
	         m_Member			=  MEM_FEDERATION;

	         m_nMaxShieldEnergy =  500;
             m_nPhaserEnergy	=  1000;
             m_strName           = "Federation Base";

	    break;

	    case ID_ROMULAN_BASE:
	         SetImage(g_lstStarBases[1]);
	         m_Member			=  MEM_ROMULAN;

	         m_nMaxShieldEnergy =  800;
             m_nPhaserEnergy	=  1000;
             m_strName           = "Romulan Base";

	    break;

	    case ID_KLINGON_BASE:
	         SetImage(g_lstStarBases[2]);
	         m_Member			=  MEM_KLINGON;

	         m_nMaxShieldEnergy =  400;
             m_nPhaserEnergy	=  1000;
             m_strName           = "Klingon Base";

	    break;


	    default:
	        throw A5Exception("Unimplemented Starbase type");

        break;

	}

	m_nShieldEnergy = std::min(m_nShieldEnergy, m_nMaxShieldEnergy);

	for (int i=0;i<9;i++)
    {
		m_lstHealth[i] = std::min(m_lstHealth[i], m_nMaxHealth);
    }
}

void TStarbase::Save(ofstream & a_SaveStream)
{
    TShip::Save(a_SaveStream);
}




void TStarbase::fire_photon(double a_dFireAngle)
{

if ((m_nPhotonTimer<=0)&&(m_nTorpedoes>0)&&(m_lstHealth[HLT_PHOTON]>50)&&(m_nEnergy>200))
   {
	 TBullet * b=NULL;
	 if (m_Member == MEM_ROMULAN)
     {
         b=new TBullet(m_dX+cos(a_dFireAngle)*30, m_dY+sin(a_dFireAngle)*30, 500 , a_dFireAngle, m_nZ+1,ID_PLASMA ,m_Member);
     }
     else
     {
         b=new TBullet(m_dX+cos(a_dFireAngle)*30, m_dY+sin(a_dFireAngle)*30, 500 , a_dFireAngle, m_nZ+1,ID_PHOTON,m_Member);
     }


	 m_pEngine->Add(b);
     m_nPhotonTimer=40;
     m_nTorpedoes--;
     m_nEnergy-=200;
	}

}


void TStarbase::Do_ai()
{
	// Destruction rules
	if (m_lstHealth[HLT_HULL]<=0)
	{
		Explode();
	}
	else
	{
		double tsx,tsy,shotlead;

		if (m_lstHealth[HLT_SENSOR]<30) m_pTarget=NULL;
		// ****************** AI routines ***********************************************

		if (m_nEnergy>0)
        {
            m_pTarget = (TShip *) m_pEngine->Seek(m_Member,true,m_dViewDistance,m_dX,m_dY);
            if ((m_pTarget!=NULL)&&(m_pTarget->m_blDestroyed==true)) m_pTarget=NULL;

            if (m_pTarget!=NULL)
            {
                m_dTargetDistance = Distance(m_dX,m_dY,m_pTarget->GetX(),m_pTarget->GetY());
			    shotlead= 100;
			    tsx=m_pTarget->GetX()+cos(m_pTarget->GetAngle())*shotlead;
                tsy=m_pTarget->GetY()+sin(m_pTarget->GetAngle())*shotlead;
			    m_dAngleSeek=WayPoint(tsx,tsy);

			   if ((m_dTargetDistance < 600)&&(m_dTargetDistance > 150))
			   {
				   fire_photon(m_dAngleSeek);
			   }
		   }

		   DoEngineering();
		}
        else if (m_lstHealth[HLT_WARPCORE]<20)
        {
            m_lstHealth[HLT_HULL]=0;
        }
	}
}


bool TStarbase::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Starbases");
    al_set_path_filename(pPath,"FedBase.png");
    ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));


	if (pBitmap!=NULL)
	{
		g_lstStarBases.push_back(pBitmap);
	}
	else
	{
		return false;
    }


    pBitmap = NULL;
    al_set_path_filename(pPath,"RomBase.png");
    pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));

	if (pBitmap!=NULL)
	{
		g_lstStarBases.push_back(pBitmap);
	}
	else
	{
		return false;
    }

    pBitmap = NULL;
    al_set_path_filename(pPath,"KlinBase.png");
    pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));

	if (pBitmap!=NULL)
	{
		g_lstStarBases.push_back(pBitmap);
		return true;
	}
	else
	{
		return false;
    }



    al_destroy_path(pPath);
}



void TStarbase::DeInit()
{
    ALLEGRO_BITMAP * pBitmap = NULL;
    for (size_t i=0; i< g_lstStarBases.size();i++)
    {
        pBitmap = g_lstStarBases[i];
        al_destroy_bitmap(pBitmap);
    }
    g_lstStarBases.clear();
}









