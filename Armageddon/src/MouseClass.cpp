#include "MouseClass.h"
#include "Events/MouseEvent.h"
#include "Macros.h"
#include "Log.h"
void MouseClass::onLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEvent::MEventType::Lpress, x, y);
	this->eventBuffer.push(me);
	Armageddon::Log::GetLogger()->info("LEFT PRESSED");

}

void MouseClass::onLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Lrelease, x, y));
	Armageddon::Log::GetLogger()->info("LEFT released");
}

void MouseClass::onRightPressed(int x, int y)
{
	this->rightIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Rpress, x, y));
}

void MouseClass::onRightReleased(int x, int y)
{
	this->rightIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Rrelease, x, y));
}

void MouseClass::onMiddlePressed(int x, int y)
{
	this->mbuttonDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Mpress, x, y));
}

void MouseClass::onMiddleReleased(int x, int y)
{
	this->mbuttonDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Mrelease, x, y));
}

void MouseClass::onWheelUp(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Wheelup, x, y));
}

void MouseClass::onWheelDown(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Wheeldown, x, y));
}

void MouseClass::onMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::Move, x, y));
}

void MouseClass::onRawMouseMove(int x, int y)
{
	return this->eventBuffer.push(MouseEvent(MouseEvent::MEventType::RAW_MOVE, x, y));
}

bool MouseClass::isLeftDown()
{
	return this->leftIsDown;
}

bool MouseClass::isMiddleDown()
{
	return this->mbuttonDown;
}

bool MouseClass::isRightDown()
{
	return this->rightIsDown;
}

int MouseClass::GetX()
{
	return this->x;
}

int MouseClass::GetY()
{
	return this->y;
}

MousePoint MouseClass::GetPos()
{
	return{ this->x, this->y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = eventBuffer.front(); //Get first event from buffer
		eventBuffer.pop(); //Remove first event from buffer
	//	Armageddon::Log::GetLogger()->info("CURRENT SIZE {0}", this->eventBuffer.size());
		return e;
	}
}
