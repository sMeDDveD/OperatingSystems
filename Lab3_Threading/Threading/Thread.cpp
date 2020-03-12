#include "Thread.h"



Thread::Thread(Function f, void* pArgs)
{
	hThread = CreateThread(
		nullptr,
		0,
		f,
		pArgs,
		CREATE_SUSPENDED,
		&dwThreadID);

	if (hThread == nullptr)
	{
		throw std::runtime_error("Cannot create thread: " + GetErrorDescription());
	}
}

void Thread::Start() const
{
	if (ResumeThread(hThread) == -1)  // NOLINT(clang-diagnostic-sign-compare)
	{
		throw std::runtime_error("Cannot start thread: " + GetErrorDescription());
	}
}

void Thread::Join() const
{
	WaitForSingleObject(hThread, INFINITE);
}

DWORD Thread::GetDwThreadId() const
{
	return dwThreadID;
}


Thread::~Thread()
{
	CloseHandle(hThread);
}

std::string Thread::GetErrorDescription()
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
