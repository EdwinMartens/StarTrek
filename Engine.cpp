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

#include <stdio.h>
#include <iostream>
#include <ostream>

#include "Global.h"
#include "Engine.h"
#include "Ship.h"
#include "Bullet.h"
#include "Animation.h"
#include "SpaceObject.h"
#include "Enterprise.h"
#include "Klingon_BC.h"
#include "Klingon_BOP.h"
#include "Romulan_BOP.h"
#include "Federation_Ship.h"
#include "Reliant.h"
#include "Probe.h"
#include "particle.h"
#include "Universe.h"
#include "SoundManager.h"
#include "Communication.h"
#include "utils.h"
#include "FontManager.h"

#define Number_Of_Stars 300
class TEnterprise;

extern TEnterprise * g_pEnterprise;
extern Universe * g_pUniverse;
extern Communication * g_pCommunication;

bool Comp(const TSprite *v1, const TSprite *v2);


char g_szMessage[255];


// Sprite methods..
void TSprite::Draw(double a_dCamX,double a_dCamY)
{
	double DrawX=int(m_pEngine->m_nScreenMidX+(m_dX-a_dCamX))-m_nBitmapMidX;
	double DrawY=int(m_pEngine->m_nScreenMidY+(m_dY-a_dCamY))-m_nBitmapMidY;
	// only draw what we see
	if ((DrawX+m_nBitmapWidth>0)&&(DrawX-m_nBitmapWidth<m_pEngine->m_nScreenWidth)&&(DrawY+m_nBitmapHeight>0)&& (DrawY-m_nBitmapHeight<m_pEngine->m_nScreenHeight))
	{
       al_draw_bitmap(m_pImage, DrawX, DrawY, 0);
	}
}

void TSprite::Move(double a_dLagCount)
{
	if ((m_pEngine!=NULL)&&(m_pEngine->m_blHasBlackHole))
    {
        if ((m_ID >= ID_PROBE)&&(m_ID < ID_BASE_BOTTOM))
        {
            double DD = Distance(m_dX,m_dY,(double)g_pUniverse->m_nSectorCenter,(double)g_pUniverse->m_nSectorCenter);

            m_dX-= (m_dX-g_pUniverse->m_nSectorCenter)/(DD+1 * 8);
            m_dY-= (m_dY-g_pUniverse->m_nSectorCenter)/(DD+1 * 8);
        }
    }


	m_dX+=cos(m_dAngle)*m_dSpeed * a_dLagCount;
	m_dY+=sin(m_dAngle)*m_dSpeed * a_dLagCount;
}


void TSprite::SetImage(ALLEGRO_BITMAP * a_pImage)
{
	m_pImage		=	a_pImage;
	m_nBitmapWidth	=	al_get_bitmap_width(m_pImage);
    m_nBitmapHeight	=	al_get_bitmap_height(m_pImage);
	m_nBitmapMidX = m_nBitmapWidth/2;
	m_nBitmapMidY = m_nBitmapHeight/2;
}


double TSprite::GetX()
{
	return m_dX;
}

double TSprite::GetY()
{
	return m_dY;
}


void TSprite::Do_ai()
{


}

void TSprite::DoCollision(TSprite * const a_pSprite)
{

}


void TSprite::SetPosition(double a_dX, double a_dY, double a_dAngle)
{
	m_dX= a_dX;
	m_dY= a_dY;
	m_dAngle = a_dAngle;
}

void TSprite::SetPosition(double a_dX, double a_dY)
{
	m_dX= a_dX;
	m_dY= a_dY;
}


void TSprite::SetHeight(int a_nHeight)
{
	m_nZ = a_nHeight;
}

void TSprite::SetSpeed(double a_dSpeed)
{
	m_dSpeed = a_dSpeed;
}

TSprite::TSprite()
{
	m_dX      =  0;
    m_dY      =  0;
    m_nZ      =  0;
	m_dSpeed  =  0;
    m_dAngle  =  0;

    m_blDestroyed  = false;
	m_blCanCollide = false;
	m_blCanFind    = false;
	#ifdef _DEBUG
    cout << "Sprite created ! \n" << std::flush;
    #endif // _DEBUG
}


TSprite::TSprite(TSprite & a_rSprite)
{
    #ifdef _DEBUG
    cout << "Warning  sprite copied ! \n"<< std::flush;
    #endif // _DEBUG
}



