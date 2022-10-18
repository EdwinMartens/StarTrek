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

#include <cmath>
#include <algorithm>
#include "Ship.h"
#include "Animation.h"
//#include "Explosion.h"
//#include "Smoke.h"

#include "Bullet.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "Starfleet.h"
#include "Enterprise.h"

extern Starfleet * g_pStarfleet;
extern TEnterprise   * g_pEnterprise;
extern Communication * g_pCommunication;

TShip::TShip()
{
	m_dSteer				=	  0.02;
	m_nPhotonTimer          =     0;
	m_dSpeed				=     0;
	m_dMaxSpeed             =     100;
	m_nTranslucency			=	  0;

	m_nCloakCharge          =     0;
	m_nCloakCounter         =     0;
	m_nEnergy               = 10000;
	m_nRepairItem			=	 -1;
	m_nPreferedTarget		=	 -1;
    m_nCrew					=	100;
	m_nShieldTimer			=	  0;
	m_nMaxShieldEnergy      =   100;
	m_nEnergyTimer          =     0;
	m_nPhaserTimer			=     0;
	m_dViewDistance			=   800;
    m_nMaxHealth			=   100;
    m_dAngleSeek            =     0;
    m_nRepairTimer			=   10000/m_nCrew;
    m_nShieldEnergy         =   m_nMaxShieldEnergy;

	m_ID					=	ID_SHIP_BOTTOM;
	m_Member				=	MEM_NEUTRAL;
	m_AI                    =   AI_NONE;
    m_CloakState           =   CS_UNCLOAKED;
    m_PreferedBase			=	ID_BASE_BOTTOM;
	m_CloakState           =   CS_UNCLOAKED;

    m_blDocked				=	false;
    m_blDocking				=	false;
	m_blReleasing           =   false;
	m_blCanCloak            =   false;
    m_blCanFind				=   true;
	m_blMustBeDestroyed		=	false;
	m_blMustSurvive			=	false;
	m_blMustReachPosition	=	false;
	m_blDock				=	false;
	m_blNoRelease			=	false;
	m_blCanCollide	    	=   true;
	m_blShieldOn            =   true;  // todo: set false for release game !!!


	m_pTarget				=   NULL;
	m_pBaseTarget			=	NULL;

	m_nPhaserFireTimer		=     0;
	m_PhaserColor			=	al_map_rgb(255,0,0);
	m_nPhaserPower			=	1000;
	m_dPhaserX				=	   0;
	m_dPhaserY				=      0;
	m_dPhaserAngle          =      0;
	m_nZ					=	  38;
    m_nFlyheight			=	m_nZ;

	m_dSafePosX				=	   -1;
	m_dSafePosY				=      -1;
	m_strName               =   "UNKNOWN";
	m_blPhaserSoundPlaying  = false;
	m_blEvading             = false;
	m_blShieldOn            = true;
	m_blHadConversation     = false;
	m_nTask                 = TSK_STANDARD;
	m_MissionCritical       = MC_NONE;
}

TShip::~TShip()
{
	m_lstHealth.clear();
}


