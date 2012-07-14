#include "Sound.h"



Sound::Sound()
{
	mEngine = irrklang::createIrrKlangDevice();
	mMusic = NULL;
	mMusicSource = "none";
	// should read from a text file
	mMusicMuted = false;
	mEffectsMuted = false;
	mVolume = .5;
}

Sound::~Sound()
{
	if(mMusic != NULL)	{
		mMusic->stop();
		mMusic->drop();
	}
	mEngine->removeAllSoundSources();
	mEngine->drop(); 
}

irrklang::ISound* Sound::playMusic(string source, bool loop, bool track)
{
	irrklang::ISound *music;
	if(!mMusicMuted)	
	{
		if(mMusicSource != source)
		{
			if(mMusic != NULL)	{
				mMusic->stop();
				mMusic->drop();
				mMusic = NULL;
			}

			mMusicSource = source;
			mMusic = mEngine->play2D(mMusicSource.c_str(), loop, true, true);
			mMusic->setVolume(mVolume);
			mMusic->setIsPaused(false);
		}
	}

	return NULL;
}

irrklang::ISound* Sound::playEffect(string source, float volume)
{
	//irrklang::ISound *effect;
	if(!mEffectsMuted)	{
		irrklang::ISound* soundEffect = mEngine->play2D(source.c_str(), false, true, true);
		if(volume == -1)
			soundEffect->setVolume(mVolume);
		else 
			soundEffect->setVolume(volume);

		soundEffect->setIsPaused(false);
		//soundEffect->setVolume(mVolume);
	}

	return NULL;
}

void Sound::muteMusic(bool mute)
{
	if(mute && mMusic != NULL)	{
		mMusic->stop();
		mMusic->drop();
		mMusic = NULL;
		mMusicSource = "none";
	}

	mMusicMuted = mute;
}

void Sound::muteEffects(bool mute)
{
	mEffectsMuted = mute;
}

bool Sound::getMusicMuted(void)
{
	return mMusicMuted;
}

bool Sound::getEffectsMuted(void)
{
	return mEffectsMuted;
}

void Sound::stopMusic(bool stop)
{
	if(stop)
		mMusic->stop();
}

void Sound::setVolume(float volume)
{
	mVolume = volume;
	if(mMusic != NULL && !mMusicMuted)
		mMusic->setVolume(mVolume);
}