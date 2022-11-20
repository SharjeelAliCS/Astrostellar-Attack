/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a sound class

 */

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

/*
This class was taken from our COMP 2501 project winter 2019 last year. The code was entirely copy/pasted. As such, it is not a "part" of the work for this game and COMP 3501, aside from polish points. 

Links: 
https://docs.microsoft.com/en-gb/windows/desktop/Multimedia/setaudio
https://github.com/SharjeelAliBCS/Assault-On-Terra-RTS-game/blob/master/Comp2501-master/include/Audio.h
*/

class Audio
{
public:
	Audio();
	/*Input:       s (string)
	  Output:      const char
	  Description: Converts a string into a constant char
	*/
	const char* strToChar(const std::string& s);

	/*Input:       s (string)
	  Output:      whchar_t
	  Description: Converts a string into a whar_t
	*/
	wchar_t* strToLPCWSTR(const std::string& s);

	/*Input:       file (filename), alias (sound name)
	  Output:      void
	  Description: Adds the audio in filename and saves it to alias
	*/
	void addAudio(std::string file, std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: Pauses the track at alias
	*/
	void pause(std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: Pauses the track at alias
	*/
	void resume(std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: resumes the track at alias
	*/
	void play(std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: plays the track at alias
	*/
	void playRepeat(std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: plays the track at alias repeately
	*/
	void playAgain(std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: plays the track at alias again
	*/
	void stop(std::string alias);

	/*Input:       alias (sound name)
	  Output:      void
	  Description: Stops the track at alias
	*/
	void close(std::string alias);

	/*Input:       alias (sound name), volume (volume of sound)
	  Output:      void
	  Description: Changes track volume
	*/
	void volume(std::string alias, int volume);


private:

	//stores the audio tracks
	std::map < std::string, std::string> audiofiles;
};
#endif