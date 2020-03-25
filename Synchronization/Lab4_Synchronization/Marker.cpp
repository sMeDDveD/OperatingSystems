#include "Marker.h"

Marker::Marker(Function f, int number)
{
	hThread = CreateThread(
		nullptr,
		0,
		f,
		reinterpret_cast<LPVOID>(number),
		CREATE_SUSPENDED,
		&dwThreadID);

	if (hThread == nullptr)
	{
		throw std::runtime_error("Cannot create thread: " + GetErrorDescription());
	}
}

void Marker::Start() const
{
	if (ResumeThread(hThread) == -1)
	{
		throw std::runtime_error("Cannot start thread: " + GetErrorDescription());
	}
}

void Marker::Join() const
{
	WaitForSingleObject(hThread, INFINITE);
}

void Marker::Stop()
{
	stopped = true;
}

bool Marker::IsStopped() const
{
	return stopped;
}

DWORD Marker::GetDwThreadId() const
{
	return dwThreadID;
}


Marker::~Marker()
{
	CloseHandle(hThread);
}

std::string Marker::GetErrorDescription()
{
	LPVOID lpMsgBuf;
	const DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		dw,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&lpMsgBuf),
		0, nullptr);

	std::string retValue = static_cast<LPTSTR>(lpMsgBuf);
	LocalFree(lpMsgBuf);

	return retValue;
}

Marker::Marker(Marker&& other) noexcept: hThread(other.hThread),
                                         dwThreadID(other.dwThreadID),
                                         stopped(other.stopped)
{
	other.hThread = nullptr;
}

Marker& Marker::operator=(Marker&& other) noexcept
{
	std::swap(hThread, other.hThread);
	std::swap(dwThreadID, other.dwThreadID);
	std::swap(stopped, other.stopped);

	return *this;
}
