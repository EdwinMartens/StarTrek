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


#include "Enterprise.h"
#include "Bullet.h"
#include "Probe.h"
#include "Communication.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "Statement.h"


extern bool g_blGodMode;
extern Communication * g_pCommunication;
extern Universe * g_pUniverse;
extern SCREEN_MODE g_nScreenMode;

ALLEGRO_BITMAP * g_pEnterpriseBitmap    = NULL;
ALLEGRO_BITMAP * g_pEngineeringBitmap   = NULL;
ALLEGRO_BITMAP * g_pCommunicationBitmap = NULL;
ALLEGRO_BITMAP * g_pSwitchBitmap        = NULL;
ALLEGRO_BITMAP * g_pPhotonCasing        = NULL;
ALLEGRO_BITMAP * g_pProbeCasing         = NULL;



TEnterprise::TEnterprise()
{
	m_nWarpFactor      =     0;
	m_nProbes          =     5;
	m_nEnergyTimer     =    50;
	m_nMaxShieldEnergy =   100;
	m_nPhaserEnergy	   =   100;
	m_nEnergy	   	   = 10000;
	m_nMaxHealth       =   100;
	m_dMaxSpeed        =   200;

	m_dAimangle		   =     0;
	m_nZ			   =    40;
	m_nPhaserPower	   = ENTERPRISE_PHASER_POWER;
	m_dPhaserAngle     = DEGTORAD(60);
	m_PhaserColor	   = al_map_rgb(255,0,0);

	m_blTorploading    = false;
	m_blProbeloading   = false;
	m_blCrewLoading    = false;
	m_blRepaired       = false;
	m_blWasDocked      = false;
	m_blShieldOn       = false;
	m_blPhaserKeyDown  = false;
	m_blToggleEnable   = true;
	m_blEnergyWarning  = false;

	m_nDestructTimer = 600;
    m_blSelfDestruct = false;

    m_nFlyheight       =  m_nZ;
    m_Member		   =  MEM_FEDERATION;
    m_nShieldEnergy    =  m_nMaxShieldEnergy;
	m_nTorpedoes	   =  MAX_TORPEDOES;
	m_nCrew			   =  MAX_CREW_FEDERATION;

	m_dViewDistance	    = SECTORSIZE;
	m_ID                = ID_PLAYER;
	m_NavigationMode    = NAV_TOPO;
	m_AI                = AI_NONE;
	m_nRepairTimer      = 10000/m_nCrew;
	m_pTransportTarget  = NULL;

	for (int i=0;i<10;i++)
    {
		m_lstHealth.push_back(m_nMaxHealth);
    }

    for (int i =0; i < PLAYED_END; i++)
    {
        m_blPlayed[i] = false;
    }

    for (int i =0; i < CREW_END; i++)
    {
        m_blHasCrewMember[i] = true;
    }

    ResetCrewMember(CREW_SPOCK);


    SetImage(g_pEnterpriseBitmap);
	m_pTransporter = new CTransporter();
    m_pDialog      = new CDialog();
    m_ScanInfo.m_Valid = false;
    m_MissionCritical = MC_SURVIVE;
}

TEnterprise::TEnterprise(ifstream & a_LoadStream, ID a_id )
:TShip(a_LoadStream, a_id)
{
   char chCrewmembers=0;
   a_LoadStream.read((char*)& m_nSectorPositionX, sizeof (m_nSectorPositionX));
   a_LoadStream.read((char*)& m_nSectorPositionY, sizeof (m_nSectorPositionY));
   a_LoadStream.read((char*)& m_nWarpFactor, sizeof (m_nWarpFactor));
   a_LoadStream.read((char*)& m_nProbes, sizeof (m_nProbes));
   a_LoadStream.read((char*)& m_blShieldOn, sizeof (m_blShieldOn));
   a_LoadStream.read(&chCrewmembers, sizeof (char));
   m_pTransporter = new CTransporter(a_LoadStream);
   m_pTransportTarget = NULL;

   SetImage(g_pEnterpriseBitmap);
   m_pDialog      = new CDialog();

   m_dViewDistance	   = SECTORSIZE;
   m_ID                = ID_PLAYER;
   m_NavigationMode    = NAV_TOPO;
   m_AI                = AI_NONE;
   m_PhaserColor	   = al_map_rgba(255,0,0,100);

   m_nPhaserPower	   = ENTERPRISE_PHASER_POWER;
   m_dPhaserAngle      = DEGTORAD(60);

   m_blTorploading    = false;
   m_blProbeloading   = false;
   m_blCrewLoading    = false;
   m_blRepaired       = false;
   m_blWasDocked      = false;
   m_blWasHinted      = false;
   m_blEnergyWarning  = false;
   m_nWarpFactor      =     0;

   m_nDestructTimer   = 600;
   m_blSelfDestruct   = false;

   m_nEnergyTimer     =    50;
   m_nMaxShieldEnergy =   100;
   m_nMaxHealth       =   100;
   m_dMaxSpeed        =   200;
   m_ScanInfo.m_Valid = false;

   for (int i =0; i < CREW_END; i++)
   {
        m_blHasCrewMember[i] = (chCrewmembers >> i) & 1;
   }

   for (int i =0; i < PLAYED_END; i++)
   {
        m_blPlayed[i] = false;
   }

   #ifdef _DEBUG
	std::cout << "-Enterprise loaded\n";
   #endif // _DEBUG
}


void TEnterprise::Save(ofstream & a_SaveStream)
{
   TShip::Save(a_SaveStream);

   char chCrewmembers=0;
   for (int i =0; i < CREW_END; i++)
   {
        if (m_blHasCrewMember[i])
        {
            chCrewmembers |= (1 << i);
        }
        else
        {
            chCrewmembers &= ~(1 << i);
        }
   }

   a_SaveStream.write((char*)& m_nSectorPositionX, sizeof (m_nSectorPositionX));
   a_SaveStream.write((char*)& m_nSectorPositionY, sizeof (m_nSectorPositionY));
   a_SaveStream.write((char*)& m_nWarpFactor, sizeof (m_nWarpFactor));
   a_SaveStream.write((char*)& m_nProbes, sizeof (m_nProbes));
   a_SaveStream.write((char*)& m_blShieldOn, sizeof (m_blShieldOn));
   a_SaveStream.write(&chCrewmembers, sizeof(char));

   if (m_pTransporter != NULL)
   {
       m_pTransporter->Save(a_SaveStream);
   }

   #ifdef _DEBUG
	std::cout << "-Enterprise saved\n";
   #endif // _DEBUG
}


TEnterprise::~TEnterprise()
{
    delete m_pDialog;
    delete m_pTransporter;
}


CTransporter * TEnterprise::GetTransporter()
{
    return m_pTransporter;
}


double TEnterprise::GetX()
{
    return m_dX;
}

double TEnterprise::GetY()
{
    return m_dY;
}

void TEnterprise::SetX(double a_dX)
{
    m_dX = a_dX;
}

void TEnterprise::SetY(double a_dY)
{
    m_dY = a_dY;
}

CDialog * TEnterprise::GetDialog()
{
    return m_pDialog;
}

void TEnterprise::CheatRandomDamage()
{
    for (size_t i=0;i< m_lstHealth.size();i++)
	{
		m_lstHealth[i]-= rand() % 10;
		if (m_lstHealth[i] < 0 ) m_lstHealth[i]=0;
	}
}