// load constructor
TSprite::TSprite(ifstream & a_LoadStream,ID a_id)
{
    m_ID = a_id;
	m_blDestroyed  = false;
    a_LoadStream.read((char *)&m_Member, sizeof (m_Member));
    a_LoadStream.read((char *)&m_dX, sizeof (m_dX));
    a_LoadStream.read((char *)&m_dY, sizeof (m_dY));
    a_LoadStream.read((char *)&m_nZ, sizeof (m_nZ));
    a_LoadStream.read((char *)&m_dAngle, sizeof (m_dAngle));
    a_LoadStream.read((char *)&m_dSpeed, sizeof (m_dSpeed));
    a_LoadStream.read((char *)&m_blCanFind, sizeof (m_blCanFind));

    #ifdef _DEBUG
    cout << "Sprite Loaded ! \n"<< std::flush;
    #endif // _DEBUG
}

// save function
void TSprite::Save(ofstream & a_SaveStream)
{
    // save ID, this is read by Engine.load !,
    // so do not read in constructor !
    a_SaveStream.write((char*)& m_ID, sizeof (m_ID));

    a_SaveStream.write((char*)& m_Member, sizeof (m_Member));
    a_SaveStream.write((char*)& m_dX, sizeof (m_dX));
    a_SaveStream.write((char*)& m_dY, sizeof (m_dY));
    a_SaveStream.write((char*)& m_nZ, sizeof (m_nZ));
    a_SaveStream.write((char*)& m_dAngle, sizeof (m_dAngle));
    a_SaveStream.write((char*)& m_dSpeed, sizeof (m_dSpeed));
    a_SaveStream.write((char*)& m_blCanFind, sizeof (m_blCanFind));

    #ifdef _DEBUG
    cout << "Sprite saved ! \n"<< std::flush;
    #endif // _DEBUG
}


TSprite::~TSprite()
{
    #ifdef _DEBUG
    cout << "Sprite Destroyed ! \n"<< std::flush;
    #endif // _DEBUG
}


// Engine methods



void TEngine::Add(TSprite * const sprite)
{
	if (sprite == NULL)
    {
        throw A5Exception("Out of memory !");
    }

	sprite->m_pEngine = this;
	if (sprite->m_ID==ID_BLACK_HOLE)
    {
         m_blHasBlackHole = true;
    }

	m_lstItems.push_back(sprite);
    m_lstItems.sort(Comp);
}

void TEngine::Sort()
{
     m_lstItems.sort(Comp);
}



double TEngine::GetDx()
{
    return m_dX;
}

double TEngine::GetDy()
{
    return m_dY;
}



void TEngine::Kill()
{

	// set pointers to objects to be removed to null
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=(TSprite *)(*p);
		if ((hulp!=NULL)&&(!hulp->m_blDestroyed)&&(hulp->m_ID>ID_SHIP_BOTTOM)&&(hulp->m_ID<ID_SHIP_TOP))
		{
			if  ( (((TShip *) hulp)->m_pTarget!=NULL) && (((TShip *) hulp)->m_pTarget->m_blDestroyed) )
			{
				if (((TShip *) hulp)->m_ID == ID_PLAYER)
				{
                    g_pEnterprise->SignalTargetDesytoyed();
				}

				((TShip *) hulp)->m_pTarget=NULL;

			}

			if  ( (((TShip *) hulp)->m_pBaseTarget!=NULL) && (((TShip *) hulp)->m_pBaseTarget->m_blDestroyed) )
			{
				((TShip *) hulp)->m_pBaseTarget=NULL;
			}
		}
		++p;
   }

	std::list<TSprite *>::iterator d=m_lstItems.begin();
	while (d!=m_lstItems.end())
	{
		TSprite*hulp=(TSprite *)(*d);
		if (hulp->m_blDestroyed)
		{
			if ((hulp->m_ID>ID_SHIP_BOTTOM)&&(hulp->m_ID<ID_SHIP_TOP))
			{
				if (((TShip *) hulp)->m_blMustSurvive)
				{
				     // Game over
				     m_nGameOver=GO_LOOSE_CRITICAL;
				     sprintf(g_szMessage,"Critical unit %s was destroyed",((TShip *) hulp)->m_strName.c_str());
				}

			}
			if (hulp->m_ID==ID_PLAYER)
			{
				m_nGameOver=GO_ENTERPRISE;
				sprintf(g_szMessage,"The enterprise was destroyed");

			}
			d=m_lstItems.erase(d);
			delete hulp;
		}
		else
		{
			++d;
		}
	}
}



void TEngine::Draw()
{
	std::list<TSprite *>::iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite * hulp=(TSprite *)(*p);
		if (hulp != NULL)
        {
            hulp->Draw(m_dX,m_dY);
        }
		else
        {
            // Something went horribly wrong
            cout << "NULL in Spritelist !!";
            exit(-1);

        }
		++p;
	}

}


