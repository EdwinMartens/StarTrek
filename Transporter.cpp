#include "Transporter.h"
#include "SoundManager.h"

static std::vector<ALLEGRO_BITMAP *> g_lstInventoryImages;

ALLEGRO_COLOR g_BeamColor;

ALLEGRO_BITMAP * g_pTransporterControl = NULL;

CTransporter::CTransporter()
{
    m_State = TS_EMPTY;
    m_TransporterItem = INV_NONE;
    m_nBeamHeight = 0;
    m_nKnobOffset = 0;
    m_blBeamUp = true;
    m_Selection = INV_NONE;
}

void CTransporter::StartBeam( TInventory a_TransporterItem, bool a_blUp)
{
    m_TransporterItem = a_TransporterItem;
    m_nBeamHeight = 0;
    m_blBeamUp = a_blUp;
    if (m_blBeamUp)
    {
        SoundManager::PlaySound(SOUND::TRANSPORTER1,1.0);
        m_State= TS_BEAMDOWN;
    }
    else
    {
        SoundManager::PlaySound(SOUND::TRANSPORTER1,1.0);
        m_State= TS_OCCUPIED;
    }
}



void CTransporter::update()
{
     switch(m_State)
     {
        case TS_BEAMDOWN:
            m_nBeamHeight+=2;
        break;

        case TS_BEAMUP:
            m_nBeamHeight-=2;
        break;

        case TS_OCCUPIED:
        if (m_blBeamUp)
        {
            addToInventory(m_TransporterItem);
            m_State = TS_EMPTY;
            m_TransporterItem = INV_NONE;
        }
        else
        {
            m_State = TS_BEAMDOWN;
        }
        break;

        case TS_EMPTY:
             m_nKnobOffset = 0;
        break;
     }

     if ((m_State == TS_BEAMUP)||(m_State == TS_BEAMDOWN))
     {
        if ((m_blBeamUp) && (m_nKnobOffset < 140))
        {
            m_nKnobOffset +=2;
        }
        else if ((!m_blBeamUp)&&(m_nKnobOffset > -140))
        {
            m_nKnobOffset -=2;
        }
     }
}


void CTransporter::draw(int a_nX, int a_nY, int a_nWidth,int a_nHeight,ALLEGRO_DISPLAY * a_pDisplay)
{
    if (m_TransporterItem > INV_NONE)
    {
        switch(m_State)
        {
            case TS_BEAMDOWN:
            {
                float alpha;
                if (m_blBeamUp)
                {
                    alpha = (float)m_nBeamHeight/(float)a_nHeight;
                    al_draw_tinted_bitmap_region(g_lstInventoryImages[m_TransporterItem-1],
                                                 al_map_rgba_f(alpha, alpha, alpha, alpha), 0, 0, 100,
                                                 m_nBeamHeight-50, a_nX, a_nY+50, 0);
                }
                else
                {
                    al_draw_bitmap(g_lstInventoryImages[m_TransporterItem-1],a_nX, a_nY+50, 0);
                }


                pLockedRegion = al_lock_bitmap(al_get_backbuffer(a_pDisplay), ALLEGRO_PIXEL_FORMAT_ANY, 0);
                int nX;
                int nY;
                for (int i=0;i<m_nBeamHeight*20;i++)
                {
                    nX = a_nX + rand() % a_nWidth;
                    nY = a_nY + rand() % m_nBeamHeight;
                    al_put_pixel(nX, nY,g_BeamColor);
                }
                al_unlock_bitmap(al_get_backbuffer(a_pDisplay));

                if (m_nBeamHeight>a_nHeight)
                {
                    m_State=TS_BEAMUP;
                }

            }
            break;

        case TS_BEAMUP:
        {
            if (m_blBeamUp)
            {
                al_draw_bitmap(g_lstInventoryImages[m_TransporterItem-1],a_nX, a_nY+50, 0);
            }
            else
            {
                float alpha;
                alpha = (float)m_nBeamHeight/(float)a_nHeight;
                al_draw_tinted_bitmap_region(g_lstInventoryImages[m_TransporterItem-1],
                                             al_map_rgba_f(alpha, alpha, alpha, alpha), 0, 0, 100,
                                              m_nBeamHeight-50, a_nX, a_nY+50, 0);
            }

            pLockedRegion = al_lock_bitmap(al_get_backbuffer(a_pDisplay), ALLEGRO_PIXEL_FORMAT_ANY, 0);
            int nX;
            int nY;
            for (int i=0;i<m_nBeamHeight*20;i++)
            {
                nX = a_nX + rand() % a_nWidth;
                nY = a_nY + rand() % m_nBeamHeight;
                al_put_pixel(nX, nY,g_BeamColor);
            }
            al_unlock_bitmap(al_get_backbuffer(a_pDisplay));

            if (m_nBeamHeight<=0)
            {
                if (m_blBeamUp)
                {
                    m_State=TS_OCCUPIED;
                }
                else
                {
                      m_State = TS_EMPTY;
                      m_TransporterItem = INV_NONE;
                }
             }
        }
        break;

        case TS_OCCUPIED:
            al_draw_bitmap(g_lstInventoryImages[m_TransporterItem-1],a_nX, a_nY+50, 0);
        // draw Transported item

        break;

        case TS_EMPTY:

        break;

        }
    }

    drawKnob();
    drawInventory();
}


