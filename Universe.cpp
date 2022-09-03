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


#include <time.h>
#include <ostream>

#include "Universe.h"
#include "Communication.h"
#include "Engine.h"
#include "Enterprise.h"
#include "Romulan_BOP.h"
#include "Klingon_BC.h"
#include "Klingon_BOP.h"
#include "SpaceObject.h"
#include "Starbase.h"
#include "Federation_Ship.h"
#include "Reliant.h"
#include "Transporter.h"
#include "Animation.h"


extern ofstream logfile;
extern Communication * g_pCommunication;

GamePlanetRec::GamePlanetRec()
{
	m_Member    = MEM_NONE;
	m_PlanetType = ID_NONE;
	m_nX=0;
	m_nY=0;
	m_nPopulation = 0;
	m_Item = INV_NONE;
}

GameSpecialRec::GameSpecialRec()
{
	m_Member     = MEM_NONE;
	m_SpecialType = ID_NONE;
	m_nX=0;
	m_nY=0;

}

GameStarbaseRec::GameStarbaseRec(ID a_nStarbaseType)
{
	switch (a_nStarbaseType)
	{
        case ID_FEDERATIONBASE:
	         m_Member			=  MEM_FEDERATION;
             m_StarbaseType     =  a_nStarbaseType;
             for (int i=0;i<10;i++)
             {
                m_nHealth[i]=MAX_STARBASE_HEALTH;
             }
	    break;

	    case ID_ROMULAN_BASE:
	         m_Member			=  MEM_ROMULAN;
             m_StarbaseType     =  a_nStarbaseType;
	         for (int i=0;i<10;i++)
             {
                m_nHealth[i]=MAX_STARBASE_HEALTH;
             }
	    break;


	    case ID_KLINGON_BASE:
	         m_Member			=  MEM_KLINGON;
             m_StarbaseType     =  a_nStarbaseType;
	         for (int i=0;i<10;i++)
             {
                m_nHealth[i]=MAX_STARBASE_HEALTH;
             }
	    break;

	    default:
            m_Member		   =  MEM_NONE;
            m_StarbaseType     =  ID_NONE;
            for (int i=0;i<10;i++)
            {
                m_nHealth[i]=MAX_STARBASE_HEALTH;
            }
	}

	m_nX=0;
	m_nY=0;
	m_Item = INV_NONE;
}



Sector::Sector()
{
	m_blKnown            = false;
	m_blHasAsteroidField = false;
	m_nMember            = 0;
	m_nKlingonBC         = 0;
    m_nKlingonBOP        = 0;
    m_nRomulanBOP        = 0;
    m_nFederation1       = 0;
    m_nFederation2       = 0;
    m_nFederation3       = 0;
    m_nTimer1            = 0;
    m_nTimer2            = 0;
}


void Sector::Clear()
{
    m_blHasAsteroidField = false;
    m_nKlingonBC   = 0;
    m_nKlingonBOP  = 0;
    m_nRomulanBOP  = 0;
    m_nFederation1 = 0;
    m_nFederation2 = 0;
    m_nFederation3 = 0;

    m_Starbase.clear();
    m_Planet.clear();
    m_Special.clear();
}


bool Sector::HasSpaceObjectType(ID id)
{
    size_t nIndex =0;
    bool blFound= false;
    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if (m_Planet[nIndex].m_PlanetType == id)
        {
            blFound = true;
        }
	    nIndex++;
	}

	if (blFound)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Sector::Build(TEngine * a_pEngine)
{
	m_blKnown = true;
	a_pEngine->Clear(true);

	int nMid = SECTORSIZE / 2;


	if (m_blHasAsteroidField)
    {
        for (double i=0; i<2*PI; i+=0.3)
        {
            TSpaceObject * pObject = new TSpaceObject(ID_ASTEROID);

            int dX=0;
            int dY=0;

            dX = nMid + cos(i) * (1000 + (rand() % 600));
            dY = nMid + sin(i) * (1000 + (rand() % 600));

            pObject->SetPosition(dX,dY);
            a_pEngine->Add(pObject);
        }
    }



	for (size_t i=0;i<m_Planet.size();i++)
	{
		// create planet
		TSpaceObject * pObject = new TSpaceObject(m_Planet[i].m_PlanetType);
		pObject->SetPosition(m_Planet[i].m_nX,m_Planet[i].m_nY);

		if (m_Planet[i].m_Item != INV_NONE)
		{
            TInventoryItem item;
            item.imageID = m_Planet[i].m_Item;
            item.m_blValid = true;
            pObject->SetInventoryItem(item);
		}
		else
		{
            pObject->RemoveInventoryItem();
        }

		a_pEngine->Add(pObject);
	}

	for (size_t i=0; i<m_Special.size(); i++)
    {
        GameSpecialRec specrec = m_Special[i];
        switch(specrec.m_SpecialType)
        {
            case ID_RELIANT:
            {
                CReliant * pReliant = new CReliant();
                int RX = rand() % SECTORSIZE;
                int RY = rand() % SECTORSIZE;
                pReliant->SetPosition(RX,RY);
                a_pEngine->Add(pReliant);
            }
            break;

            default:

            break;
        }

    }

   //size_t s= m_Starbase.size();
   for (size_t i=0;i<m_Starbase.size();i++)
	{
       TStarbase * pStarbase = new TStarbase(m_Starbase[i].m_StarbaseType);
       pStarbase->SetPosition(m_Starbase[i].m_nX,m_Starbase[i].m_nY);
	   for (size_t j=0;j<pStarbase->m_lstHealth.size();j++)
	   {
		  pStarbase->m_lstHealth[j] = m_Starbase[i].m_nHealth[j];
       }

       if (m_Starbase[i].m_Item != INV_NONE)
		{
            TInventoryItem item;
            item.imageID = m_Starbase[i].m_Item;
            item.m_blValid = true;
            pStarbase->SetInventoryItem(item);
		}
		else
		{
            pStarbase->RemoveInventoryItem();
        }

	   a_pEngine->Add(pStarbase);
	}

	for (int i=0;i<m_nKlingonBC; i++)
	{
		int RX = rand() % SECTORSIZE;
		int RY = rand() % SECTORSIZE;

		TKlingonBC * pKlingon = new TKlingonBC();
		pKlingon->SetPosition(RX,RY);
		a_pEngine->Add(pKlingon);
	}

	for (int i=0;i<m_nRomulanBOP; i++)
	{
		int RX = rand() % SECTORSIZE;
		int RY = rand() % SECTORSIZE;

		TRomulanBop * pRomulan = new TRomulanBop();
		pRomulan->SetPosition(RX,RY);
		a_pEngine->Add(pRomulan);

	}

	for (int i=0;i<m_nKlingonBOP; i++)
	{
		int RX = rand() % SECTORSIZE;
		int RY = rand() % SECTORSIZE;

		TKlingonBOP * pKlingon = new TKlingonBOP();
		pKlingon->SetPosition(RX,RY);
		a_pEngine->Add(pKlingon);
	}

	for (int i=0;i<m_nFederation1; i++)
	{
		int RX =  rand() % SECTORSIZE;
		int RY =  rand() % SECTORSIZE;

		TFederation_Ship * pFed = new TFederation_Ship();
		pFed->SetPosition(RX,RY);
		a_pEngine->Add(pFed);

	}

	for (int i=0;i<m_nFederation2; i++)
	{
//		int RX = rand() % SECTORSIZE;
//		int RY = rand() % SECTORSIZE;

	}
}


