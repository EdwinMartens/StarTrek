#include "KeyMapper.h"

TKeyMapper::TKeyMapper(ALLEGRO_COLOR a_ColorMapped, ALLEGRO_COLOR a_ColorMapping,ALLEGRO_FONT * a_pFont)
{
    m_ColorMapped  = a_ColorMapped;
    m_ColorMapping = a_ColorMapping;
    m_pFont        = a_pFont;

    m_nMapping = 0;
    SetDefaultMapping();
}

void TKeyMapper::SetDefaultMapping()
{

    m_KeyMap[UP]                    = ALLEGRO_KEY_UP;
    m_KeyMap[DOWN]                  = ALLEGRO_KEY_DOWN;
    m_KeyMap[LEFT]                  = ALLEGRO_KEY_LEFT;
    m_KeyMap[RIGHT]                 = ALLEGRO_KEY_RIGHT;
    m_KeyMap[PROBE]                 = ALLEGRO_KEY_P;
    m_KeyMap[SHIELD]                = ALLEGRO_KEY_S;
    m_KeyMap[PHOTON]                = ALLEGRO_KEY_TAB;
    m_KeyMap[TARGET_UP]             = ALLEGRO_KEY_PGDN;
    m_KeyMap[TARGET_DOWN]           = ALLEGRO_KEY_PGUP;
    m_KeyMap[PHASER]                = ALLEGRO_KEY_SPACE;
    m_KeyMap[PHOTON_BACK]           = ALLEGRO_KEY_LSHIFT;
    m_KeyMap[SEEK_TARGET]           = ALLEGRO_KEY_T;
    m_KeyMap[SEEK_FRIEND]           = ALLEGRO_KEY_F;
    m_KeyMap[DOCK]                  = ALLEGRO_KEY_D;
    m_KeyMap[RELEASE]               = ALLEGRO_KEY_R;
    m_KeyMap[SCREEN_MAIN]           = ALLEGRO_KEY_F1;
    m_KeyMap[SCREEN_ENGINEERING]    = ALLEGRO_KEY_F2;
    m_KeyMap[SCREEN_NAVIGATION]     = ALLEGRO_KEY_F3;
    m_KeyMap[SCREEN_COM_TRANS]      = ALLEGRO_KEY_F4;
    m_KeyMap[WARP_1]                = ALLEGRO_KEY_1;
    m_KeyMap[WARP_2]                = ALLEGRO_KEY_2;
    m_KeyMap[WARP_3]                = ALLEGRO_KEY_3;
    m_KeyMap[WARP_4]                = ALLEGRO_KEY_4;
    m_KeyMap[WARP_5]                = ALLEGRO_KEY_5;
    m_KeyMap[WARP_0]                = ALLEGRO_KEY_TILDE;

    m_KeyMap[NAVIGATION_VIEW_TOPO]       = ALLEGRO_KEY_F5;
    m_KeyMap[NAVIGATION_VIEW_TACTICAL]   = ALLEGRO_KEY_F6;
    m_KeyMap[NAVIGATION_VIEW_OCCUPATION] = ALLEGRO_KEY_F7;
}


bool TKeyMapper::MapKey(int a_nKey)
{
    if (a_nKey!=ALLEGRO_KEY_ESCAPE)
    {
        if (m_nMapping < KEY_MAX)
        {
            m_KeyMap[m_nMapping] = a_nKey;
            m_nMapping++;
            return true;
        }
        else
        {
            m_nMapping = 0;
            return false;
        }
    }
    else
    {
        m_nMapping = 0;
        return false;
    }

}

