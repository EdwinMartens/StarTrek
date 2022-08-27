#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "utils.h"

ALLEGRO_PATH * g_pLogPath = NULL;
std::ofstream logfile;

int screenshot(const char *destination_path, const char *gamename)
{
	ALLEGRO_PATH *path;
	char *filename, *filename_wp;
	struct tm *tmp;
	time_t t;
	unsigned int i;
	const char *path_cstr;

	if(!destination_path)
		path = al_get_standard_path(ALLEGRO_USER_DOCUMENTS_PATH);
	else
		path = al_create_path_for_directory(destination_path);

	if(!path)
		return -1;

	if(!gamename) {
		if( !(gamename = al_get_app_name()) ) {
			al_destroy_path(path);
			return -2;
		}
	}

	t = time(0);
	tmp = localtime(&t);
	if(!tmp) {
		al_destroy_path(path);
		return -3;
	}

	// Length of gamename + length of "-YYYYMMDD" + length of maximum [a-z] + NULL terminator
	if ( !(filename_wp = filename = (char *)malloc(strlen(gamename) + 9 + 2 + 1)) ) {
		al_destroy_path(path);
		return -4;
	}

	strcpy(filename, gamename);
	// Skip to the '.' in the filename, or the end.
	for(; *filename_wp != '.' && *filename_wp != 0; ++filename_wp);

	*filename_wp++ = '-';
	if(strftime(filename_wp, 9, "%Y%m%d", tmp) != 8) {
		free(filename);
		al_destroy_path(path);
		return -5;
	}
	filename_wp += 8;

	for(i = 0; i < 26*26; ++i) {
		if(i > 25) {
			filename_wp[0] = (i / 26) + 'a';
			filename_wp[1] = (i % 26) + 'a';
			filename_wp[2] = 0;
		}
		else {
			filename_wp[0] = (i % 26) + 'a';
			filename_wp[1] = 0;
		}

		al_set_path_filename(path, filename);
		al_set_path_extension(path, ".png");
		path_cstr = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

		if (al_filename_exists(path_cstr))
			continue;

		al_save_bitmap(path_cstr, al_get_target_bitmap());
		free(filename);
		al_destroy_path(path);
		return 0;
	}

	free(filename);
	al_destroy_path(path);

	return -6;
}



void InitLog()
{
    g_pLogPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(g_pLogPath, "Startrek.LOG");
    logfile.open(al_path_cstr(g_pLogPath,ALLEGRO_NATIVE_PATH_SEP));
}

void Log(const std::string a_strLog)
{
   logfile << a_strLog << "\n";
   logfile.flush();
}

const char * IDToString(ID a_id)
{
    switch (a_id)
    {
        case ID_NONE:
            return "NONE";
        break;

        case ID_ANIMATION:
            return "ANIMATION";
        break;

        case ID_CLASS_A:
            return "A PLANET";
        break;

	    case ID_CLASS_B:
	        return "B PLANET";
        break;

	    case ID_CLASS_L:
	        return "L PLANET";
        break;

	    case ID_CLASS_N:
	        return "N PLANET";
        break;

	    case ID_CLASS_M1:
	        return "M1 PLANET";
        break;

        case ID_CLASS_M2:
            return "M2 PLANET";
        break;

        case ID_CLASS_M3:
            return "M3 PLANET";
        break;

        case ID_ASTEROID:
            return "ASTEROID";
        break;

        case ID_STAR_BLUE:
            return "BLUE STAR";
        break;

	    case ID_STAR_WHITE:
	        return "WHITE STAR";
        break;

	    case ID_STAR_YELLOW:
	        return "YELLOW STAR";
        break;

	    case ID_STAR_RED:
	        return "RED STAR";
        break;

	    case ID_BLACK_HOLE:
            return "BLACK HOLE";
        break;

	    case ID_PROBE:
	        return "PROBE";
        break;

        case ID_DISRUPTOR:
            return "DISRUPTOR";
        break;

        case ID_PHOTON:
            return "PHOTON";
        break;

        case ID_PROTON:
            return "PROTON";
        break;

        case ID_PLASMA:
            return "PLASMA";
        break;

        case ID_GALAXYCLASS:
            return "GALAXY CLASS";
        break;

        case ID_PLAYER:
            return "PLAYER";
        break;

        case ID_KLINGONBC:
            return "KLINGON BC";
        break;

        case ID_KLINGONBOP:
            return "KLINGON BOP";
        break;

        case ID_ROMULANBOP:
            return "ROMULAN BOP";
        break;

        case ID_FEDERATIONBASE:
            return "FEDERATION BASE";
        break;

        case ID_KLINGON_BASE:
            return "KLINGON BASE";
        break;

        case ID_ROMULAN_BASE:
            return "ROMULAN BASE";
        break;


        default:
            return "UNKNOWN";

    }
}






std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

