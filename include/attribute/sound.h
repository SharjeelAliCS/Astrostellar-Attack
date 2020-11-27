#ifndef SOUND_H_
#define SOUND_H_
#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include <map>

#pragma comment(lib, "Winmm.lib")

//taken from 2501 project found in the 2nd link. The code below was NOT changed at all
//and simply copy/pasted. This means we intend to not get any marks for adding audio
//into the game as it is not our work for comp3501. 
//https://docs.microsoft.com/en-gb/windows/desktop/Multimedia/setaudio
//https://github.com/SharjeelAliBCS/Assault-On-Terra-RTS-game/blob/master/Comp2501-master/include/Audio.h

class Audio
{
public:
	Audio();

	const char* strToChar(const std::string& s);
	wchar_t* strToLPCWSTR(const std::string& s);
	void addAudio(std::string file, std::string alias);
	void pause(std::string alias);
	void resume(std::string alias);
	void play(std::string alias);
	void playRepeat(std::string alias);
	void playAgain(std::string alias);
	void stop(std::string alias);
	void close(std::string alias);
	void volume(std::string alias, int volume);


private:
	std::map < std::string, std::string> audiofiles;
};
#endif