void TEngine::Move(double a_dLagCount)
{
    std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=(TSprite *)(*p);
		if ((hulp != NULL)&&(!hulp->m_blDestroyed))
        {
            hulp->Move(a_dLagCount);
        }
		++p;
	}
}




void TEngine::Do_ai()
{
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=(TSprite *)(*p);
		if (hulp != NULL)
        {

        if (! hulp->m_blDestroyed)
		{
			hulp->Do_ai();

			if ((hulp->m_ID>ID_SHIP_BOTTOM)&&(hulp->m_ID<ID_SHIP_TOP))
			{

				if ((((TShip *) hulp)->m_blMustReachPosition)&&(Distance(hulp->m_dX,hulp->m_dY,((TShip *) hulp)->m_dSafePosX,((TShip *) hulp)->m_dSafePosY)<=40))
				{
                    // Game over
                    if (((TShip *) hulp)->m_blDock)
                    {
                        if (((TShip *) hulp)->m_blDocked)
                        {
                            m_nGameOver=GO_WIN_REACHED;
                            sprintf(g_szMessage,"Mission Succes: %s safely docked",((TShip *) hulp)->m_strName.c_str());
                        }
                    }
                    else
                    {
                        m_nGameOver=GO_WIN_REACHED;
                        sprintf(g_szMessage,"Mission Succes: %s Was safely escorted",((TShip *) hulp)->m_strName.c_str());
                    }
				}
    		}



			if (hulp->m_blCanCollide)
			{
				std::list<TSprite *>::const_iterator q=m_lstItems.begin();

				SPoint Pnt;
                Pnt.m_nX = -1;
                Pnt.m_nY = -1;
                double dClosest = 1000000;
                TSprite * pTargetPhaser = NULL;
				while (q!=m_lstItems.end())
				{
					TSprite*hulp2=*q;

					if ((hulp2!=hulp)&&(!hulp->m_blDestroyed)&&(!hulp2->m_blDestroyed) && (hulp2->m_ID != ID_NONE))
					{
						// Check collision !!!
						if (Detect_collision(hulp,hulp2)) hulp->DoCollision(hulp2);

						if ((((hulp->m_ID>ID_SHIP_BOTTOM)&&(hulp->m_ID<ID_BASE_TOP))||(hulp->m_ID == ID_RELIANT))&&(hulp2->m_blCanCollide))
                        {
                            if ((((TShip *)hulp)->IsPhaserActive()))
                            {
                                Pnt = Detect_collision_Phaser(hulp2, hulp->GetX(),hulp->GetY(),((TShip *)hulp)->m_nPhaserEndX,((TShip *)hulp)->m_nPhaserEndY);
                                if ((Pnt.m_nX != -1) && (Pnt.m_nY != -1))
                                {
                                    double dTargetDistance = Distance(hulp->GetX(),hulp->GetY(),(double)Pnt.m_nX,(double)Pnt.m_nY);
                                    if (dTargetDistance < dClosest)
                                    {
                                        dClosest = dTargetDistance;
                                        pTargetPhaser = hulp2;
                                        ((TShip *)hulp)->m_nPhaserEndX = Pnt.m_nX;
                                        ((TShip *)hulp)->m_nPhaserEndY = Pnt.m_nY;
                                    }
                                }
                            }
                        }
					}
					++q;
				}

                if ((pTargetPhaser != NULL)&& (((hulp->m_ID>ID_SHIP_BOTTOM)&&(hulp->m_ID<ID_SHIP_TOP))||(hulp->m_ID == ID_RELIANT))  )
                {
                    for (int i=0;i<5;i++)
                    {
                        TParticle *p=new TParticle(EFF_NORMAL);
                        p->SetPosition(((TShip *)hulp)->m_nPhaserEndX,((TShip *)hulp)->m_nPhaserEndY);
                        Add(p);
                    }

                    if (((pTargetPhaser->m_ID>ID_SHIP_BOTTOM)&&(pTargetPhaser->m_ID<ID_BASE_TOP))||(pTargetPhaser->m_ID==ID_RELIANT))
                    {
                        double dPower = ((TShip *)hulp)->m_nPhaserPower;
                        double Eeff = dPower/(dClosest+1);
			            if (Eeff>7) Eeff=7;
			            ((TShip *)pTargetPhaser)->CalcPhaserDamage(Eeff,((TShip *)hulp)->m_nPreferedTarget);
                    }
                }

			}
		}
        }
        else
        {
            // Something went horribly wrong
            cout << "NULL in Spritelist !!";
            exit(-1);

        }

		++p;

	}
}