void TEnterprise::DoEngineering()
{
	if (m_blSelfDestruct)
    {
        m_nDestructTimer--;
        if (m_nDestructTimer <=0)
        {
            Explode();
        }
    }


	if ((m_blDocked)&&(!m_blReleasing))
	{
		m_dSpeed = 0;
		m_nRepairItem =	-1;
		m_nRepairTimer--;

		bool hlt_ok      = true;
		if (m_nRepairTimer<=0)
		{
			for (size_t i=0;i< m_lstHealth.size();i++)
			{
				if (m_lstHealth[i]<m_nMaxHealth)
				{
					m_lstHealth[i]++;
					hlt_ok=false;
					m_blRepaired = true;
				}
				if (m_lstHealth[i]>m_nMaxHealth) m_lstHealth[i]=m_nMaxHealth;
			}

			if (hlt_ok)
            {
                if (m_blRepaired)
                {
                   g_pCommunication->AddMessage(11,CREW_SCOTTY," She's as good as she can be captain.");
                   m_blRepaired = false;
                }

                if (m_nTorpedoes < MAX_TORPEDOES)
                {
                    if (! m_blTorploading)
                    {
                       g_pCommunication->AddMessage(10,CREW_SCOTTY," Loading Photon torpedoes.");
                       m_blTorploading = true;
                    }
                    m_nTorpedoes++;
                }
                else
                {
                    m_blTorploading = false;

                    if (m_nProbes < MAX_PROBES)
                    {
                        if (! m_blProbeloading)
                        {
                            g_pCommunication->AddMessage(9,CREW_SCOTTY," Loading Probes Now");
                            m_blProbeloading = true;
                        }

                        m_nProbes++;
                    }
                    else
                    {
                        m_blProbeloading = false;
                        //ToDo: if in home sector and crew not max  add crew

                        if ( (m_nSectorPositionX == g_pUniverse->GetHomeX()) &&
                              (m_nSectorPositionY == g_pUniverse->GetHomeY()) &&
                              (m_nCrew < MAX_CREW_FEDERATION))
                              {
                                if (! m_blCrewLoading )
                                {
                                    g_pCommunication->AddMessage(9,CREW_KIRK," Crew is boarding");
                                    m_blCrewLoading = true;
                                }
                                m_nCrew += 5;
                              }
                              else if ( m_blCrewLoading )
                              {
                                 g_pCommunication->AddMessage(9,CREW_SPOCK," The crew is ready Jim !");
                                 m_blCrewLoading = false;
                              }
                    }
                }


                if ((! m_blProbeloading ) && (! m_blTorploading)&&(! m_blWasHinted) && (! m_blCrewLoading))
                {
                    g_pCommunication->AddMessage(8,CREW_SPOCK," I'ts about time to go jim.");
                    m_blWasHinted = true;
                }
            }
            m_nRepairTimer=60;
		}
	}

	if ((m_nRepairItem>-1) && (!m_blDocked))
	{
		// repair item
		m_nRepairTimer--;
		if (m_nRepairTimer<=0)
		{
			m_lstHealth[m_nRepairItem]++;
			if (m_nCrew > 1)
            {
                if (m_nCrew<200) m_nRepairTimer=10000/m_nCrew;
   			    else m_nRepairTimer = ENTERPRISE_REPAIR_DELAY;
            }
            else
            {
                m_nRepairTimer=100000;
            }

			if (m_lstHealth[m_nRepairItem]>=m_nMaxHealth)
			{
				switch (m_nRepairItem)
				{
                   case HLT_COMPUTER:
                        g_pCommunication->AddMessage(1,CREW_SCOTTY," The starship computer is repaired captain !");
                   break;

                   case HLT_HULL:
                        g_pCommunication->AddMessage(2,CREW_SCOTTY," Her hull is stable again !");
                   break;

                   case HLT_IMPULSE:
                        g_pCommunication->AddMessage(3,CREW_SCOTTY," Impulse engines are fully operational !");
                   break;

                   case HLT_PHASER:
                        g_pCommunication->AddMessage(4,CREW_SCOTTY," Phasers are at their best captain !");
                   break;

                   case HLT_PHOTON:
                        g_pCommunication->AddMessage(5,CREW_SCOTTY," Sir, Photon tubes are repaired !");
                   break;

                   case HLT_SENSOR:
                        g_pCommunication->AddMessage(6,CREW_SCOTTY," We have reliable sensors again sir !");
                   break;

                   case HLT_SHIELD:
                        g_pCommunication->AddMessage(7,CREW_SCOTTY," Shield emitters fully functional !");
                   break;

                   case HLT_THRUSTER:
                        g_pCommunication->AddMessage(8,CREW_SCOTTY," Thrusters are repaired !");
                   break;

                   case HLT_WARPCORE:
                        g_pCommunication->AddMessage(9,CREW_SCOTTY," Warpcore is stable captain !");
                        m_blPlayed[WARP_DMG] = false;
                   break;
				}

				m_lstHealth[m_nRepairItem]=m_nMaxHealth;
				m_nRepairItem=-1;
			}
		}
	}


	if (m_nPhaserFireTimer>0) m_nPhaserFireTimer--;
	if (m_nPhotonTimer>0) m_nPhotonTimer--;
	if ((IsPhaserActive())&&(m_nPhaserFireTimer<=0)&&(m_lstHealth[HLT_PHASER]>20))
	{
		m_nPhaserEnergy-=2;
		m_nPhaserFireTimer=5;

		/*
		if (m_pTarget!=NULL)
		{
			m_dTargetDistance = Distance(m_dX,m_dY,m_pTarget->GetX(),m_pTarget->GetY());
			float Eeff=m_nPhaserPower/(m_dTargetDistance+1);
			if (Eeff>10) Eeff=10;
			m_pTarget->CalcPhaserDamage(Eeff,m_nPreferedTarget);
		}
        */
	}
	if ((m_nPhaserEnergy<=0)||(m_lstHealth[HLT_PHASER]<=20)) StopPhaser();

	if (m_nEnergyTimer<=0)
	{
		m_nEnergy-=int(m_dSpeed/10);
		if (m_lstHealth[HLT_WARPCORE]>CORE_CRITICAL)
        {
            m_nEnergy+=int(m_lstHealth[HLT_WARPCORE]*0.6);

            if (m_nEnergy<0) m_nEnergy=0;
		    else if (m_nEnergy>10000) m_nEnergy=10000;
        }
        else
        {
            m_nEnergy-=40;
        }

        if (m_blShieldOn)
        {
            m_nEnergy -=30;
        }

		m_nEnergyTimer = ENTERPRISE_ENERGY_DELAY;
	}
	else m_nEnergyTimer--;



	if ((m_nPhaserTimer<=0) && (m_nEnergy>50) &&(m_nPhaserEnergy<m_lstHealth[HLT_PHASER]))
	{
		m_nPhaserEnergy++;
		m_nEnergy-=6;
		m_nPhaserTimer = ENTERPRISE_PHASERLOAD_DELAY;
	}


	if (m_nPhaserTimer>0) m_nPhaserTimer--;
	if (m_lstHealth[HLT_SENSOR]<40)
	{
		m_pTarget = NULL;
	}

	if ((m_nEnergy>100)&&(m_nShieldTimer<=0)&&(m_nShieldEnergy<(m_nMaxShieldEnergy*(m_lstHealth[HLT_SHIELD])/100)))
	{
		m_nShieldEnergy += 1;
		m_nEnergy -= 2;
		m_nShieldTimer=ENTERPRISE_SHIELDLOAD_DELAY;
	}


    if ((m_nEnergy < 1000) && m_blEnergyWarning)
    {
        g_pCommunication->AddMessage(10,CREW_SCOTTY," Captain, energy is getting very low");
        m_blEnergyWarning  = false;
    }

    if ((m_nEnergy > 2000)&& (!m_blEnergyWarning))
    {
        m_blEnergyWarning  = true;
    }


	if (m_nShieldTimer>0) m_nShieldTimer--;

	if ((m_lstHealth[HLT_WARPCORE] < BREACH_WARP_HLT)&&(m_nEnergy<=0))
   {
       m_lstHealth[HLT_HULL]=0;
   }


   if ((m_lstHealth[HLT_WARPCORE] < ENTERPRISE_CRITICAL_WARP_HLT) && (!m_blPlayed[WARP_DMG]))
   {
        m_blPlayed[WARP_DMG] = true;
        g_pCommunication->AddMessage(10,CREW_SCOTTY," The warpcore is a pile of junk !");
   }




   m_nEnergy = m_nEnergy > 0 ? m_nEnergy : 0;
}


