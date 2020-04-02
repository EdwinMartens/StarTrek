#include <stdio.h>
#include "FontManager.h"


ALLEGRO_PATH   * g_pFontPath = NULL;
vector<ALLEGRO_FONT *> g_vFont;

namespace FontManager
{

void Init_FontManager()
{
    g_pFontPath  = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(g_pFontPath, "font");
    al_init_font_addon();
	al_init_ttf_addon();
}


void DeInit_FontManager()
{
    for (size_t i=0; i< g_vFont.size();i++)
    {
        al_destroy_font(g_vFont[i]);
    }
    g_vFont.clear();
}


void LoadFonts()
{
    ALLEGRO_FONT * pFont = NULL;
#ifdef _DEBUG
    al_set_path_filename(g_pFontPath,"1.ttf");
	pFont = al_load_ttf_font(al_path_cstr(g_pFontPath,ALLEGRO_NATIVE_PATH_SEP),10,0);
	if (pFont == NULL )
    {
        string strError = "Could not load _DEBUG font";
        throw FontManager::FontManagerException(strError);
    }
    else
    {
        g_vFont.push_back(pFont);
    }
#endif // _DEBUG

    pFont = NULL;
    al_set_path_filename(g_pFontPath,"font.ttf");
	pFont = al_load_ttf_font(al_path_cstr(g_pFontPath,ALLEGRO_NATIVE_PATH_SEP),40,0);
	if (pFont == NULL )
    {
        string strError = "Could not load menu font";
        throw FontManager::FontManagerException(strError);
    }
    else
    {
        g_vFont.push_back(pFont);
    }

    pFont = NULL;
    pFont = al_load_ttf_font(al_path_cstr(g_pFontPath,ALLEGRO_NATIVE_PATH_SEP),15,ALLEGRO_TTF_MONOCHROME);
	if (pFont == NULL )
    {
        string strError = "Could not load menu font";
        throw FontManager::FontManagerException(strError);
    }
    else
    {
        g_vFont.push_back(pFont);
    }

    pFont = NULL;
    pFont = al_load_ttf_font(al_path_cstr(g_pFontPath,ALLEGRO_NATIVE_PATH_SEP),15,0);
	if (pFont == NULL )
    {
        string strError = "Could not load menu font";
        throw FontManager::FontManagerException(strError);
    }
    else
    {
        g_vFont.push_back(pFont);
    }

    pFont = NULL;
    pFont = al_load_ttf_font(al_path_cstr(g_pFontPath,ALLEGRO_NATIVE_PATH_SEP),25,0);
	if (pFont == NULL )
    {
        string strError = "Could not load menu font";
        throw FontManager::FontManagerException(strError);
    }
    else
    {
        g_vFont.push_back(pFont);
    }

    pFont = NULL;
    pFont = al_load_ttf_font(al_path_cstr(g_pFontPath,ALLEGRO_NATIVE_PATH_SEP),18,0);
	if (pFont == NULL )
    {
        string strError = "Could not load menu font";
        throw FontManager::FontManagerException(strError);
    }
    else
    {
        g_vFont.push_back(pFont);
    }
}

ALLEGRO_FONT * GetFont(FONT::FONTNAME a_Name)
{
    return g_vFont[a_Name];
}

}