bool TEngine::Detect_collision(TSprite * const sprite1,TSprite * const sprite2)
{
bool hit=false;
if ((sprite1!=NULL)&&(sprite2!=NULL)&&(sprite1->m_blDestroyed==false)&&(sprite2->m_blDestroyed==false))
   {
   if (sprite1->m_ID==ID_ASTEROID)
   {
       if((sprite2->m_ID > ID_SHIP_BOTTOM)&&(sprite2->m_ID < ID_SHIP_TOP)&&(Distance(sprite1->m_dX,sprite1->m_dY,sprite2->m_dX,sprite2->m_dY)<((sprite1->m_nBitmapMidX) + (sprite2->m_nBitmapMidX))))
       {
            hit=true;
       }
       else if((sprite2->m_ID > ID_BULLET_BOTTOM)&&(sprite2->m_ID < ID_BULLET_TOP)&&(Distance(sprite1->m_dX,sprite1->m_dY,sprite2->m_dX,sprite2->m_dY)<80))
       {
           double V = CalcVolume(sprite2->GetX(),sprite2->GetY());
	       SoundManager::PlaySound(SOUND::EXPLOSION_SMALL,V);

		   TAnimation * pAnimation;
		   pAnimation = new TAnimation( ANIMATION::SMOKE,
                                        sprite2->GetX(),sprite2->GetY(),
                                        0,
                                        0,
                                        sprite1->m_nZ+1);
            Add(pAnimation);
            sprite2->m_blDestroyed=true;

            sprite1->m_dAngle = sprite2->m_dAngle;
            sprite1->m_dSpeed = (sprite2->m_dSpeed / 8);


       }
       else if((sprite2->m_ID == ID_PROBE)&&(Distance(sprite1->m_dX,sprite1->m_dY,sprite2->m_dX,sprite2->m_dY)<80))
       {
           double V = CalcVolume(sprite2->GetX(),sprite2->GetY());
	       SoundManager::PlaySound(SOUND::EXPLOSION_SMALL,V);

		   TAnimation * pAnimation;
		   pAnimation = new TAnimation( ANIMATION::SMOKE,
                                        sprite2->GetX(),sprite2->GetY(),
                                        0,
                                        0,
                                        sprite1->m_nZ+1);
            Add(pAnimation);
            sprite2->m_blDestroyed=true;

            g_pCommunication->AddMessage(2, CREW_SPOCK,"Our probe crashed into an asteroid !");
       }
       else if((sprite2->m_ID == ID_ASTEROID)&&(Distance(sprite1->m_dX,sprite1->m_dY,sprite2->m_dX,sprite2->m_dY)<150))
       {
           //TODO (Edwin#1#) Asteroid-Asteroid collision handling !
           double dTemp;
           dTemp = sprite2->m_dAngle;
           sprite2->m_dAngle = sprite1->m_dAngle;
           sprite1->m_dAngle=dTemp;

           dTemp = sprite2->m_dSpeed;
           sprite2->m_dSpeed = sprite1->m_dSpeed;
           sprite1->m_dSpeed = dTemp;
       }
   }

   else if (Distance(sprite1->m_dX,sprite1->m_dY,sprite2->m_dX,sprite2->m_dY) < (sprite1->m_nBitmapMidX + (sprite2->m_nBitmapMidX/2)))
      {
			hit=true;
      }
   }
return hit;
}



SPoint TEngine::Detect_collision_Phaser(TSprite * const sprite, double a_dStartX, double a_dStartY,double a_dEndX, double a_dEndY)
{
    double dCX = sprite->GetX();
    double dCY = sprite->GetY();
    double R = sprite->m_nBitmapWidth / 2;

    SPoint Intersection;
    Intersection.m_nX = -1;
    Intersection.m_nY = -1;
    SPoint LP1;
    LP1.m_nX = a_dStartX - dCX;
    LP1.m_nY = a_dStartY - dCY;

    SPoint LP2;
    LP2.m_nX = a_dEndX - dCX;
    LP2.m_nY = a_dEndY - dCY;

    SPoint P2minP1;
    P2minP1.m_nX = LP2.m_nX - LP1.m_nX;
    P2minP1.m_nY = LP2.m_nY - LP1.m_nY;

    double dA = (P2minP1.m_nX * P2minP1.m_nX) + (P2minP1.m_nY * P2minP1.m_nY);
    double dB = 2 * ((P2minP1.m_nX * LP1.m_nX) + (P2minP1.m_nY * LP1.m_nY));
    double dC = (LP1.m_nX * LP1.m_nX) + (LP1.m_nY * LP1.m_nY) - (R * R);

    double dD = dB * dB -(4 * dA * dC);

    double dU;
    if (dD < 0)
    {
        // No Solution -1,-1
        return Intersection;
    }
    else if (dD > 0)
    {
        double dSquareRootDelta = sqrt(dD);
        dU = (-dB - dSquareRootDelta) / (2 * dA);
	}
	else
    {
        dU = -dB / (2 * dA);
    }

    if ((dU >= 0) && (dU <= 1))
    {
       Intersection.m_nX = a_dStartX + (dU * P2minP1.m_nX);
	   Intersection.m_nY = a_dStartY + (dU * P2minP1.m_nY);
    }

    return Intersection;
}





