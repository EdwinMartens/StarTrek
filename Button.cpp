#include "Button.h"
#include "WindowDialog.h"
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
CButton::CButton(int a_nX, int a_nY, std::string a_strCaption, CWindowDialog * a_pParent, void (* a_pCallBack)(CWindowDialog *))
{
    m_pParent   = a_pParent;
    m_nX        = a_nX;
    m_nY        = a_nY;
    m_strCaption = a_strCaption;
    m_blHighlight = false;
    m_pCallBack   = a_pCallBack;


    m_nWidth = al_get_text_width(m_pParent->m_pFont, a_strCaption.c_str());
    m_nWidth += 20;
    m_nHeight = al_get_font_line_height(m_pParent->m_pFont);
    m_nHeight += 10;

}

/** \brief
 *  Mouse event handler for menu item.
 * \param ALLEGRO_MOUSE_EVENT * mouse_event
 * a mouse event passed by allegro
 */
void CButton::ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event,int a_nX, int a_nY)
{

    if ((mouse_event.x > a_nX + m_nX) &&
        (mouse_event.x < a_nX + m_nX + m_nWidth) &&
        (mouse_event.y > a_nY + m_nY) &&
        (mouse_event.y < a_nY + m_nY + m_nHeight))
    {
        m_blHighlight = true;
        if ((m_pCallBack!=NULL)&&(mouse_event.button == 1))
        {
            m_pCallBack(m_pParent);
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
void CButton::Draw(int a_nX, int a_nY)
{

    if (m_blHighlight)
    {
        al_draw_filled_rounded_rectangle((float)a_nX + m_nX,
                                     (float)a_nY + m_nY,
                                     (float)(a_nX + m_nX + m_nWidth),
                                     (float)(a_nY + m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorHighlight);

    }
    else
    {
        al_draw_filled_rounded_rectangle((float)a_nX + m_nX,
                                     (float)a_nY + m_nY,
                                     (float)(a_nX + m_nX + m_nWidth),
                                     (float)(a_nY + m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorInner);
    }

    al_draw_rounded_rectangle((float)a_nX + m_nX,
                                     (float)a_nY + m_nY,
                                     (float)(a_nX + m_nX + m_nWidth),
                                     (float)(a_nY + m_nY + m_nHeight),
                                     10, 10,
                                     m_pParent->m_ColorOuter, 2);


   if ((m_pParent!=NULL) && (m_pParent->m_pFont != NULL))
   {
      al_draw_text(m_pParent->m_pFont,
                   m_pParent->m_ColorOuter,
                   a_nX + m_nX + (m_nWidth/2), a_nY + m_nY + (m_nHeight/4),
                   ALLEGRO_ALIGN_CENTRE,
                   m_strCaption.c_str());

   }
}