void TEnterprise::SetSelfDestruct()
{
    m_blSelfDestruct = true;
}


void TEnterprise::StartWarp(int a_nWarpFactor)
{
    if (m_nEnergy > ENTERPRISE_MIN_WARP_ENERGY)
    {
        if (m_lstHealth[HLT_WARPCORE]>ENTERPRISE_CRITICAL_WARP_HLT)
        {
            m_nWarpFactor = a_nWarpFactor;
        }
        else
        {
            g_pCommunication->AddMessage(10,CREW_SCOTTY," The warpcore won't hold captain");
        }
    }
    else
    {
        g_pCommunication->AddMessage(10,CREW_SCOTTY," We do not have enough energy !");
    }
}

void TEnterprise::DoWarp()
{
    if ((m_lstHealth[HLT_WARPCORE]>ENTERPRISE_CRITICAL_WARP_HLT) && (m_nEnergy > ENTERPRISE_MIN_WARP_ENERGY))
    {
        m_dSpeed = 300 * m_nWarpFactor;
        m_nEnergy-= m_nWarpFactor;
    }
    else
    {
        StopWarp();
        g_pCommunication->AddMessage(20,CREW_SCOTTY," Dropping out of warp captain ");
    }

}

void TEnterprise::StopWarp()
{
   m_nWarpFactor = 0;
   if (m_dSpeed > m_lstHealth[HLT_IMPULSE]*2)
   {
       m_dSpeed = m_lstHealth[HLT_IMPULSE]*2;
   }
}


void TEnterprise::SignalTargetDesytoyed()
{
    if (!m_blPlayed[TARGET_DESTROYED])
    {
        g_pCommunication->AddMessage(13,CREW_SPOCK," Target destroyed !");
        m_blPlayed[TARGET_DESTROYED] = true;
    }
}