double TEngine::CalcVolume(double a_dX, double a_dY)
{
    double D = Distance(a_dX, a_dY,m_dX, m_dY);
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




TEngine::TEngine(int a_nWidth,int a_nHeight)
{
	m_blHasBlackHole = false;
	m_blSensorStatic=false;
	m_nCount    =   0;
	m_nGameOver =   GO_PLAYING;
	m_dX        =   5000;
	m_dY        =   5000;
	m_dPosx     =   m_dX;
	m_dPosy     =   m_dY;

	m_nScreenWidth = a_nWidth;
	m_nScreenHeight = a_nHeight;
	m_nScreenMidX = m_nScreenWidth/2;
    m_nScreenMidY = m_nScreenHeight/2;

	// set some colors
	m_clBLACK   = al_map_rgb(0,0,0);
	m_clRED     = al_map_rgb(255,0,0);
	m_clGREEN   = al_map_rgb(0,255,0);
	m_clBLUE    = al_map_rgb(0,0,255);
	m_clYELLOW  = al_map_rgb(255,255,0);
	m_clBROWN   = al_map_rgb(100,100,70);
	m_clWHITE   = al_map_rgb(255,255,255);
	m_clMAGENTA = al_map_rgb(255,0,255);
    m_clAQUA    = al_map_rgb(0,255,255);
    m_clATHM    = al_map_rgb(80,80,60);

    for(int i=0; i<KEY_MAX;i++)
	{
		m_blKeys[i]=false;
	}

}

TEngine::~TEngine()
{
	if (!m_lstItems.empty())
    {
      std::list<TSprite *>::iterator p=m_lstItems.begin();
	  while (p!=m_lstItems.end())
	  {
		TSprite * pDummy=((TSprite *)*p);
		p=m_lstItems.erase(p);
		delete pDummy;
      }
    }
}


void TEngine::Clear(bool a_blKeepPlayer)
{
	m_blHasBlackHole = false;
	if (!m_lstItems.empty())
    {
      std::list<TSprite *>::iterator p=m_lstItems.begin();
	  while (p!=m_lstItems.end())
	  {
		TSprite * pDummy=((TSprite *)*p);

		if ((!a_blKeepPlayer)||(pDummy->m_ID!=ID_PLAYER))
		{
			p=m_lstItems.erase(p);
			delete pDummy;
		}
		else
		{
			++p;
		}
      }
    }
}






void TEngine::SetOrigin(double a_dX, double a_dY)
{
	m_dX = a_dX;
	m_dY = a_dY;
}

void TEngine::Pan(double a_dX, double a_dY)
{
    m_dX+=a_dX;
    m_dY+=a_dY;
}



void TEngine::Folow(TSprite * const a_pSprite)
{
	m_dX=a_pSprite->m_dX;
	m_dY=a_pSprite->m_dY;
}

void TEngine::DrawHud(TShip * const a_pPlayer)
{
	if ((a_pPlayer!=NULL) && (!a_pPlayer->m_blDestroyed) && (a_pPlayer->m_pTarget!=NULL) && (a_pPlayer->m_blDocked==false))
    {
          int S=45;
		  int L=15;
		  ALLEGRO_COLOR color = m_clRED;
	      double dAngle=a_pPlayer->WayPoint(a_pPlayer->m_pTarget->GetX(),a_pPlayer->m_pTarget->GetY());
          if (a_pPlayer->m_pTarget->m_Member==MEM_FEDERATION) color = m_clGREEN;
          if ((a_pPlayer->m_pTarget->m_ID>ID_BASE_BOTTOM)&&(a_pPlayer->m_pTarget->m_ID<ID_BASE_TOP)&&(a_pPlayer->m_pTarget->m_Member==MEM_FEDERATION)) color = m_clBLUE;

          double x1=(S*cos(dAngle));
          double y1=(S*sin(dAngle));
          double x2=((S+L)*cos(dAngle));
          double y2=((S+L)*sin(dAngle));
		  al_draw_line(m_nScreenMidX+x1, m_nScreenMidY+y1, m_nScreenMidX+x2, m_nScreenMidY+y2, color, 1);
   }
}


TSprite * TEngine::Seek(int a_nID, double a_dViewDistance, double a_dX, double a_dY)
{
	TSprite * target=NULL;
	int dist=a_dViewDistance;
	int D;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if ( (hulp->m_ID == a_nID) && (hulp->m_blDestroyed == false) &&
		     ((hulp->m_ID<ID_SHIP_BOTTOM) || (hulp->m_ID>ID_BASE_TOP) || ((TShip *) hulp)->m_CloakState != CS_CLOAKED))
		{
			D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
			if ((D<=dist)&&(D!=0))
			{
				target=hulp;
				dist=D;
			}
		}
		++p;
	}
	return target;
}






