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

#include "StarTrekV3.h"
#include "Romulan_BOP.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "Enterprise.h"
#include "Communication.h"
#include <iostream>


extern Communication * g_pCommunication;
extern TEnterprise   * g_pEnterprise;
extern Universe      * g_pUniverse;

ALLEGRO_BITMAP * g_pRomulanBOPBitmap = NULL;
ALLEGRO_PATH   * pRomulanConversationPath;

bool TRomulanBop::m_blConversing = false;
CDialog * TRomulanBop::m_pDialog = NULL;


TRomulanBop::TRomulanBop()
{
	m_dSteer	          =	  0.01;
	m_nMaxShieldEnergy    =   100;
	m_nMaxHealth          =   100;
	m_nPhaserEnergy	      =	  100;
	m_nTorpedoes	      =	  200;
	m_nEnergy	          =	10000;
	m_nShieldTimer		  =     0;
	m_dAimAngle			  =     0;
	m_dMaxSpeed           =   190;
	m_nZ				  =    39;
	m_nPhaserPower	      = ROMULAN_BOP_PHASER_POWER;
	m_dViewDistance	      = 10000;
	m_nPhaserPower		  =  2000;
	m_nMaxPhaserDist      =   300;

	m_nFlyheight		  = m_nZ;
	m_PhaserColor	      = al_map_rgba(0,255,0,150);
	m_dPhaserAngle        = DEGTORAD(5);
	m_nShieldEnergy       = m_nMaxShieldEnergy;

	m_blCanCloak          = true;
	m_blBeamSound         = false;

    m_nCrew			      = MAX_CREW_ROMULAN;
	m_Member			  = MEM_ROMULAN;
	m_PreferedBase		  = ID_ROMULAN_BASE;
	m_ID                  = ID_ROMULANBOP;
	m_AI				  = AI_WANDER;
	m_CloakState          = CS_UNCLOAKED;
	m_nDisposition        = 10;

    m_strName             = "ROMULAN BOP";
	SetImage(g_pRomulanBOPBitmap);

	for (int i=0;i<10;i++)
    {
		m_lstHealth.push_back(m_nMaxHealth);
    }
}

TRomulanBop::TRomulanBop(ifstream & a_LoadStream,ID a_id)
:TShip(a_LoadStream,a_id)
{
   	a_LoadStream.read((char*)& m_nDisposition, sizeof (m_nDisposition));

   	m_dViewDistance	      = 10000;
	m_nPhaserPower		  =  2000;
   	m_nShieldTimer		  =    0;
    m_dSteer	          =	0.01;
    m_nMaxHealth          = 100;
    m_nMaxShieldEnergy    = 100;
    m_PhaserColor	      = al_map_rgb(0,255,0);
	m_dPhaserAngle        = DEGTORAD(5);
    m_Member			  = MEM_ROMULAN;
	m_PreferedBase		  = ID_ROMULAN_BASE;
	m_ID                  = ID_ROMULANBOP;
    m_strName             = "ROMULAN WB";
    m_blBeamSound         = false;

	SetImage(g_pRomulanBOPBitmap);

	#ifdef _DEBUG
	std::cout << "-Romulan WB loaded\n";
	#endif // _DEBUG
}

void TRomulanBop::Save(ofstream & a_SaveStream)
{
    TShip::Save(a_SaveStream);
    a_SaveStream.write((char*)& m_nDisposition, sizeof (m_nDisposition));

    #ifdef _DEBUG
	std::cout << "-Romulan WB saved\n";
	#endif // _DEBUG
}


