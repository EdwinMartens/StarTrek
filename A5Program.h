/*
Copyright (C) 2015  E.J.M. Martens

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

#ifndef A5_GAME_ENGINE
#define A5_GAME_ENGINE

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <exception>
#include <string>

using namespace std;

class A5Program
{
    public:
    A5Program(int a_nScreenWidth, int a_nScreenheight, bool a_blFullscreen);


    virtual ~A5Program();

    // handle all current events, returns false on program close
    bool HandleEvents();

    void Quit();

    inline ALLEGRO_DISPLAY * GetDisplay()
    {
        return m_pDisplay;
    }

    protected:

    // event handlers
    virtual void OnKeyDown(const ALLEGRO_KEYBOARD_EVENT & a_KeyboardEvent);
    virtual void OnKeyUp(const ALLEGRO_KEYBOARD_EVENT & a_KeyboardEvent);
    virtual void OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    virtual void OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    virtual void OnMouseButtonUp(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    virtual void OnDraw();
    virtual void OnUpdate(double a_dLagTime);


    private:

    double m_dLastTime;


    static A5Program * s_pProgram;

    // Render display
    ALLEGRO_DISPLAY     * m_pDisplay;

    // the event queue
    ALLEGRO_EVENT_QUEUE * m_pEventQueue;

    // game timer
    ALLEGRO_TIMER       * m_pTimer;

    // Shutdown game engine ?
    bool m_blExit;



};

class A5Exception: public exception
{
  private:
  string m_strError;

  public:

  A5Exception(string a_strError)
  {
     m_strError = a_strError;
  }

  virtual const char* what() const throw()
  {
    return m_strError.c_str();
  }

  virtual ~A5Exception() throw() { }
};



#endif // A5_GAME_ENGINE
