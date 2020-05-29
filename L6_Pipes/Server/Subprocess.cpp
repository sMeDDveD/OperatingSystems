#include "Subprocess.h"


Subprocess::Subprocess(const std::string& processName)
{
	arguments << processName << " ";
	ZeroMemory(&startupInfo, sizeof startupInfo);
	startupInfo.cb = sizeof startupInfo;
	ZeroMemory(&processInfo, sizeof processInfo);
}

Subprocess::Subprocess(Subprocess&& other) noexcept
{
	std::swap(arguments, other.arguments);
	std::swap(startupInfo, other.startupInfo);
	std::swap(processInfo, other.processInfo);
}

Subprocess& Subprocess::operator=(Subprocess other)
{
	std::swap(*this, other);
	return *this;
}


HANDLE Subprocess::GetProcessHandle() const
{
	return processInfo.hProcess;
}

void Subprocess::CreateSubprocess(bool waiting)
{
	if (!CreateProcess(nullptr,	    // No module name (use command line)
		const_cast<LPSTR>(arguments.str().c_str()), // Command line
		nullptr,					// Thread handle not inheritable
		nullptr,					    // Process handle not inheritable
		FALSE,						    // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,			    // No creation flags
		nullptr,					    	// Use parent's environment block
		nullptr,					    // Use parent's starting directory 
		&startupInfo,							    // Pointer to STARTUPINFO structure
		&processInfo)							    // Pointer to PROCESS_INFORMATION structure
		)
	{
		throw std::runtime_error(GetErrorDescription());
	}

	if (waiting)
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
	}
}

Subprocess::~Subprocess()
{
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}


std::string Subprocess::GetErrorDescription()
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