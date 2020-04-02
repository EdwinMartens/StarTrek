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

#include "CustomProgram.h"
#include <allegro5/allegro_primitives.h>



CustomProgram::CustomProgram(int a_nScreenWidth, int a_nScreenheight, bool a_blFullscreen)
:A5Program(a_nScreenWidth, a_nScreenheight, a_blFullscreen)
{
    m_pParalax = new Paralax(a_nScreenWidth, a_nScreenheight,300,5);
    //m_pGameEngine = new GameEngine(a_nScreenWidth, a_nScreenheight);




}


CustomProgram::~CustomProgram()
{
    delete m_pParalax;
    //delete m_pGameEngine;


}




void CustomProgram::OnKeyDown(const ALLEGRO_KEYBOARD_EVENT & a_KeyboardEvent)
{
    switch(a_KeyboardEvent.keycode)
    {
        case ALLEGRO_KEY_ESCAPE:
               Quit();
             break;
    }


}

void CustomProgram::OnKeyUp(const ALLEGRO_KEYBOARD_EVENT & a_KeyboardEvent)
{

}


void CustomProgram::OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{



}

void CustomProgram::OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{

}

void CustomProgram::OnMouseButtonUp(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{

}




void CustomProgram::OnUpdate(double a_dLagTime)
{




    //m_pGameEngine->Update(a_dLagTime);

}



void CustomProgram::OnDraw()
{
    m_pParalax->Draw(GetDisplay());
    //m_pGameEngine->draw();
}