void Sector::StoreStarbase(TStarbase * a_pStarbase)
{
	// store playfield starbase back into sector
    if (a_pStarbase != NULL)
    {
        GameStarbaseRec SRec(a_pStarbase->m_ID);
        SRec.m_nX= (int)a_pStarbase->GetX();
        SRec.m_nY= (int)a_pStarbase->GetY();
        int i=0;
        std::vector<int>::const_iterator p=a_pStarbase->m_lstHealth.begin();
        while ((p!=a_pStarbase->m_lstHealth.end())&&(i<11))
        {
            SRec.m_nHealth[i] = *p;
            i++;
        }

        if (a_pStarbase->HasInventoryItem())
        {
            SRec.m_Item   = a_pStarbase->GetInventoryItem().imageID;
        }
        else
        {
            SRec.m_Item  = INV_NONE;
        }
        m_Starbase.push_back(SRec);
    }
}


bool Sector::HasBase(MEMBER a_Member)
{
    bool blResult = false;
    std::vector<GameStarbaseRec>::const_iterator p = m_Starbase.begin();
    while (!blResult && (p != m_Starbase.end()))
	{
       if ((*p).m_Member == a_Member)
       {
           blResult = true;
       }
       p++;
	}
	return blResult;
}


void Sector::StorePlanet(TSpaceObject * a_pPlanet)
{
    if (a_pPlanet != NULL)
    {
        GamePlanetRec PRec;
        PRec.m_PlanetType = a_pPlanet->m_ID;
        PRec.m_Member = a_pPlanet->m_Member;
        PRec.m_nX      = a_pPlanet->GetX();
        PRec.m_nY      = a_pPlanet->GetY();

        if (a_pPlanet->HasInventoryItem())
        {
            PRec.m_Item   = a_pPlanet->GetInventoryItem().imageID;
        }
        else
        {
            PRec.m_Item  = INV_NONE;
        }

        //TODO (Edwin#5#): store Population

        m_Planet.push_back(PRec);
    }
}


void Sector::StoreSpecial(TSprite * a_pSpecial)
{
	// store playfield starbase back into sector
    if (a_pSpecial != NULL)
    {
        GameSpecialRec SRec;

        SRec.m_Member = a_pSpecial->m_Member;
        SRec.m_nX= (int)a_pSpecial->GetX();
        SRec.m_nY= (int)a_pSpecial->GetY();
        SRec.m_SpecialType = a_pSpecial->m_ID;

        m_Special.push_back(SRec);
    }
}


GamePlanetRec * Sector::GetPlanetForStarbase()
{
    GamePlanetRec * pPlanetRec = NULL;

    size_t nIndex =0;
    bool blFound= false;
    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_M1)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }

    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_M2)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }

    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_M3)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }


    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_A)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }

    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_B)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }

    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_L)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }

    while ((nIndex < m_Planet.size())&&(!blFound))
    {
        if(m_Planet[nIndex].m_PlanetType == ID_CLASS_N)
        {
            pPlanetRec = &m_Planet[nIndex];
            blFound = true;
        }
        nIndex++;
    }
    return pPlanetRec;
}



void Sector::Store(TEngine * a_pEngine)
{
	Clear();

	std::list<TSprite *>::const_iterator p=a_pEngine->m_lstItems.begin();
	while (p!=a_pEngine->m_lstItems.end())
	{
		if ((((*p)->m_ID)>ID_SHIP_BOTTOM) && (((*p)->m_ID)<ID_SHIP_TOP) && (((*p)->m_ID)!=ID_PLAYER))
		{
			// store ships and starbases except player
			switch ((*p)->m_ID)
			{
				case ID_GALAXYCLASS:
					m_nFederation1++;
				break;

			    case ID_KLINGONBC:
				     m_nKlingonBC++;
				break;

				case ID_KLINGONBOP:
					m_nKlingonBOP++;
                break;

				case ID_ROMULANBOP:
					m_nRomulanBOP++;
				break;

				default:
				break;
			}
		}

		else if ((((*p)->m_ID)>ID_BASE_BOTTOM) && (((*p)->m_ID)<ID_BASE_TOP))
        {
            StoreStarbase((TStarbase *)(*p));
        }

        else if ((((*p)->m_ID)>ID_PLANET_BOTTOM) && (((*p)->m_ID)<ID_STAR_TOP))
        {
            StorePlanet((TSpaceObject *)(*p));
        }

        else if ((((*p)->m_ID)>ID_SPECIAL_BOTTOM) && (((*p)->m_ID)<ID_SPECIAL_TOP))
        {
            StoreSpecial((TSprite *)(*p));
        }
        p++;
	}

	//TODO (Edwin#5#):adjust occupation m_nMember



}



void Sector::Save(ofstream & a_SaveStream)
{
    // save known universe sector
    a_SaveStream.write((char*)&m_blKnown,sizeof (char));
    a_SaveStream.write((char*)&m_blHasAsteroidField,sizeof (char));

    // save occupation
    a_SaveStream.write((char*)&m_nMember,sizeof (char));
    // save ships
    a_SaveStream.write((char*)&m_nKlingonBC,sizeof (char));
    a_SaveStream.write((char*)&m_nKlingonBOP,sizeof (char));
    a_SaveStream.write((char*)&m_nRomulanBOP,sizeof (char));
    a_SaveStream.write((char*)&m_nFederation1,sizeof (char));
    a_SaveStream.write((char*)&m_nFederation2,sizeof (char));
    a_SaveStream.write((char*)&m_nFederation3,sizeof (char));

    a_SaveStream.write((char*)&m_nTimer1,sizeof (char));
    a_SaveStream.write((char*)&m_nTimer2,sizeof (char));

    size_t Count=0;

    // save starbase
    Count = m_Starbase.size();
    a_SaveStream.write((char*)&Count,sizeof (Count));
    for (size_t i=0; i<Count ; i++)
    {
        a_SaveStream.write((char*)&m_Starbase[i],sizeof (GameStarbaseRec));
    }

    // save planets
    Count = m_Planet.size();
    a_SaveStream.write((char*)&Count,sizeof (Count));
    for (size_t i=0; i<Count ; i++)
    {
        a_SaveStream.write((char*)&m_Planet[i],sizeof (GamePlanetRec));
    }

    // save special
    Count = m_Special.size();
    a_SaveStream.write((char*)&Count,sizeof (Count));
    for (size_t i=0; i<Count ; i++)
    {
        a_SaveStream.write((char*)&m_Special[i],sizeof (GameSpecialRec));
    }

}

