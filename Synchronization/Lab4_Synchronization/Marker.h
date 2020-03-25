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

	Marker(Marker&& other) noexcept;

	Marker& operator=(Marker&& other) noexcept;

	explicit Marker(Function f, int number);

	void Start() const;
	void Join() const;
	void Stop();

	bool IsStopped() const;
	DWORD GetDwThreadId() const;
	~Marker();

};
