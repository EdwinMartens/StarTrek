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


#ifndef _FONTMANAGER
#define _FONTMANAGER

#include <vector>
#include <list>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;


namespace FONT
{
    enum FONTNAME
    {
#ifdef _DEBUG
        DBG = 0,
        MENU,
#else
        MENU =0,
#endif // _DEBUG

        SMALL,
        INFO,
        TEXT,
        KEYS,
        FNT_END
    };

}

namespace FontManager
{

void Init_FontManager();
void DeInit_FontManager();
void LoadFonts();
ALLEGRO_FONT * GetFont(FONT::FONTNAME a_Name);

class FontManagerException: public exception
{
  private:
  string m_strError;

  public:

  FontManagerException(string a_strError)
  {
     m_strError = a_strError;
  }

  virtual const char* what() const throw()
  {
    return m_strError.c_str();
  }

  virtual ~FontManagerException() throw() { }
};

}


#endif // _SOUNDMANAGER