TShip::TShip(ifstream & a_LoadStream,ID a_id)
:TSprite(a_LoadStream,a_id)
{
    a_LoadStream.read((char*)& m_AI, sizeof (m_AI));
    a_LoadStream.read((char*)& m_blDocked, sizeof (m_blDocked));
    a_LoadStream.read((char*)& m_blDocking, sizeof (m_blDocking));
    a_LoadStream.read((char*)& m_blReleasing, sizeof (m_blReleasing));
    a_LoadStream.read((char*)& m_blMustBeDestroyed, sizeof (m_blMustBeDestroyed));
    a_LoadStream.read((char*)& m_blMustSurvive, sizeof (m_blMustSurvive));
    a_LoadStream.read((char*)& m_blMustReachPosition, sizeof (m_blMustReachPosition));
    a_LoadStream.read((char*)& m_blDock, sizeof (m_blDock));
    a_LoadStream.read((char*)& m_blNoRelease, sizeof (m_blNoRelease));
    a_LoadStream.read((char*)& m_blCanFind, sizeof (m_blCanFind));
    a_LoadStream.read((char*)& m_dWaypointX, sizeof (m_dWaypointX));
    a_LoadStream.read((char*)& m_dWaypointY, sizeof (m_dWaypointY));
    a_LoadStream.read((char*)& m_nTorpedoes, sizeof (m_nTorpedoes));
    a_LoadStream.read((char*)& m_nRepairItem, sizeof (m_nRepairItem));
    a_LoadStream.read((char*)& m_nCrew, sizeof (m_nCrew));
    a_LoadStream.read((char*)& m_nEnergy, sizeof (m_nEnergy));
    a_LoadStream.read((char*)& m_nShieldEnergy, sizeof (m_nShieldEnergy));
    a_LoadStream.read((char*)& m_nPhaserEnergy, sizeof (m_nPhaserEnergy));
    a_LoadStream.read((char*)& m_nPreferedTarget, sizeof (m_nPreferedTarget));
    a_LoadStream.read((char*)& m_nPhaserPower, sizeof (m_nPhaserPower));
    a_LoadStream.read((char*)& m_blCanCloak, sizeof (m_blCanCloak));
    a_LoadStream.read((char*)& m_nPhotonTimer, sizeof (m_nPhotonTimer));
    a_LoadStream.read((char*)& m_nTranslucency, sizeof (m_nTranslucency));
    a_LoadStream.read((char*)& m_CloakState, sizeof (m_CloakState));
    a_LoadStream.read((char*)& m_dAngleSeek, sizeof (m_dAngleSeek));
    a_LoadStream.read((char*)& m_nCloakCharge, sizeof (m_nCloakCharge));
    a_LoadStream.read((char*)& m_nCloakCounter, sizeof (m_nCloakCounter));
    a_LoadStream.read((char*)& m_nTask, sizeof (m_nTask));
    a_LoadStream.read((char*)& m_MissionCritical, sizeof (m_MissionCritical));

    m_lstHealth.clear();
    size_t nSystems;
    a_LoadStream.read((char*)& nSystems, sizeof (nSystems));

    for (size_t i =0; i< nSystems; i++)
    {
        int health;
        a_LoadStream.read((char*)& health, sizeof (health));
        m_lstHealth.push_back(health);
    }

    m_dSteer				=	  0.02;
	m_dMaxSpeed             =     100;
	m_nTranslucency			=	  0;
	m_pTarget				=   NULL;
	m_pBaseTarget			=	NULL;
	m_blCanCollide          =   true;
	m_nRepairItem			=	 -1;
	m_nPreferedTarget		=	 -1;

	m_nPhotonTimer          =     0;
	m_dMaxSpeed             =     100;
	m_nTranslucency			=	  0;

	m_nCloakCounter         =     0;
	m_nRepairItem			=	 -1;
	m_nPreferedTarget		=	 -1;
    m_nShieldTimer			=	  0;
	m_nMaxShieldEnergy      =   100;
	m_nEnergyTimer          =     0;
	m_nPhaserTimer			=     0;
	m_dViewDistance			=   800;
    m_nMaxHealth			=   100;
    m_dAngleSeek            =     0;
    m_nRepairTimer			=   10000/(m_nCrew + 1);
    m_nZ					=	  38;
    m_nFlyheight			=	m_nZ;

    m_nPhaserFireTimer		=     0;
	m_PhaserColor			=	al_map_rgb(255,0,0);
	m_nPhaserPower			=	1000;
	m_nMaxPhaserDist        =   500;
	m_dPhaserX				=	   0;
	m_dPhaserY				=      0;
	m_dPhaserAngle          =      0;
	m_blPhaserSoundPlaying  = false;
	m_blEvading = false;
	m_blShieldOn          = true;

	#ifdef _DEBUG
	std::cout << "-- Ship loaded\n";
	#endif // _DEBUG

}


