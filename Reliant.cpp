#include "Reliant.h"


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


#include "Reliant.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "Communication.h"
#include "Enterprise.h"
#include <fstream>

using namespace std;

extern Communication * g_pCommunication;
extern TEnterprise   * g_pEnterprise;

ALLEGRO_PATH   * pConversationPath;

bool CReliant::m_blConversing = false;
CDialog * CReliant::m_pDialog = NULL;

ALLEGRO_SAMPLE_ID  rel_phaser_id;


ALLEGRO_BITMAP * g_pReliant=NULL;


CReliant::CReliant()
{
	m_dSteer		   =     0.02;
	m_nMaxHealth       =   100;
	m_nMaxShieldEnergy =   100;
	m_nPhaserEnergy	   =   100;
	m_nTorpedoes	   =    2;
	m_nEnergy		   = 10000;
	m_nRepairItem	   =    -1;
	m_nShieldTimer	   =	 0;
	m_nPreferedTarget  =    -1;
	m_dAimAngle		   =     0;
	m_dMaxSpeed        =   150;
	m_nZ			   =    39;
	m_nPhaserPower	   =  1000;
	m_dViewDistance	   = 100000;
	m_nPhaserPower	   =  2000;
	m_dPhaserAngle     =     8;
	m_nCloseIn         =   150;
	m_PhaserColor      = al_map_rgb(200,155,0);
	m_nDisposition     = 20;
	m_blSpockMessage   = false;

    m_nFlyheight	   = m_nZ;
	m_Member		   = MEM_FEDERATION;
	m_PreferedBase	   = ID_FEDERATIONBASE;
	m_nShieldEnergy    = m_nMaxShieldEnergy;
	m_nCrew	           = MAX_CREW_FEDERATION;
	m_nTask            = TSK_STANDARD;
	m_ID               = ID_RELIANT;
	m_AI        	   = AI_WANDER;

    m_strName          = "RELIANT";
	SetImage(g_pReliant);

	for (int i=0;i<9;i++)
    {
		m_lstHealth.push_back(m_nMaxHealth);
    }
}


CReliant::CReliant(ifstream & a_LoadStream, ID a_id)
:TShip(a_LoadStream,a_id)
{
    a_LoadStream.read((char*)& m_nDisposition, sizeof (m_nDisposition));
    a_LoadStream.read((char*)& m_Member, sizeof (m_Member));

    m_dViewDistance	      = 100000;
	m_nPhaserPower		  =  2000;
   	m_nShieldTimer		  =    0;
    m_dSteer	          =	0.01;
    m_nMaxHealth          =  100;
    m_nMaxShieldEnergy    =  120;
    m_nCloseIn            = 150;
    m_PhaserColor         = al_map_rgb(200,155,0);
	m_dPhaserAngle        = DEGTORAD(5);
    m_PreferedBase		  = ID_NONE;
	m_ID                  = ID_RELIANT;
	m_blSpockMessage      = false;
    m_strName             = "RELIANT";
	SetImage(g_pReliant);

	#ifdef _DEBUG
	std::cout << "-Reliant loaded\n";
	#endif // _DEBUG
}

void CReliant::Save(ofstream & a_SaveStream)
{
    TShip::Save(a_SaveStream);
    a_SaveStream.write((char*)& m_nDisposition, sizeof (m_nDisposition));
    a_SaveStream.write((char*)& m_Member, sizeof (m_Member));

    #ifdef _DEBUG
	std::cout << "-Reliant saved\n";
	#endif // _DEBUG
}




void CReliant::Do_ai()
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
			m_pTarget = g_pEnterprise;
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

if ((m_pTarget !=NULL) && (m_nDisposition > 10) && (m_dTargetDistance < 300))
{
    if ((m_pTarget->m_lstHealth[HLT_PHOTON]<  40) &&
        (m_pTarget->m_lstHealth[HLT_IMPULSE]< 10))

    {
       m_AI = AI_CONVERSE;
       m_nTorpedoes = 50;
       StopPhaser();
    }
}



if (m_pTarget != NULL)
{
     m_nPhaserEndX = m_pTarget->GetX();
     m_nPhaserEndY = m_pTarget->GetY();

    if ((((m_lstHealth[HLT_PHOTON] < 40) && (m_lstHealth[HLT_PHASER] < 50) &&(m_Member == MEM_ENEMY)))|| (m_nDisposition>=500))
    {
        if (m_dTargetDistance < 1000)
        {
            m_AI = AI_EVADE;
        }
        else
        {
            m_AI = AI_HIDE;
        }
    }
}





