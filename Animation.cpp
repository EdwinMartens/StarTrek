#include "Animation.h"

extern ALLEGRO_DISPLAY * g_pDisplay;
extern TEngine         * g_pEngine;

static std::vector<ALLEGRO_BITMAP *> g_lstAnimationFrames;



TAnimation::TAnimation(ANIMATION::TYPE a_Type,
                       double a_dX, double a_dY,
                       double a_dSpeed = 0,
                       double a_dAngle = 0,
                       int a_nZ = 100)
{
    m_Type   = a_Type;
    m_blReverse = false;

    m_dX = a_dX;
    m_dY = a_dY;
    m_nZ = a_nZ;
    m_nWait  = m_nDelay;

    m_dSpeed = a_dSpeed;
    m_dAngle = a_dAngle;
    m_blBackward = false;
    m_ID=ID_ANIMATION;
    SetAnimation();
}

TAnimation::TAnimation()
{
   	m_nZ	        = 100;
	m_nFrame        =  0;
	m_blReverse     = false;
	m_blDestroyed   = false;
	m_blCanCollide  = false;
	m_ID            = ID_ANIMATION;
	m_Type          = ANIMATION::EXPLOSION;
	m_blBackward = false;
	SetAnimation();
}


TAnimation::TAnimation(ifstream & a_LoadStream,ID a_id)
:TSprite(a_LoadStream,a_id)
{
    m_nZ	        = 100;
    m_blReverse     = false;
	m_blDestroyed   = false;
	m_blCanCollide  = false;
	m_blBackward = false;

    a_LoadStream.read((char *)&m_nFrame, sizeof (m_nFrame));
    a_LoadStream.read((char *)&m_nWait, sizeof (m_nWait));
    a_LoadStream.read((char *)&m_Type, sizeof (m_Type));
    SetAnimation();
}


void TAnimation::Save(ofstream & a_SaveStream)
{
    TSprite::Save(a_SaveStream);
    a_SaveStream.write((char*)& m_nFrame, sizeof (m_nFrame));
    a_SaveStream.write((char*)& m_nWait, sizeof (m_nWait));
    a_SaveStream.write((char*)& m_Type, sizeof (m_Type));
}


void TAnimation::SetAnimation()
{
    switch (m_Type)
    {
        case ANIMATION::EXPLOSION:
        m_nStartFrame = 0;
        m_nEndFrame   = 15;
        m_nDelay      = 6;
        m_Mode        = ONCE;
        break;


        case ANIMATION::SMOKE:
        m_nStartFrame = 16;
        m_nEndFrame   = 27;
        m_nDelay      = 2;
        m_Mode        = ONCE;

        break;

        case ANIMATION::WARP_IN:
        m_nStartFrame = 28;
        m_nEndFrame   = 37;
        m_nDelay      = 6;
        m_Mode        = ONCE;
        break;

        case ANIMATION::WARP_OUT:

        break;

        default:
          m_nFrame      = -1;
          m_nStartFrame = -1;
          m_nEndFrame   = -1;

    }

    m_nFrame = m_nStartFrame;
    m_nWait  = m_nDelay;
    if (m_Mode != NOFRAME)
    {
        SetImage(g_lstAnimationFrames[m_nFrame]);
    }

}


void TAnimation::Do_ai()
{
	if (m_nWait <=0)
	{
		switch (m_Mode)
		{
            case ONCE:
            {
                 m_nFrame ++;
                 if ((m_nFrame >= m_nStartFrame)&&(m_nFrame <= m_nEndFrame))
                 {
                     SetImage(g_lstAnimationFrames[m_nFrame]);
                 }
                 else
                 {
                     m_blDestroyed = true;
                 }
            }
            break;

            case LOOP:
            {
                 m_nFrame ++;
                 if ((m_nFrame >= m_nStartFrame)&&(m_nFrame <= m_nEndFrame))
                 {
                     SetImage(g_lstAnimationFrames[m_nFrame]);
                 }
                 else
                 {
                     if (m_nFrame > m_nEndFrame)
                     {
                         m_nFrame = m_nStartFrame;
                     }
                     else if (m_nFrame < m_nStartFrame)
                     {
                         m_nFrame = m_nEndFrame;
                     }
                     SetImage(g_lstAnimationFrames[m_nFrame]);
                 }
            }
            break;

            case BACK_FORTH:
                 if (m_blBackward)
                 {
                     m_nFrame --;
                 }
                 else
                 {
                     m_nFrame ++;
                 }

            if ((m_nFrame >= m_nStartFrame)&&(m_nFrame <= m_nEndFrame))
            {
                 SetImage(g_lstAnimationFrames[m_nFrame]);
            }
            else
            {
                 if (m_nFrame > m_nEndFrame)
                 {
                      m_nFrame = m_nEndFrame;
                      m_blBackward = true;
                 }
                 else if (m_nFrame < m_nStartFrame)
                 {
                     m_nFrame = m_nStartFrame;
                     m_blBackward = false;
                 }
                 SetImage(g_lstAnimationFrames[m_nFrame]);
            }
            break;


            default:



            break;


		}
		m_nWait=m_nDelay;
	}
	else
	{
		m_nWait--;
	}
}


bool TAnimation::Init()
{
	ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Explosion");

    for (int i=0;i<16;i++)
	{
		char szFileName[255];
		sprintf(szFileName,"Expl%02d.png",i+1);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstAnimationFrames.push_back(pBitmap);
	    }
	    else
	    {
            al_destroy_path(pPath);
            return false;
	    }
	}

	al_destroy_path(pPath);

	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "smoke");

    for (int i=0;i<12;i++)
	{
		char szFileName[255];
		sprintf(szFileName,"S%02d.png",i);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstAnimationFrames.push_back(pBitmap);
	    }
	    else
	    {
            al_destroy_path(pPath);
            return false;
	    }
	}
	al_destroy_path(pPath);

    int xx,yy;
    for (int i = 0; i < 10; i++)
    {
        ALLEGRO_BITMAP * pBitmap = al_create_bitmap(80,80);
        al_set_target_bitmap(pBitmap);
        al_clear_to_color(al_map_rgba(0,0,0,255));
        for (int a = 0; a < 50; a++)
        {
            xx = 40 + (rand() % (8*i +1)) - (4*i);
            yy = 40 + (rand() % (8*i +1)) - (4*i);
            al_draw_line(40,40,xx,yy,al_map_rgb(255,255,255),2);
        }

        g_lstAnimationFrames.push_back(pBitmap);
    }
    al_set_target_backbuffer(g_pDisplay);


	return true;
}


void TAnimation::DeInit()
{
    ALLEGRO_BITMAP * pBitmap = NULL;
    for (size_t i=0; i< g_lstAnimationFrames.size();i++)
    {
        pBitmap = g_lstAnimationFrames[i];
        al_destroy_bitmap(pBitmap);
    }
    g_lstAnimationFrames.clear();
}




