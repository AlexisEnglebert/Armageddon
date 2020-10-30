#pragma once
#include "Event.h"
#include "../Macros.h"
class DLL KeyboardEvent : public Event
{
public:

	KeyboardEvent();
	KeyboardEvent(EventType type, const unsigned char key);
	bool Ispressed();
	bool IsReleased();
	bool IsInvalid();

private:
	EventType type;
	unsigned char key;



};