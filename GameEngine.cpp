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

#include "Global.h"
#include "GameEngine.h"
#include "Ship.h"

#include "Enterprise.h"
#include "Romulan_BOP.h"
#include "Klingon_BC.h"
#include "SpaceObject.h"
#include "Starbase.h"


extern TEnterprise * g_pEnterprise;

bool Comp(const TSprite *v1, const TSprite *v2);

struct TStar
{
	double x,y;
	int z;
};



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

void TSprite::Move()
{
	m_dX+=cos(m_dAngle)*m_dSpeed;
	m_dY+=sin(m_dAngle)*m_dSpeed;
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

void TSprite::DoCollision(TSprite * a_pSprite)
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
	m_blDestroyed=false;
	m_blCanCollide=false;
	m_dSpeed=0;
    m_dX=0;
    m_dY=0;
    m_nZ=0;
	m_blCanFind=false;
    m_dAngle=0;
}

TSprite::~TSprite()
{

}


// Engine methods

void GameEngine::Add(TSprite * sprite)
{
	sprite->m_pEngine = this;
	m_lstItems.push_back(sprite);
}

void GameEngine::Kill()
{

	// set pointers to objects to be removed to null
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if ((hulp!=NULL)&&(!hulp->m_blDestroyed)&&(hulp->m_nID>=ID_SHIP))
		{
			if  ( (((TShip *) hulp)->m_pTarget!=NULL) && (((TShip *) hulp)->m_pTarget->m_blDestroyed) )
			{
				((TShip *) hulp)->m_pTarget=NULL;
			}

			if  ( (((TShip *) hulp)->m_pBaseTarget!=NULL) && (((TShip *) hulp)->m_pBaseTarget->m_blDestroyed) )
			{
				((TShip *) hulp)->m_pBaseTarget=NULL;
			}
		}
		p++;
   }

	std::list<TSprite *>::iterator d=m_lstItems.begin();
	while (d!=m_lstItems.end())
	{
		TSprite*hulp=*d;
		if (hulp->m_blDestroyed)
		{
			if (hulp->m_nID>=ID_SHIP)
			{
				/*
				if (((TShip *) hulp)->blMustSurvive)
				{
				// Game over
				Game_Over=GO_LOOSE_CRITICAL;
				sprintf(pMessage,"Critical unit %s was destroyed",((TShip *) hulp)->name);

				}
				*/
			}
			if (hulp->m_nID==ID_PLAYER)
			{
				//Game_Over=GO_ENTERPRISE;
				//sprintf(pMessage,"The enterprise was destroyed");

			}
			d=m_lstItems.erase(d);
			delete hulp;
		}
		else
		{
			d++;
		}
	}
}



void GameEngine::Draw()
{
	m_lstItems.sort(Comp);
	std::list<TSprite *>::iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		hulp->Draw(m_dX,m_dY);
		p++;
	}

}



void GameEngine::Move()
{
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if (! hulp->m_blDestroyed)
		{
			hulp->Do_ai();
			hulp->Move();

			if (hulp->m_nID>=ID_SHIP)
			{
				/*
				if ((((TShip *) hulp)->blMustReachPosition)&&(distance(hulp->x,hulp->y,((TShip *) hulp)->Safe_PosX,((TShip *) hulp)->Safe_PosY)<=40))
				{
				// Game over
				if (((TShip *) hulp)->blDock)
				{
				if (((TShip *) hulp)->Docked)
				{
				Game_Over=GO_WIN_REACHED;
				sprintf(pMessage,"Mission Succes: %s safely docked",((TShip *) hulp)->name);
				}
				}
				else
				{
				Game_Over=GO_WIN_REACHED;
				sprintf(pMessage,"Mission Succes: %s Was safely escorted",((TShip *) hulp)->name);
				}
				}
				*/
			}



			if (hulp->m_blCanCollide)
			{
				std::list<TSprite *>::const_iterator q=m_lstItems.begin();
				while (q!=m_lstItems.end())
				{
					TSprite*hulp2=*q;

					if ((hulp2!=hulp)&&(!hulp->m_blDestroyed)&&(!hulp2->m_blDestroyed)&&(hulp2->m_nID>=ID_BULLET_BOTTOM)&&(hulp2->m_nID<=ID_BULLET_TOP))
					{
						// Check collision !!!
						if (Detect_collision(hulp,hulp2)) hulp->DoCollision(hulp2);
					}
					q++;
				}
			}
		}
		p++;

	}
}