void TShip::Save(ofstream & a_SaveStream)
{
    TSprite::Save(a_SaveStream);

    a_SaveStream.write((char*)& m_AI, sizeof (m_AI));
    a_SaveStream.write((char*)& m_blDocked, sizeof (m_blDocked));
    a_SaveStream.write((char*)& m_blDocking, sizeof (m_blDocking));
    a_SaveStream.write((char*)& m_blReleasing, sizeof (m_blReleasing));
    a_SaveStream.write((char*)& m_blMustBeDestroyed, sizeof (m_blMustBeDestroyed));
    a_SaveStream.write((char*)& m_blMustSurvive, sizeof (m_blMustSurvive));
    a_SaveStream.write((char*)& m_blMustReachPosition, sizeof (m_blMustReachPosition));
    a_SaveStream.write((char*)& m_blDock, sizeof (m_blDock));
    a_SaveStream.write((char*)& m_blNoRelease, sizeof (m_blNoRelease));
    a_SaveStream.write((char*)& m_blCanFind, sizeof (m_blCanFind));
    a_SaveStream.write((char*)& m_dWaypointX, sizeof (m_dWaypointX));
    a_SaveStream.write((char*)& m_dWaypointY, sizeof (m_dWaypointY));
    a_SaveStream.write((char*)& m_nTorpedoes, sizeof (m_nTorpedoes));
    a_SaveStream.write((char*)& m_nRepairItem, sizeof (m_nRepairItem));
    a_SaveStream.write((char*)& m_nCrew, sizeof (m_nCrew));
    a_SaveStream.write((char*)& m_nEnergy, sizeof (m_nEnergy));
    a_SaveStream.write((char*)& m_nShieldEnergy, sizeof (m_nShieldEnergy));
    a_SaveStream.write((char*)& m_nPhaserEnergy, sizeof (m_nPhaserEnergy));
    a_SaveStream.write((char*)& m_nPreferedTarget, sizeof (m_nPreferedTarget));
    a_SaveStream.write((char*)& m_nPhaserPower, sizeof (m_nPhaserPower));
    a_SaveStream.write((char*)& m_blCanCloak, sizeof (m_blCanCloak));
    a_SaveStream.write((char*)& m_nPhotonTimer, sizeof (m_nPhotonTimer));
    a_SaveStream.write((char*)& m_nTranslucency, sizeof (m_nTranslucency));
    a_SaveStream.write((char*)& m_CloakState, sizeof (m_CloakState));
    a_SaveStream.write((char*)& m_dAngleSeek, sizeof (m_dAngleSeek));
    a_SaveStream.write((char*)& m_nCloakCharge, sizeof (m_nCloakCharge));
    a_SaveStream.write((char*)& m_nCloakCounter, sizeof (m_nCloakCounter));
    a_SaveStream.write((char*)& m_nTask, sizeof (m_nTask));
    a_SaveStream.write((char*)& m_MissionCritical, sizeof (m_MissionCritical));


    size_t nSystems = m_lstHealth.size();
    a_SaveStream.write((char*)& nSystems, sizeof (nSystems));
    // store health
    for (size_t i=0; i < m_lstHealth.size(); i++)
    {
         int Health = m_lstHealth[(HEALTH)i];
         a_SaveStream.write((char*)& Health, sizeof (Health));
    }
    #ifdef _DEBUG
	std::cout << "-- Ship saved\n";
	#endif // _DEBUG
}


double TShip::GetX()
{
	return m_dX;
}

double TShip::GetY()
{
	return m_dY;
}

double TShip::GetAngle()
{
	return m_dAngle;
}

double TShip::GetSpeed()
{
	return m_dSpeed;
}

int TShip::GetCloackState()
{
	return m_CloakState;
}

void TShip::LooseTarget()
{
    m_pTarget=NULL;
}

void TShip::Die()
{
    for (int i=0;i<int(m_lstHealth.size());i++)
	{
		m_lstHealth[i]=0;
	}
}


double TShip::CalcVolume()
{
    double D = Distance(m_dX, m_dY,m_pEngine->m_dX, m_pEngine->m_dY);
	double V =0;

	if (D!=0)
    {
        V = (400.0/D);
        if (V>1.0) V=1.0;
    }
    else
    {
        V=1.0;
    }
    return V;
}

bool TShip::IsDestroyed()
{
    return m_blDestroyed;
}


void TShip::Explode()
{
	if (! m_blDestroyed)
	{
		double V = CalcVolume();
        SoundManager::PlaySound(SOUND::EXPLOSION_SMALL,V);
		m_blDestroyed  = true;
		m_pTarget      = NULL;
		m_blCanCollide = false;

		TAnimation * pAnimation;
		if ((m_ID > ID_BASE_BOTTOM) && (m_ID < ID_BASE_TOP))
        {
           for (int i=0; i<15; i++)
           {
               int nDX = rand() % m_nBitmapWidth  - (m_nBitmapWidth  / 2);
               int nDY = rand() % m_nBitmapHeight - (m_nBitmapHeight / 2);
               pAnimation = new TAnimation( ANIMATION::EXPLOSION,
                                     m_dX + nDX,
                                     m_dY + nDY,
                                     m_dSpeed,
                                     m_dAngle,
                                     m_nZ+1);
		       m_pEngine->Add(pAnimation);
           }
        }
		else
        {
            pAnimation = new TAnimation( ANIMATION::EXPLOSION,
                                     m_dX,m_dY,
                                     m_dSpeed,
                                     m_dAngle,
                                     m_nZ+1);
		    m_pEngine->Add(pAnimation);
        }

        missionEvent event;
        //event.m_strObject = m_strName;
        if (g_pEnterprise != NULL)
        {
            event.m_nSectorX = g_pEnterprise->GetX();
            event.m_nSectorY = g_pEnterprise->GetY();
        }

        switch (m_MissionCritical)
        {
            case MC_DESTROY:
                event.m_blFailMission=false;
            break;

            case MC_SURVIVE:
                event.m_blFailMission=true;
            break;

            case MC_REACH:
            break;

            default:
            break;
        }

        if (g_pStarfleet != NULL)
        {
            g_pStarfleet->PostEvent(event);
        }
    }
}


