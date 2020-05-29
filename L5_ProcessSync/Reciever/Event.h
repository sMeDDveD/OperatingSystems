#pragma once
#include <Windows.h>
#include <exception>
#include <string>

class Event final
{

	HANDLE hEvent = nullptr;
	bool owner = true;

public:
	Event(bool isManual = true, bool state = false, const std::string& eventName = "");

	static Event Open(const std::string& eventName);


	Event(Event&& other) noexcept;
    Event& operator=(Event&& other) noexcept;

    Event& operator=(Event other) = delete;
    Event(const Event& other) = delete;

	HANDLE GetHandle() const;
	bool Pulse() const;

	bool Set();
	bool Reset();


	~Event();
};