void CTransporter::addToInventory(TInventory a_Item)
{
     m_lstInventory.push_back(a_Item);
}

void CTransporter::removeFromInventory(TInventory a_Item)
{
    list<TInventory>::iterator P = m_lstInventory.begin();
    bool blRemoved = false;
    while ((P != m_lstInventory.end()) && (!blRemoved))
    {
        if ((*P) == a_Item)
        {
            P = m_lstInventory.erase(P);
            blRemoved = true;
        }
        else
        {
            P++;
        }
    }
}

void CTransporter::drawInventory()
{
    list<TInventory>::const_iterator P = m_lstInventory.begin();
    int nStartX = 530;
    int nStartY = 390;

    int x = nStartX;
    int y = nStartY;

    while (P != m_lstInventory.end())
    {
        int nItem = (int)(*P);
        al_draw_bitmap(g_lstInventoryImages[nItem-1], x, y, 0);

        int incx = al_get_bitmap_width(g_lstInventoryImages[nItem-1]);
        int incy = al_get_bitmap_height(g_lstInventoryImages[nItem-1]);

        x+=incx;
        if (x > 900)
        {
            x = nStartX;
            y+= incy;
        }

        P++;
    }

}

void CTransporter::drawKnob()
{
    int nX = 425;
    int nY = 550;
    al_draw_bitmap(g_pTransporterControl,nX ,nY + m_nKnobOffset,0);
}


void CTransporter::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    if (mouse_event.button == 1)
    {
        list<TInventory>::const_iterator P = m_lstInventory.begin();
        while (P != m_lstInventory.end())
        {
            P++;
        }
    }

}

bool CTransporter::Init()
{
    g_BeamColor = al_map_rgb(255,255,255);
    ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Inventory");

    for (int i=INV_NONE;i<(INV_END-1);i++)
	{
		char szFileName[255];
		sprintf(szFileName,"INV_%03d.png",i);
		al_set_path_filename(pPath,szFileName);
        ALLEGRO_BITMAP * pBitmap = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	    if (pBitmap!=NULL)
	    {
	        g_lstInventoryImages.push_back(pBitmap);
	    }
	    else
	    {
            al_destroy_path(pPath);
            return false;
	    }
	}

	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Interface");
    al_set_path_filename(pPath,"btn01.png");
    g_pTransporterControl = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);

	if (g_pTransporterControl == NULL)  return false;
	return true;
}



