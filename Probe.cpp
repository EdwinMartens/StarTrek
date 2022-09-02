#include "Probe.h"
#include "particle.h"


ALLEGRO_BITMAP * g_pProbeBitmap=NULL;




TProbe::TProbe(double a_dX, double a_dY,double a_dSpeed, double a_dAngle, int a_nZ, TEnterprise * a_pOwner)
{
  m_pOwner        =   a_pOwner;
  m_dX		    =   a_dX;
	m_dY			=   a_dY;
	m_dAngle		=   a_dAngle;
	m_nZ            =   a_nZ;
	m_dSpeed		=	a_dSpeed;
	m_ID			=	ID_PROBE;
	m_blCanCollide	=   true;
	m_nBitmapWidth	=   10;
	m_nBitmapHeight =   10;
	m_Member	    =   MEM_NEUTRAL;
	SetImage(g_pProbeBitmap);
}

TProbe::TProbe()
{
    m_pOwner        = NULL;
    m_blCanCollide	=   true;
	m_nBitmapWidth	=    10;
	m_nBitmapHeight =    10;
}


TProbe::TProbe(ifstream & a_LoadStream, ID a_id)
:TSprite(a_LoadStream, a_id)
{
    m_pOwner        = NULL;
    m_blCanCollide	=   true;
	m_nBitmapWidth	=    10;
	m_nBitmapHeight =    10;

	#ifdef _DEBUG
	std::cout << "-Probe loaded\n";
	#endif // _DEBUG
}

void TProbe::Save(ofstream & a_SaveStream)
{
    TSprite::Save(a_SaveStream);
    #ifdef _DEBUG
	std::cout << "-Probe saved\n";
	#endif // _DEBUG
}


void TProbe::Do_ai()
{
   //TODO (Edwin#1#): add object/ planet probing

   if ((m_pOwner!=NULL) && (m_pOwner->m_blDestroyed))
   {
       m_pOwner = NULL;
   }

   if (m_dSpeed < 2000)
   {
       m_dSpeed += 2;
   }


  int DX = 0;
  int DY = 0;

  if (m_dX < 0)
  {
      DX = -1;
  }
  else if (m_dX > SECTORSIZE)
  {
      DX = 1;
  }
  if (m_dY < 0)
  {
     DY= -1;
  }
  else if (m_dY > SECTORSIZE)
  {
     DY = 1;
  }

  if ((DX != 0)||(DY != 0))
  {
     // update starmap
     if (m_pOwner != NULL)
     {
        m_pOwner->UpdateStarmap(DX,DY);

     }

     // end of probe life
     m_blDestroyed=true;
  }

  if (m_dSpeed < 1500)
  {
      for (int i=0;i<10;i++)
      {
         TParticle *p=new TParticle(EFF_PROBE);
         p->SetPosition(m_dX,m_dY);
         m_pEngine->Add(p);
      }
  }

}


void TProbe::DoCollision(TSprite * sprite)
{
	if (((sprite->m_ID >ID_PLANET_BOTTOM) && (sprite->m_ID < ID_STAR_TOP)) || (sprite->m_ID==ID_ASTEROID))
    {
        TSpaceObject * pObject = (TSpaceObject *) sprite;
        ScanObject(pObject);
        m_blDestroyed=true;
    }
}



void TProbe::ScanObject(TSpaceObject * a_pObject)
{
    TScanInfo scaninfo;
    switch (a_pObject->m_ID)
    {
        case ID_CLASS_M1:
        {
            scaninfo.m_vLines.push_back("A class M planet");

            std::string strOccupation ="Occupation :";
            //strOccupation.append(std::to_string(a_pObject->));
            //scaninfo.m_vLines.push_back("Occupation :");
        }
        break;

        default:

        break;
    }

    scaninfo.m_Valid = true;

    if (m_pOwner != NULL)
    {
       m_pOwner->SetScanInfo(scaninfo);
    }
}





void TProbe::Draw(double a_dCamX, double a_dCamY)
{
    int DrawX=int(m_pEngine->m_nScreenMidX+(m_dX-a_dCamX));
	int DrawY=int(m_pEngine->m_nScreenMidY+(m_dY-a_dCamY));
	if ((DrawX+m_nBitmapMidX>0)&&(DrawX-m_nBitmapMidX<m_pEngine->m_nScreenWidth)&&(DrawY+m_nBitmapMidY>0)&& (DrawY-m_nBitmapMidY<m_pEngine->m_nScreenHeight))
    {
        al_draw_rotated_bitmap(m_pImage, m_nBitmapMidX, m_nBitmapMidY, DrawX, DrawY,m_dAngle, 0);
    }
}

bool TProbe::Init()
{
    ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "probe");
    al_set_path_filename(pPath,"probe.png");
    g_pProbeBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(pPath);

	if (g_pProbeBitmap==NULL)
	{
		return false;
	}
	else
	{
		return true;
	}

}




