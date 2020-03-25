#pragma once
#include <Windows.h>
#include <exception>

class Event final
{

	HANDLE hEvent = nullptr;

public:
	Event(bool isManual = true, bool state = false);


	Event(Event&& other) noexcept;

	Event& operator=(Event other);

	HANDLE GetHandle() const;
	bool Pulse() const;
	
	bool Set();
	bool Reset();


	~Event();
};