void TShip::DoCloak()
{
	if ((m_CloakState == CS_CLOAKING)&&(m_nTranslucency<30))
	{
		m_nShieldEnergy=0;
		m_nTranslucency+=1;
	}
	else if ((m_CloakState == CS_DECLOAKING)&&(m_nTranslucency>0))
	{
		m_nTranslucency-=1;
	}

	if (m_nTranslucency==0)
	{
		m_CloakState= CS_UNCLOAKED;

	}
	else if (m_nTranslucency==30)
	{
		m_nShieldEnergy=0;
		m_CloakState= CS_CLOAKED;
	}
}


void TShip::DoCollision(TSprite * sprite)
{
	if (sprite->m_ID==ID_BLACK_HOLE)
    {
        Explode();
    }

    if (sprite->m_ID==ID_ASTEROID)
    {
        double dAngle = WayPoint(sprite->GetX(),sprite->GetY());
        double dDistance = Distance(m_dX,m_dY,sprite->GetX(),sprite->GetY());
        double dDisplacement = 90 - dDistance;
        if (dDisplacement >0)
        {
            m_dX += cos(-dAngle) * -dDisplacement;
            m_dY += sin(-dAngle) * dDisplacement;
        }


        m_lstHealth[HLT_HULL] -= (int)m_dSpeed + rand()% 10;
        m_lstHealth[HLT_SENSOR] -= (int)m_dSpeed + rand()% 10;;

        m_dSpeed = 0;

    }

	else if ((sprite!=NULL)&& (sprite->m_ID>=ID_BULLET_BOTTOM)&&(sprite->m_ID<=ID_BULLET_TOP)&&(sprite->m_Member!=m_Member))
	{
      	int nRemainingDamage =0;
      	if (m_blShieldOn)
        {
            int shielddec=((TBullet *)sprite)->m_nDamage;
            nRemainingDamage = ((TBullet *)sprite)->m_nDamage - m_nShieldEnergy;
            if (nRemainingDamage<0) nRemainingDamage=0;
            m_nShieldEnergy-=shielddec;
            if (m_nShieldEnergy<0) m_nShieldEnergy=0;
        }
        else
        {
            nRemainingDamage =((TBullet *)sprite)->m_nDamage;
        }

		if (nRemainingDamage>0)
        {
            for (size_t i=0;i < m_lstHealth.size();i++)
            {
                m_lstHealth[i] -= rand()% nRemainingDamage;
                if (m_lstHealth[i]<0) m_lstHealth[i]=0;
            }

            int crewloss=(nRemainingDamage * 3)-m_lstHealth[HLT_HULL];
            if (crewloss<0) crewloss=0;
            m_nCrew-=crewloss;
            if (m_nCrew<0) m_nCrew=0;
            m_lstHealth[HLT_HULL]-= nRemainingDamage/10;
        }


		double V = CalcVolume();
	    SoundManager::PlaySound(SOUND::EXPLOSION_SMALL,V);

		TAnimation * pAnimation;
		pAnimation = new TAnimation( ANIMATION::SMOKE,
                                     sprite->GetX(),sprite->GetY(),
                                     0,
                                     0,
                                     m_nZ+1);
		m_pEngine->Add(pAnimation);

		sprite->m_blDestroyed=true;
	}

}



