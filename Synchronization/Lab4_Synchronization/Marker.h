#pragma once
#include <Windows.h>
#include <stdexcept>
#include <vector>

class Marker final
{
	HANDLE hThread{};
	DWORD dwThreadID{};

	bool stopped = false;

	static std::string GetErrorDescription();
public:
	using Function = LPTHREAD_START_ROUTINE;

	Marker() = default;

	Marker(Marker&& other) noexcept
		: hThread(other.hThread),
		  dwThreadID(other.dwThreadID),
		  stopped(other.stopped)
	{
		other.hThread = nullptr;
	}

	Marker& operator=(Marker&& other) noexcept
	{
		std::swap(hThread, other.hThread);
		std::swap(dwThreadID, other.dwThreadID);
		std::swap(stopped, other.stopped);

		return *this;
	}

	explicit Marker(Function f, int number);

	void Start() const;
	void Join() const;
	void Stop();

	bool IsStopped() const;
	DWORD GetDwThreadId() const;
	~Marker();

};
