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


#ifndef _SOUNDMANAGER
#define _SOUNDMANAGER

#include <vector>
#include <list>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;


namespace SOUND
{
    enum SOUNDNAME
    {
        PHOTON = 0,
        EXPLOSION_SMALL,
        EXPLOSION_MED,
        PHASER,
        INTERCOM,
        ON,
        OFF,
        TRANSPORTER1 = 8,
        TRANSPORTER2 = 9,
        DISRUPTOR = 11,
        CLOACK,
        DECLOACK,
        ROMULAN_BEAM,
        SND_END
    };

}

namespace SoundManager
{

void Init_SoundManager(int a_nNumberOfSamples);
void DeInit_SoundManager();
void LoadSounds();
void PlaySound(SOUND::SOUNDNAME a_Name,double a_dVolume );
bool LoopSound(SOUND::SOUNDNAME a_Name,double a_dVolume, ALLEGRO_SAMPLE_ID * a_rID );
void StopSound(ALLEGRO_SAMPLE_ID * a_rID);
ALLEGRO_SAMPLE_ID * GetFreePhaserAddress();
void ReleasePhaserAddress(ALLEGRO_SAMPLE_ID * a_pPhaserAddress);


class SoundManagerException: public exception
{
  private:
  string m_strError;

  public:

  SoundManagerException(string a_strError)
  {
     m_strError = a_strError;
  }

  virtual const char* what() const throw()
  {
    return m_strError.c_str();
  }

  virtual ~SoundManagerException() throw() { }
};

}


#endif // _SOUNDMANAGER