void TShip::DoEngineering()
{


	// If Docked repair faster and simultaneous
	if ((m_blDocked)&&(!m_blReleasing))
	{
		m_nRepairItem	 =	-1;
		m_nRepairTimer	--;
		if (m_nTorpedoes<50)
        {
            m_nTorpedoes++;
        }

		bool hlt_ok = true;
		if (m_nRepairTimer<=0)
		{
			for (size_t i=0;i < m_lstHealth.size();i++)
			{
				if (m_lstHealth[i]<m_nMaxHealth)
				{
					m_lstHealth[i]++;
					hlt_ok=false;
				}
				if (m_lstHealth[i]>m_nMaxHealth) m_lstHealth[i]=m_nMaxHealth;
			}
			if ((m_nTorpedoes < 50)&&(hlt_ok)) m_nTorpedoes++;
			m_nRepairTimer = 60;
		}
	}

	if (m_nRepairItem==-1)
	{
		for (size_t i=0;i < m_lstHealth.size();i++)
		{
			if (m_lstHealth[i]<m_nMaxHealth) m_nRepairItem=i;
		}

		if (m_lstHealth[HLT_IMPULSE]<40) m_nRepairItem = HLT_IMPULSE;
		if (m_lstHealth[HLT_THRUSTER]<40) m_nRepairItem = HLT_THRUSTER;
		if (m_lstHealth[HLT_PHOTON]<40) m_nRepairItem = HLT_PHOTON;
		if (m_lstHealth[HLT_SHIELD]<60) m_nRepairItem = HLT_SHIELD;
	}
	else
	{
		// repair item
		m_nRepairTimer--;
		if (m_nRepairTimer<=0)
		{
			m_lstHealth[m_nRepairItem]++;
			if (m_nCrew<1)
			{
              m_nRepairTimer=10000;
			}
			else if (m_nCrew<200) m_nRepairTimer=10000/m_nCrew;
			else m_nRepairTimer=80;

			if (m_lstHealth[m_nRepairItem]>m_nMaxHealth)
			{
				m_lstHealth[m_nRepairItem]=m_nMaxHealth;
				m_nRepairItem=-1;
			}
		}
	}


	if (m_nPhaserFireTimer>0) m_nPhaserFireTimer--;
	if (m_nPhotonTimer>0) m_nPhotonTimer--;

	double dFireAngle = m_dAngleSeek-m_dAngle;
    if (dFireAngle<0) dFireAngle=-dFireAngle;
    if (((IsPhaserActive())&&(dFireAngle>m_dPhaserAngle)) || (m_dTargetDistance > m_nMaxPhaserDist))
    {
       StopPhaser();
    }


    if ((m_nPhaserEnergy<=0)||(m_lstHealth[HLT_PHASER]<=20)) StopPhaser();
	if ((IsPhaserActive())&&(m_nPhaserFireTimer<=0))
	{
		m_nPhaserEnergy-=2;
		m_nPhaserFireTimer=10;

		if (m_pTarget!=NULL)
		{

		}

	}

	if (m_nEnergyTimer<=0)
	{
		m_nEnergy-=int(m_dSpeed/10);
		if (m_lstHealth[HLT_WARPCORE]>CORE_CRITICAL)
        {
            if(m_nEnergy<MAX_ENERGY)
            {
               m_nEnergy+=int(m_lstHealth[HLT_WARPCORE]*0.4);
                if (m_nEnergy<0) m_nEnergy=0;
                else if (m_nEnergy>MAX_ENERGY) m_nEnergy=MAX_ENERGY;
            }
        }
        else
        {
            m_nEnergy-=40;
        }
		m_nEnergyTimer=50;
	}
	else m_nEnergyTimer--;


	if ((m_nPhaserTimer<=0)&&(m_nEnergy>30)&&(m_nPhaserEnergy<(m_lstHealth[HLT_PHASER]*2)))
	{
		m_nPhaserEnergy++;
		m_nEnergy-=6;
		m_nPhaserTimer=40;
	}
	if (m_nPhaserTimer>0) m_nPhaserTimer--;

	if ((m_blShieldOn)&&(m_nEnergy>0)&&(m_nShieldTimer<=0)&&(m_nShieldEnergy<(m_nMaxShieldEnergy*(m_lstHealth[HLT_SHIELD])/m_nMaxHealth)))
	{
		m_nShieldEnergy+=1;
		m_nEnergy-=6;
		m_nShieldTimer=40;
	}
	if (m_nShieldTimer>0) m_nShieldTimer--;


	if (m_blCanCloak)
    {
        if ((m_nEnergy>20)&&(m_nCloakCharge<CLOAK_DELAY)&&(m_CloakState==CS_UNCLOAKED))
        {
            m_nCloakCharge++;
        }

        if ((m_lstHealth.size()>HLT_CLOAK)&&(m_nCloakCounter<=0))
        {
            if (((m_lstHealth[HLT_CLOAK]<60)||(m_nEnergy<500))&&(m_CloakState = CS_CLOAKED))
            {
            m_CloakState = CS_DECLOAKING;
            }

            DoCloak();
            m_nCloakCounter=5;
        }
        else if (m_nCloakCounter>0)
        {
            m_nCloakCounter--;
        }
    }
}


