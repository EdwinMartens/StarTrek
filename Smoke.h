//
// ___.--------._____________,------' -----.`----._
// \                           `  -  .  _         /\
//  `.__________                           `  -  |  |                 __,---"-._
//              `-----------.______             ,'_/  ________,------'___________`----.___________
//                                 `-----.____,'      \===========================================/
//                                       |   :|        >--------------.----------.---------------'
//                                       /   :|   _,--'          ,--'  `--.__.--'
//                                      /   : |--'______________/_
//                                ,---,'    :  \__________________`--.
//                               `__________                        |/|
//                                          `------._               |\|
//                                                    `--._________,-'
//
//
//
//      Credits go to Paramount pictures for the star - trek concept and universe.


//      Copyright (C) 2015 - 2016  E.J.M. Martens
//
//      This program is free software; you can redistribute it and/or
//      modify it under the terms of the GNU General Public License
//      as published by the Free Software Foundation; either version 2
//      of the License, or (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


#ifndef _SMOKE
#define _SMOKE


#include <vector>
#include "types.h"
#include "Engine.h"

class TSmoke:public TSprite
{
    friend class TEngine;
private:

	int m_nFrame;
	int m_nWait;

public:
	TSmoke();
	void Do_ai();
	static bool Init();
	static void DeInit();
};



#endif