void TEnterprise::Do_ai()
{

    if (g_blGodMode)
    {
        for (size_t i=0;i< m_lstHealth.size();i++)
        {
            if (m_lstHealth[i]<m_nMaxHealth)
            {
                m_lstHealth[i] = m_nMaxHealth;
            }
        }
	m_nEnergy  = 10000;
    }


	// Destruction rules
	if (m_lstHealth[HLT_HULL]<=0)
	{
		if (IsPhaserActive()) StopPhaser();
		Explode();
	}
	else if (m_blDocking)
	{
		Dock(m_pBaseTarget);
	}
	else if (m_blReleasing)
	{
		Release(m_pBaseTarget);
	}
	else
	{
		if (((m_pTarget!=NULL)&&((m_pTarget->m_blDocked)||
             (m_pTarget->m_CloakState == CS_CLOAKED)))||
             (m_lstHealth[HLT_SENSOR]<30)||
             (m_lstHealth[HLT_COMPUTER]<30)) m_pTarget=NULL;

		if (m_pEngine->m_blKeys[SEEK_TARGET])
        {
            if (!m_blDocked)
            {
                if (m_lstHealth[HLT_SENSOR]>30)
                {
                    if (m_lstHealth[HLT_COMPUTER]>30)
                    {
                        m_blPlayed[TARGET_DESTROYED] = false;
                        m_pTarget = (TShip*)m_pEngine->Seek(m_Member,true,m_dViewDistance,m_dX,m_dY);
                        if (m_pTarget!=NULL)
                        {
                            g_pCommunication->AddMessage(11,CREW_SPOCK," Phasers locked on target");
                        }
                    }
                    else
                    {
                        g_pCommunication->AddMessage(11,CREW_SPOCK," The computer is having trouble");
                    }
                }
                else
                {
                    g_pCommunication->AddMessage(12,CREW_SPOCK," Our sensors are damaged");
                }
            }
            else
            {
                g_pCommunication->AddMessage(13,CREW_SPOCK," We are docked jim !");

            }

        }
        else if (m_pEngine->m_blKeys[SEEK_FRIEND])
        {
            if (!m_blDocked)
            {
                if (m_lstHealth[HLT_SENSOR]>30)
                {
                    if (m_lstHealth[HLT_COMPUTER]>30)
                    {
                        m_pTarget = (TShip*)m_pEngine->Seek(m_Member,false,m_dViewDistance,m_dX,m_dY);
                    }
                    else
                    {
                        g_pCommunication->AddMessage(11,CREW_SPOCK," The computer is having trouble");
                    }
                }
                else
                {
                    g_pCommunication->AddMessage(12,CREW_SPOCK," Our sensors are in a weak condition");
                }
            }
            else
            {
                g_pCommunication->AddMessage(13,CREW_SPOCK," We are docked jim !");
            }
        }

        if (m_pTarget != NULL)
        {
            m_nPhaserEndX = m_pTarget->GetX();
            m_nPhaserEndY = m_pTarget->GetY();
        }

        if (m_pEngine->m_blKeys[PHASER])
        {
            if (!m_blDocked)
            {
                if (m_lstHealth[HLT_PHASER]>30)
                {
                    if (m_pTarget!=NULL)
                    {
                        if (m_dTargetDistance < ENTERPRISE_MIN_PHASER_DIST)
                        {
                            if ( m_nPhaserEnergy > 10)
                            {
                                StartPhaser(m_ID);
                                switch(m_pTarget->m_Member)
                                {
                                    case MEM_ROMULAN:
                                        g_pUniverse->StartRomulanWar();
                                    break;

                                    case MEM_KLINGON:
                                        g_pUniverse->StartKlingonWar();
                                    break;

                                    default:
                                    break;
                                }

                            }
                            else
                            {
                                if (IsPhaserActive()) StopPhaser();
                            }
                        }
                        else
                        {
                            g_pCommunication->AddMessage(15,CREW_SPOCK," Target out of range");
                            if (IsPhaserActive()) StopPhaser();
                        }
                    }
                    else
                    {

                        if (IsPhaserActive())
                        {
                            StopPhaser();
                        }
                        else
                        {
                             if (!m_blPhaserKeyDown)
                             {
                                g_pCommunication->AddMessage(15,CREW_SPOCK," There is nothing to lock on");
                             }
                        }
                    }
                }
                else
                {
                    StopPhaser();
                }
            }
            else
            {
                g_pCommunication->AddMessage(14,CREW_SPOCK," Your actions seem illogical");
                StopPhaser();
            }
            m_blPhaserKeyDown = true;
        }
        else
        {
            m_blPhaserKeyDown = false;
            StopPhaser();
        }

		if (m_pEngine->m_blKeys[PHOTON])
    	{
	    	m_dAimangle=m_dAngle;
		    PrecalcFireAngles(75);
			FirePhoton(false);
		}
		else if (m_pEngine->m_blKeys[PHOTON_BACK])
		{
		     m_dAimangle=m_dAngle+PI;
			 PrecalcFireAngles(30);
			 FirePhoton(true);
        }
        else if (m_pEngine->m_blKeys[PROBE])
        {
            LaunchProbe();
        }

        if (m_pEngine->m_blKeys[SHIELD])
        {
            if (m_blToggleEnable)
            {
                ToggleShield();
                m_blToggleEnable = false;
            }
        }
        else
        {
            m_blToggleEnable = true;
        }


		if ((!m_blDocked)&&(!m_blDocking)&&(!m_blReleasing))
		{
            m_blWasDocked = false;
    		if ((!m_blDocked)&&(!m_blDocking)&&(m_pEngine->m_blKeys[DOCK]))
			{
				m_pBaseTarget=(TShip *) m_pEngine->Seekstarbase(MEM_FEDERATION,false,m_dViewDistance,m_dX,m_dY);
				if ((m_pBaseTarget!=NULL)&&(!m_pBaseTarget->m_blDestroyed))
				{
					double b_distance = Distance(m_dX,m_dY,m_pBaseTarget->GetX(),m_pBaseTarget->GetY());
					if (b_distance < 250)
					{
						g_pCommunication->AddMessage(5,CREW_SULU,"Docking at starbase sir.");
						m_pTarget=m_pBaseTarget;
						m_blDocking=true;
						m_blReleasing=false;
					}
				}
			}

            if (m_nWarpFactor>0)
            {
                if ((m_pEngine->m_blKeys[DOWN])||(m_pEngine->m_blKeys[WARP_0]))
                {
                    StopWarp();
                }
                else if (m_pEngine->m_blKeys[LEFT])
                {
                    if (g_nScreenMode == MODE_MAINSCREEN)
                    {
                        m_dAngle-=m_lstHealth[HLT_THRUSTER]*0.0001;
                    }

                }
                else if (m_pEngine->m_blKeys[RIGHT])
                {
                    if (g_nScreenMode == MODE_MAINSCREEN)
                    {
                         m_dAngle+=m_lstHealth[HLT_THRUSTER]*0.0001;
                    }
                }
                else
                {
                    DoWarp();
                }

            }
            else
            {

                if (m_pEngine->m_blKeys[WARP_1])
                {
                    StartWarp(1);
                }
                else if (m_pEngine->m_blKeys[WARP_2])
                {
                    StartWarp(2);
                }
                else if (m_pEngine->m_blKeys[WARP_3])
                {
                    StartWarp(3);
                }
                else if (m_pEngine->m_blKeys[WARP_4])
                {
                    StartWarp(4);
                }
                else if (m_pEngine->m_blKeys[WARP_5])
                {
                    StartWarp(5);
                }

                if (m_pEngine->m_blKeys[UP])
                {
                    if (g_nScreenMode == MODE_MAINSCREEN)
                    {
                        SetSpeed(m_dSpeed+5);
                    }
                }
                else if (m_pEngine->m_blKeys[DOWN])
                {
                    if (g_nScreenMode == MODE_MAINSCREEN)
                    {
                        SetSpeed(m_dSpeed-5);
                    }
                }

                if (m_pEngine->m_blKeys[LEFT])
                {
                    if (g_nScreenMode == MODE_MAINSCREEN)
                    {
                        m_dAngle-=m_lstHealth[HLT_THRUSTER]*0.0006;
                    }


                }
                else if (m_pEngine->m_blKeys[RIGHT])
                {
                    if (g_nScreenMode == MODE_MAINSCREEN)
                    {
                         m_dAngle+=m_lstHealth[HLT_THRUSTER]*0.0006;
                    }
                }
            }

		}

		if ((m_blDocked)&&(!m_blReleasing)&&(m_pEngine->m_blKeys[RELEASE]))
		{
			g_pCommunication->AddMessage(5,CREW_SULU,"Taking her out of dock, captain");
			m_blReleasing=true;
			m_blDocking=false;
		}

		DoEngineering();

		if ((m_pTarget!=NULL)&&(!m_pTarget->m_blDestroyed))
		{
			m_dTargetDistance = Distance(m_dX,m_dY,m_pTarget->GetX(),m_pTarget->GetY());
		}

		if (m_lstHealth[HLT_SENSOR]<30)
		{
			m_pEngine->m_blSensorStatic=true;
		}
		else
		{
			m_pEngine->m_blSensorStatic=false;
		}
	}



   if ((!m_blDocking) && (m_nWarpFactor==0) && (m_dSpeed>(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100))
   {
        m_dSpeed=(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100;
   }

    if (m_dSpeed <0)
    {
        m_dSpeed =0;
    }

    if ((IsPhaserActive())&&(m_pTarget==NULL))
	{
        StopPhaser();
	}


	if ((m_blDocked)&&
        ((m_pBaseTarget==NULL)||((m_pBaseTarget!=NULL)&&
        (m_pBaseTarget->m_blDestroyed))))
    {
       Die();
    }

    if (!m_blWasDocked && m_blDocked)
    {
        g_pCommunication->AddMessage(3,CREW_CHECOV," Enterprise is docked captain !");
        m_blWasDocked = true;
        m_blWasHinted = false;
        m_nRepairTimer=60;
    }

}




void TEnterprise::ToggleShield()
{
     m_blShieldOn = ! m_blShieldOn;
}



void TEnterprise::LaunchProbe()
{
   if (!m_blDocked)
   {
      if (m_nProbes>0)
      {
         if (m_lstHealth[HLT_COMPUTER]>60)
         {

            if (m_lstHealth[HLT_PHOTON]>50)
            {
                if ((m_nPhotonTimer<=0) && (m_nEnergy>200))
                {
                    TProbe * P = NULL;
                    P = new TProbe(m_dX+cos(m_dAngle)*30, m_dY+sin(m_dAngle)*30,m_dSpeed, m_dAngle, m_nZ-1, this);
                    m_pEngine->Add(P);
                    m_nPhotonTimer=40;
                    m_nEnergy-=200;
                    m_nProbes--;
                }
            }
            else
            {
                g_pCommunication->AddMessage(20,CREW_SPOCK,"To launch a probe our photon tubes must be in better shape");
            }
         }
         else
         {
             g_pCommunication->AddMessage(20,CREW_SPOCK,"The enterprise\'s computer is too damaged to analyse the data");
         }
      }
      else
      {
          g_pCommunication->AddMessage(21,CREW_SPOCK,"We ran out of probes captain !");
      }
   }
   else
   {
       g_pCommunication->AddMessage(22,CREW_SPOCK,"Cannot launch a probe inside a starbase !");
   }
}



void TEnterprise::FirePhoton(bool bf)
{
   if (!m_blDocked)
   {
      if (m_nTorpedoes>0)
      {
         if (m_lstHealth[HLT_PHOTON]>50)
         {
            if (m_nEnergy>200)
            {
                if (m_nPhotonTimer<=0)
                {
                    SoundManager::PlaySound(SOUND::PHOTON,1.0);
                    TBullet * b=NULL;
                    if (bf)
                    {
                        b=new TBullet(m_dX+cos(m_dAngle), m_dY+sin(m_dAngle),400-m_dSpeed, m_dAimangle, m_nZ+1,ID_PHOTON,m_Member);
                    }
                    else
                    {
                        b=new TBullet(m_dX+cos(m_dAngle)*30, m_dY+sin(m_dAngle)*30,m_dSpeed + 400, m_dAimangle, m_nZ-1,ID_PHOTON,m_Member);
                    }

                    m_pEngine->Add(b);
                    m_nPhotonTimer=40;
                    m_nTorpedoes--;
                    m_nEnergy-=200;
                }
            }
            else
            {
                g_pCommunication->AddMessage(20,CREW_SPOCK," Not enough energy for Photontubes");
            }

        }
        else
        {
            g_pCommunication->AddMessage(20,CREW_SPOCK," Photontubes are damaged jim, we cannot fire");
        }
     }
     else
     {
         g_pCommunication->AddMessage(21,CREW_SPOCK," There are no photontorpedoes left, we should get to a starbase");
     }
   }
   else
   {
       g_pCommunication->AddMessage(22,CREW_SPOCK," Firing a photon torpedo inside a starbase would ruin your carreer jim");
   }
}



void TEnterprise::LockUp()
{
    if ((m_pTarget!=NULL)&&(!m_pTarget->m_blDestroyed))
    {
       if (m_nPreferedTarget<(int)m_pTarget->m_lstHealth.size())
        {
            m_nPreferedTarget++;
        }
    }
    else
	{
	    m_nPreferedTarget=-1;
	}
}


void TEnterprise::LockDown()
{
    if ((m_pTarget!=NULL)&&(!m_pTarget->m_blDestroyed))
	{
        if (m_nPreferedTarget>-1)
        {
            m_nPreferedTarget--;
        }
	}
	else
	{
	    m_nPreferedTarget=-1;
	}

}


void TEnterprise::DrawTargetInfo(int a_nSelection,int a_nXLocation, int a_nYLocation)
{
	if ((m_pTarget!=NULL)&&(!m_pTarget->m_blDestroyed))
	{
	    al_draw_text(FontManager::GetFont(FONT::INFO),m_pEngine->m_clWHITE, a_nXLocation, a_nYLocation-18, 0,m_pTarget->m_strName.c_str());

	    int nBar = (m_pTarget->m_nShieldEnergy*100)/m_pTarget->m_nMaxShieldEnergy;
	    int nEBar = (m_pTarget->m_nEnergy*100)/10000;
	    al_draw_filled_rectangle(a_nXLocation-5, a_nYLocation+104, a_nXLocation, (a_nYLocation+104)-(nBar),m_pEngine->m_clBLUE);
		al_draw_filled_rectangle(a_nXLocation-10, a_nYLocation+104, a_nXLocation-5, (a_nYLocation+104)-(nEBar),m_pEngine->m_clYELLOW);

		char szShipPart[10];
		ALLEGRO_COLOR BarCol;
		for ( size_t i=0; i<m_pTarget->m_lstHealth.size();i++)
		{
			switch(i)
			{
			case HLT_WARPCORE:
				strcpy(szShipPart,"CORE ");
				break;

			case HLT_IMPULSE:
				 if ( (m_pTarget->m_ID > ID_BASE_BOTTOM)&& (m_pTarget->m_ID < ID_BASE_TOP))
				 {
				    strcpy(szShipPart,"DOCK");
				 }
				 else
				 {
                    strcpy(szShipPart,"IMPLS");
				 }

				break;

			case HLT_THRUSTER:
				 if ( (m_pTarget->m_ID > ID_BASE_BOTTOM)&& (m_pTarget->m_ID < ID_BASE_TOP))
				 {
				    strcpy(szShipPart,"FACIL");
				 }
				 else
				 {
                    strcpy(szShipPart,"THRST");
				 }

				break;

			case HLT_COMPUTER:
				 strcpy(szShipPart,"COMP ");
				break;

			case HLT_SENSOR:
				 strcpy(szShipPart,"SENS ");
				break;

			case HLT_SHIELD:
				 strcpy(szShipPart,"SHLD ");
				break;

			case HLT_PHASER:
				 if (m_pTarget->m_ID == ID_KLINGONBOP)
                 {
                   strcpy(szShipPart,"DISRU");
                 }
                 else
                 {
                   strcpy(szShipPart,"PHAS ");
                 }

				break;

			case HLT_PHOTON:
				 strcpy(szShipPart,"PHOT ");
				break;

			case HLT_HULL:
				 strcpy(szShipPart,"HULL ");
				break;

			case HLT_CLOAK:
				 strcpy(szShipPart,"CLOAK");
				break;


			}

			if ((m_nPreferedTarget!=-1)&&(m_nPreferedTarget==(int)i))
			{
			    al_draw_text(FontManager::GetFont(FONT::INFO),m_pEngine->m_clRED, a_nXLocation, a_nYLocation+(12*i)-4, 0,szShipPart);
			}
			else
			{
                al_draw_text(FontManager::GetFont(FONT::INFO),m_pEngine->m_clWHITE, a_nXLocation, a_nYLocation+(12*i)-4, 0,szShipPart);
			}



			int nHealth = m_pTarget->m_lstHealth[i];
			if (nHealth > 60)
			{
				BarCol=m_pEngine->m_clGREEN;
			}
			else if (nHealth > 30)
			{
				BarCol=m_pEngine->m_clYELLOW;
			}
			else
			{
				BarCol=m_pEngine->m_clRED;
			}

			al_draw_filled_rectangle(a_nXLocation+35, a_nYLocation+(12*i)-1,
                                     a_nXLocation+35+(nHealth*100/(m_pTarget->m_nMaxHealth)),
                                     a_nYLocation+(12*i)+7 ,BarCol);

		}
	}
}




void TEnterprise::SetPhaserTarget(HEALTH a_Subsystem)
{
    m_nPreferedTarget = a_Subsystem;
}


bool TEnterprise::HasCrewMember(crewmember member)
{
    return m_blHasCrewMember[member];
}

void TEnterprise::SetCrewMember(crewmember member)
{
      m_blHasCrewMember[member] = true;
}

void TEnterprise::ResetCrewMember(crewmember member)
{
      m_blHasCrewMember[member] = false;
}


void TEnterprise::draw_engineering()
{
	int nStartX = m_pEngine->m_nScreenMidX-500;
	al_draw_bitmap(g_pEngineeringBitmap, nStartX, 100, 0);
	ALLEGRO_COLOR BarCol;
	for ( size_t i=0; i<m_lstHealth.size();i++)
	{
		int Xpos = (i * 80)+(nStartX + 140);

		if (m_lstHealth[i] > 60)
		{
			BarCol=m_pEngine->m_clGREEN;
		}
		else if (m_lstHealth[i] > 30)
		{
			BarCol=m_pEngine->m_clYELLOW;
		}
		else
		{
			BarCol=m_pEngine->m_clRED;
		}


		al_draw_filled_rectangle(Xpos, 610, Xpos+20, 610-(m_lstHealth[i]*2),BarCol);

		if (m_nRepairItem == (int)i)
		{
			al_draw_bitmap(g_pSwitchBitmap, Xpos+2, 613, ALLEGRO_FLIP_VERTICAL);
		}
		else
		{
			al_draw_bitmap(g_pSwitchBitmap, Xpos+2, 633, 0);
		}

	}

	nStartX+=194;
	al_draw_filled_rectangle(nStartX, 688, nStartX+(m_nEnergy/20), 708,m_pEngine->m_clGREEN);
	if (m_blShieldOn)
    {
        al_draw_filled_rectangle(nStartX, 716, nStartX+(m_nShieldEnergy*5), 736,m_pEngine->m_clGREEN);
    }

    nStartX-=100;
    for (int i=0;i < m_nTorpedoes;i++)
    {
        al_draw_bitmap(g_pPhotonCasing, nStartX+(20*i), 30, 0);
    }
	al_draw_text(FontManager::GetFont(FONT::SMALL),m_pEngine->m_clWHITE, nStartX, 10, 0,"Torpedoes");

	for (int i=0;i < m_nProbes;i++)
    {
        al_draw_bitmap(g_pProbeCasing, nStartX+100+((m_nTorpedoes) * 20)  +(24*i), 30, 0);
    }
	al_draw_text(FontManager::GetFont(FONT::SMALL),m_pEngine->m_clWHITE, nStartX+100+((m_nTorpedoes) * 20), 10, 0,"Probes");

	al_draw_text(FontManager::GetFont(FONT::SMALL),m_pEngine->m_clWHITE, nStartX, 76, 0,"Crew");
	for (int i=0; i < (m_nCrew/2); i++)
    {
        ALLEGRO_COLOR col = m_pEngine->m_clGREEN;
        if (i< 150) col = m_pEngine->m_clYELLOW;
        if (i< 60) col = m_pEngine->m_clRED;

        al_draw_line(nStartX +(2*i),90,nStartX+(2*i),100, col,1);
    }

}


void TEnterprise::draw_communication()
{
    int nStartX = m_pEngine->m_nScreenMidX-510;

	al_draw_bitmap(g_pCommunicationBitmap, nStartX, 0, 0);
	if ((m_pDialog!= NULL)&&(! m_pDialog->IsEmpty()))
    {
        m_pDialog->Draw();
    }
    else
    {
        if (m_ScanInfo.m_Valid)
        {
            draw_probeScan();
        }
    }
}


void TEnterprise::draw_probeScan()
{





}

void TEnterprise::SetScanInfo(TScanInfo scaninfo)
{
    m_ScanInfo = scaninfo;
}




void TEnterprise::DrawBaseInfo(const Sector & a_sector,int a_nX, int a_nY)
{
    size_t n = a_sector.m_Starbase.size();

    if (n>0)
    {
       // plot starbases
       ALLEGRO_COLOR col;
       for ( size_t i=0; i<n; i++)
       {
            switch(a_sector.m_Starbase[i].m_Member)
            {
                case MEM_FEDERATION:
                     col =  m_pEngine->m_clWHITE;
                break;

                case MEM_KLINGON:
                     col =  m_pEngine->m_clRED;
                break;

                case MEM_ROMULAN:
                     col =  m_pEngine->m_clGREEN;
                break;

                default:
                     col =  m_pEngine->m_clYELLOW;
                break;
            }
            int x1= a_nX-2;
            int y1= a_nY-12;
            int x2= a_nX+6;
            int y2= a_nY-32;
            int x3= a_nX+14;
            int y3= a_nY-12;
            al_draw_filled_triangle(x1, y1, x2, y2, x3, y3,col);
        }
    }
}


void TEnterprise::DrawPlanetInfo(const Sector & a_sector,int a_nX, int a_nY)
{
    size_t n = a_sector.m_Planet.size();

    a_nX -= 22;

    if (a_sector.m_blHasAsteroidField)
    {
        al_draw_circle(a_nX, a_nY,15, m_pEngine->m_clYELLOW,2);
    }

    if (n>0)
    {
       // plot starbases
       ALLEGRO_COLOR col;
       int nSize =10;
       int nShift =0;

       for ( size_t i=0; i<n; i++)
       {
            switch(a_sector.m_Planet[i].m_PlanetType)
            {

                case ID_STAR_BLUE:
                    col =  m_pEngine->m_clBLUE;
                    nSize = 7;
                    nShift= 11;
                break;

                case ID_STAR_WHITE:
                    col =  m_pEngine->m_clWHITE;
                    nSize =6;
                    nShift= 10;
                break;

                case ID_STAR_YELLOW:
                    col =  m_pEngine->m_clYELLOW;
                    nSize =8;
                    nShift= 12;
                break;

                case ID_STAR_RED:
                    col =  m_pEngine->m_clRED;
                    nSize =10;
                    nShift= 14;
                break;


                case ID_CLASS_A:
                     col = m_pEngine->m_clRED;
                     nSize = 2;
                     nShift= 5;
                break;

                case ID_CLASS_B:
                     col = m_pEngine->m_clMAGENTA;
                     nSize = 3;
                     nShift= 7;
                break;

                case ID_CLASS_L:
                    col = m_pEngine->m_clBROWN;
                     nSize = 2;
                     nShift= 5;
                break;

                case ID_CLASS_N:
                     col = m_pEngine->m_clYELLOW;
                     nSize = 4;
                     nShift= 9;
                break;

                case ID_CLASS_M1:
                    col = m_pEngine->m_clAQUA;
                    nSize = 2;
                    nShift= 5;
                break;

                case ID_CLASS_M2:
                    col = m_pEngine->m_clAQUA;
                    nSize = 3;
                    nShift= 7;
                break;

                case ID_CLASS_M3:
                    col = m_pEngine->m_clAQUA;
                    nSize = 4;
                    nShift= 9;
                break;

                default:
                     col = m_pEngine->m_clBLACK;
                     nSize = 10;
                     nShift= 14;
                break;

           }

           al_draw_filled_circle(a_nX, a_nY,nSize,col);
           a_nX += nShift;

        }
    }


}


void TEnterprise::DrawOccupationInfo(const Sector & a_sector,int a_nX, int a_nY, int a_nSizeX, int a_nSizeY)
{
    ALLEGRO_COLOR col;
    a_nX -= 38;
    a_nY -= 34;
    switch(a_sector.m_nMember)
    {

        case MEM_FEDERATION:
             col =  m_pEngine->m_clWHITE;
             al_draw_rectangle(a_nX, a_nY, a_nX+(a_nSizeX), a_nY+(a_nSizeY), col,4);
             break;

        case MEM_ROMULAN:
             col =  m_pEngine->m_clGREEN;
             al_draw_rectangle(a_nX, a_nY, a_nX+(a_nSizeX), a_nY+(a_nSizeY), col,4);
             break;

        case MEM_NEUTRAL_ZONE:
             col =  m_pEngine->m_clBROWN;
             al_draw_rectangle(a_nX, a_nY, a_nX+(a_nSizeX), a_nY+(a_nSizeY), col,4);
             break;

        case MEM_KLINGON:
             col =  m_pEngine->m_clRED;
             al_draw_rectangle(a_nX, a_nY, a_nX+(a_nSizeX), a_nY+(a_nSizeY), col,4);
             break;

        default:


        break;

    }


}


void TEnterprise::DrawTacticalInfo(const Sector & a_sector,int a_nX, int a_nY, int a_nSizeX, int a_nSizeY)
{
    for (int i=0; i< (a_sector.m_nFederation1 + a_sector.m_nFederation2 +a_sector.m_nFederation3 );i++)
    {
        int xx = a_nX + (2 * i);
        int yy = a_nY;
        al_draw_filled_circle(xx, yy,1,m_pEngine->m_clWHITE);
    }

    for (int i=0; i< (a_sector.m_nKlingonBC + a_sector.m_nKlingonBOP);i++)
    {
        int xx = a_nX + (2 * i);
        int yy = a_nY + 4;
        al_draw_filled_circle(xx, yy,1,m_pEngine->m_clRED);
    }

    for (int i=0; i< (a_sector.m_nRomulanBOP);i++)
    {
        int xx = a_nX+ (2 * i);
        int yy = a_nY + 6;
        al_draw_filled_circle(xx, yy,1,m_pEngine->m_clGREEN);
    }
}





void TEnterprise::DrawNavigationLegenda(int a_nX, int a_nY)
{
    al_draw_rectangle(a_nX, a_nY,a_nX+300, a_nY+638,m_pEngine->m_clRED,2);

    a_nX +=10;
    a_nY +=20;

    al_draw_filled_circle(a_nX+15,a_nY+5,5,m_pEngine->m_clWHITE);
    al_draw_filled_rectangle(a_nX+3,a_nY+3,a_nX+17,a_nY+7,m_pEngine->m_clWHITE);
    al_draw_filled_rectangle(a_nX,a_nY-3,a_nX+8,a_nY,m_pEngine->m_clWHITE);
    al_draw_filled_rectangle(a_nX,a_nY+10,a_nX+8,a_nY+13,m_pEngine->m_clWHITE);
    al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY-3, 0,"Enterprise is in this sector");

    a_nY +=50;
    int x1= a_nX;
    int y1= a_nY;
    int x2= a_nX+8;
    int y2= a_nY-20;
    int x3= a_nX+16;
    int y3= a_nY;
    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3,m_pEngine->m_clWHITE);
    al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY-18, 0,"Federation starbase");

     a_nY +=40;
     x1= a_nX;
     y1= a_nY;
     x2= a_nX+8;
     y2= a_nY-20;
     x3= a_nX+16;
     y3= a_nY;
     al_draw_filled_triangle(x1, y1, x2, y2, x3, y3,m_pEngine->m_clRED);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY-18, 0,"Klingon starbase");

     a_nY +=40;
     x1= a_nX;
     y1= a_nY;
     x2= a_nX+8;
     y2= a_nY-20;
     x3= a_nX+16;
     y3= a_nY;
     al_draw_filled_triangle(x1, y1, x2, y2, x3, y3,m_pEngine->m_clGREEN);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY-18, 0,"Romulan starbase");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,7,m_pEngine->m_clBLUE);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class B star");

     a_nY +=40;
     al_draw_filled_circle(a_nX+10, a_nY+20,6,m_pEngine->m_clWHITE);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class F star");

     a_nY +=40;
     al_draw_filled_circle(a_nX+10, a_nY+20,8,m_pEngine->m_clYELLOW);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class G star");

     a_nY +=40;
     al_draw_filled_circle(a_nX+10, a_nY+20,10,m_pEngine->m_clRED);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class M star");

     a_nY +=50;
     al_draw_filled_circle(a_nX+10, a_nY+20,2,m_pEngine->m_clRED);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class A planet (Hostile)");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,3,m_pEngine->m_clMAGENTA);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class B planet (Hostile)");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,2,m_pEngine->m_clBROWN);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class L planet (Marginal)");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,4,m_pEngine->m_clYELLOW);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class N planet (Gass Gigant)");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,2,m_pEngine->m_clAQUA);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class M1 planet (Terrestrial)");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,3,m_pEngine->m_clAQUA);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class M2 planet (Terrestrial)");

     a_nY +=30;
     al_draw_filled_circle(a_nX+10, a_nY+20,4,m_pEngine->m_clAQUA);
     al_draw_text(FontManager::GetFont(FONT::TEXT),m_pEngine->m_clWHITE, a_nX+30, a_nY+12, 0,"A class M3 planet (Terrestrial)");

}


