#include "Menu.h"
#include <allegro5/allegro_primitives.h>


/** \brief
 *  Menu item Constructor.
 *
 * \param int a_nX
 * X position of menu item
 * \param int a_nY
 * Y position of menu item
 * \param int a_nWidth
 * Width of menu item
 * \param int a_nHeight
 * Height of menu item
 * \param TMenu * a_pParent
 * the owner of this menu item... A menu
 * \param void (* a_pCallBack)()
 * Pointer to a function to be called when this menuitem is clicked
 */
TMenuItem::TMenuItem (int a_nX, int a_nY, int a_nWidth, int a_nHeight, TMenu * a_pParent, void (* a_pCallback)())
{
    m_pParent   = a_pParent;
    m_nX        = a_nX;
    m_nY        = a_nY;
    m_nWidth    = a_nWidth;
    m_nHeight   = a_nHeight;
    m_blHighlight = false;
    m_pCallBack   = a_pCallback;
}

/** \brief
 *  Mouse event handler for menu item.
 * \param ALLEGRO_MOUSE_EVENT * mouse_event
 * a mouse event passed by allegro
 */
void TMenuItem::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event)
{

    if ((mouse_event.x > m_nX) &&
        (mouse_event.x < m_nX + m_nWidth) &&
        (mouse_event.y > m_nY) &&
        (mouse_event.y < m_nY + m_nHeight))
    {
        m_blHighlight = true;
        if ((m_pCallBack!=NULL)&&(mouse_event.button == 1))
        {
            m_pCallBack();
        }
    }
    else
    {
       //TODO (Anyone #1#): when clicking a menu item sometimes this generates an annoying segmentation fault
        m_blHighlight = false;
    }
}

/** \brief
 *  Draw this menu item
 * \param ALLEGRO_FONT * a_pFont
 * a pointer to a font to use for text
 */
void TMenuItem::Draw(ALLEGRO_FONT * a_pFont)
{

    if (m_blHighlight)
    {
        al_draw_filled_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorHighlight);

    }
    else
    {
        al_draw_filled_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorInner);
    }

    al_draw_rounded_rectangle((float)m_nX,
                                     (float)m_nY,
                                     (float)(m_nX + m_nWidth),
                                     (float)(m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorOuter,2);


   if (a_pFont != NULL)
   {

      al_draw_text(a_pFont,
                   m_pParent->m_ColorOuter,
                   m_nX + (m_nWidth/2), m_nY + (m_nHeight/4),
                   ALLEGRO_ALIGN_CENTRE,
                   m_strCaption.c_str());

   }
}



TMenu::TMenu()
{
    m_ColorInner     = al_map_rgb(0,0,255);
    m_ColorOuter     = al_map_rgb(255,255,255);
    m_ColorHighlight = al_map_rgb(100,100,255);
    m_nIndex         = 1;
}

TMenu::~TMenu()
{
    m_vMenuItems.clear();
}

void TMenu::ClearItems()
{
    m_vMenuItems.clear();
    m_nIndex = 1;
}


void TMenu::AddMenuItem(int a_nBasePositionX,string a_strCaption,void (* a_pCallBack)())
{
    TMenuItem item(a_nBasePositionX-300,90 * m_nIndex, 600, 80,this, a_pCallBack);
    item.m_strCaption = a_strCaption;
    m_vMenuItems.push_back(item);
    m_nIndex++;
}

void TMenu::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event)
{
    if (! m_vMenuItems.empty())
    {
        for (size_t i=0; i< m_vMenuItems.size(); i++ )
        {
            m_vMenuItems[i].ProcessMouseEvent(mouse_event);
        }
    }
}

void TMenu::Draw(ALLEGRO_FONT * a_pFont)
{

    for (size_t i=0; i< m_vMenuItems.size(); i++ )
    {
       m_vMenuItems[i].Draw(a_pFont);
    }

}


void TMenu::DeleteLast()
{
    m_vMenuItems.erase(m_vMenuItems.end());
}

int TMenu::Count()
{
    return m_nIndex -1;
}