TSprite * TEngine::Seek(int a_nID, double a_dViewDistance, double a_dX, double a_dY,double a_dAngle, double a_dRadius)
{
    TSprite * target=NULL;
	double nFrontPointX = a_dX + cos(a_dAngle) * a_dViewDistance;
	double nFrontPointY = a_dY + sin(a_dAngle) * a_dViewDistance;

	int D  = a_dViewDistance;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if ((hulp->m_ID==a_nID) && (hulp->m_blDestroyed==false))
		{
			D=int(Distance(nFrontPointX,nFrontPointY,hulp->m_dX,hulp->m_dY));
            if ((D <= a_dRadius)&&( D != 0))
            {
                target=hulp;
            }
		}
		++p;
	}
	return target;
}


TSprite * TEngine::SeekClosestTransportTarget(double a_dX, double a_dY)
{
    TSprite * target=NULL;
	int dist = 10000;
	int D;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if ( (hulp->m_blDestroyed == false) &&
		     ((((hulp->m_ID> ID_PLANET_BOTTOM) &&
		     (hulp->m_ID<ID_PLANET_TOP))||(hulp->m_ID== ID_FEDERATIONBASE))))
		{
			D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
			if ((D<=dist)&&(D!=0))
			{
				target=hulp;
				dist=D;
			}
		}
		++p;
	}
	return target;
}


TSprite * TEngine::Seek(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY)
{
	TSprite * target=NULL;
	int dist=a_dViewDistance;
	int D;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if ( (!hulp->m_blDestroyed) &&
             (hulp->m_blCanFind) &&
             (((hulp->m_ID>ID_SHIP_BOTTOM)&&(hulp->m_ID<ID_BASE_TOP))||((hulp->m_ID>ID_SPECIAL_BOTTOM)&&(hulp->m_ID<ID_SPECIAL_TOP))) &&
             (((TShip *)hulp)->m_CloakState != CS_CLOAKED))
		{
            if (a_blEnemy)
            {
                if (hulp->m_Member!=a_nMember)
                {
                    D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
                    if ((D<=dist)&&(D!=0))
                    {
                        target=hulp;
                        dist=D;
                    }
                }
            }
            else
            {
                if (hulp->m_Member==a_nMember)
                {
                    D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
                    if ((D<=dist)&&(D!=0))
                    {
                        target=hulp;
                        dist=D;
                    }
                }
            }
		}
		++p;
	}
	return  target;
}



TSprite * TEngine::Seekstarbase(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY)
{
	TSprite * target=NULL;
	int dist=a_dViewDistance;
	int D;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if (a_blEnemy)
		{
			if ((hulp->m_ID > ID_BASE_BOTTOM)&&(hulp->m_ID < ID_BASE_TOP)&&(hulp->m_blDestroyed==false)&&(hulp->m_Member!=a_nMember))
			{
				D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
				if ((D<=dist)&&(D!=0))
				{
					target=hulp;
					dist=D;
				}
			}
		}
		else
		{
			if ((hulp->m_ID > ID_BASE_BOTTOM)&&(hulp->m_ID < ID_BASE_TOP)&&(hulp->m_blDestroyed==false)&&(hulp->m_Member==a_nMember))
			{
				D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
				if ((D<=dist)&&(D!=0))
				{
					target=hulp;
					dist=D;
				}
			}
		}
		++p;
	}
	return target;
}


TSprite * TEngine::GetBlackHole()
{
    TSprite * target=NULL;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if (hulp->m_ID > ID_BLACK_HOLE)
        {
			target=hulp;
		}
		++p;
	}
	return target;
}




