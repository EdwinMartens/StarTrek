#include "Transporter.h"
#include "SoundManager.h"
#include "Enterprise.h"
#include "Communication.h"

extern TEnterprise * g_pEnterprise;
extern Communication * g_pCommunication;
extern TEngine       * g_pEngine;

static std::vector<ALLEGRO_BITMAP *> g_lstInventoryImages;
ALLEGRO_COLOR g_BeamColor;
ALLEGRO_BITMAP * g_pTransporterControl = NULL;

CTransporter::CTransporter()
{
    m_State = TS_EMPTY;
    m_TransporterItem.imageID = INV_NONE;
    m_TransporterItem.b_blUnique = false;
    m_TransporterItem.m_nWidth =0;
    m_TransporterItem.m_nHeight =0;
    m_TransporterItem.m_blValid = false;

    m_nBeamHeight = 0;
    m_nKnobOffset = 0;
    m_blBeamUp = false;
    m_pSelection = NULL;

    m_nKnobX = g_pEngine->m_nScreenMidX-85;
    m_nKnobY = 550;
}

CTransporter::CTransporter(ifstream & a_LoadStream)
{
    m_State = TS_EMPTY;
    m_TransporterItem.imageID = INV_NONE;
    m_TransporterItem.b_blUnique = false;
    m_TransporterItem.m_nWidth =0;
    m_TransporterItem.m_nHeight =0;
    m_nBeamHeight = 0;
    m_nKnobOffset = 0;
    m_blBeamUp = false;
    m_pSelection = NULL;
    m_nKnobX = g_pEngine->m_nScreenMidX-85;
    m_nKnobY = 550;


    a_LoadStream.read((char*)& m_blBeamUp, sizeof (m_blBeamUp));
    a_LoadStream.read((char*)& m_State, sizeof (m_State));
    a_LoadStream.read((char*)& m_TransporterItem, sizeof (m_TransporterItem));
    size_t nItems;
    a_LoadStream.read((char*)& nItems, sizeof (nItems));
    for (size_t i =0; i < nItems; i++)
    {
        TInventoryItem item;
        a_LoadStream.read((char*)& item.imageID, sizeof (item.imageID));
        a_LoadStream.read((char*)& item.b_blUnique, sizeof (item.b_blUnique));
        a_LoadStream.read((char*)& item.m_nWidth, sizeof (item.m_nWidth));
        a_LoadStream.read((char*)& item.m_nHeight, sizeof (item.m_nHeight));
        a_LoadStream.read((char*)& item.m_blValid, sizeof (item.m_blValid));
        m_lstInventory.push_back(item);
    }
}

void CTransporter::Save(ofstream & a_SaveStream)
{
    a_SaveStream.write((char*)& m_blBeamUp, sizeof (m_blBeamUp));
    a_SaveStream.write((char*)& m_State, sizeof (m_State));
    a_SaveStream.write((char*)& m_TransporterItem, sizeof (m_TransporterItem));
    size_t nItems = m_lstInventory.size();
    a_SaveStream.write((char*)& nItems, sizeof (nItems));
    for (size_t i =0; i < m_lstInventory.size(); i++)
    {
        TInventoryItem item = m_lstInventory[i];
        a_SaveStream.write((char*)& item.imageID, sizeof (item.imageID));
        a_SaveStream.write((char*)& item.b_blUnique, sizeof (item.b_blUnique));
        a_SaveStream.write((char*)& item.m_nWidth, sizeof (item.m_nWidth));
        a_SaveStream.write((char*)& item.m_nHeight, sizeof (item.m_nHeight));
        a_SaveStream.write((char*)& item.m_blValid, sizeof (item.m_blValid));
    }
}



void CTransporter::StartBeam(bool a_blUp)
{
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
        m_State= TS_BEAMDOWN;
    }
}

void CTransporter::Enter(TInventoryItem a_Item)
{
     if ((IsEmpty()) && (m_blBeamUp == false))
     {
        m_TransporterItem = a_Item;
        m_TransporterItem.m_blValid = true;
        m_TransporterItem.m_nHeight = al_get_bitmap_height(g_lstInventoryImages[m_TransporterItem.imageID]);
        m_TransporterItem.m_nWidth = al_get_bitmap_width(g_lstInventoryImages[m_TransporterItem.imageID]);
        m_blBeamUp = false;
        m_State = TS_OCCUPIED;
     }
}

