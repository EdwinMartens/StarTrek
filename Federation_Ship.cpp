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


#include "Federation_Ship.h"
#include "Bullet.h"
#include "SoundManager.h"
#include <fstream>

using namespace std;

ALLEGRO_BITMAP * g_pFederation_Ship1=NULL;


TFederation_Ship::TFederation_Ship()
{
	m_dSteer		   =     0.02;
	m_nMaxHealth       =   100;
	m_nMaxShieldEnergy =   100;
	m_nPhaserEnergy	   =   100;
	m_nTorpedoes	   =    50;
	m_nEnergy		   = 10000;
	m_nRepairItem	   =    -1;
	m_nShieldTimer	   =	 0;
	m_nPreferedTarget  =    -1;
	m_dAimAngle		   =     0;
	m_dMaxSpeed        =   100;
	m_nZ			   =    39;
	m_nPhaserPower	   =  1000;
	m_dViewDistance	   = 10000;
	m_nPhaserPower	   =  2000;
	m_dPhaserAngle     =     8;
	m_PhaserColor      = al_map_rgb(200,155,0);


	m_nFlyheight	   = m_nZ;
	m_Member		   = MEM_FEDERATION;
	m_PreferedBase	   = ID_FEDERATIONBASE;
	m_nShieldEnergy    = m_nMaxShieldEnergy;
	m_nCrew	           = MAX_CREW_FEDERATION;
	m_nTask            = TSK_STANDARD;
	m_ID               = ID_GALAXYCLASS;
	m_AI        	   = AI_WANDER;

    m_strName          = "GALAXY";
	SetImage(g_pFederation_Ship1);

	for (int i=0;i<9;i++)
    {
		m_lstHealth.push_back(m_nMaxHealth);
    }
}


TFederation_Ship::TFederation_Ship(ifstream & a_LoadStream, ID a_id)
:TShip(a_LoadStream,a_id)
{
    m_dViewDistance	      = 10000;
	m_nPhaserPower		  =  2000;
   	m_nShieldTimer		  =    0;
    m_dSteer	          =	0.01;
    m_nMaxHealth          =  100;
    m_nMaxShieldEnergy    =  120;
    m_PhaserColor	      = al_map_rgb(0,255,0);
	m_dPhaserAngle        = DEGTORAD(5);
    m_Member			  = MEM_FEDERATION;
	m_PreferedBase		  = ID_FEDERATIONBASE;
	m_ID                  = ID_GALAXYCLASS;
    m_strName          = "GALAXY";
	SetImage(g_pFederation_Ship1);

	#ifdef _DEBUG
	std::cout << "-Federation ship loaded\n";
	#endif // _DEBUG
}

void TFederation_Ship::Save(ofstream & a_SaveStream)
{
    TShip::Save(a_SaveStream);

    #ifdef _DEBUG
	std::cout << "-Federation ship saved\n";
	#endif // _DEBUG
}