void TEnterprise::IncNavX()
{
    if (m_nNavigationXOffset<UNIVERSE_SIZE-12)
    {
        m_nNavigationXOffset++;
    }
}

void TEnterprise::DecNavX()
{
    if (m_nNavigationXOffset>12)
    {
        m_nNavigationXOffset--;
    }
}

void TEnterprise::IncNavY()
{
    if (m_nNavigationYOffset<UNIVERSE_SIZE-12)
    {
        m_nNavigationYOffset++;
    }
}

void TEnterprise::DecNavY()
{
    if (m_nNavigationYOffset>12)
    {
        m_nNavigationYOffset--;
    }
}



void TEnterprise::Draw_Navigation(Universe * a_pUniverse)
{
// To_Do: Implement Long range screen !
    int XStart = m_pEngine->m_nScreenMidX-500;
    int YStart = 50;
    int XEnd   = XStart + 638;
    int YEnd   = 688;

    int DeltaX  = XEnd - XStart;
    int DeltaY  = YEnd - YStart;
    int StepX   = DeltaX / 11;
    int StepY   = DeltaY / 11;
    int CenterX = StepX / 2 +9;
    int CenterY = StepY / 2 + 3;

    DrawNavigationLegenda(XStart + 640, 50);

    for (int y=0;y<=11;y++)
    {
        al_draw_line(XStart,YStart + (y*StepY),XEnd,YStart + (y*StepY),m_pEngine->m_clRED,2);

        for (int x=0; x<=11;x++)
        {
            al_draw_line(XStart+ (x*StepX),YStart ,XStart+ (x*StepX),YEnd ,m_pEngine->m_clRED,2);

            if ((x>0) && (y>0))
            {
                int nColumn =  x+m_nNavigationXOffset;
                int nRow    =  y+m_nNavigationYOffset;

                if (nColumn >= UNIVERSE_SIZE)
                {
                    nColumn -= UNIVERSE_SIZE;
                }
                else if (nColumn <0)
                {
                    nColumn += UNIVERSE_SIZE;
                }

                if (nRow >= UNIVERSE_SIZE)
                {
                    nRow -= UNIVERSE_SIZE;
                }
                else if (nRow <0)
                {
                    nRow += UNIVERSE_SIZE;
                }

                // Draw the Sector Info
                // plot enterprise position
                if ((nColumn == m_nSectorPositionX)&& (nRow == m_nSectorPositionY))
                {
                    al_draw_filled_circle   (XStart + (x-1)*StepX+15,YStart + (y-1)*StepY+7,5,m_pEngine->m_clWHITE);
                    al_draw_filled_rectangle(XStart + (x-1)*StepX+3, YStart + (y-1)*StepY+5,  XStart +(x-1)*StepX+17, YStart + (y-1)*StepY+9, m_pEngine->m_clWHITE);
                    al_draw_filled_rectangle(XStart + (x-1)*StepX,   YStart + (y-1)*StepY-1,  XStart +(x-1)*StepX+8,  YStart + (y-1)*StepY+2, m_pEngine->m_clWHITE);
                    al_draw_filled_rectangle(XStart + (x-1)*StepX,   YStart + (y-1)*StepY+12, XStart +(x-1)*StepX+8,  YStart + (y-1)*StepY+15,m_pEngine->m_clWHITE);
                }

                switch (m_NavigationMode)
                {
                    case NAV_TOPO:
                         if ((a_pUniverse->m_nSector[nColumn][nRow].m_blKnown)||(g_blGodMode))
                         {
                            DrawPlanetInfo(a_pUniverse->m_nSector[nColumn][nRow],XStart + (x-1)*StepX+CenterX,YStart + (y-1)*StepY+CenterY);
                         }
                         else
                         {
                             al_draw_text(FontManager::GetFont(FONT::SMALL),m_pEngine->m_clWHITE, XStart + (x-1)*StepX+CenterX,YStart + (y-1)*StepY+CenterY, 0,"?");
                         }
                         break;

                        case NAV_TACTICAL:
                             if ((a_pUniverse->m_nSector[nColumn][nRow].m_blKnown)||(g_blGodMode))
                             {
                                DrawTacticalInfo(a_pUniverse->m_nSector[nColumn][nRow],XStart + (x-1)*StepX+CenterX,YStart + (y-1)*StepY+CenterY,StepX,StepY);
                             }
                             else
                             {
                                al_draw_text(FontManager::GetFont(FONT::SMALL),m_pEngine->m_clWHITE, XStart + (x-1)*StepX+CenterX,YStart + (y-1)*StepY+CenterY, 0,"?");
                             }

                        break;

                        case NAV_OCCUPATION:
                             DrawOccupationInfo(a_pUniverse->m_nSector[nColumn][nRow],XStart + (x-1)*StepX+CenterX,YStart + (y-1)*StepY+CenterY,StepX,StepY);
                             DrawBaseInfo(a_pUniverse->m_nSector[nColumn][nRow],XStart + (x-1)*StepX+CenterX-4,YStart + (y-1)*StepY+CenterY+4);
                        break;

                        default:
                        break;
                    }

            }


        }
    }
    int nVal = m_lstHealth[HLT_COMPUTER]+m_lstHealth[HLT_SENSOR];

    for (int i=0;i< (150 - nVal) ;i++)
    {
        int nX = rand() % 800;
        int nY = rand() % 800;
        al_draw_filled_circle(nX,nY,(150 - nVal),m_pEngine->m_clBLACK);
    }

    //al_draw_rectangle(XStart+ (5*StepX),YStart + (5*StepY),XStart+ (6*StepX),YStart + (6*StepY),m_pEngine->m_clBLUE,2);

}