void CTransporter::Target(TInventoryItem a_Item)
{
     if (IsEmpty())
     {
        m_TransporterItem = a_Item;
        m_TransporterItem.m_blValid = true;

        m_TransporterItem.m_nHeight = al_get_bitmap_height(g_lstInventoryImages[m_TransporterItem.imageID]);
        m_TransporterItem.m_nWidth = al_get_bitmap_width(g_lstInventoryImages[m_TransporterItem.imageID]);
        m_blBeamUp = true;
        m_State = TS_EMPTY;
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
            m_blBeamUp = false;
        }
        else
        {

        }
        break;

        case TS_EMPTY:
             m_nKnobOffset = 0;
             m_TransporterItem.m_blValid = false;
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


void CTransporter::draw(int a_nX, int a_nY, int a_nHeight,ALLEGRO_DISPLAY * a_pDisplay)
{
   int nCenter = m_TransporterItem.m_nWidth/2.0;
   switch(m_State)
   {
        case TS_BEAMDOWN:
        {
            float alpha;

            if (m_blBeamUp)
            {
                alpha = (float)m_nBeamHeight/(float)a_nHeight;
                al_draw_tinted_bitmap_region(g_lstInventoryImages[m_TransporterItem.imageID],
                                          al_map_rgba_f(alpha, alpha, alpha, alpha), 0, 0, 100,
                                          m_nBeamHeight, a_nX-(nCenter/2), a_nY+50, 0);
            }
            else
            {
                al_draw_bitmap(g_lstInventoryImages[m_TransporterItem.imageID],a_nX-(nCenter/2), a_nY+50, 0);
            }


            pLockedRegion = al_lock_bitmap(al_get_backbuffer(a_pDisplay), ALLEGRO_PIXEL_FORMAT_ANY, 0);
            int nX;
            int nY;
            for (int i=0;i<m_nBeamHeight*20;i++)
            {
                nX = a_nX-nCenter + (rand() % m_TransporterItem.m_nWidth);
                nY = 50+a_nY + rand() % m_nBeamHeight;
                al_put_pixel(nX, nY,g_BeamColor);
            }
            al_unlock_bitmap(al_get_backbuffer(a_pDisplay));

            if (m_nBeamHeight>(a_nHeight-50))
            {
                m_State=TS_BEAMUP;
            }
        }
        break;

        case TS_BEAMUP:
        {
            if (m_blBeamUp)
            {
                al_draw_bitmap(g_lstInventoryImages[m_TransporterItem.imageID],a_nX-(nCenter/2), a_nY+50, 0);
            }
            else
            {
                float alpha;
                alpha = (float)m_nBeamHeight/(float)a_nHeight;
                al_draw_tinted_bitmap_region(g_lstInventoryImages[m_TransporterItem.imageID],
                                             al_map_rgba_f(alpha, alpha, alpha, alpha), 0, 0, 100,
                                              m_nBeamHeight, a_nX-(nCenter/2), a_nY+50, 0);
            }

            pLockedRegion = al_lock_bitmap(al_get_backbuffer(a_pDisplay), ALLEGRO_PIXEL_FORMAT_ANY, 0);
            int nX;
            int nY;
            for (int i=0;i<m_nBeamHeight*20;i++)
            {
                nX = a_nX-nCenter + rand() % (m_TransporterItem.m_nWidth);
                nY = 50 + a_nY + rand() % m_nBeamHeight;
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
                }
             }
        }
        break;

        case TS_OCCUPIED:
            al_draw_bitmap(g_lstInventoryImages[m_TransporterItem.imageID],a_nX-(nCenter/2), a_nY+50, 0);
        // draw Transported item

        break;

        case TS_EMPTY:

        break;

    }

    drawKnob();
    drawInventory();
}


