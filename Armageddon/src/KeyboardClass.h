#pragma once
#include"Events/KeyboardEventManager.h"
#include <queue>
#include "Log.h"
#include "Macros.h"
class DLL KeyboardClass
{

public:
	KeyboardClass();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
private:
	bool autoRepeatKey = false;
	bool autoRepeatChar = false;
	bool keystate[256]; 
	std::queue<KeyboardEvent> keyBuffer; // on crée un buffeur qui vas stock  l'event
	std::queue<unsigned char> charBuffer; // on crée un buffeur qui vas stock le caractère

};