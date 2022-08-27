/*
   ___________________          _-_
   \==============_=_/ ____.---'---`---.____
               \_ \    \----._________.----/
                 \ \   /  /    `-_-'
             __,--`.`-'..'-_
            /____          ||
                 `--.____,-'



      Credits go to Paramount pictures for the star - trek concept and universe.


      Copyright (C) 2015 - 2020  E.J.M. Martens

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

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <string>


#ifndef _BUTTON
#define _BUTTON

class CWindowDialog;


class CButton
{
    private:
    int m_nX, m_nY;
    // Callback function
    void (* m_pCallBack)(CWindowDialog *);
    // Parent Menu
    CWindowDialog * m_pParent;
    // draw position
    int m_nWidth, m_nHeight;
    std::string m_strCaption;
    bool m_blHighlight;

    //ALLEGRO_FONT * m_pFont;

    protected:


    public:
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
    CButton(int a_nX, int a_nY, std::string a_strCaption, CWindowDialog * a_pParent, void (* a_pCallBack)(CWindowDialog *));
/** \brief
 *  Mouse event handler for menu item.
 * \param ALLEGRO_MOUSE_EVENT * mouse_event
 * a mouse event passed by allegro
 */
    void ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event,int a_nX, int a_nY);

/** \brief
 *  Draw this menu item
 * \param ALLEGRO_FONT * a_pFont
 * a pointer to a font to use for text
 */
   void Draw(int a_nX, int a_nY);
};





#endif