void TEnterprise::SetNavigationMode(NAVIGATION_MODE a_Mode)
{
    m_NavigationMode = a_Mode;
}


void TEnterprise::SetDefaultNavigationOrigin()
{
    m_nNavigationXOffset = m_nSectorPositionX-6;
    m_nNavigationYOffset = m_nSectorPositionY-6;
}

void TEnterprise::DNavOriginX(int a_nOffset)
{
   m_nNavigationXOffset+=a_nOffset;
}

void TEnterprise::DNavOriginY(int a_nOffset)
{
    m_nNavigationYOffset+=a_nOffset;
}


void TEnterprise::Hail(MEMBER a_Member)
{
   g_pCommunication->AddMessage(1,CREW_UHURA," We are being hailed !");
}

void TEnterprise::UpdateStarmap(int a_dXOffset, int a_dYOffset)
{
    int cUpdateX = m_nSectorPositionX + a_dXOffset -1;
    int cUpdateY = m_nSectorPositionY + a_dYOffset -1;

    for ( int y=0; y<3 ; y++)
    {
        for (int x=0; x<3; x++)
        {
           if (((cUpdateX + x) >= 0)&&((cUpdateX + x) <UNIVERSE_SIZE)&&
               ((cUpdateY + y) >= 0)&&((cUpdateY + y) <UNIVERSE_SIZE))
              {
                    if (g_pUniverse!=NULL)
                    {
                       g_pUniverse->m_nSector[cUpdateX + x][cUpdateY + y].m_blKnown = true;
                    }
              }
        }
    }

    g_pCommunication->AddMessage(10,CREW_SPOCK,"Captain, our probe has returned the data !");
}


