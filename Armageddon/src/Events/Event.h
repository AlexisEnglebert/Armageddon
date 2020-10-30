#pragma once

enum class EventType
{
	invalid,
	KeyPressed,
	KeyReleased,
	MousePressed,
	MouseReleased,
	MouseMove,
	MouseScroll,
	WindowResize,
	WindowMoved,
	WindowClose,
};
class Event
{
public:
	EventType GetEventType();
	bool Handle = false;
	EventType type;
};