bool GameEngine::Detect_collision(TSprite * sprite1,TSprite * sprite2)
{
bool hit=false;
if ((sprite1!=NULL)&&(sprite2!=NULL)&&(sprite1->m_blDestroyed==false)&&(sprite2->m_blDestroyed==false))
   {
   if (Distance(sprite1->m_dX,sprite1->m_dY,sprite2->m_dX,sprite2->m_dY) < sprite1->m_nBitmapMidX)
      {
			hit=true;
      }
   }
return hit;
}




GameEngine::GameEngine(int a_nWidth,int a_nHeight)
{
	m_blSensorStatic=false;
	m_nCount=0;
	m_dX=5000;
	m_dY=5000;
	m_dPosx=m_dX;
	m_dPosy=m_dY;
	m_nScreenWidth = a_nWidth;
	m_nScreenHeight = a_nHeight;
	m_nScreenMidX = m_nScreenWidth/2;
    m_nScreenMidY = m_nScreenHeight/2;

	// set some colors
	m_clBLACK  = al_map_rgb(0,0,0);
	m_clRED    = al_map_rgb(255,0,0);
	m_clGREEN  = al_map_rgb(0,255,0);
	m_clBLUE   = al_map_rgb(0,0,255);
	m_clYELLOW = al_map_rgb(255,255,0);
	m_clBROWN  = al_map_rgb(100,100,70);
	m_clWHITE  = al_map_rgb(255,255,255);


	for(int i=0; i<NUMBER_OF_KEYS;i++)
	{
		m_blKeys[i]=false;
	}

}

GameEngine::~GameEngine()
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


void GameEngine::Clear(bool a_blKeepPlayer)
{
	if (!m_lstItems.empty())
    {
      std::list<TSprite *>::iterator p=m_lstItems.begin();
	  while (p!=m_lstItems.end())
	  {
		TSprite * pDummy=((TSprite *)*p);

		if ((!a_blKeepPlayer)||(pDummy->m_nID!=ID_PLAYER))
		{
			p=m_lstItems.erase(p);
			delete pDummy;
		}
		else
		{
			p++;
		}
      }
    }
}






void GameEngine::SetOrigin(double a_dX, double a_dY)
{
	m_dX = a_dX;
	m_dY = a_dY;
}


void GameEngine::Folow(TSprite * a_pSprite)
{
	m_dX=a_pSprite->m_dX;
	m_dY=a_pSprite->m_dY;
}



void GameEngine::DrawHud(TShip * a_pPlayer)
{
	if ((a_pPlayer!=NULL)&& (a_pPlayer->m_pTarget!=NULL)&&(a_pPlayer->m_blDocked==false))
    {
          int S=45;
		  int L=15;
		  ALLEGRO_COLOR color = m_clRED;
	      double dAngle=a_pPlayer->WayPoint(a_pPlayer->m_pTarget->GetX(),a_pPlayer->m_pTarget->GetY());
          if (a_pPlayer->m_pTarget->m_nMember==MEM_FEDERATION) color = m_clGREEN;
          if ((a_pPlayer->m_pTarget->m_nID==ID_STARBASE)&&(a_pPlayer->m_pTarget->m_nMember==MEM_FEDERATION)) color = m_clBLUE;

          double x1=(S*cos(dAngle));
          double y1=(S*sin(dAngle));
          double x2=((S+L)*cos(dAngle));
          double y2=((S+L)*sin(dAngle));
		  al_draw_line(m_nScreenMidX+x1, m_nScreenMidY+y1, m_nScreenMidX+x2, m_nScreenMidY+y2, color, 1);
   }

}


TSprite * GameEngine::Seek(int a_nID, double a_dViewDistance, double a_dX, double a_dY)
{
	TSprite * target=NULL;
	int dist=a_dViewDistance;
	int D;
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	while (p!=m_lstItems.end())
	{
		TSprite*hulp=*p;
		if ((hulp->m_nID==a_nID) && (hulp->m_blDestroyed==false))
		{
			D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
			if ((D<=dist)&&(D!=0))
			{
				target=hulp;
				dist=D;
			}
		}
		p++;
	}
	return target;
}



