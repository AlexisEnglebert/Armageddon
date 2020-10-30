#include "KeyboardEventManager.h"

KeyboardEvent::KeyboardEvent() : type(EventType::invalid), key(0u)
{

}

KeyboardEvent::KeyboardEvent(EventType type, const unsigned char key) : type(type),key(key)
{

}

bool KeyboardEvent::Ispressed()
{
	return this->type == EventType::KeyPressed;
}

bool KeyboardEvent::IsReleased()
{
	return this->type == EventType::KeyReleased;
}

bool KeyboardEvent::IsInvalid()
{
	return this->type == EventType::invalid;
}
