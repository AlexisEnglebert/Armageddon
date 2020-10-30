#pragma once
#include "Events/MouseEvent.h";
#include <queue>;
#include "Macros.h"
 class DLL MouseClass
{
public:
	void onLeftPressed(int x, int y);
	void onRightPressed(int x, int y);
	void onMiddlePressed(int x, int y);

	void onLeftReleased(int x, int y);
	void onRightReleased(int x, int y);
	void onMiddleReleased(int x, int y);
	
	void onWheelUp(int x, int y);
	void onWheelDown(int x, int y);
	void onMouseMove(int x, int y);
	void onRawMouseMove(int x, int y);

	bool isLeftDown();
	bool isRightDown();
	bool isMiddleDown();

	int GetX();
	int GetY();
	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;
	int x = 0;
	int y = 0;
};