TSprite * GameEngine::Seek(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY)
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
			if ((hulp->m_nID >= ID_SHIP)&&(hulp->m_blDestroyed == false)&&(hulp->m_nMember!=a_nMember) && (hulp->m_blCanFind) )
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
			if ((hulp->m_nID >= ID_SHIP)&&(hulp->m_blDestroyed == false)&&(hulp->m_nMember==a_nMember) && (hulp->m_blCanFind) )
			{
				D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
				if ((D<=dist)&&(D!=0))
				{
					target=hulp;
					dist=D;
				}
			}
		}
		p++;
	}
	return  target;
}





TSprite * GameEngine::Seekstarbase(int a_nMember, bool a_blEnemy,double a_dViewDistance, double a_dX, double a_dY)
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
			if ((hulp->m_nID >= ID_STARBASE)&&(hulp->m_blDestroyed==false)&&(hulp->m_nMember!=a_nMember))
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
			if ((hulp->m_nID >= ID_STARBASE)&&(hulp->m_blDestroyed==false)&&(hulp->m_nMember==a_nMember))
			{
				D=int(Distance(a_dX,a_dY,hulp->m_dX,hulp->m_dY));
				if ((D<=dist)&&(D!=0))
				{
					target=hulp;
					dist=D;
				}
			}
		}
		p++;
	}
	return target;
}


void GameEngine::Draw_sensor(int dx,int dy,TShip * a_pPlayer)
{

	if ((a_pPlayer!=NULL)&&(! a_pPlayer->m_blDestroyed))
	{
		al_draw_filled_rectangle(dx+55, dy+50, dx+60, (dy+50)-(a_pPlayer->m_nShieldEnergy),m_clBLUE);
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
				dd=Distance(m_dX,m_dY,(*p)->m_dX,(*p)->m_dY);
				if ((dd<2500)&& ((*p)->m_nID>0))
				{
					xx=int(((*p)->m_dX-m_dX)/50);
					yy=int(((*p)->m_dY-m_dY)/50);
					col = m_clGREEN;
					if (((*p)->m_nID>ID_STAR_BOTTOM)&&((*p)->m_nID<ID_STAR_TOP))
					{
						al_draw_filled_circle(dx+xx, dy+yy, 3, m_clYELLOW);
					}
					else if (((*p)->m_nID>ID_PLANET_BOTTOM)&&((*p)->m_nID<ID_PLANET_TOP))
					{
						al_draw_filled_circle(dx+xx, dy+yy, 2, m_clBROWN);
					}
					else if ((*p)->m_nID==ID_FEDERATIONBASE)
					{
						al_draw_filled_circle(dx+xx, dy+yy, 2, m_clWHITE);
					}
					else if (((*p)->m_nID>100)&&((*p)->m_nID<1000))
					{
						if ( ((*p)->m_nMember==MEM_KLINGON)||
							((*p)->m_nMember==MEM_ROMULAN)||
							((*p)->m_nMember==MEM_ENEMY) ) col = m_clRED;

						al_draw_filled_circle(dx+xx, dy+yy, 1, col);
					}
				}
				p++;
			}
		}
	}

}






