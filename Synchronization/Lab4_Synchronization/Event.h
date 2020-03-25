#pragma once
#include <Windows.h>
#include <exception>

class Event final
{

	HANDLE hEvent = nullptr;

public:
	Event(bool isManual = true, bool state = false);


	Event(Event&& other) noexcept
		: hEvent(other.hEvent)
	{
		other.hEvent = nullptr;
	}

	Event& operator=(Event other)
	{
		std::swap(hEvent, other.hEvent);
		other.hEvent = nullptr;
		return *this;
	}

	HANDLE GetHandle() const;
	bool Pulse() const;
	
	bool Set();
	bool Reset();


	~Event();
};

