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

#include <stdlib.h>
#include <time.h>
#include "CustomProgram.h"



CustomProgram * g_pProgram = NULL;




// Main function
int main(int argc, const char * argv[])
{
  srand (time(NULL));
  g_pProgram = new CustomProgram(1024,768,true);
  while (g_pProgram->HandleEvents())
  {




  }

  delete g_pProgram;
  return 0;
}