void TEngine::Draw_sensor(int dx,int dy,TEnterprise * const a_pPlayer)
{

	if ((a_pPlayer!=NULL)&&(! a_pPlayer->m_blDestroyed))
	{
		if (a_pPlayer->IsShieldOn())
        {
            if (a_pPlayer->m_pTarget != NULL)
            {
                al_draw_text(FontManager::GetFont(FONT::SMALL),m_clRED, dx-50, dy-70, 0,"CONDITION: RED");
            }
            else
            {
                al_draw_text(FontManager::GetFont(FONT::SMALL),m_clYELLOW, dx-50, dy-70, 0,"CONDITION: YELLOW");
            }

            al_draw_filled_rectangle(dx+55, dy+50, dx+60, (dy+50)-(a_pPlayer->m_nShieldEnergy),m_clBLUE);
        }
        else
        {
            al_draw_text(FontManager::GetFont(FONT::SMALL),m_clGREEN, dx-50, dy-70, 0,"CONDITION: GREEN");
        }

		al_draw_filled_rectangle(dx+64, dy+50, dx+69, (dy+50)-(a_pPlayer->m_nPhaserEnergy),m_clRED);

		int xx,yy;
		float dd;
		std::list<TSprite *>::const_iterator p=m_lstItems.begin();
		ALLEGRO_COLOR col;
		al_draw_filled_rectangle(dx-50, dy-50, dx+50, dy+50,m_clBLACK);
		al_draw_rectangle(dx-50, dy-50, dx+50, dy+50,m_clWHITE,1);


		if (m_blSensorStatic)
		{
			for (int i=0;i<25;i++)
			{
				xx=(rand()%100)-50;
				yy=(rand()%100)-50;
				al_draw_filled_circle(dx+xx, dy+yy, 1, m_clWHITE);
			}

		}
		else
		{
			while (p!=m_lstItems.end())
			{
				if (((*p)->m_ID<ID_SHIP_BOTTOM) || (((TShip *)(*p))->m_CloakState != CS_CLOAKED))
				{
				dd=Distance(m_dX,m_dY,(*p)->m_dX,(*p)->m_dY);
				if (dd<2500)
				{
					xx=int(((*p)->m_dX-m_dX)/50);
					yy=int(((*p)->m_dY-m_dY)/50);
					col = m_clGREEN;
					if (((*p)->m_ID>=ID_STAR_BLUE)&&((*p)->m_ID<=ID_STAR_RED))
					{
						al_draw_filled_circle(dx+xx, dy+yy, 3, m_clYELLOW);
					}
					else if (((*p)->m_ID>=ID_CLASS_A)&&((*p)->m_ID<=ID_CLASS_M3))
					{
						al_draw_filled_circle(dx+xx, dy+yy, 2, m_clBROWN);
					}
					else if ((*p)->m_ID==ID_ASTEROID)
                    {
                        al_draw_filled_circle(dx+xx, dy+yy, 1, m_clYELLOW);
                    }
					else if ((*p)->m_ID==ID_FEDERATIONBASE)
					{
						al_draw_filled_circle(dx+xx, dy+yy, 2, m_clWHITE);
					}
					else if (((*p)->m_ID==ID_ROMULAN_BASE)||((*p)->m_ID==ID_KLINGON_BASE))
					{
						al_draw_filled_circle(dx+xx, dy+yy, 2, m_clRED);
					}
					else if ((((*p)->m_ID>ID_SHIP_BOTTOM)&&((*p)->m_ID<ID_SHIP_TOP))||(*p)->m_ID==ID_RELIANT)
					{
						if ( ((*p)->m_Member==MEM_KLINGON)||
							((*p)->m_Member==MEM_ROMULAN)||
							((*p)->m_Member==MEM_ENEMY) ) col = m_clRED;

						al_draw_filled_circle(dx+xx, dy+yy, 1, col);
					}
					else
                    {

                    }
				}
				}
				++p;
			}
		}
	}

}

int TEngine::CountSaveObjects()
{
    int result = 0;
    std::list<TSprite *>::const_iterator p=m_lstItems.begin();
    while (p!=m_lstItems.end())
	{
        if ( (!(*p)->m_blDestroyed) && ((*p)->m_ID != ID_NONE) /* && ((*p)->m_ID != ID_SMOKE)*/ )
        {
            result++;
        }
	++p;
	}
	return result;
}


bool TEngine::Save(ofstream & a_SaveStream)
{
	char szVersion[18]="CURRENT_SECTOR_V1";
    a_SaveStream.write((char*)szVersion,sizeof (szVersion));

	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	int m_nObjects = CountSaveObjects();
	a_SaveStream.write((char*)&m_nObjects,sizeof (int));
	while (p!=m_lstItems.end())
	{
        if ( (!(*p)->m_blDestroyed) && ((*p)->m_ID != ID_NONE) )
        {
            (*p)->Save(a_SaveStream);
        }
	    ++p;
	}
	return true;
}