bool GameEngine::Save(FILE * a_pSaveFile)
{
	std::list<TSprite *>::const_iterator p=m_lstItems.begin();
	int m_nObjects = m_lstItems.size();
	fwrite (&m_nObjects, sizeof(int), 1, a_pSaveFile);
	while (p!=m_lstItems.end())
	{
       sGameObject object;
	   object.m_nID = (*p)->m_nID;
	   object.m_nMember = (*p)->m_nMember;
	   object.m_dX = (*p)->m_dX;
	   object.m_dY = (*p)->m_dY;
	   object.m_nZ = (*p)->m_nZ;
	   object.m_dAngle = (*p)->m_dAngle;
	   object.m_blCanFind = (*p)->m_blCanFind;
	   object.m_dSpeed = (*p)->m_dSpeed;

	   if (((*p)->m_nID>ID_BULLET_BOTTOM)&&((*p)->m_nID<ID_BULLET_TOP))
	   {
		  object.m_nTarget = ((TBullet *)(*p))->m_nTarget;
	   }


	   if ((*p)->m_nID>ID_SHIP)
	   {
		   object.m_blDocked = ((TShip *)(*p))->m_blDocked;
		   object.m_blDocking = ((TShip *)(*p))->m_blDocking;
           object.m_blReleasing = ((TShip *)(*p))->m_blReleasing;
           object.m_blMustBeDestroyed = ((TShip *)(*p))->m_blMustBeDestroyed;
           object.m_blMustSurvive = ((TShip *)(*p))->m_blMustSurvive;
           object.m_blMustReachPosition = ((TShip *)(*p))->m_blMustReachPosition;
           object.m_blDock = ((TShip *)(*p))->m_blDock;
           object.m_blNoRelease = ((TShip *)(*p))->m_blNoRelease;
           object.m_blCanFind = ((TShip *)(*p))->m_blCanFind;
		   object.m_dWaypointX = ((TShip *)(*p))->m_dWaypointX;
		   object.m_dWaypointY = ((TShip *)(*p))->m_dWaypointY;

		   object.m_nTorpedoes = ((TShip *)(*p))->m_nTorpedoes;
		   object.m_nRepairItem = ((TShip *)(*p))->m_nRepairItem;
		   object.m_nCrew = ((TShip *)(*p))->m_nCrew;
           object.m_nEnergy = ((TShip *)(*p))->m_nEnergy;
           object.m_nShieldEnergy = ((TShip *)(*p))->m_nShieldEnergy;
           object.m_nPhaserEnergy = ((TShip *)(*p))->m_nPhaserEnergy;
           object.m_nPreferedTarget = ((TShip *)(*p))->m_nPreferedTarget;
           object.m_nPhaserPower = ((TShip *)(*p))->m_nPhaserPower;

		   // store health
		   for (size_t i=0; i< ((TShip *)(*p))->m_lstHealth.size();i++)
		   {
			   object.m_nHealth[i]=((TShip *)(*p))->m_lstHealth[i];
		   }
	   }

	   if ((*p)->m_nID==ID_PLAYER)
	   {
		   // player specific !





	   }

	   fwrite (&object, sizeof (sGameObject), 1, a_pSaveFile);
	   p++;
	}
	return true;
}

