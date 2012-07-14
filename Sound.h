#pragma once

#include <string>
#include "C:\Program Files (x86)\irrKlang-1.3.0\include\irrKlang.h"
#pragma comment(lib, "C:\\Program Files (x86)\\irrKlang-1.3.0\\lib\\Win32-visualStudio\\irrKlang.lib")
using namespace std;

class Sound
{
public:
	Sound();
	~Sound();

	irrklang::ISound* playMusic(string source, bool loop, bool track);
	irrklang::ISound* playEffect(string source, float volume = -1);	// - 1 = use mVolume
	void stopMusic(bool stop);

	void muteMusic(bool mute);
	void muteEffects(bool mute);

	bool getMusicMuted(void);
	bool getEffectsMuted(void);

	void setVolume(float volume);
private:
	bool mMusicMuted;
	bool mEffectsMuted;
	float mVolume;

	irrklang::ISoundEngine* mEngine;
	irrklang::ISound* mMusic;
	std::string mMusicSource;
};

extern Sound* gSound;