void TEnterprise::MouseMove(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    switch (g_nScreenMode)
    {
        case MODE_COMMUNICATION:

            if (m_pDialog != NULL )
            {
                m_pDialog->OnMouseMove(mouse_event);
            }
        break;

        default:

        break;
    }
}

void TEnterprise::MouseButtonDown(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
     switch (g_nScreenMode)
     {
        case MODE_ENGINEERING:
        if ((mouse_event.button == 1)&&(mouse_event.y > 613)&&(mouse_event.y < 663))
        {
            int nStartX = m_pEngine->m_nScreenMidX-500;
            int Xpos; bool blFound = false;
            size_t i=0;
            while((i < m_lstHealth.size())&&(!blFound))
            {
                Xpos = (i * 80)+(nStartX + 140);
                if ((mouse_event.x > Xpos)&&(mouse_event.x < Xpos+20))
				{
					SoundManager::PlaySound(SOUND::ON,0.2);
					m_nRepairItem = i;
					blFound = true;
				}
				i++;
            }
        }
        break;

        case MODE_COMMUNICATION:

             if ((m_pDialog != NULL )&&
                 (mouse_event.y < 380))
             {
                m_pDialog->OnMouseButtonDown(mouse_event);
             }
             else if ((m_pTransporter != NULL)&&
                      (mouse_event.button == 1) && (mouse_event.y > 380))
             {
                if ((m_pTransporter->IsEmpty()) && (m_pTransportTarget!= NULL))
                {
                    if ((m_pTransportTarget->m_ID>ID_PLANET_BOTTOM) && (m_pTransportTarget->m_ID<ID_PLANET_TOP))
                    {
                        TSpaceObject * pTarget = (TSpaceObject * )m_pTransportTarget;
                        if (pTarget->HasInventoryItem())
                        {
                            TInventoryItem item = pTarget->GetInventoryItem();
                            if (item.m_blValid)
                            {
                                m_pTransporter->Target(item);
                            }
                        }
                    }
                    else if (m_pTransportTarget->m_ID == ID_FEDERATIONBASE)
                    {
                        TStarbase * pTarget = (TStarbase * )m_pTransportTarget;
                        if (pTarget->HasInventoryItem())
                        {
                            TInventoryItem item = pTarget->GetInventoryItem();
                            if (item.m_blValid)
                            {
                                m_pTransporter->Target(item);
                            }
                        }
                    }
                }

                int nDist = 10000;
                if (m_pTransportTarget != NULL)
                {
                    nDist = Distance(m_dX, m_dY,m_pTransportTarget->GetX(), m_pTransportTarget->GetY());
                }

                TResponse response =  m_pTransporter->ProcessMouseEvent(mouse_event, m_lstHealth[HLT_TRANS], nDist, m_blShieldOn);


                switch (response)
                {
                    case RE_NOTARGET:
                        g_pCommunication->AddMessage(13,CREW_SCOTTY," There is nothing there captain");
                    break;

                    case RE_DAMAGED:
                       g_pCommunication->AddMessage(13,CREW_SCOTTY," We cannot use the transporter sir...");
                       g_pCommunication->AddMessage(14,CREW_SCOTTY," It's a total mess !");
                    break;

                    case RE_NOTINRANGE:
                        g_pCommunication->AddMessage(13,CREW_SCOTTY," We have to get closer");
                    break;

                    case RE_SHIELD:
                        g_pCommunication->AddMessage(13,CREW_SCOTTY," We cannot use the transporter with our shield up");
                    break;

                    case RE_BEAM_UP:
                    if (m_pTransportTarget != NULL)
                    {
                        if ((m_pTransportTarget->m_ID>ID_PLANET_BOTTOM) && (m_pTransportTarget->m_ID<ID_PLANET_TOP))
                        {
                           ((TSpaceObject *) m_pTransportTarget)->RemoveInventoryItem();
                        }
                        else if (m_pTransportTarget->m_ID == ID_FEDERATIONBASE)
                        {
                          ((TStarbase *) m_pTransportTarget)->RemoveInventoryItem();
                        }
                    }
                    break;

                    case RE_BEAM_DOWN:
                    {
                        TInventoryItem item = m_pTransporter->GetBeamedDownItem();
                         if (m_pTransportTarget != NULL)
                        {
                            if ((m_pTransportTarget->m_ID>ID_PLANET_BOTTOM) && (m_pTransportTarget->m_ID<ID_PLANET_TOP))
                            {
                                ((TSpaceObject *) m_pTransportTarget)->SetInventoryItem(item);
                            }
                            else if (m_pTransportTarget->m_ID == ID_FEDERATIONBASE)
                            {
                                ((TStarbase *) m_pTransportTarget)->SetInventoryItem(item);
                            }
                        }
                    }
                    break;

                    default:
                    break;

                }

        }
        break;

        default:
        break;
     }

}





