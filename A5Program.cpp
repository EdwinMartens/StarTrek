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


#include "A5Program.h"


A5Program::A5Program(int a_nScreenWidth, int a_nScreenheight, bool a_blFullscreen)
{
   m_blExit = false;

   if(!al_init())
   {
        throw A5Exception("failed to initialize allegro!");
   }

   al_init_font_addon();
   al_init_ttf_addon();

   if (!al_init_image_addon())
   {
        throw A5Exception("failed to initialize image add on!");
   }

   if (!al_init_primitives_addon())
   {
        throw A5Exception("failed to initialize primitives add on!");
   }

   if(!al_install_audio())
   {
       throw A5Exception("failed to initialize audio!");
   }

   if(!al_init_acodec_addon())
   {
       throw A5Exception("failed to initialize codec add on!");
   }

   if (!al_reserve_samples(5))
   {
       throw A5Exception("Could not reserve samples!");
   }

   if (a_blFullscreen)
   {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
   }
   else
   {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
   }

   m_pDisplay = al_create_display(a_nScreenWidth, a_nScreenheight);
   if (!m_pDisplay)
   {
        throw A5Exception("failed to create a display!");
   }

    m_pEventQueue = al_create_event_queue();
    if (!m_pEventQueue)
    {
        throw A5Exception("failed to create an event queue!");
    }

    if(!al_install_keyboard())
    {
        throw A5Exception("failed to install keyboard!");
    }

    if (!al_install_mouse())
    {
        throw A5Exception("failed to install mouse!");
    }


        // register the event sources
    al_register_event_source(m_pEventQueue, al_get_display_event_source(m_pDisplay));
    al_register_event_source(m_pEventQueue, al_get_keyboard_event_source());
    al_register_event_source(m_pEventQueue, al_get_mouse_event_source());

    m_dLastTime = al_get_time();
}




A5Program::~A5Program()
{
    al_destroy_display(m_pDisplay);
    al_destroy_event_queue(m_pEventQueue);
}



bool A5Program::HandleEvents()
{
    ALLEGRO_EVENT event;
     if (al_get_next_event(m_pEventQueue, &event))
        {
            switch (event.type)
            {
                case ALLEGRO_EVENT_KEY_DOWN:
                    OnKeyDown(event.keyboard);
                break;

                case ALLEGRO_EVENT_KEY_UP:
                    OnKeyUp(event.keyboard);
                break;

                case ALLEGRO_EVENT_MOUSE_AXES:
                     OnMouseMove(event.mouse);
                break;

                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                     OnMouseButtonDown(event.mouse);
                break;

                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                     OnMouseButtonUp(event.mouse);
                break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    m_blExit = true;
                break;

            }
       }
       else
       {
           double dDeltaTime = al_get_time() - m_dLastTime;
           m_dLastTime = al_get_time();
           OnUpdate(dDeltaTime);
           OnDraw();
           al_flip_display();
           al_clear_to_color(al_map_rgb(0,0,0));
       }


   return !m_blExit;
}

void A5Program::Quit()
{
    m_blExit = true;
}

void A5Program::OnKeyDown(const ALLEGRO_KEYBOARD_EVENT & a_KeyboardEvent)
{

}

void A5Program::OnKeyUp(const ALLEGRO_KEYBOARD_EVENT & a_KeyboardEvent)
{

}


void A5Program::OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{

}

void A5Program::OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{

}

void A5Program::OnMouseButtonUp(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{

}


void A5Program::OnUpdate(double a_dLagTime)
{



}


void A5Program::OnDraw()
{

}