void TRomulanBop::Do_ai()
{

	// Destruction rules
	if (m_lstHealth[HLT_HULL]<=0)
	{
		Explode();
	}
	else
	{
        int nState = GetCurrentSectorState();

		double tsx,tsy,t,shotlead;
		shotlead=0;
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
				m_pTarget = (TShip *) m_pEngine->Seek(MEM_ROMULAN,true,m_dViewDistance,m_dX,m_dY);
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
			if (m_pTarget != NULL)
            {
               m_nPhaserEndX = m_pTarget->GetX();
               m_nPhaserEndY = m_pTarget->GetY();
            }

			m_dTargetDistance = Distance(m_dX,m_dY,m_pTarget->GetX(),m_pTarget->GetY());
			shotlead = m_dTargetDistance/600;
            if (m_AI==AI_WANDER) m_AI=AI_CHASE;

            if ((m_pTarget->m_ID == ID_PLAYER)&&(g_pEnterprise!=NULL))
            {
                bool blWar = g_pUniverse->WarWithRomulan();
                if ((m_nDisposition > 20) && (!blWar))
                {
                    if (nState == MEM_ROMULAN)
                    {
                        if (m_AI!=AI_ESCORT)
                        {
                            m_nEscortTimer = 1000;
                        }
                        m_AI=AI_ESCORT;
                    }
                    else
                    {
                        m_AI=AI_WANDER;
                    }

                }
                else if ((m_nDisposition > 5) && (!blWar) && (!m_blHadConversation))
                {
                    if (m_dTargetDistance <= 600)
                    {
                        m_AI = AI_CONVERSE;
                    }
                }
                else if ((m_nDisposition <= 0)||(blWar))
                {
                    if (m_AI==AI_WANDER) m_AI=AI_CHASE;
                    int nState = GetCurrentSectorState();
                    if ((!blWar)&&((nState == MEM_ROMULAN)))
                    {
                        g_pUniverse->StartRomulanWar();
                    }
                }
                else
                {
                    m_AI=AI_WANDER;
                }
            }

           //preferedtarget=HLT_SENSOR+1;

           if (m_nTask==TSK_STANDARD)
			{
				if ((m_lstHealth[HLT_PHOTON]<50)&&(m_lstHealth[HLT_PHASER]<=20)) m_AI=AI_EVADE;
            }
   }

if (((m_lstHealth[HLT_HULL]<60)||(m_lstHealth[HLT_PHOTON]<40)||(m_nTorpedoes==0)) && (m_pTarget!=NULL))
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
           if (m_lstHealth[i] < (m_nMaxHealth)) blShouldDock = true;
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


switch(m_AI)
      {
      case AI_WANDER:
		  {
           SetSpeed(200);
           m_dAngleSeek=WayPoint(m_dWaypointX,m_dWaypointY);
		   double dDistance = Distance(m_dX,m_dY,m_dWaypointX,m_dWaypointY);
		   if (dDistance<40)
           {
                m_dWaypointX=rand()% (SECTORSIZE * 80 / 100 );
                m_dWaypointY=rand()% (SECTORSIZE * 80  / 100 );
           }
		   if ((m_nCloakCharge>=CLOAK_DELAY)&&(m_lstHealth[HLT_CLOAK]>60)&&(m_nEnergy>500)&&((m_CloakState == CS_UNCLOAKED)||(m_CloakState == CS_DECLOAKING)))
			{
                double V = CalcVolume();
                SoundManager::PlaySound(SOUND::CLOACK,V);
				m_CloakState = CS_CLOAKING;
			}
		  }
      break;


      case AI_ESCORT:
           if ((nState == MEM_ROMULAN)&&(g_pEnterprise != NULL) && (!g_pEnterprise->IsDestroyed()))
           {
                m_pTarget = g_pEnterprise;
                m_dAngleSeek=WayPoint(g_pEnterprise->GetX(),g_pEnterprise->GetY());
                SetSpeed(m_dMaxSpeed);
                m_nEscortTimer--;
                if (m_nEscortTimer<=0)
                {
                    m_nDisposition = 0;
                    m_AI = AI_CHASE;
                }
           }
           else
           {
               m_AI = AI_WANDER;
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
                       bool blOk = false;

                       switch (nState)
                       {
                            case MEM_ROMULAN:
                            al_set_path_filename(pRomulanConversationPath,"romulanspace.xml");
                            blOk = true;
                            break;

                            case MEM_FEDERATION:

                            break;

                            case MEM_NEUTRAL_ZONE:
                            al_set_path_filename(pRomulanConversationPath,"neutralzone.xml");
                            blOk = true;
                            break;

                       }

                       if (blOk)
                       {
                           ifstream xmlStream;
                           xmlStream.open(al_path_cstr(pRomulanConversationPath,ALLEGRO_NATIVE_PATH_SEP));
                           if (xmlStream.is_open())
                           {
                               CDialogParser * pParser = CDialogParser::GetInstance();
                               pParser->SetOutput(m_pDialog);
                               pParser->Read(xmlStream);
                               m_blConversing = true;
                               g_pEnterprise->Hail(m_Member);
                               xmlStream.close();
                               m_nEscortTimer = 300;
                           }
                           else
                           {
                               m_pDialog = NULL;
                           }
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

              if ((m_pDialog->IsEmpty()) && (m_nDisposition > 20))
              {
                  m_nEscortTimer--;
              }

              if (m_nEscortTimer <= 0)
              {
                  m_AI=AI_WANDER;
                  m_blHadConversation = true;
                  m_nDisposition = 0;
              }

              if (g_pEnterprise == NULL)
              {
                   m_pDialog = NULL;
                   m_AI=AI_WANDER;
                   m_blHadConversation = true;
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

                  // handle response

                 if (m_pDialog->IsEmpty())
                  {
                      m_nDisposition = m_pDialog->GetResult();
                      m_AI=AI_WANDER;
                      m_blHadConversation = true;
                      m_pDialog = NULL;
                      m_blConversing = false;
                  }

              }
          }
      }
      break;


      case AI_CHASE:
        if (m_pTarget!=NULL)
           {
              if (((m_CloakState == CS_CLOAKED)||(m_CloakState == CS_CLOAKING))&&(m_dTargetDistance<1000))
			  {
			  		double V = CalcVolume();
                    SoundManager::PlaySound(SOUND::DECLOACK,V);
					m_CloakState = CS_DECLOAKING;
					m_nCloakCharge= 0;
			  }

			  tsx=m_pTarget->GetX()+cos(m_pTarget->GetAngle())*m_pTarget->GetSpeed()*shotlead;
              tsy=m_pTarget->GetY()+sin(m_pTarget->GetAngle())*m_pTarget->GetSpeed()*shotlead;
              m_dAngleSeek=WayPoint(tsx,tsy);
              if (m_dTargetDistance < 250)
              {
                  SetSpeed(m_pTarget->GetSpeed()+2);
              }
              else
              {
                  SetSpeed(m_dMaxSpeed);
              }


			  double t=m_dAngleSeek-m_dAngle;
              if (t<0) t=-t;


			  if (m_nTask!=TSK_CONTACT)
			  {

                 if ((m_dTargetDistance<=600)&&(m_dTargetDistance>150)&&(t<0.1))
                 {
				     fire_photon();
				 }

                 if ((m_nPhaserEnergy>=100)&&(m_dTargetDistance<m_nMaxPhaserDist)&&(t<m_dPhaserAngle))
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


				 if (m_dTargetDistance<200) m_AI=AI_EVADE;
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
        if (m_pTarget!=NULL)
           {
              if ((m_nCloakCharge>=CLOAK_DELAY)&&(m_lstHealth[HLT_CLOAK]>60)&&(m_nEnergy>500)&&((m_CloakState == CS_UNCLOAKED)||(m_CloakState == CS_DECLOAKING)))
			  {
					double V = CalcVolume();
                    SoundManager::PlaySound(SOUND::CLOACK,V);
					m_CloakState = CS_CLOAKING;
			  }

           m_dAngleSeek=WayPoint(m_pTarget->GetX(),m_pTarget->GetY())+PI;
           SetSpeed(m_dMaxSpeed/2);
           if (m_dTargetDistance>800) m_AI=AI_CHASE;
           tsx=m_pTarget->GetX()+cos(m_pTarget->GetAngle())*m_pTarget->GetSpeed()*shotlead;
           tsy=m_pTarget->GetY()+sin(m_pTarget->GetAngle())*m_pTarget->GetSpeed()*shotlead;
           double sangleseek=WayPoint(tsx,tsy);
           t=sangleseek-m_dAngle;
           if (t<0) t=-t;
           if ((m_dTargetDistance<=600)&&(m_dTargetDistance>150)&&(t<0.2))
              {
                 fire_photon();
              }

              if (m_dTargetDistance>800) m_AI=AI_CHASE;
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

if ((m_nEnergy<=0)&& (m_dSpeed>0)) m_dSpeed=0;



DoEngineering();

if (m_nEnergy>0)
   {
      m_dSteer = m_lstHealth[HLT_THRUSTER]*0.0003;

      m_dAngleSeek = GetEvasiveAngle(m_dAngleSeek);

      Control();
   }
   else if (m_lstHealth[HLT_WARPCORE]<20)
   {
       m_lstHealth[HLT_HULL]=0;
   }
}

if (((m_nEnergy<20)||(m_lstHealth[HLT_CLOAK]<50))&&((m_CloakState == CS_CLOAKED)||(m_CloakState == CS_CLOAKING)))
{
	double V = CalcVolume();
    SoundManager::PlaySound(SOUND::DECLOACK,V);
	m_CloakState = CS_DECLOAKING;
	m_nCloakCharge=0;
}

if (m_dSpeed>(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100)
    {
        m_dSpeed=(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100;
    }

    if (m_dSpeed <0)
    {
        m_dSpeed =0;
    }

    if ((m_blDocked)&&
        ((m_pBaseTarget==NULL)||((m_pBaseTarget!=NULL)&&
        (m_pBaseTarget->m_blDestroyed))))
    {
       Die();
    }


}



void TRomulanBop::DoCollision(TSprite * a_pSprite)
{
    TShip::DoCollision(a_pSprite);

    if ( (a_pSprite!=NULL) &&
         (a_pSprite->m_ID >= ID_BULLET_BOTTOM) &&
         (a_pSprite->m_ID<=ID_BULLET_TOP) &&
         (a_pSprite->m_Member==MEM_FEDERATION) )
    {
        if ((m_AI == AI_CONVERSE)||(m_AI == AI_ESCORT))
        {
            m_AI = AI_CHASE;
        }
        m_nDisposition = 0;

        if ( g_pEnterprise != NULL)
        {
             CDialog * pDialog = g_pEnterprise->GetDialog();

             if (pDialog != NULL)
             {
                 pDialog->Clear();
                 g_pCommunication->AddMessage(20,CREW_UHURA," Hail was canceled");
             }

             g_pUniverse->StartRomulanWar();
        }
    }
}



void TRomulanBop::fire_photon()
{
   if ((m_nPhotonTimer<=0)&&(m_nTorpedoes>0)&&(m_lstHealth[HLT_PHOTON]>50)&&(m_nEnergy>200)&&(m_CloakState == CS_UNCLOAKED))
   {
	TBullet * b=NULL;
	 b=new TBullet(m_dX+cos(m_dAngle), m_dY+sin(m_dAngle),m_dSpeed+400 , m_dAngle, m_nZ+1,ID_PLASMA,m_Member);

	 if (m_lstHealth[HLT_COMPUTER]<50) m_nPreferedTarget=-1;
     m_pEngine->Add(b);
     m_nPhotonTimer=40;
     m_nTorpedoes--;
     m_nEnergy-=200;
	}
}


bool TRomulanBop::Init()
{
	pRomulanConversationPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pRomulanConversationPath, "conversation");
    al_append_path_component(pRomulanConversationPath, "default");
	al_append_path_component(pRomulanConversationPath, "romulan");

	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Ships");
    al_set_path_filename(pPath,"romulanbop.png");
    g_pRomulanBOPBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(pPath);

	if (g_pRomulanBOPBitmap==NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}