void TFederation_Ship::Do_ai()
{
	// Destruction rules
	if (m_lstHealth[HLT_HULL]<=0)
	{
		Explode();
	}
	else
	{

		double tsx,tsy;
        if (m_lstHealth[HLT_SENSOR]<30)
		{
			m_pTarget=NULL;
		}
		else
		{
			if ((m_nTask==TSK_CONTACT)||(m_nTask==TSK_STRIKE))
			{
				m_pTarget=(TShip *) m_pEngine->Seek(ID_PLAYER,m_dViewDistance,m_dX,m_dY);
			}
			else
			{
				m_pTarget = (TShip *) m_pEngine->Seek(m_Member,true,m_dViewDistance,m_dX,m_dY);
			}

			if ((m_pTarget!=NULL)&&(m_pTarget->m_blDestroyed)) m_pTarget=NULL;
		}

        if (((m_pTarget!=NULL)&&((m_pTarget->m_blDestroyed)||(m_pTarget->m_CloakState == CS_CLOAKED)))||(m_lstHealth[HLT_SENSOR]<30)||(m_lstHealth[HLT_COMPUTER]<30)) m_pTarget=NULL;

		if ((m_pTarget==NULL)||(m_pTarget->m_blDocked==true))
		{
			if ((m_AI==AI_CHASE)||(m_AI==AI_EVADE))
			{
				m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
                m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
				m_AI=AI_WANDER;
			}
	   }
	   else
	   {
			m_dTargetDistance = Distance(m_dX,m_dY,m_pTarget->GetX(),m_pTarget->GetY());
			if (m_AI==AI_WANDER) m_AI=AI_CHASE;

            if (m_nTask==TSK_STANDARD)
			{
				if ((m_lstHealth[HLT_PHOTON]<50)&&(m_lstHealth[HLT_PHASER]<=20)) m_AI=AI_EVADE;
            }
   }

if ( ((m_lstHealth[HLT_HULL]<40)||(m_lstHealth[HLT_PHOTON]<40)||(m_nTorpedoes==0)) && (m_pTarget!=NULL))
   {
	  if (! TryEnterDocking())
      {
          m_AI=AI_EVADE;
      }
   }
   else if (m_pTarget==NULL)
   {
       bool blShouldDock = false;
       for (size_t i=0;i<m_lstHealth.size();i++)
       {
           if (m_lstHealth[i] < (m_nMaxHealth-20)) blShouldDock = true;
           if (m_nTorpedoes<50) blShouldDock = true;
       }
       if (blShouldDock)
       {
           if ( (! TryEnterDocking()) && (m_AI!=AI_WANDER))
           {
                 m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
                 m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
                 m_AI=AI_WANDER;
           }
       }
   }


if (m_pTarget != NULL)
{
     m_nPhaserEndX = m_pTarget->GetX();
     m_nPhaserEndY = m_pTarget->GetY();
}


switch(m_AI)
      {
      case AI_WANDER:
		  {
                SetSpeed(100);
                m_dAngleSeek=WayPoint(m_dWaypointX,m_dWaypointY);
                double dDistance = Distance(m_dX,m_dY,m_dWaypointX,m_dWaypointY);
                if (dDistance<40)
                {
                    m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
                    m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
                }
		  }
      break;

      case AI_CHASE:
        if (m_pTarget!=NULL)
           {
              tsx=m_pTarget->GetX()+cos(m_pTarget->GetAngle())*m_pTarget->GetSpeed();
              tsy=m_pTarget->GetY()+sin(m_pTarget->GetAngle())*m_pTarget->GetSpeed();
              m_dAngleSeek=WayPoint(tsx,tsy);
              SetSpeed(m_dTargetDistance);

			  double t=m_dAngleSeek-m_dAngle;
              if (t<0) t=-t;


			  if (m_nTask!=TSK_CONTACT)
			  {
                 if ((m_dTargetDistance<=600)&&(m_dTargetDistance>150)&&(t<0.1))
                 {
				     fire_photon();
				 }

                 if (m_dTargetDistance<100) m_AI=AI_EVADE;
			  }


			   if ((m_nPhaserEnergy>=100)&&(m_dTargetDistance<300)&&((m_lstHealth[HLT_PHASER]>=40)))
               {
                	phaser_on();
               }

           }
        else
           {
					m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
                    m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
					m_AI=AI_WANDER;
           }
      break;

      case AI_EVADE:
           StopPhaser();
        if (m_pTarget!=NULL)
           {
              tsx=m_pTarget->GetX()+cos(m_pTarget->GetAngle())*m_pTarget->GetSpeed();
              tsy=m_pTarget->GetY()+sin(m_pTarget->GetAngle())*m_pTarget->GetSpeed();
              m_dAngleSeek=WayPoint(tsx,tsy);
              SetSpeed(m_dMaxSpeed);
			  double t=m_dAngleSeek-m_dAngle;
              if (t<0) t=-t;

			  if (m_nTask!=TSK_CONTACT)
			  {
                 if ((m_dTargetDistance<=600)&&(m_dTargetDistance>100)&&(t<0.1))
                 {
				     fire_photon();
				 }

                 if (m_dTargetDistance>400) m_AI=AI_CHASE;
			  }
			 m_dAngleSeek-=PI;
           }
        else
           {
               m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
               m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
               m_AI=AI_WANDER;
           }
      break;


     case AI_DOCK:
          if (! m_blDocked)
          {
              m_pBaseTarget = (TShip *) m_pEngine->Seekstarbase(m_Member,false,SECTORSIZE * 2,m_dX,m_dY);
              if ((m_pBaseTarget!=NULL) && ( !Dock(m_pBaseTarget) ))
              {
                  m_dAngleSeek=WayPoint(m_pBaseTarget->GetX(),m_pBaseTarget->GetY());
              }
          }
          else
          {
              bool can_go=true;
              if (m_blNoRelease) can_go=false;
              for (size_t i=0;i<m_lstHealth.size();i++)
              {
                 if (m_lstHealth[i]<m_nMaxHealth) can_go=false;
                 if (m_nTorpedoes<50) can_go=false;
              }

              if (can_go)
              {
                 m_blDocked = false;
                 m_blDocking = false;
                 m_blReleasing = true;
                 m_AI=AI_RELEASE;
              }
          }
    break;

     case AI_RELEASE:
          if (m_blReleasing)
          {
              Release(m_pBaseTarget);
          }
          else
          {
              m_blDocked = false;
              m_blDocking = false;
              m_blReleasing = false;
              m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
              m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
              m_AI=AI_WANDER;
          }
     break;


     default:

     break;

     }


//*******************************************************************************

if ((m_pTarget==NULL)||(m_lstHealth[HLT_PHASER]<40)||(m_nPhaserEnergy<=20)||(m_dTargetDistance>500))
{
   	StopPhaser();
}



DoEngineering();


if (m_nEnergy>0)
   {
      m_dSteer = m_lstHealth[HLT_THRUSTER]*0.0003;

      m_dAngleSeek = GetEvasiveAngle(m_dAngleSeek);

      Control();
   }
   else if (m_lstHealth[HLT_WARPCORE]<BREACH_WARP_HLT)
   {
       m_lstHealth[HLT_HULL]=0;
   }
}

if (m_dSpeed>(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100)
    {
        m_dSpeed=(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100;
    }

    if (m_dSpeed <0)
    {
        m_dSpeed =0;
    }

}

void TFederation_Ship::fire_photon()
{
   if ((m_nPhotonTimer<=0)&&(m_nTorpedoes>0)&&(m_lstHealth[HLT_PHOTON]>50)&&(m_nEnergy>200)&&(m_CloakState == CS_UNCLOAKED))
   {
	 double V = CalcVolume();
     SoundManager::PlaySound(SOUND::PHOTON,V);

	 TBullet * b=NULL;
	 b=new TBullet(m_dX+cos(m_dAngle)*30, m_dY+sin(m_dAngle)*30, m_dSpeed+400 , m_dAngle, m_nZ+1,ID_PHOTON,m_Member);

	 if (m_lstHealth[HLT_COMPUTER]<50) m_nPreferedTarget=-1;
     m_pEngine->Add(b);
     m_nPhotonTimer=50;
     m_nTorpedoes--;
     m_nEnergy-=200;
	}
}


void TFederation_Ship::phaser_on()
{
     // what would be good to destroy....
     if (!IsPhaserActive())
     {
        m_nPreferedTarget=HLT_WARPCORE;
        if (m_pTarget->m_lstHealth[HLT_PHOTON]>20) m_nPreferedTarget   = HLT_PHOTON;
        if (m_pTarget->m_lstHealth[HLT_THRUSTER]>1) m_nPreferedTarget  = HLT_THRUSTER;
        if (m_pTarget->m_lstHealth[HLT_IMPULSE]>1) m_nPreferedTarget   = HLT_IMPULSE;
        if (m_pTarget->m_lstHealth[HLT_SHIELD]>10) m_nPreferedTarget   = HLT_SHIELD;
        StartPhaser(m_ID);
     }
}



bool TFederation_Ship::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Ships");
    al_set_path_filename(pPath,"fed_const01.png");
    g_pFederation_Ship1 = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(pPath);

	if (g_pFederation_Ship1==NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}