void CTransporter::addToInventory(TInventoryItem a_Item)
{
     if (a_Item.imageID == INV_SPOCK)
     {
         g_pEnterprise->SetCrewMember(CREW_SPOCK);
         g_pCommunication->AddMessage(12,CREW_KIRK,"Welcome aboard mr Spock");
         g_pCommunication->AddMessage(13,CREW_SPOCK,"Thank you sir !");
     }
     else
     {
         m_lstInventory.push_back(a_Item);
     }

}

void CTransporter::removeFromInventory(int a_nIndex)
{
    m_lstInventory.erase(m_lstInventory.begin() + a_nIndex);
}

void CTransporter::drawInventory()
{
    int nStartX = 530;
    int nStartY = 390;

    int x = nStartX;
    int y = nStartY;

    for (size_t i=0; i < m_lstInventory.size(); i++)
    {
        TInventoryItem item = m_lstInventory[i];
        al_draw_bitmap(g_lstInventoryImages[item.imageID], x, y, 0);
        m_lstInventory[i].m_nX = x;
        m_lstInventory[i].m_nY = y;

        int incx = al_get_bitmap_width(g_lstInventoryImages[item.imageID]);
        int incy = al_get_bitmap_height(g_lstInventoryImages[item.imageID]);

        x+=incx+ 10;
        if (x > 900)
        {
            x = nStartX;
            y+= incy;
        }
    }
}


void CTransporter::drawKnob()
{
    al_draw_bitmap(g_pTransporterControl,m_nKnobX ,m_nKnobY + m_nKnobOffset,0);
}


bool CTransporter::IsEmpty()
{
    return (m_State == TS_EMPTY);
}

TInventoryItem CTransporter::GetBeamedDownItem()
{
    return m_TransporterItem;
}



TResponse CTransporter::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event, int a_nHealth,int a_nDistance, bool a_blShield)
{
    TResponse blResult = RE_NOTHING;
    if (mouse_event.button == 1)
    {
        if ((mouse_event.x > 20) && (mouse_event.x < 300) && (mouse_event.y >400) && (mouse_event.y < 600))
        {
            m_lstInventory.push_back(m_TransporterItem);
            m_State = TS_EMPTY;
        }
        else if ((mouse_event.x > m_nKnobX) && (mouse_event.x < m_nKnobX+75) && (mouse_event.y >550) && (mouse_event.y < 570))
        {
            if (!a_blShield)
            {
                if (a_nDistance  < ENTERPRISE_TRAND_DIST)
                {
                    if (a_nHealth > 40)
                    {
                        if (m_State == TS_OCCUPIED)
                        {
                            blResult = RE_BEAM_DOWN;
                            StartBeam(false);
                        }
                        else
                        {
                            if (m_TransporterItem.m_blValid)
                            {
                                blResult = RE_BEAM_UP;
                                StartBeam(true);
                            }
                            else
                            {
                                return RE_NOTARGET;
                            }
                        }
                    }
                    else
                    {
                        blResult = RE_DAMAGED;
                    }
                }
                else
                {
                    blResult = RE_NOTINRANGE;
                }
            }
            else
            {
                blResult = RE_SHIELD;
            }
        }
        else
        {
            for (size_t i =0; i < m_lstInventory.size(); i++)
            {
                TInventoryItem item = m_lstInventory[i];
                if ((mouse_event.x > item.m_nX) && (mouse_event.x < item.m_nX + item.m_nWidth) &&
                    (mouse_event.y > item.m_nY) && (mouse_event.y < item.m_nY + item.m_nHeight))
                    {
                        if (!m_blBeamUp)
                        {
                            m_lstInventory.erase(m_lstInventory.begin() + i);
                            Enter(item);
                        }
                    }
            }
        }
    }
    return blResult;
}

bool CTransporter::Init()
{
    g_BeamColor = al_map_rgb(255,255,255);
    ALLEGRO_PATH * pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Inventory");

    for (int i=INV_NONE+1;i<(INV_END);i++)
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
    al_destroy_path(pPath);
	pPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(pPath, "graphics");
    al_append_path_component(pPath, "Interface");
    al_set_path_filename(pPath,"btn01.png");
    g_pTransporterControl = al_load_bitmap(al_path_cstr(pPath,ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(pPath);

	if (g_pTransporterControl == NULL)  return false;
	return true;
}



