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

#ifndef _MENU
#define _MENU

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <string>
#include <vector>

using namespace std;

class TMenu;


/**
 * \class TMenuItem
 * TMenuItem is a clickable menuItem that calls a given callback function when clicked
 *
 */
class TMenuItem
{
    friend class TMenu;
    private:
    int m_nX, m_nY;
    // Callback function
    void (* m_pCallBack)();
    // Parent Menu
    TMenu * m_pParent;
    // draw position
    int m_nWidth, m_nHeight;
    string m_strCaption;
    bool m_blHighlight;

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
    TMenuItem (int a_nX, int a_nY, int a_nWidth, int a_nHeight, TMenu * a_pParent,void (* a_pCallBack)());
/** \brief
 *  Mouse event handler for menu item.
 * \param ALLEGRO_MOUSE_EVENT * mouse_event
 * a mouse event passed by allegro
 */
    void ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event);

/** \brief
 *  Draw this menu item
 * \param ALLEGRO_FONT * a_pFont
 * a pointer to a font to use for text
 */
   void Draw(ALLEGRO_FONT * a_pFont);
};



/**
 * \class TMenu
 *  Container for menu items
 *
 */
class TMenu
{
    friend class TMenuItem;
    private:
    ALLEGRO_COLOR m_ColorInner;
    ALLEGRO_COLOR m_ColorOuter;
    ALLEGRO_COLOR m_ColorHighlight;
    int m_nIndex;
    vector <TMenuItem> m_vMenuItems;
    protected:

    public:
    TMenu();
    ~TMenu();
    void ClearItems();
    void AddMenuItem(int a_nBasePositionX,string a_strCaption,void (* a_pCallBack)());
    void DeleteLast();
    void ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event);
    void Draw(ALLEGRO_FONT * a_pFont);
    int Count();

};



#endif