bool TEngine::Load(ifstream & a_LoadStream)
{
	Clear(false);
	char szVersion[18];
	a_LoadStream.read((char *)szVersion,sizeof(char)*18);

	int m_nObjects;
	a_LoadStream.read((char *)&m_nObjects,sizeof(int));

	for (int i=0;i<m_nObjects;i++)
	{
        ID id;
        a_LoadStream.read((char *)&id,sizeof(id));

        if (id > ID_BULLET_BOTTOM && id < ID_BULLET_TOP)
        {
            // bullet
            TBullet * pBullet = new TBullet(a_LoadStream,id);
            Add(pBullet);
        }
        else if ( (id == ID_ASTEROID) ||
                  ((id > ID_PLANET_BOTTOM) && (id < ID_PLANET_TOP)) ||
                  ((id > ID_STAR_BOTTOM) && (id < ID_STAR_TOP)) )
        {
            // Space Object
            TSpaceObject * pSpaceObject = new TSpaceObject(a_LoadStream,id);
            Add(pSpaceObject);
        }
        else if (id > ID_BASE_BOTTOM && id < ID_BASE_TOP)
        {
            // star base
            TStarbase * pStarBase = new TStarbase(a_LoadStream,id);
            Add(pStarBase);
        }
        else if (id == ID_ANIMATION)
        {
           // animation
           TAnimation * pAnimation = new TAnimation(a_LoadStream,id);
           Add(pAnimation);
        }
        else if (id == ID_KLINGONBC)
        {
            TKlingonBC * pKlingonbc = new TKlingonBC(a_LoadStream,id);
            Add(pKlingonbc);
        }
        else if (id == ID_KLINGONBOP)
        {
            TKlingonBOP * pKlingonbop = new TKlingonBOP(a_LoadStream,id);
            Add(pKlingonbop);
        }
        else if (id == ID_ROMULANBOP)
        {
            TRomulanBop * pRomulanbop = new TRomulanBop(a_LoadStream,id);
            Add(pRomulanbop);
        }
        else if (id == ID_GALAXYCLASS)
        {
            TFederation_Ship * pFederationShip = new TFederation_Ship(a_LoadStream,id);
            Add(pFederationShip);
        }
        else if (id == ID_RELIANT)
        {
            CReliant * pReliant = new CReliant(a_LoadStream,id);
            Add(pReliant);
        }
        else if (id == ID_PLAYER)
        {
            g_pEnterprise = new TEnterprise(a_LoadStream,id);
            Add(g_pEnterprise);
        }
        else
        {
            // error reading file
            char buff[100];
            sprintf(buff,"Unknown sprite type %i",id);
            throw A5Exception(buff);

        }
    }



std::list<TSprite *>::const_iterator p=m_lstItems.begin();
while (p!=m_lstItems.end())
{
	if (((*p)->m_ID>ID_SHIP_BOTTOM)&&(((TShip *)(*p))->m_blDocked))
	{
		((TShip *)(*p))->m_blCanFind = false;
		((TShip *)(*p))->m_blDocking = false;
		((TShip *)(*p))->m_blReleasing = false;
		((TShip *)(*p))->m_dSpeed      = 0;
		((TShip *)(*p))->m_pBaseTarget=(TShip *) Seekstarbase(((TShip *)(*p))->m_Member,false,100000,((TShip *)(*p))->m_dX,((TShip *)(*p))->m_dY);
		((TShip *)(*p))->m_pTarget = ((TShip *)(*p))->m_pBaseTarget;
		((TShip *)(*p))->m_dAngleSeek = ((TShip *)(*p))->m_dAngle;

	}
	else if (((*p)->m_ID>ID_SHIP_BOTTOM)&&(((TShip *)(*p))->m_blDocking))
	{
		((TShip *)(*p))->m_blDocking=true;
		((TShip *)(*p))->m_blReleasing = false;
		((TShip *)(*p))->m_pBaseTarget=(TShip *) Seekstarbase(((TShip *)(*p))->m_Member,false,100000,((TShip *)(*p))->m_dX,((TShip *)(*p))->m_dY);
		((TShip *)(*p))->m_pTarget = ((TShip *)(*p))->m_pBaseTarget;
	}
	++p;
}


return true;

}

bool Comp(const TSprite *v1, const TSprite *v2)
{
	return (v1->m_nZ < v2->m_nZ);
}