void Sector::Load(ifstream & a_LoadStream)
{
   Clear();
    // load known universe sector
    a_LoadStream.read((char *)&m_blKnown,sizeof(char));
    a_LoadStream.read((char *)&m_blHasAsteroidField,sizeof(char));

    // load occupation
    a_LoadStream.read((char *)&m_nMember,sizeof(char));
    // Load ships
    a_LoadStream.read((char *)&m_nKlingonBC,sizeof(char));
    a_LoadStream.read((char *)&m_nKlingonBOP,sizeof(char));
    a_LoadStream.read((char *)&m_nRomulanBOP,sizeof(char));
    a_LoadStream.read((char *)&m_nFederation1,sizeof(char));
    a_LoadStream.read((char *)&m_nFederation2,sizeof(char));
    a_LoadStream.read((char *)&m_nFederation3,sizeof(char));

    a_LoadStream.read((char *)&m_nTimer1,sizeof(char));
    a_LoadStream.read((char *)&m_nTimer2,sizeof(char));

    size_t Count=0;

    // Load Starbase
    a_LoadStream.read((char *)&Count,sizeof(size_t));
    for (size_t i=0;i<Count;i++)
    {
        GameStarbaseRec SRec(ID_NONE);
        a_LoadStream.read((char *)&SRec,sizeof(GameStarbaseRec));
        m_Starbase.push_back(SRec);

    }

    // Load planets
    a_LoadStream.read((char *)&Count,sizeof(size_t));
    for (size_t i=0;i<Count;i++)
    {
        GamePlanetRec PRec;
        a_LoadStream.read((char *)&PRec,sizeof(GamePlanetRec));
        m_Planet.push_back(PRec);
    }

    // Load special
    a_LoadStream.read((char *)&Count,sizeof(size_t));
    for (size_t i=0;i<Count;i++)
    {
        GameSpecialRec SPRec;
        a_LoadStream.read((char *)&SPRec,sizeof(GameSpecialRec));
        m_Special.push_back(SPRec);
    }
}



void Universe::BuildSector(int a_nX, int a_nY,TEngine * a_pEngine)
{
	m_nSector[a_nX][a_nY].Build(a_pEngine);
}

void Universe::StoreSector(int a_nX, int a_nY,TEngine * a_pEngine)
{
	m_nSector[a_nX][a_nY].Store(a_pEngine);
}


Universe::Universe()
{
	m_nSectorCenter= SECTORSIZE / 2;
	m_nHomeY = m_nHomeX = UNIVERSE_SIZE/2;
	m_blWarWithRomulan = false;
    m_blWarWithKlingon = false;
}


int Universe::GetHomeX()
{
    return m_nHomeX;
}

int Universe::GetHomeY()
{
    return m_nHomeY;
}


void Universe::GenerateStarSystems()
{

    for (int y=0;y<UNIVERSE_SIZE;y++)
    {
        for (int x=0;x<UNIVERSE_SIZE;x++)
        {
            int nChance = rand() % 1000;
            if (nChance >950)
            {
                GenerateBlackHoleSystem(m_nSector[x][y]);
            }
            else if (nChance >800)
            {
                GenerateRedStarSystem(m_nSector[x][y]);
            }
            else if (nChance >450)
            {
                GenerateYellowStarSystem(m_nSector[x][y]);
            }
            else if (nChance >350)
            {
                GenerateWhiteStarSystem(m_nSector[x][y]);
            }
            else if (nChance >300)
            {
                GenerateBlueStarSystem(m_nSector[x][y]);
            }

        }
    }
}




