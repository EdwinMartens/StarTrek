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

#ifndef _SAVE_SLOTS
#define _SAVE_SLOTS

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <string>
#include <vector>
#include "WindowDialog.h"


using namespace std;


class SlotManager;


class Slot
{
friend class SlotManager;
private:
string m_strCaption;
bool   m_blActive;
bool   m_blNewSave;
bool   m_blHighLight;
//int    m_nIndex;
int    m_nX;
int    m_nY;
int    m_nWidth;
int    m_nHeight;
SlotManager * m_pParent;

public:

  Slot(SlotManager * a_pParent);
  Slot(int a_nX, int a_nY,const string a_strCaption, SlotManager * a_pParent);
  ~Slot();
  string GetCaption();
  void Draw(ALLEGRO_FONT * a_pFont);
  void ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event);
  void ProcessKeyEvent(int a_nUniChar);
};


class SlotManager
{
    friend class Slot;
    private:
    // Callback function
    void (* m_pCallBack)(string);

    CWindowDialog * m_pWindowDialog = NULL;


    Slot * m_pActiveSlot;
    Slot * m_pPendingActive;
    ALLEGRO_COLOR m_ColorInner;
    ALLEGRO_COLOR m_ColorOuter;
    ALLEGRO_COLOR m_ColorHighlight;
    int m_nIndex;
    bool m_blLoading;
    vector <Slot> m_vSlots;
    protected:

    public:
    SlotManager(bool a_blLoading, ALLEGRO_PATH * a_pSearchpath,void (* a_pCallBack)(string));
    ~SlotManager();
    void DeactivateSlots();
    void AddSlot(string a_strCaption);
    void DeleteSlot(int a_nIndex);
    void ProcessMouseEvent(const ALLEGRO_MOUSE_EVENT & mouse_event);
    void ProcessKeyEvent(int a_nUniChar);
    void Draw(ALLEGRO_FONT * a_pFont);

    void CloseDialog();
    int Count();

};


#endif // _SAVE_SLOTS

