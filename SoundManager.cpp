#include <stdio.h>
#include "SoundManager.h"


#define MaxSimultaneousPhasers 10

ALLEGRO_PATH   * g_pSoundPath = NULL;
vector<ALLEGRO_SAMPLE *> g_vSample;

ALLEGRO_SAMPLE_ID phaserSampleID[MaxSimultaneousPhasers];
int nPhaserIndex =0;

namespace SoundManager
{

void Init_SoundManager(int a_nNumberOfSamples)
{
    al_install_audio();
	al_init_acodec_addon();

    g_pSoundPath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(g_pSoundPath, "sound");

    if (!al_reserve_samples(a_nNumberOfSamples))
    {
        throw SoundManager::SoundManagerException("Could not Reserve samples");
    }


}


void DeInit_SoundManager()
{
    for (size_t i=0; i< g_vSample.size();i++)
    {
        al_destroy_sample(g_vSample[i]);
    }
    g_vSample.clear();
    al_uninstall_audio();
}


void LoadSounds()
{
    for (int i=0;i < SOUND::SND_END;i++)
	{
        char szFileName[255];
		sprintf(szFileName,"%04d.wav",i+1);
		al_set_path_filename(g_pSoundPath,szFileName);
		ALLEGRO_SAMPLE * pSound = NULL;
		pSound  = al_load_sample(al_path_cstr(g_pSoundPath,ALLEGRO_NATIVE_PATH_SEP));

		if (pSound == NULL)
		{
		    string strError = "Wrong Sample ! \n";
		    strError.append(szFileName);
		    throw SoundManager::SoundManagerException(strError);
		}
		else
        {
            g_vSample.push_back(pSound);
        }
    }
}


void PlaySound(SOUND::SOUNDNAME a_Name,double a_dVolume )
{
    al_play_sample( g_vSample[a_Name],
                    a_dVolume,
                    ALLEGRO_AUDIO_PAN_NONE,
                    1.2,
                    ALLEGRO_PLAYMODE_ONCE,
                    NULL);

}


bool LoopSound(SOUND::SOUNDNAME a_Name,double a_dVolume, ALLEGRO_SAMPLE_ID * a_rID )
{
    return al_play_sample(g_vSample[a_Name], a_dVolume, ALLEGRO_AUDIO_PAN_NONE, 1.0,ALLEGRO_PLAYMODE_LOOP, a_rID);
}


void StopSound(ALLEGRO_SAMPLE_ID * a_ID)
{
     al_stop_sample(a_ID);
}


ALLEGRO_SAMPLE_ID * GetFreePhaserAddress()
{
    StopSound(&phaserSampleID[nPhaserIndex]);
    return &phaserSampleID[nPhaserIndex];
    nPhaserIndex++;
    if (nPhaserIndex > MaxSimultaneousPhasers)
    {
        nPhaserIndex = 0;
        StopSound(&phaserSampleID[nPhaserIndex]);
    }
}


}



