#include "sound.h"



Audio::Audio()
{
}
wchar_t* Audio::strToLPCWSTR(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	//delete[] buf;

	return buf;
	//LPCWSTR result = r.c_str();
	//return result;
}

const char* Audio::strToChar(const std::string& s) {
	const char *cstr = s.c_str();
	return cstr;
}

void Audio::addAudio(std::string file, std::string alias) {
	audiofiles.insert({ alias,file });
	std::string inputStr = "open " + file + " alias " + alias;
	mciSendString(strToChar(inputStr), NULL, 0, 0);

}
void Audio::pause(std::string alias) {
	std::string inputStr = "pause " + alias;
	mciSendString(strToChar(inputStr), NULL, 0, 0);
}

void Audio::resume(std::string alias) {
	std::string inputStr = "resume " + alias;
	mciSendString(strToChar(inputStr), NULL, 0, 0);
}

void Audio::play(std::string alias) {
	std::string inputStr = "play " + alias;


	mciSendString(strToChar(inputStr), NULL, 0, NULL);
}

void Audio::playRepeat(std::string alias) {
	std::string inputStr = "play " + alias + " repeat";


	mciSendString(strToChar(inputStr), NULL, 0, NULL);
}

void Audio::playAgain(std::string alias) {

	std::string inputStr = "play " + alias + " from 0";
	mciSendString(strToChar(inputStr), NULL, 0, NULL);
}

void Audio::stop(std::string alias) {
	std::string inputStr = "stop " + alias;
	mciSendString(strToChar(inputStr), NULL, 0, 0);
}

void Audio::close(std::string alias) {
	std::string inputStr = "close " + alias;
	mciSendString(strToChar(inputStr), NULL, 0, 0);
}

void Audio::volume(std::string alias, int volume) {
	std::string inputStr = "setaudio " + alias + " volume to " + std::to_string(volume);
	mciSendString(strToChar(inputStr), NULL, 0, 0);
}