bool TShip::TryEnterDocking()
{
    m_pBaseTarget = (TShip *) m_pEngine->Seekstarbase(m_Member,false,m_dViewDistance,m_dX,m_dY);
    if ((m_pBaseTarget!=NULL)&&(!m_pBaseTarget->m_blDestroyed))
    {
        m_AI=AI_DOCK;
        m_blDocking=false;
        m_blReleasing=false;
        return true;
    }
    else
    {
        return false;
    }
}




void TShip::CalcPhaserDamage(double a_dEnergy, int a_nTarget)
{
	int shielddec;
	if (m_blShieldOn)
    {
        shielddec = int(a_dEnergy);
        a_dEnergy -= m_nShieldEnergy;
    }
    else
    {
        shielddec = 0;
    }


	if (a_dEnergy<0)
	{
	    a_dEnergy=0;
	}
    m_nShieldEnergy-=shielddec;
	if (m_nShieldEnergy<0)
	{
	    m_nShieldEnergy=0;
	}


    if (a_dEnergy>0)
    {
        int place=0;
        if (a_nTarget==-1)
        {
            place=HLT_WARPCORE;
        }
        else
        {
            place=a_nTarget;
        }

        m_lstHealth[place]-=int(a_dEnergy);

        if (m_lstHealth[place]<0)
        {
            m_lstHealth[place]=0;
        }

        int crewloss = int(a_dEnergy-m_lstHealth[HLT_HULL]);
        if (crewloss<0)
        {
            crewloss=0;
        }

        m_nCrew-=crewloss;
        if (m_nCrew<0)
        {
            m_nCrew=0;
        }
    }
}




double TShip::WayPoint(double a_dX,double a_dY)
{
	double DX=m_dX-a_dX;
	double DY=m_dY-a_dY;
	if (DX!=0) return atan2(DY,DX)+PI;
	else return 0;
}




double TShip::GetEvasiveAngle(double a_dDefaultAngle)
{
    TSprite * pObject = m_pEngine->Seek(ID_ASTEROID, 60, m_dX, m_dY,m_dAngle, 80);
    if (pObject != NULL)
    {
        if (!m_blEvading)
        {
            m_dEvadeAngle = WayPoint(pObject->GetX(),pObject->GetY());

            if (m_dEvadeAngle > a_dDefaultAngle)
            {
                m_dEvadeAngle += (PI / 2.00);
            }
            else
            {
                m_dEvadeAngle -= (PI / 2.00);
            }
            m_blEvading = true;
        }


        double dDistance = Distance(m_dX, m_dY, pObject->GetX(), pObject->GetY());
        m_dSpeed = dDistance / 8;
        if (m_dSpeed > m_dMaxSpeed) m_dSpeed = m_dMaxSpeed;
    }
    else
    {
        if (m_blEvading)
        {
            TSprite * pObject = m_pEngine->Seek(ID_ASTEROID, 200, m_dX, m_dY);
            if (pObject == NULL)
            {
                m_blEvading = false;
            }
        }
        else
        {
            m_dEvadeAngle = a_dDefaultAngle;
        }
    }
    return m_dEvadeAngle;
}



bool TShip::Dock(TShip * a_pBaseTarget)
{
	if ((a_pBaseTarget!=NULL)&&(a_pBaseTarget->m_ID>ID_BASE_BOTTOM)&&(a_pBaseTarget->m_ID<ID_BASE_TOP)&&(a_pBaseTarget->m_Member==m_Member))
	{
		m_dTargetDistance = Distance(m_dX,m_dY,a_pBaseTarget->m_dX,a_pBaseTarget->m_dY);

		if ( (m_dTargetDistance < 250) || (m_blDocking))
		{
			m_dAngleSeek=WayPoint(a_pBaseTarget->m_dX,a_pBaseTarget->m_dY);

			if (abs(m_dAngleSeek-m_dAngle) < 1.0)
			{
				SetSpeed(std::fmin( (m_dMaxSpeed * (m_dTargetDistance/500)), m_dMaxSpeed));
            }

			if ((m_dTargetDistance<150)&&(m_nZ>DOCK_HEIGHT))
			{
				m_dAngleSeek+=PI;
			}
			else
			{
				m_nZ=DOCK_HEIGHT;
				m_pEngine->Sort();
			}

			Control();


			if (m_dTargetDistance > 300)
			{
                m_blDocked=false;
                m_blCanFind = true;
                m_blDocking=false;
                m_blReleasing=false;
                m_nZ=m_nFlyheight;
                m_pEngine->Sort();
                return false;
			}


			if ((m_dTargetDistance<40)&&(m_nZ==DOCK_HEIGHT))
			{
				m_blDocked=true;
				m_blCanFind = false;
				m_blDocking=false;
				m_dSpeed=0;
				if (a_pBaseTarget->m_blDestroyed)
				{
					Die();
				}
				return false;
			}
			else
			{
				return true;
			}

		}
		else
		{
			m_blDocked=false;
			m_blCanFind = true;
			m_blDocking=false;
			m_blReleasing=false;
			m_nZ=m_nFlyheight;
			m_pEngine->Sort();
			return false;
		}
	}
	else
	{
		m_blDocked=false;
		m_blCanFind = true;
		m_blDocking=false;
		m_blReleasing=false;
		m_nZ=m_nFlyheight;
		m_pEngine->Sort();
		return false;
	}
}