void TKeyMapper::DrawMapping()
{
        string strMessage;
        string strKey;
        switch(m_nMapping)
        {
             case UP:
                  strMessage = "Press a key to speed up";
                  break;

             case DOWN:
                  strMessage = "Press a key to slow down";
                  break;

             case LEFT:
                  strMessage = "Press a key to turn left";
                  break;

             case RIGHT:
                  strMessage = "Press a key to turn right";
                  break;

             case PROBE:
                  strMessage = "Press a key to launch a probe";
                  break;

             case SHIELD:
                  strMessage ="Press a key to toggle shield";
                  break;

             case PHOTON:
                  strMessage = "Press a key to fire forward photon tube";
                  break;

             case PHOTON_BACK:
                  strMessage = "Press a key to fire backward photon tube";
                  break;

             case PHASER:
                  strMessage = "Press a key to fire phaser";
                  break;

             case SEEK_TARGET:
                  strMessage = "Press a key to Target closest enemy";
                  break;

             case SEEK_FRIEND:
                  strMessage = "Press a key to select closest friend";
                  break;

             case TARGET_UP:
                  strMessage = "Press a key to target next ship part";
                  break;

             case TARGET_DOWN:
                  strMessage = "Press a key to target previous ship part";
                  break;

             case DOCK:
                  strMessage = "Press a key to initiate docking procedure";
                  break;

             case RELEASE:
                  strMessage = "Press a key to release dock";
                  break;

             case SCREEN_MAIN:
                  strMessage = "Press a key to access main screen";
                  break;

             case SCREEN_ENGINEERING:
                  strMessage = "Press a key to access engineering screen";
                  break;


             case SCREEN_NAVIGATION:
                  strMessage = "Press a key to access navigation screen";
                  break;

             case SCREEN_COM_TRANS:
                  strMessage = "Press a key to access Communications/Transporter";
                  break;

             case NAVIGATION_VIEW_TOPO:
                  strMessage = "Press a key to view topographical info when in navigation screen";
                  break;

             case NAVIGATION_VIEW_TACTICAL:
                  strMessage = "Press a key to view tactical info when in navigation screen";
                  break;

             case NAVIGATION_VIEW_OCCUPATION:
                  strMessage = "Press a key to view starbases and empires when in navigation screen";
                  break;

             case WARP_1:
                  strMessage = "Press a key to initiate WARP 1";
                  break;

             case WARP_2:
                  strMessage = "Press a key to initiate WARP 2";
                  break;

             case WARP_3:
                  strMessage = "Press a key to initiate WARP 3";
                  break;

             case WARP_4:
                  strMessage = "Press a key to initiate WARP 4";
                  break;

             case WARP_5:
                  strMessage = "Press a key to initiate WARP 5";
                  break;

             case WARP_0:
                  strMessage = "Press a key to drop out of warp";
                  break;




        }

    for (int i = 0;i < KEY_MAX; i++)
    {
         switch(i)
         {
             case UP:
                  strKey = "Speed up";
                  break;

             case DOWN:
                  strKey = "Slow down";
                  break;

             case LEFT:
                  strKey = "Turn left";
                  break;

             case RIGHT:
                  strKey = "Turn right";
                  break;

             case PROBE:
                  strKey = "Launch Probe";
                  break;

             case SHIELD:
                  strKey = "Toggle Shield";
                  break;


             case PHOTON:
                  strKey = "Fire forward photon tube";
                  break;

             case PHOTON_BACK:
                  strKey = "Fire backward photon tube";
                  break;

             case PHASER:
                  strKey = "Fire phaser";
                  break;

             case SEEK_TARGET:
                  strKey = "Target closest enemy";
                  break;

             case SEEK_FRIEND:
                  strKey = "Select closest friend";
                  break;

             case TARGET_UP:
                  strKey = "Target next ship part";
                  break;

             case TARGET_DOWN:
                  strKey = "Target previous ship part";
                  break;

             case DOCK:
                  strKey = "Initiate docking procedure";
                  break;

             case RELEASE:
                  strKey = "Release dock";
                  break;

             case SCREEN_MAIN:
                  strKey = "Access main screen";
                  break;

             case SCREEN_ENGINEERING:
                  strKey = "Access engineering screen";
                  break;

              case SCREEN_NAVIGATION:
                  strKey = "Access navigation screen";
                  break;

              case SCREEN_COM_TRANS:
                  strKey = "Access Communication/Transporter";
                  break;

             case NAVIGATION_VIEW_TOPO:
                  strKey = "View topographical info when in navigation screen";
                  break;

             case NAVIGATION_VIEW_TACTICAL:
                  strKey = "View tactical info when in navigation screen";
                  break;

             case NAVIGATION_VIEW_OCCUPATION:
                  strKey = "View starbases and empires when in navigation screen";
                  break;


             case WARP_1:
                  strKey = "Initiate WARP 1";
                  break;

             case WARP_2:
                  strKey = "Initiate WARP 2";
                  break;

             case WARP_3:
                  strKey = "Initiate WARP 3";
                  break;

             case WARP_4:
                  strKey = "Initiate WARP 4";
                  break;

             case WARP_5:
                  strKey = "Initiate WARP 5";
                  break;

             case WARP_0:
                  strKey = "Drop out of warp";
                  break;


         }

         if (i==m_nMapping)
         {
             al_draw_text(m_pFont,m_ColorMapping, 50, 20 + 22*i, 0,strMessage.c_str());
         }
         else
         {
            al_draw_text(m_pFont,m_ColorMapped, 50, 20 + 22*i, 0,strKey.c_str());
            al_draw_text(m_pFont,m_ColorMapped, 550, 20 + 22*i, 0,al_keycode_to_name(m_KeyMap[i]));
         }

    }
    al_draw_text(m_pFont,m_ColorMapped, 750,5 , 0,"Press [Esc] to save and quit");
}

bool TKeyMapper::IsKey(int a_nKeyNr,KEYS a_MappedKey)
{
    return (a_nKeyNr == m_KeyMap[a_MappedKey]);
}


void TKeyMapper::Save(std::ofstream & a_SaveStream)
{
    for (int i = 0;i < KEY_MAX; i++)
    {
        a_SaveStream << m_KeyMap[i] << ",";
    }

}

void TKeyMapper::Load(std::ifstream & a_LoadStream)
{
    char n;
    for (int i = 0;i < KEY_MAX; i++)
    {
         a_LoadStream >> m_KeyMap[i];
         a_LoadStream >> n;
    }
}

