bool GameEngine::Load(FILE * a_pLoadFile)
{
	Clear(false);
	int m_nObjects;
	fread (&m_nObjects, sizeof(int), 1, a_pLoadFile);

	for (int i=0;i<m_nObjects;i++)
	{
       sGameObject object;
	   fread (&object, sizeof(sGameObject), 1, a_pLoadFile);
	   if ((object.m_nID>ID_PLANET_BOTTOM)&&(object.m_nID<ID_STAR_TOP))
	   {
		   TSpaceObject * pObject = new TSpaceObject(object.m_nID);
		   pObject->m_dX              = object.m_dX;
		   pObject->m_dY              = object.m_dY;
		   pObject->m_nMember         = object.m_nMember;
		   Add(pObject);
	   }
	   else if ((object.m_nID>ID_BULLET_BOTTOM)&&(object.m_nID<ID_BULLET_TOP))
	   {
		   TBullet * pBullet = new TBullet( object.m_dX ,
			                                object.m_dY,
											object.m_dSpeed,
											object.m_dAngle,
											object.m_nZ,
											object.m_nID,
											object.m_nMember);
		   pBullet->m_nTarget = object.m_nTarget;

           Add(pBullet);

	   }
	   else if (object.m_nID>ID_SHIP)
	   {
			switch (object.m_nID)
			{
			    case ID_PLAYER:
                   g_pEnterprise                    = new TEnterprise();
				   g_pEnterprise->m_dX              = object.m_dX;
				   g_pEnterprise->m_dY              = object.m_dY;
				   g_pEnterprise->m_nZ              = object.m_nZ;
				   g_pEnterprise->m_dSpeed          = object.m_dSpeed;
				   g_pEnterprise->m_dAngle          = object.m_dAngle;
				   g_pEnterprise->m_nCrew           = object.m_nCrew;
				   g_pEnterprise->m_nRepairItem     = object.m_nRepairItem;
                   g_pEnterprise->m_nTorpedoes      = object.m_nTorpedoes;
				   g_pEnterprise->m_nShieldEnergy   = object.m_nShieldEnergy;
				   g_pEnterprise->m_nPhaserEnergy   = object.m_nPhaserEnergy;
                   g_pEnterprise->m_nPreferedTarget = object.m_nPreferedTarget;
                   g_pEnterprise->m_nPhaserPower    = object.m_nPhaserPower;
                   g_pEnterprise->m_nMember         = object.m_nMember;

				   g_pEnterprise->m_blDocked             = object.m_blDocked;
				   g_pEnterprise->m_blDocking            = object.m_blDocking;
				   g_pEnterprise->m_blReleasing          = object.m_blReleasing;
				   g_pEnterprise->m_blMustBeDestroyed   = object.m_blMustBeDestroyed;
				   g_pEnterprise->m_blMustSurvive       = object.m_blMustSurvive;
				   g_pEnterprise->m_blNoRelease         = object.m_blNoRelease;
				   g_pEnterprise->m_blCanFind           = object.m_blCanFind;


				   for (int i=0;i<9;i++)
				   {
					   g_pEnterprise->m_lstHealth[i]=object.m_nHealth[i];
				   }




				   Add(g_pEnterprise);

				break;

				case ID_GALAXYCLASS:


				break;

				case ID_KLINGONBC:
					{
						TKlingonBC * pKlingon = new TKlingonBC();
						pKlingon->m_dX              = object.m_dX;
						pKlingon->m_dY              = object.m_dY;
						pKlingon->m_nZ              = object.m_nZ;
				        pKlingon->m_dSpeed          = object.m_dSpeed;
						pKlingon->m_dAngle          = object.m_dAngle;
						pKlingon->m_nCrew           = object.m_nCrew;
						pKlingon->m_nRepairItem     = object.m_nRepairItem;
						pKlingon->m_nTorpedoes      = object.m_nTorpedoes;
						pKlingon->m_nShieldEnergy   = object.m_nShieldEnergy;
						pKlingon->m_nPhaserEnergy   = object.m_nPhaserEnergy;
						pKlingon->m_nPreferedTarget = object.m_nPreferedTarget;
						pKlingon->m_nPhaserPower    = object.m_nPhaserPower;
						pKlingon->m_nMember    = object.m_nMember;

						pKlingon->m_dWaypointX = object.m_dWaypointX;
		                pKlingon->m_dWaypointX = object.m_dWaypointY;

						pKlingon->m_blDocked             = object.m_blDocked;
				        pKlingon->m_blDocking            = object.m_blDocking;
				        pKlingon->m_blReleasing          = object.m_blReleasing;
				        pKlingon->m_blMustBeDestroyed   = object.m_blMustBeDestroyed;
				        pKlingon->m_blMustSurvive       = object.m_blMustSurvive;
				        pKlingon->m_blMustReachPosition = object.m_blMustReachPosition;
				        pKlingon->m_blNoRelease         = object.m_blNoRelease;
				        pKlingon->m_blCanFind           = object.m_blCanFind;



						for (int i=0;i<10;i++)
						{
							pKlingon->m_lstHealth[i]=object.m_nHealth[i];
						}
						Add(pKlingon);
					}
				break;

				case ID_KLINGONBOP:

				break;

				case ID_ROMULANBOP:
					{
						TRomulanBop * pRomulan = new TRomulanBop();
						pRomulan->m_dX              = object.m_dX;
						pRomulan->m_dY              = object.m_dY;
						pRomulan->m_nZ              = object.m_nZ;
				        pRomulan->m_dSpeed          = object.m_dSpeed;
						pRomulan->m_dAngle          = object.m_dAngle;
						pRomulan->m_nCrew           = object.m_nCrew;
						pRomulan->m_nRepairItem     = object.m_nRepairItem;
						pRomulan->m_nTorpedoes      = object.m_nTorpedoes;
						pRomulan->m_nShieldEnergy   = object.m_nShieldEnergy;
						pRomulan->m_nPhaserEnergy   = object.m_nPhaserEnergy;
						pRomulan->m_nPreferedTarget = object.m_nPreferedTarget;
						pRomulan->m_nPhaserPower    = object.m_nPhaserPower;
						pRomulan->m_nMember    = object.m_nMember;

						pRomulan->m_dWaypointX = object.m_dWaypointX;
		                pRomulan->m_dWaypointX = object.m_dWaypointY;

						pRomulan->m_blDocked             = object.m_blDocked;
				        pRomulan->m_blDocking            = object.m_blDocking;
				        pRomulan->m_blReleasing          = object.m_blReleasing;
				        pRomulan->m_blMustBeDestroyed   = object.m_blMustBeDestroyed;
				        pRomulan->m_blMustSurvive       = object.m_blMustSurvive;
				        pRomulan->m_blMustReachPosition = object.m_blMustReachPosition;
				        pRomulan->m_blNoRelease         = object.m_blNoRelease;
				        pRomulan->m_blCanFind           = object.m_blCanFind;

						for (int i=0;i<10;i++)
						{
							pRomulan->m_lstHealth[i]=object.m_nHealth[i];
						}
						Add(pRomulan);
					}
				break;

				case ID_FEDERATIONBASE:
					    TStarbase * pFedbase        = new TStarbase(ID_FEDERATIONBASE);
						pFedbase->m_dX              = object.m_dX;
						pFedbase->m_dY              = object.m_dY;
						pFedbase->m_nZ              = object.m_nZ;
				        pFedbase->m_dAngle          = object.m_dAngle;
						pFedbase->m_nCrew           = object.m_nCrew;
						pFedbase->m_nRepairItem     = object.m_nRepairItem;
						pFedbase->m_nTorpedoes      = object.m_nTorpedoes;
						pFedbase->m_nShieldEnergy   = object.m_nShieldEnergy;
						pFedbase->m_nPhaserEnergy   = object.m_nPhaserEnergy;
						pFedbase->m_nPreferedTarget = object.m_nPreferedTarget;
						pFedbase->m_nPhaserPower    = object.m_nPhaserPower;
						pFedbase->m_nMember    = object.m_nMember;

						pFedbase->m_blMustBeDestroyed   = object.m_blMustBeDestroyed;
				        pFedbase->m_blMustSurvive       = object.m_blMustSurvive;

						for (int i=0;i<10;i++)
						{
							pFedbase->m_lstHealth[i]=object.m_nHealth[i];
						}
						Add(pFedbase);

				break;


			}
	   }

	   }

std::list<TSprite *>::const_iterator p=m_lstItems.begin();
while (p!=m_lstItems.end())
{
	if (((*p)->m_nID>ID_SHIP)&&(((TShip *)(*p))->m_blDocked))
	{
		((TShip *)(*p))->m_blCanFind = false;
		((TShip *)(*p))->m_blDocking=false;
		((TShip *)(*p))->m_blReleasing = false;
		((TShip *)(*p))->m_dSpeed=0;
		((TShip *)(*p))->m_pBaseTarget=(TShip *) Seekstarbase(((TShip *)(*p))->m_nMember,false,100000,((TShip *)(*p))->m_dX,((TShip *)(*p))->m_dY);
		((TShip *)(*p))->m_pTarget = ((TShip *)(*p))->m_pBaseTarget;
	}
	else if (((*p)->m_nID>ID_SHIP)&&(((TShip *)(*p))->m_blDocking))
	{
		((TShip *)(*p))->m_blDocking=true;
		((TShip *)(*p))->m_blReleasing = false;
		((TShip *)(*p))->m_pBaseTarget=(TShip *) Seekstarbase(((TShip *)(*p))->m_nMember,false,100000,((TShip *)(*p))->m_dX,((TShip *)(*p))->m_dY);
		((TShip *)(*p))->m_pTarget = ((TShip *)(*p))->m_pBaseTarget;
	}

	p++;
}


return true;

}



double Distance(double a_dX1,double a_dY1, double a_dX2,double a_dY2)
{
	double DX=a_dX1-a_dX2;
	double DY=a_dY1-a_dY2;
	if (DX==0) return (DY < 0) ? -DY : DY;
	else if (DY==0) return (DX < 0) ? -DX : DX;
	else return (sqrt((DX*DX)+(DY*DY)));
}



bool Comp(const TSprite *v1, const TSprite *v2)
{
	return (v1->m_nZ < v2->m_nZ);
}



