#include "KeyboardClass.h"


KeyboardClass::KeyboardClass()
{

	for (int i = 0; i < 256; i++)
	{
	//	Armageddon::Log::GetLogger()->trace("Init KeyState {0}", i);
		//OutputDebugStringA("Initialise KeyState: {0}" +i);
		this->keystate[i] = false; //Initialize all key states to off (false)
	}

}
bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
	return this->keystate[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
	return this->keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
	return this->charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if (this->keyBuffer.empty()) //If no keys to be read?
	{
		return KeyboardEvent(); //return empty keyboard event
	}
	else
	{
		KeyboardEvent e = this->keyBuffer.front(); //Get first Keyboard Event from queue
		this->keyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

unsigned char KeyboardClass::ReadChar()
{
	if (this->charBuffer.empty()) //If no keys to be read?
	{
		return 0u; //return 0 (NULL char)
	}
	else
	{
		unsigned char e = this->charBuffer.front(); //Get first char from queue
		this->charBuffer.pop(); //Remove first char from queue
		return e; //Returns char
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{

	this->keystate[key] = true;
	this->keyBuffer.push(KeyboardEvent(EventType::KeyPressed, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	this->keystate[key] = false;
	this->keyBuffer.push(KeyboardEvent(EventType::KeyReleased, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
//	OutputDebugStringA("Char");
	this->charBuffer.push(key);
}