void TShip::Release(TShip * a_pBaseTarget)
{
	if (a_pBaseTarget!=NULL)
	{
		m_dTargetDistance = Distance(m_dX,m_dY,a_pBaseTarget->m_dX,a_pBaseTarget->m_dY);
		SetSpeed(20);
		if (m_dTargetDistance>150)
		{
			m_nZ=m_nFlyheight;
			m_blCanFind = true;
			m_blDocked=false;
			m_blDocking=false;
			m_blReleasing=false;
			m_pEngine->Sort();
			m_pTarget=NULL;
			if (m_ID == ID_PLAYER)
            {
               g_pCommunication->AddMessage(6,CREW_SULU," Captain, you have the helm !");
            }
		}
		else
        {
            m_blCanFind = false;
			m_blDocked=false;
			m_blDocking=false;
			m_blReleasing=true;
			m_pTarget=NULL;
        }
	}
}



void TShip::Draw(double a_dCamX, double a_dCamY)
{
	int DrawX=int(m_pEngine->m_nScreenMidX+(m_dX-a_dCamX));
	int DrawY=int(m_pEngine->m_nScreenMidY+(m_dY-a_dCamY));
// only draw what we see

    if ((IsPhaserActive())&&(m_pTarget!=NULL)&&(!m_pTarget->m_blDestroyed))
    {
        int DrawXT = int(m_pEngine->m_nScreenMidX+(m_nPhaserEndX-a_dCamX));
        int DrawYT = int(m_pEngine->m_nScreenMidY+(m_nPhaserEndY-a_dCamY));
        int px,py;

        if (m_dPhaserX==0)
        {
            px=DrawX;
        }
        else
        {
            px=int(DrawX+(m_dPhaserX *cos(m_dAngle)));
        }


        if (m_dPhaserY==0)
        {
            py=DrawY;
        }
        else
        {
            py=int(DrawY+(m_dPhaserY*sin(m_dAngle)));
        }

        al_draw_line(px,py,DrawXT,DrawYT,m_PhaserColor,2);

	}

if ((DrawX+m_nBitmapMidX>0)&&(DrawX-m_nBitmapMidX<m_pEngine->m_nScreenWidth)&&(DrawY+m_nBitmapMidY>0)&& (DrawY-m_nBitmapMidY<m_pEngine->m_nScreenHeight))
   {
       if (m_nTranslucency>0)
	   {
		   if (m_nTranslucency<30)
		   {
				al_draw_tinted_rotated_bitmap(m_pImage,al_map_rgba_f(1.00/m_nTranslucency, 1.00/m_nTranslucency, 1.00/m_nTranslucency, 1.00/m_nTranslucency),m_nBitmapMidX, m_nBitmapMidY, DrawX, DrawY,m_dAngle, 0);
		   }
	   }
	   else
	   {
	   	   al_draw_rotated_bitmap(m_pImage, m_nBitmapMidX, m_nBitmapMidY, DrawX, DrawY,m_dAngle, 0);
	   }
   }


#ifdef _DEBUG

   switch(m_AI)
   {
        case AI_WANDER:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"WANDER");
        break;

        case AI_CHASE:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"CHASE");
        break;

        case AI_EVADE:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"EVADE");
        break;

        case AI_DOCK:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"DOCK");
        break;

        case AI_RELEASE:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"RELEASE");
        break;

        case AI_MOVE:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"MOVE");
        break;

        case AI_NONE:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"NO_AI");
        break;

        case AI_ASTER:
             al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"AI_ASTER");
        break;

        case AI_CONVERSE:
             al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"AI_CONVERSE");
        break;

        case AI_ESCORT:
             al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"AI_ESCORT");
        break;

        default:
            al_draw_text(FontManager::GetFont(FONT::DBG),m_pEngine->m_clRED, DrawX, DrawY, 0,"ERROR");
        break;

   }
