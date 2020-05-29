#include "Event.h"


Event::Event(bool isManual, bool state, const std::string& eventName)
{
	hEvent = CreateEvent(
		nullptr,
		isManual,
		state,
        eventName.empty() ? nullptr : eventName.c_str());

	owner = true;

	if (hEvent == nullptr)
	{
		throw std::exception("Unable to create event");
	}
}

Event Event::Open(const std::string& eventName)
{
	Event opened{};

	opened.hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
	opened.owner = false;

    if (opened.hEvent == nullptr)
    {
		throw std::exception("Unable to open event");
    }
	return opened;
}

Event::Event(Event&& other) noexcept : hEvent(other.hEvent)
{
	other.hEvent = nullptr;
}


Event& Event::operator=(Event&& other) noexcept
{
	std::swap(hEvent, other.hEvent);
	other.hEvent = nullptr;
	return *this;
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
	if (hEvent != nullptr && owner)
	{
		CloseHandle(hEvent);
	}
}