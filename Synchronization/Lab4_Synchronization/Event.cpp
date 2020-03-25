#include "Event.h"


Event::Event(bool isManual, bool state)
{
	hEvent = CreateEvent(nullptr, isManual, state, nullptr);

	if (hEvent == nullptr)
	{
		throw std::exception("Unable to create event");
	}
}

HANDLE Event::GetHandle() const
{
	return hEvent;
}

bool Event::Set()
{
	return SetEvent(hEvent);
}

bool Event::Reset()
{
	return ResetEvent(hEvent);
}

bool Event::Pulse() const
{
	return PulseEvent(hEvent);
}

Event::~Event()
{
	if (hEvent != nullptr)
	{
		CloseHandle(hEvent);
	}
}