#endif // _DEBUG


}


void TShip::SetSpeed(double a_dSpeed)
{
    m_dSpeed = a_dSpeed;

    if (m_dSpeed>(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100)
    {
        m_dSpeed=(m_dMaxSpeed*m_lstHealth[HLT_IMPULSE])/100;
    }

    if (m_dSpeed <0)
    {
        m_dSpeed =0;
    }

}





void TShip::Control()
{
	float str=m_dSteer;
	if ((m_dAngle<m_dAngleSeek)&&(m_dAngleSeek-m_dAngle>PI)) str=-str;
	if ((m_dAngleSeek<m_dAngle)&&(m_dAngle-m_dAngleSeek>PI)) str=-str;
	if (m_dAngleSeek-m_dAngle>0.05) m_dAngle=m_dAngle+str;
	if (m_dAngle-m_dAngleSeek>0.05) m_dAngle=m_dAngle-str;
	if (m_dAngle>2*PI) m_dAngle=m_dAngle-(2*PI);
	if (m_dAngle<0)    m_dAngle=(2*PI)+m_dAngle;
	if (m_dAngleSeek>2*PI) m_dAngleSeek=m_dAngleSeek-(2*PI);
	if (m_dAngleSeek<0)    m_dAngleSeek=(2*PI)+m_dAngleSeek;
}



void TShip::StartPhaser(ID a_ID)
{
    if (! m_blPhaserSoundPlaying)
    {
        m_pPhaser_sound_id = SoundManager::GetFreePhaserAddress();

        double V = CalcVolume();

        switch (a_ID)
        {
           case ID_GALAXYCLASS:
                m_blPhaserSoundPlaying = SoundManager::LoopSound(SOUND::PHASER,V,m_pPhaser_sound_id);
           break;

           case ID_ROMULANBOP:
               m_blPhaserSoundPlaying = SoundManager::LoopSound(SOUND::ROMULAN_BEAM,V,m_pPhaser_sound_id);
           break;

           case ID_RELIANT:
               m_blPhaserSoundPlaying = SoundManager::LoopSound(SOUND::PHASER,V,m_pPhaser_sound_id);
           break;

           case ID_PLAYER:
               m_blPhaserSoundPlaying = SoundManager::LoopSound(SOUND::PHASER,V,m_pPhaser_sound_id);
            break;

           default:

           break;

        }

    }

}

void TShip::StopPhaser()
{
    if (m_blPhaserSoundPlaying)
    {
        al_stop_sample(m_pPhaser_sound_id);
        m_blPhaserSoundPlaying = false;
    }
}

bool TShip::IsPhaserActive()
{
    return m_blPhaserSoundPlaying;
}



void TShip::PrecalcFireAngles(int a_nLimit)
{
	if ((m_lstHealth[HLT_SENSOR]>60)&&(m_lstHealth[HLT_COMPUTER]>80)&&(m_pTarget!=NULL)&&(!m_pTarget->m_blDestroyed))
	{
		for (int i=0;i<a_nLimit;i++)
        {
			double aimangleseek;
			double shotlead=m_dTargetDistance/16;
			int sx=int(m_pTarget->GetX()+cos(m_pTarget->GetAngle())*shotlead);
			int sy=int(m_pTarget->GetY()+sin(m_pTarget->GetAngle())*shotlead);
			aimangleseek=WayPoint(sx,sy);
			double str=0.01;
			if ((m_dAimangle<aimangleseek)&&(aimangleseek-m_dAimangle>PI)) str=-str;
			if ((aimangleseek<m_dAimangle)&&(m_dAimangle-aimangleseek>PI)) str=-str;
			if (aimangleseek-m_dAimangle>0.02) m_dAimangle += str;
			if (m_dAimangle-aimangleseek>0.02) m_dAimangle -= str;
			if (m_dAimangle>2*PI) m_dAimangle=m_dAimangle-(2*PI);
			if (m_dAimangle<0)    m_dAimangle=(2*PI)+m_dAimangle;
			if (aimangleseek>2*PI) aimangleseek=aimangleseek-(2*PI);
			if (aimangleseek<0)    aimangleseek=(2*PI)+aimangleseek;
		}
	}
	else
	{

	}

}