void Universe::GenerateBlueStarSystem(Sector & a_Sector)
{
    GamePlanetRec star;
    star.m_PlanetType  = ID_STAR_BLUE;
    star.m_Member      = MEM_NEUTRAL;
    star.m_nX          = m_nSectorCenter;
    star.m_nY          = m_nSectorCenter;
    a_Sector.m_Planet.push_back(star);

    int nChance = rand() % 1000;
    if (nChance > 700)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_A;
        planet.m_Member       =  MEM_NONE;
        planet.m_nX           = m_nSectorCenter+(200 * cos(dAngle));
        planet.m_nY           = m_nSectorCenter+(200 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 900)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_B;
        planet.m_Member       =  MEM_NONE;
        planet.m_nX           = m_nSectorCenter+(400 * cos(dAngle));
        planet.m_nY           = m_nSectorCenter+(400 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

}

void Universe::GenerateWhiteStarSystem(Sector & a_Sector)
{
    GamePlanetRec star;
    star.m_PlanetType  = ID_STAR_WHITE;
    star.m_Member      = MEM_NEUTRAL;
    star.m_nX          = m_nSectorCenter;
    star.m_nY          = m_nSectorCenter;
    a_Sector.m_Planet.push_back(star);

    int nChance = rand() % 1000;
    if (nChance > 950)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_A;
        planet.m_Member       =  MEM_NONE;
        planet.m_nX           = m_nSectorCenter+(200 * cos(dAngle));
        planet.m_nY           = m_nSectorCenter+(200 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 700)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_B;
        planet.m_Member       =  MEM_NONE;
        planet.m_nX           = m_nSectorCenter+(500 * cos(dAngle));
        planet.m_nY           = m_nSectorCenter+(500 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 650)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_L;
        planet.m_Member       =  MEM_NEUTRAL;
        planet.m_nPopulation  =  rand() % 100;
        planet.m_nX           =  m_nSectorCenter+(800 * cos(dAngle));
        planet.m_nY           =  m_nSectorCenter+(800 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }


}

void Universe::GenerateYellowStarSystem(Sector & a_Sector)
{
    GamePlanetRec star;
    star.m_PlanetType  = ID_STAR_YELLOW;
    star.m_Member      = MEM_NEUTRAL;
    star.m_nX          = m_nSectorCenter;
    star.m_nY          = m_nSectorCenter;
    a_Sector.m_Planet.push_back(star);

    int nChance = rand() % 1000;
    if (nChance > 950)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_A;
        planet.m_Member       =  MEM_NONE;
        planet.m_nX           = m_nSectorCenter+(200 * cos(dAngle));
        planet.m_nY           = m_nSectorCenter+(200 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 700)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_B;
        planet.m_Member       =  MEM_NONE;
        planet.m_nX           = m_nSectorCenter+(800 * cos(dAngle));
        planet.m_nY           = m_nSectorCenter+(800 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 500)
    {
         a_Sector.m_blHasAsteroidField = true;
        int nChance2 = rand() % 1000;
        if (nChance2>600)
        {
             double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
             GamePlanetRec planet;
             planet.m_PlanetType   =  ID_CLASS_M3;
             planet.m_Member       =  MEM_NEUTRAL;
             planet.m_nPopulation  =  rand() % 80000;
             planet.m_nX           =  m_nSectorCenter+(500 * cos(dAngle));
             planet.m_nY           =  m_nSectorCenter+(1000 * sin(dAngle));
             a_Sector.m_Planet.push_back(planet);
        }
        else if (nChance2>400)
        {
             double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
             GamePlanetRec planet;
             planet.m_PlanetType   =  ID_CLASS_M2;
             planet.m_Member       =  MEM_NEUTRAL;
             planet.m_nPopulation  =  rand() % 40000;
             planet.m_nX           =  m_nSectorCenter+(1000 * cos(dAngle));
             planet.m_nY           =  m_nSectorCenter+(500 * sin(dAngle));
             a_Sector.m_Planet.push_back(planet);
        }
        else
        {
             double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
             GamePlanetRec planet;
             planet.m_PlanetType   =  ID_CLASS_M1;
             planet.m_Member       =  MEM_NEUTRAL;
             planet.m_nPopulation  =  rand() % 20000;
             planet.m_nX           =  m_nSectorCenter+(500 * cos(dAngle));
             planet.m_nY           =  m_nSectorCenter+(400 * sin(dAngle));
             a_Sector.m_Planet.push_back(planet);
        }
    }

}

void Universe::GenerateRedStarSystem(Sector & a_Sector)
{
    a_Sector.m_blHasAsteroidField = true;

    GamePlanetRec star;
    star.m_PlanetType  = ID_STAR_RED;
    star.m_Member      = MEM_NEUTRAL;
    star.m_nX          = m_nSectorCenter;
    star.m_nY          = m_nSectorCenter;
    a_Sector.m_Planet.push_back(star);

    int nChance = rand() % 1000;
    if (nChance > 900)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_M2;
        planet.m_Member       =  MEM_NEUTRAL;
        planet.m_nPopulation  =  rand() % 500;
        planet.m_nX           =  m_nSectorCenter+(600 * cos(dAngle));
        planet.m_nY           =  m_nSectorCenter+(600 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 700)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_L;
        planet.m_Member       =  MEM_NEUTRAL;
        planet.m_nPopulation  =  rand() % 50;
        planet.m_nX           =  m_nSectorCenter+(400 * cos(dAngle));
        planet.m_nY           =  m_nSectorCenter+(400 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

}

void Universe::GenerateBlackHoleSystem(Sector & a_Sector)
{
    GamePlanetRec star;
    star.m_PlanetType  = ID_BLACK_HOLE;
    star.m_Member      = MEM_NEUTRAL;
    star.m_nX          = m_nSectorCenter;
    star.m_nY          = m_nSectorCenter;
    a_Sector.m_Planet.push_back(star);

    int nChance = rand() % 1000;
    if (nChance > 600)
    {
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_M2;
        planet.m_Member       =  MEM_NEUTRAL;
        planet.m_nPopulation  =  rand() % 500;
        planet.m_nX           =  m_nSectorCenter+(1300 * cos(dAngle));
        planet.m_nY           =  m_nSectorCenter+(1300 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }

    if (nChance > 800)
    {
         a_Sector.m_blHasAsteroidField = true;
        double dAngle = (rand()/(double(RAND_MAX)+1))*2*PI;
        GamePlanetRec planet;
        planet.m_PlanetType   =  ID_CLASS_L;
        planet.m_Member       =  MEM_NEUTRAL;
        planet.m_nPopulation  =  rand() % 50;
        planet.m_nX           =  m_nSectorCenter+(1500 * cos(dAngle));
        planet.m_nY           =  m_nSectorCenter+(1500 * sin(dAngle));
        a_Sector.m_Planet.push_back(planet);
    }
}





void Universe::GenerateHomeSector(Sector & a_Sector)
{
    a_Sector.m_blHasAsteroidField = false;
    a_Sector.m_nMember = MEM_FEDERATION;
    a_Sector.m_Planet.clear();
    a_Sector.m_Starbase.clear();

    GamePlanetRec star;
    star.m_PlanetType = ID_STAR_YELLOW;
    star.m_Member     = MEM_FEDERATION;
    star.m_nX          = m_nSectorCenter;
    star.m_nY          = m_nSectorCenter;
    a_Sector.m_Planet.push_back(star);

    GamePlanetRec mercury;
    mercury.m_PlanetType  =  ID_CLASS_A;
    mercury.m_Member      =  MEM_NONE;
    mercury.m_nX           = m_nSectorCenter - 200;
    mercury.m_nY           = m_nSectorCenter + 200;
    a_Sector.m_Planet.push_back(mercury);

    GamePlanetRec earth;
    earth.m_PlanetType =  ID_CLASS_M2;
    earth.m_Member     =  MEM_FEDERATION;
    earth.m_nX         =  m_nSectorCenter + 1500;
    earth.m_nY         =  m_nSectorCenter;

    // for testing , remove !!
    earth.m_Item       = INV_SPOCK;
    ////////////////////////////////

    a_Sector.m_Planet.push_back(earth);


    GameStarbaseRec starbase(ID_FEDERATIONBASE);
    starbase.m_nX           = m_nSectorCenter + 800;
	starbase.m_nY           = m_nSectorCenter + 100;
	starbase.m_Item         = INV_GORKON;
    a_Sector.m_Starbase.push_back(starbase);

    a_Sector.m_blKnown = true;

}


void Universe::CreateKahnQuest(Sector & a_Sector)
{
    GameSpecialRec SRec;
    SRec.m_Member = MEM_FEDERATION;
    SRec.m_SpecialType = ID_RELIANT;
    a_Sector.m_Special.push_back(SRec);

    a_Sector.m_nFederation1 = 0;
    a_Sector.m_nFederation2 = 0;
    a_Sector.m_nFederation3 = 0;
    a_Sector.m_blHasAsteroidField = true;
}



void Universe::BuildFederationSpace()
{
    for (int y=0; y<10; y++)
    {
        for (int x=0; x<10;x++)
        {
            int secX = (m_nHomeX-5)+x;
            int secY = (m_nHomeY-5)+y;
            // occupy sector
            int dist =  Distance(m_nHomeX,m_nHomeY,secX,secY);

            if (dist<5)
            {
                m_nSector[secX][secY].m_nMember = MEM_FEDERATION;
                m_nSector[secX][secY].m_nFederation1 = (rand() % 500) / 100;


                int basechance = rand() % 100;
                if ((basechance>70)&&(!m_nSector[secX][secY].HasSpaceObjectType(ID_BLACK_HOLE)))
                {
                    // build starbase
                    GameStarbaseRec starbase(ID_FEDERATIONBASE);
                     m_nSector[secX][secY].m_blKnown = true;


                    GamePlanetRec * pPlanet = m_nSector[secX][secY].GetPlanetForStarbase();
                    if (pPlanet!=NULL)
                    {
                        starbase.m_nX           = (pPlanet->m_nX -200)+ (rand() % 400);
                        starbase.m_nY           = (pPlanet->m_nY -200)+ (rand() % 400);
                    }
                    else
                    {
                        // random position
                        starbase.m_nX           = (m_nSectorCenter -1000)+ (rand() % 2000);
                        starbase.m_nY           = (m_nSectorCenter -1000)+ (rand() % 2000);
                    }


                    m_nSector[secX][secY].m_Starbase.push_back(starbase);
                }
            }
        }
    }




    CreateKahnQuest(m_nSector[(m_nHomeX-2)][(m_nHomeY-2)]);


    GenerateHomeSector(m_nSector[m_nHomeX][m_nHomeY]);
}



int Universe::GetSectorState(int a_nX,int a_nY)
{
    return  m_nSector[a_nX][a_nY].m_nMember;
}

bool Universe::HasSingularity(int a_nX,int a_nY)
{
    size_t nIndex =0;
    bool blFound= false;
    while ((nIndex < m_nSector[a_nX][a_nY].m_Planet.size())&&(!blFound))
    {
        if(m_nSector[a_nX][a_nY].m_Planet[nIndex].m_PlanetType == ID_BLACK_HOLE)
        {
            blFound = true;
        }
        nIndex++;
    }
    return  blFound;
}





void Universe::BuildKlingonSpace()
{
    for (int y=0; y<12; y++)
    {
        for (int x=0; x<12;x++)
        {
            int secX = (KLINGON_EMPIRE_CENTER_X-6)+x;
            int secY = (KLINGON_EMPIRE_CENTER_Y-6)+y;
            int dist =  Distance(KLINGON_EMPIRE_CENTER_X,KLINGON_EMPIRE_CENTER_Y,secX,secY);

            if (dist<5)
            {
                m_nSector[secX][secY].m_nMember = MEM_KLINGON;
                m_nSector[secX][secY].m_nKlingonBC =(rand() % 500) / 100;
                m_nSector[secX][secY].m_nKlingonBOP =(rand() % 200) / 100;

                int basechance = rand() % 100;
                if ((basechance>90)&&(!m_nSector[secX][secY].HasSpaceObjectType(ID_BLACK_HOLE)))
                {
                    // build starbase
                    GameStarbaseRec starbase(ID_KLINGON_BASE);

                    GamePlanetRec * pPlanet = m_nSector[secX][secY].GetPlanetForStarbase();
                    if (pPlanet!=NULL)
                    {
                        starbase.m_nX           = (pPlanet->m_nX -200)+ (rand() % 400);
                        starbase.m_nY           = (pPlanet->m_nY -200)+ (rand() % 400);
                    }
                    else
                    {
                        // random position
                        starbase.m_nX           = (m_nSectorCenter -1000)+ (rand() % 2000);
                        starbase.m_nY           = (m_nSectorCenter -1000)+ (rand() % 2000);
                    }


                    m_nSector[secX][secY].m_Starbase.push_back(starbase);
                }

            }
        }
    }
}



void Universe::BuildRomulanSpace()
{
for (int y=0; y<22; y++)
    {
        for (int x=0; x<20;x++)
        {
            int secX = (ROMULAN_EMPIRE_CENTER_X-10)+x;
            int secY = (ROMULAN_EMPIRE_CENTER_Y-11)+y;
            int dist =  Distance(ROMULAN_EMPIRE_CENTER_X,ROMULAN_EMPIRE_CENTER_Y,secX,secY);

            if (dist<9)
            {
                m_nSector[secX][secY].m_nMember = MEM_ROMULAN;
                m_nSector[secX][secY].m_nRomulanBOP=((rand() % 400) / 100)+1;

                int basechance = rand() % 100;
                if ((basechance>80)&&(!m_nSector[secX][secY].HasSpaceObjectType(ID_BLACK_HOLE)))
                {
                    // build starbase
                    GameStarbaseRec starbase(ID_ROMULAN_BASE);

                    GamePlanetRec * pPlanet = m_nSector[secX][secY].GetPlanetForStarbase();
                    if (pPlanet!=NULL)
                    {
                        starbase.m_nX           = (pPlanet->m_nX -200)+ (rand() % 400);
                        starbase.m_nY           = (pPlanet->m_nY -200)+ (rand() % 400);
                    }
                    else
                    {
                        // random position
                        starbase.m_nX           = (m_nSectorCenter -1000)+ (rand() % 2000);
                        starbase.m_nY           = (m_nSectorCenter -1000)+ (rand() % 2000);
                    }

                    m_nSector[secX][secY].m_Starbase.push_back(starbase);
                }

            }
            else
            {
                if ((secX<ROMULAN_EMPIRE_CENTER_X)&&(secY>ROMULAN_EMPIRE_CENTER_Y)&&(dist<11))
                {
                    m_nSector[secX][secY].m_nMember = MEM_NEUTRAL_ZONE;

                }

            }
        }
    }
}



void Universe::Generate()
{
	srand (time(NULL));
	m_nHomeY = m_nHomeX = UNIVERSE_SIZE/2;

    GenerateStarSystems();
    BuildRomulanSpace();
    BuildKlingonSpace();
    BuildFederationSpace();
}


void Universe::Save(ofstream & a_SaveStream)
{
    char szVersion[18]="UNIVERSE_V0000002";
    a_SaveStream.write((char*)szVersion,sizeof (szVersion));
    a_SaveStream.write((char*)&m_blWarWithKlingon,sizeof (char));
    a_SaveStream.write((char*)&m_blWarWithRomulan,sizeof (char));
    for (int y=0;y<UNIVERSE_SIZE;y++)
    {
        for (int x=0;x<UNIVERSE_SIZE;x++)
        {
            m_nSector[x][y].Save(a_SaveStream);
        }
    }
}


void Universe::Load(ifstream & a_LoadStream)
{
    char szVersion[18];
	a_LoadStream.read((char *)szVersion,sizeof(char)*18);
	a_LoadStream.read((char *)&m_blWarWithKlingon,sizeof(char));
	a_LoadStream.read((char *)&m_blWarWithRomulan,sizeof(char));
	for (int y=0;y<UNIVERSE_SIZE;y++)
    {
        for (int x=0;x<UNIVERSE_SIZE;x++)
        {
             m_nSector[x][y].Load(a_LoadStream);
        }
    }
}


void  Universe::DoNPCActions(int a_nEnterpriseSectorX, int a_nEnterpriseSectorY,TEngine * a_pEngine)
{

    //TODO (Edwin#1#): parse all sectors for action !, remove or add ships to current sector accordingly !
    for (int y=0;y<UNIVERSE_SIZE;y++)
    {
        for (int x=0;x<UNIVERSE_SIZE;x++)
        {
            DoFederationActions(x, y, a_nEnterpriseSectorX, a_nEnterpriseSectorY, a_pEngine);
            DoKlingonActions(x, y, a_nEnterpriseSectorX, a_nEnterpriseSectorY, a_pEngine);
            DoRomulanActions(x, y, a_nEnterpriseSectorX, a_nEnterpriseSectorY, a_pEngine);
        }
    }
}



void Universe::DoFederationActions(int a_nX, int a_nY,int a_nEnterpriseSectorX, int a_nEnterpriseSectorY, TEngine * a_pEngine)
{
    // Federation actions
    if (m_nSector[a_nX][a_nY].m_nFederation1 > 0)
    {
        if (( m_nSector[a_nX][a_nY].m_nKlingonBC  == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nKlingonBOP == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nRomulanBOP == 0 ))
        {

            if ((a_nX != a_nEnterpriseSectorX)||(a_nX != a_nEnterpriseSectorY))
            {
                TDestination destination = GetDestination(a_nX,a_nY);

                if ((m_nSector[destination.m_nX][destination.m_nY].m_nFederation1 < 8) &&
                    ((m_nSector[destination.m_nX][destination.m_nY].m_nMember != MEM_KLINGON)||(m_blWarWithKlingon))&&
                    (((m_nSector[destination.m_nX][destination.m_nY].m_nMember != MEM_ROMULAN) && (m_nSector[destination.m_nX][destination.m_nY].m_nMember != MEM_NEUTRAL_ZONE))||(m_blWarWithRomulan)))
                {
                    #ifdef _DEBUG
                    std::cout << "Ships here: " << (int)m_nSector[a_nX][a_nY].m_nFederation1 << "   Move";
                    std::cout << "(" << a_nX << "," << a_nY << ") --> ";
                    std::cout << "(" << destination.m_nX << "," << destination.m_nY << ")\n";
                    std::cout << std::flush;
                    #endif // _DEBUG

                    m_nSector[a_nX][a_nY].m_nFederation1--;
                    m_nSector[destination.m_nX][destination.m_nY].m_nFederation1++;

                    if ((destination.m_nX == a_nEnterpriseSectorX) && (destination.m_nY == a_nEnterpriseSectorY))
                    {
                        // Ship reached current sector, add to engine

                        int RX = a_pEngine->GetDx() + ((rand() % 1000) - 500);
                        int RY = a_pEngine->GetDy() + ((rand() % 600) - 300);

                        TAnimation * pAnimation = new TAnimation(ANIMATION::WARP_IN,
                                                                 RX,RY,
                                                                 0,
                                                                 0,
                                                                 30);
                        a_pEngine->Add(pAnimation);
                        TFederation_Ship * pFederation = new TFederation_Ship();
                        pFederation->SetPosition(RX,RY);
                        a_pEngine->Add(pFederation);
                    }
                }

            }

            if (m_nSector[a_nX][a_nY].m_nFederation1 > 4)
            {
                m_nSector[a_nX][a_nY].m_nMember = MEM_FEDERATION;
            }
        }
        else
        {
            // Battle
            int threshold = m_nSector[a_nX][a_nY].m_nKlingonBC * 2  + m_nSector[a_nX][a_nY].m_nKlingonBOP * 5 + m_nSector[a_nX][a_nY].m_nRomulanBOP*10;

            int K = rand() % 100;
            if (K < threshold)
            {
                if (m_nSector[a_nX][a_nY].m_nFederation1 > 0)
                {
                    m_nSector[a_nX][a_nY].m_nFederation1--;
                }
                else if (m_nSector[a_nX][a_nY].m_nFederation2 > 0)
                {
                    m_nSector[a_nX][a_nY].m_nFederation2--;
                }
                else if (m_nSector[a_nX][a_nY].m_nFederation3 > 0)
                {
                    m_nSector[a_nX][a_nY].m_nFederation3--;
                }
            }

            char szMessage[100];
            sprintf(szMessage,"Starfleet reports skirmish in sector [%d,%d]",a_nX, a_nY);
            g_pCommunication->AddMessage(100,CREW_UHURA,szMessage);

        }
    }

    if (m_nSector[a_nX][a_nY].HasBase(MEM_FEDERATION))
    {
        if (m_nSector[a_nX][a_nY].m_nFederation1 < 4)
        {
            m_nSector[a_nX][a_nY].m_nFederation1++;
        }
        //else if (m_nSector[a_nX][a_nY].m_nFederation2 < 4)
        {
         //   m_nSector[a_nX][a_nY].m_nFederation2++;
        }
        //else if (m_nSector[a_nX][a_nY].m_nFederation3 < 4)
        {
         //   m_nSector[a_nX][a_nY].m_nFederation3++;
        }
    }
}

void Universe::DoKlingonActions(int a_nX, int a_nY,int a_nEnterpriseSectorX, int a_nEnterpriseSectorY, TEngine * a_pEngine)
{
    // Klingon actions
    if (m_nSector[a_nX][a_nY].m_nKlingonBC > 0)
    {
        if (( m_nSector[a_nX][a_nY].m_nFederation1 == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nFederation2 == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nFederation3 == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nRomulanBOP  == 0 ) )
        {

            if ((a_nX != a_nEnterpriseSectorX)||(a_nX != a_nEnterpriseSectorY))
            {
                TDestination destination = GetDestination(a_nX,a_nY);
                if ((m_nSector[destination.m_nX][destination.m_nY].m_nKlingonBC < 4) &&
                    ((m_nSector[destination.m_nX][destination.m_nY].m_nMember != MEM_FEDERATION)||(m_blWarWithKlingon)))
                {
                    #ifdef _DEBUG
                    std::cout << "Battlecruiser : " << (int)m_nSector[a_nX][a_nY].m_nKlingonBC << "   Move";
                    std::cout << "(" << a_nX << "," << a_nY << ") --> ";
                    std::cout << "(" << destination.m_nX << "," << destination.m_nY << ")\n";
                    std::cout << std::flush;
                    #endif // _DEBUG

                    m_nSector[a_nX][a_nY].m_nKlingonBC--;
                    m_nSector[destination.m_nX][destination.m_nY].m_nKlingonBC++;

                    if ((destination.m_nX == a_nEnterpriseSectorX) && (destination.m_nY == a_nEnterpriseSectorY))
                    {
                        int RX = a_pEngine->GetDx() + ((rand() % 1000) - 500);
                        int RY = a_pEngine->GetDy() + ((rand() % 600) - 300);

                        TAnimation * pAnimation = new TAnimation(ANIMATION::WARP_IN,
                                                                 RX,RY,
                                                                 0,
                                                                 0,
                                                                 30);
                        a_pEngine->Add(pAnimation);
                        TKlingonBC * pKlingon = new TKlingonBC();
                        pKlingon->SetPosition(RX,RY);
                        a_pEngine->Add(pKlingon);
                    }
                }
            }

        }
        else
        {
            // Battle
            int threshold = m_nSector[a_nX][a_nY].m_nFederation1 * 2  + m_nSector[a_nX][a_nY].m_nFederation2 * 2 +m_nSector[a_nX][a_nY].m_nFederation3 *2 + m_nSector[a_nX][a_nY].m_nRomulanBOP*5;

            int K = rand() % 100;
            if (K < threshold)
            {
                if (m_nSector[a_nX][a_nY].m_nKlingonBC > 0)
                {
                    m_nSector[a_nX][a_nY].m_nKlingonBC--;
                }
            }
        }

    }

    if (m_nSector[a_nX][a_nY].m_nKlingonBOP > 1)
    {
        if (( m_nSector[a_nX][a_nY].m_nFederation1 == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nFederation2 == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nFederation3 == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nRomulanBOP  == 0 ) )
        {

            if ((a_nX != a_nEnterpriseSectorX)||(a_nX != a_nEnterpriseSectorY))
            {
                TDestination destination = GetDestination(a_nX,a_nY);

                if ((m_nSector[destination.m_nX][destination.m_nY].m_nKlingonBOP <6) &&
                    ((m_nSector[destination.m_nX][destination.m_nY].m_nMember != MEM_FEDERATION)||(m_blWarWithKlingon)))
                {
                    #ifdef _DEBUG
                    std::cout << "Bird of prey: " << (int)m_nSector[a_nX][a_nY].m_nKlingonBOP << "   Move";
                    std::cout << "(" << a_nX << "," << a_nY << ") --> ";
                    std::cout << "(" << destination.m_nX << "," << destination.m_nY << ")\n";
                    std::cout << std::flush;
                    #endif // _DEBUG

                    m_nSector[a_nX][a_nY].m_nKlingonBOP-=2;
                    m_nSector[destination.m_nX][destination.m_nY].m_nKlingonBOP+=2;

                    if ((destination.m_nX == a_nEnterpriseSectorX) && (destination.m_nY == a_nEnterpriseSectorY))
                    {
                        for (int i=0; i<2; i++)
                        {
                            int RX = a_pEngine->GetDx() + ((rand() % 1000) - 500);
                            int RY = a_pEngine->GetDy() + ((rand() % 600) - 300);

                            TAnimation * pAnimation = new TAnimation(ANIMATION::WARP_IN,
                                                             RX,RY,
                                                             0,
                                                             0,
                                                             30);
                            a_pEngine->Add(pAnimation);
                            TKlingonBOP * pKlingon = new TKlingonBOP();
                            pKlingon->SetPosition(RX,RY);
                            a_pEngine->Add(pKlingon);
                        }
                    }
                }
            }
        }
        else
        {
            // Battle
            int threshold = m_nSector[a_nX][a_nY].m_nFederation1 * 2  + m_nSector[a_nX][a_nY].m_nFederation2 * 2 +m_nSector[a_nX][a_nY].m_nFederation3 *2 + m_nSector[a_nX][a_nY].m_nRomulanBOP*5;

            int K = rand() % 100;
            if (K < threshold)
            {
                if (m_nSector[a_nX][a_nY].m_nKlingonBOP > 0)
                {
                    m_nSector[a_nX][a_nY].m_nKlingonBOP--;
                }
            }
        }
    }

    if (( m_nSector[a_nX][a_nY].m_nFederation1 == 0 ) &&
        ( m_nSector[a_nX][a_nY].m_nFederation2 == 0 ) &&
        ( m_nSector[a_nX][a_nY].m_nFederation3 == 0 ) &&
        ( m_nSector[a_nX][a_nY].m_nRomulanBOP  == 0 ) &&
        (m_nSector[a_nX][a_nY].m_nKlingonBOP + m_nSector[a_nX][a_nY].m_nKlingonBC> 5))
    {
                m_nSector[a_nX][a_nY].m_nMember = MEM_KLINGON;
    }

    if (m_nSector[a_nX][a_nY].HasBase(MEM_KLINGON))
    {
        if (m_nSector[a_nX][a_nY].m_nKlingonBC < 5)
        {
            m_nSector[a_nX][a_nY].m_nKlingonBC++;
        }
        else if (m_nSector[a_nX][a_nY].m_nKlingonBOP < 3)
        {
            m_nSector[a_nX][a_nY].m_nKlingonBOP++;
        }
    }

}

void Universe::DoRomulanActions(int a_nX, int a_nY,int a_nEnterpriseSectorX, int a_nEnterpriseSectorY, TEngine * a_pEngine)
{
    // Romulan actions
    if (m_nSector[a_nX][a_nY].m_nRomulanBOP > 0)
    {
        if (( m_nSector[a_nX][a_nY].m_nKlingonBC  == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nKlingonBOP == 0 ) &&
            ( m_nSector[a_nX][a_nY].m_nFederation1 == 0 )&&
            ( m_nSector[a_nX][a_nY].m_nFederation2 == 0 )&&
            ( m_nSector[a_nX][a_nY].m_nFederation3 == 0 ))
        {

            if ((a_nX != a_nEnterpriseSectorX)||(a_nX != a_nEnterpriseSectorY))
            {
                TDestination destination = GetDestination(a_nX,a_nY);

                if ((m_nSector[destination.m_nX][destination.m_nY].m_nRomulanBOP < 6) &&
                   ((m_nSector[destination.m_nX][destination.m_nY].m_nMember != MEM_FEDERATION)||(m_blWarWithRomulan)))

                {
                    #ifdef _DEBUG
                    std::cout << "Romulan: " << (int)m_nSector[a_nX][a_nY].m_nRomulanBOP << "   Move";
                    std::cout << "(" << a_nX << "," << a_nY << ") --> ";
                    std::cout << "(" << destination.m_nX << "," << destination.m_nY << ")\n";
                    std::cout << std::flush;
                    #endif // _DEBUG

                    m_nSector[a_nX][a_nY].m_nRomulanBOP--;
                    m_nSector[destination.m_nX][destination.m_nY].m_nRomulanBOP++;

                    if ((destination.m_nX == a_nEnterpriseSectorX) && (destination.m_nY == a_nEnterpriseSectorY))
                    {
                        // Ship reached current sector, add to engine

                        int RX = a_pEngine->GetDx() + ((rand() % 1000) - 500);
                        int RY = a_pEngine->GetDy() + ((rand() % 600) - 300);

                        TAnimation * pAnimation = new TAnimation(ANIMATION::WARP_IN,
                                                                 RX,RY,
                                                                 0,
                                                                 0,
                                                                 30);
                        a_pEngine->Add(pAnimation);
                        TRomulanBop * pRomulan = new TRomulanBop();
                        pRomulan->SetPosition(RX,RY);
                        a_pEngine->Add(pRomulan);
                    }
                }

            }
            if (m_nSector[a_nX][a_nY].m_nRomulanBOP > 4)
            {
                m_nSector[a_nX][a_nY].m_nMember = MEM_ROMULAN;
            }
        }
        else
        {
            // Battle
            int threshold = m_nSector[a_nX][a_nY].m_nKlingonBC * 2   +
                            m_nSector[a_nX][a_nY].m_nKlingonBOP * 5  +
                            m_nSector[a_nX][a_nY].m_nFederation1 * 2 +
                            m_nSector[a_nX][a_nY].m_nFederation2 * 2 +
                            m_nSector[a_nX][a_nY].m_nFederation3 * 2;

            int K = rand() % 100;
            if (K < threshold)
            {
                if (m_nSector[a_nX][a_nY].m_nRomulanBOP > 0)
                {
                    m_nSector[a_nX][a_nY].m_nRomulanBOP--;
                }
            }
        }
    }

    if (m_nSector[a_nX][a_nY].HasBase(MEM_ROMULAN))
    {
        if (m_nSector[a_nX][a_nY].m_nRomulanBOP < 6)
        {
            m_nSector[a_nX][a_nY].m_nRomulanBOP++;
        }
    }
}



void Universe::StartKlingonWar()
{
    if (!m_blWarWithKlingon)
    {
       g_pCommunication->AddMessage(40,CREW_UHURA,"High Council: Klingons declared war !");
       m_blWarWithKlingon = true;
    }

}

void Universe::StartRomulanWar()
{
    if (!m_blWarWithRomulan)
    {
        g_pCommunication->AddMessage(40,CREW_UHURA,"High Council: Romulans declared war !");
        m_blWarWithRomulan = true;
    }
}

void Universe::EndKlingonWar()
{
    if (m_blWarWithKlingon)
    {
       g_pCommunication->AddMessage(40,CREW_UHURA,"High Council: War with klingons ended !");
       m_blWarWithKlingon = false;
    }
}

void Universe::EndRomulanWar()
{
     if (m_blWarWithRomulan)
     {
         g_pCommunication->AddMessage(40,CREW_UHURA,"High Council: War with romulans ended !");
         m_blWarWithRomulan = false;
     }
}

bool Universe::WarWithKlingon()
{
    return m_blWarWithKlingon;
}

bool Universe::WarWithRomulan()
{
    return m_blWarWithRomulan;
}


TDestination Universe::GetDestination(int a_nX, int a_nY)
{
    int nD = (rand() % 700) / 100;
    TDestination destination;

    switch (nD)
    {
        case 0:
        // move UP if possible
        destination.m_nX    = a_nX;
        destination.m_nY    = a_nY > 0 ? a_nY - 1 : a_nY;
        break;

        case 1:
        // move UP RIGHT
        destination.m_nX    = a_nX < (UNIVERSE_SIZE - 1) ? a_nX + 1 : a_nX;
        destination.m_nY    = a_nY > 0 ? a_nY - 1 : a_nY;
        break;

        case 2:
        // move RIGHT if possible
        destination.m_nX    = a_nX < (UNIVERSE_SIZE - 1) ? a_nX + 1 : a_nX;
        destination.m_nY    = a_nY;
        break;

        case 3:
        // Move DOWN RIGHT if possible
        destination.m_nX    = a_nX < (UNIVERSE_SIZE - 1) ? a_nX + 1 : a_nX;
        destination.m_nY    = a_nY < (UNIVERSE_SIZE - 1) ? a_nY + 1 : a_nY;
        break;

        case 4:
        // move DOWN if possible
        destination.m_nX    = a_nX;
        destination.m_nY    = a_nY < (UNIVERSE_SIZE - 1) ? a_nY + 1 : a_nY;
        break;

        case 5:
        // Move DOWN LEFT if possible
        destination.m_nX    = a_nX > 0 ? a_nX - 1 : a_nX;
        destination.m_nY    = a_nY < (UNIVERSE_SIZE - 1) ? a_nY + 1 : a_nY;
        break;

        case 6:
        // move LEFT if possible
        destination.m_nX    = a_nX > 0 ? a_nX - 1 : a_nX;
        destination.m_nY    = a_nY;
        break;

        case 7:
        // move LEFT UP if possible
        destination.m_nX    = a_nX > 0 ? a_nX - 1 : a_nX;
        destination.m_nY    = a_nY > 0 ? a_nY - 1 : a_nY;
        break;
    }
    return destination;
}