void TEnterprise::MouseButtonUp(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    switch (g_nScreenMode)
    {
        case MODE_COMMUNICATION:
            if (m_pDialog != NULL )
             {
                m_pDialog->OnMouseButtonUp(mouse_event);
             }
        break;

        default:
        break;
    }
}


void TEnterprise::GetTransportTarget()
{
    m_pTransportTarget = m_pEngine->SeekClosestTransportTarget(m_dX,m_dY);
}



bool TEnterprise::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Ships");
    al_set_path_filename(pPath,"ent01.png");
    g_pEnterpriseBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);
	if (g_pEnterpriseBitmap == NULL)  return false;


	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Interface");
    al_set_path_filename(pPath,"eng01.png");
    g_pEngineeringBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);
	if (g_pEngineeringBitmap == NULL)  return false;


	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Interface");
    al_set_path_filename(pPath,"Com_Trans.png");
    g_pCommunicationBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);
	if (g_pCommunicationBitmap == NULL)  return false;


	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Interface");
    al_set_path_filename(pPath,"Switch.png");
    g_pSwitchBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);
	if (g_pSwitchBitmap == NULL)  return false;


    pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Photon");
    al_set_path_filename(pPath,"casing.png");
    g_pPhotonCasing = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);
	if (g_pPhotonCasing == NULL)  return false;


	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "probe");
    al_set_path_filename(pPath,"casing.png");
    g_pProbeCasing = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);
	if (g_pProbeCasing == NULL)  return false;


	return true;
}