if (m_nDisposition>=500)
{
    m_AI= AI_HIDE;
}

switch(m_AI)
      {
      case AI_WANDER:
		  {
                StopPhaser();
                SetSpeed(30);
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
              SetSpeed(m_dTargetDistance/2);

			  double t=m_dAngleSeek-m_dAngle;
              if (t<0) t=-t;


			  if (m_nTask!=TSK_CONTACT)
			  {
                 if ((m_dTargetDistance<=600)&&(m_dTargetDistance>150)&&(t<0.1)&&(m_Member == MEM_ENEMY))
                 {
				     fire_photon();
				 }

                 if (m_dTargetDistance<80) m_AI=AI_EVADE;
			  }

               if ((!m_blSpockMessage) && (m_dTargetDistance < 800) && (m_Member != MEM_ENEMY))
               {
                   g_pCommunication->AddMessage(5, CREW_SPOCK,"They are locking phasers !");
                   m_blSpockMessage = true;
               }


			   if ((m_nPhaserEnergy>=100)&&(m_dTargetDistance<m_nCloseIn)&&((m_lstHealth[HLT_PHASER]>=40)))
               {
                	phaser_on();
                	m_Member = MEM_ENEMY;
                	m_nCloseIn = 300;
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
              SetSpeed(m_dMaxSpeed/2);
			  double t=m_dAngleSeek-m_dAngle;
              if (t<0) t=-t;

			  if (m_nTask!=TSK_CONTACT)
			  {
                 if ((m_nDisposition< 500)&&(m_dTargetDistance<=600)&&(m_dTargetDistance>100)&&(t<0.1))
                 {
				     fire_photon();
				 }

                 if ((m_nDisposition< 500)&&(m_dTargetDistance>300) && ((m_lstHealth[HLT_PHOTON] > 40) || (m_lstHealth[HLT_PHASER] > 50)))
                 {
                     m_AI=AI_CHASE;
                 }
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


      case AI_HIDE:
      {
          SetSpeed(30);
          double dDistance = Distance(m_dX,m_dY,m_dWaypointX,m_dWaypointY);
          if (dDistance<40)
          {
              m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
              m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
          }
      }
      break;


      case AI_CONVERSE:
      {
          if ((g_pEnterprise!= NULL) && (!m_blConversing) && (m_pTarget != NULL))
          {
              if (m_pDialog == NULL)
              {
                  m_pDialog = g_pEnterprise->GetDialog();
                  if (m_pDialog->IsEmpty())
                  {
                       al_set_path_filename(pConversationPath,"khan.xml");
                       ifstream xmlStream;
                       xmlStream.open(al_path_cstr(pConversationPath,ALLEGRO_NATIVE_PATH_SEP));
                       if (xmlStream.is_open())
                       {
                            CDialogParser * pParser = CDialogParser::GetInstance();
                            pParser->SetOutput(m_pDialog);
                            pParser->Read(xmlStream);
                            m_blConversing = true;
                            g_pEnterprise->Hail(m_Member);
                            xmlStream.close();
                       }
                       else
                       {
                             m_pDialog = NULL;
                       }
                  }

              }
              else
              {
                  if (m_pDialog->IsEmpty())
                  {
                      m_pDialog = NULL;
                      m_blConversing = false;
                  }
              }
          }

          if (m_blConversing)
          {

              if (g_pEnterprise == NULL)
              {
                   m_pDialog = NULL;
                   m_AI=AI_WANDER;
                   m_blConversing = false;
              }
              else
              {
                  m_dAngleSeek=WayPoint(g_pEnterprise->GetX(),g_pEnterprise->GetY());
                  if (m_dTargetDistance < 250)
                  {
                      SetSpeed(g_pEnterprise->GetSpeed()+2);
                  }
                  else
                  {
                      SetSpeed(m_dMaxSpeed);
                  }


                 if (m_pDialog->IsEmpty())
                  {
                      int res = m_pDialog->GetResult();
                      switch (res)
                      {
                         case 1:
                         if (g_pEnterprise !=NULL)
                         {
                             g_pCommunication->AddMessage(5, CREW_SPOCK,"Their shields are down !, phasers locked...");
                             g_pEnterprise->m_pTarget = this;
                             g_pEnterprise->SetPhaserTarget(HLT_PHOTON);
                         }
                         m_blShieldOn = false;
                         m_nShieldEnergy =0;
                         m_AI=AI_EVADE;
                         m_pDialog = NULL;
                         m_blConversing = false;
                         m_nDisposition = 0;
                         break;

                         case 2:
                         // end game
                         // Khan wins
                         if (g_pEnterprise !=NULL)
                         {
                            m_nDisposition = 500;
                            g_pCommunication->AddMessage(5, CREW_SPOCK,"Genisis cannot fall into enemy hands ");
                            g_pCommunication->AddMessage(6, CREW_MCCOY,"Are you out of your vulcan mind ?");
                            g_pEnterprise->SetSelfDestruct();
                            m_AI=AI_HIDE;
                            m_pDialog = NULL;
                            m_blConversing = false;
                            m_nDisposition = 0;
                         }
                         break;

                         default:
                          g_pCommunication->AddMessage(5, CREW_SPOCK,"That code did not work");
                         m_AI=AI_CHASE;
                         m_pDialog = NULL;
                         m_blConversing = false;
                         m_nDisposition = 0;
                         break;
                      }


                  }

              }
          }
      }
      break;

     default:
        SetSpeed(30);
        m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
        m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
        m_AI=AI_WANDER;
     break;

     }




if ((m_pTarget==NULL)||(m_lstHealth[HLT_PHASER]<40)||(m_nPhaserEnergy<=20)||(m_dTargetDistance>500)||(m_nDisposition >500))
{
   	StopPhaser();
}


if ((m_pTarget!= NULL) && (m_nDisposition > 10)&&(IsPhaserActive()))
{
    if (((m_pTarget->m_lstHealth[HLT_PHOTON]<= 50) && (m_nPreferedTarget == HLT_PHASER)) ||
        ((m_pTarget->m_lstHealth[HLT_THRUSTER]<= 5) && (m_nPreferedTarget == HLT_THRUSTER)) ||
        ((m_pTarget->m_lstHealth[HLT_PHASER]<= 60) && (m_nPreferedTarget == HLT_PHASER)) ||
        ((m_pTarget->m_lstHealth[HLT_WARPCORE]<= 60) && (m_nPreferedTarget == HLT_WARPCORE)))
        {
            StopPhaser();
        }
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

void CReliant::fire_photon()
{
   if ((m_nPhotonTimer<=0)&&(m_nTorpedoes>0)&&(m_lstHealth[HLT_PHOTON]>50)&&(m_nEnergy>200)&&(m_CloakState == CS_UNCLOAKED))
   {
	 double V = CalcVolume();
     SoundManager::PlaySound(SOUND::PHOTON,V);

	 TBullet * b=NULL;
	 b=new TBullet(m_dX+cos(m_dAngle)*30, m_dY+sin(m_dAngle)*30, m_dSpeed+400 , m_dAngle, m_nZ+1,ID_PHOTON,MEM_ENEMY);

	 if (m_lstHealth[HLT_COMPUTER]<50) m_nPreferedTarget=-1;
     m_pEngine->Add(b);
     m_nPhotonTimer=50;
     m_nTorpedoes--;
     m_nEnergy-=200;
	}
}


void CReliant::phaser_on()
{
     // what would be good to destroy....
     if (!IsPhaserActive())
     {
        m_nPreferedTarget=HLT_WARPCORE;
        if (m_pTarget->m_lstHealth[HLT_IMPULSE]> 5) m_nPreferedTarget   = HLT_IMPULSE;
        else if (m_pTarget->m_lstHealth[HLT_PHOTON]>  50) m_nPreferedTarget   = HLT_PHOTON;
        else if (m_pTarget->m_lstHealth[HLT_PHASER] > 60) m_nPreferedTarget  = HLT_PHASER;
        else if (m_pTarget->m_lstHealth[HLT_THRUSTER] > 10) m_nPreferedTarget  = HLT_THRUSTER;
        else if (m_pTarget->m_lstHealth[HLT_WARPCORE] > 60) m_nPreferedTarget  = HLT_WARPCORE;
        else if (m_pTarget->m_lstHealth[HLT_SHIELD] > 10) m_nPreferedTarget   = HLT_SHIELD;
        else if (m_pTarget->m_lstHealth[HLT_SENSOR] > 40) m_nPreferedTarget  = HLT_SENSOR;
        else m_nPreferedTarget = HLT_COMPUTER;
        StartPhaser(m_ID);
     }
}




bool CReliant::Init()
{
	pConversationPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pConversationPath, "conversation");
    al_append_path_component(pConversationPath, "default");
	al_append_path_component(pConversationPath, "reliant");


	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Ships");
    al_set_path_filename(pPath,"reliant.png");
    g_pReliant = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(pPath);

	if (g_pReliant==NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}
