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


#ifndef _WINDOW_DIALOG
#define _WINDOW_DIALOG

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <string>

#include "Button.h"
#include "SaveSlots.h"

enum DialogButtonOptions
{
    BTN_YESNO,
    BTN_YESNOCANCEL,
    BTN_OK
};

enum WDialogResult
{
    WDR_NONE,
    WDR_OK,
    WDR_YES,
    WDR_NO,
    WDR_CANCEL
};

class SlotManager;


class CWindowDialog
{
   friend class CButton;

private:
   CButton * m_pButtonYes;
   CButton * m_pButtonNo;
   CButton * m_pButtonCancel;

   SlotManager * m_pParent;

   ALLEGRO_COLOR m_ColorInner;
   ALLEGRO_COLOR m_ColorOuter;
   ALLEGRO_COLOR m_ColorHighlight;

   std::string m_strMessage;

   WDialogResult m_dialogResult;

   ALLEGRO_FONT * m_pFont;

   int m_nX, m_nY,m_nWidth,m_nHeight;

protected:

public:

 CWindowDialog(int a_nX, int a_nY, SlotManager * a_pParent, std::string a_strMessage, DialogButtonOptions a_Options);


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
   void Draw();

   void SetResult(WDialogResult a_Result);

   WDialogResult GetResult();

};


#endif
