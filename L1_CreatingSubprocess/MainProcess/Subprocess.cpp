#include "Subprocess.h"



Subprocess::Subprocess(const std::string& processPath):  processPath(processPath)
{
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));
}

void Subprocess::SetArguments(const std::string& args)
{
	arguments = args;
}

void Subprocess::CreateSubprocess(bool waiting)
{
	if (!CreateProcess(processPath.c_str(),						    // No module name (use command line)
		const_cast<LPSTR>(arguments.c_str()),        // Command line
		NULL,										  // Process handle not inheritable
		NULL,										 // Thread handle not inheritable
		FALSE,								     	// Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,						   // No creation flags
		NULL,									  // Use parent's environment block
		NULL,									 // Use parent's starting directory 
		&startupInfo,							// Pointer to STARTUPINFO structure
		&processInfo)						   // Pointer to PROCESS_INFORMATION structure
		)
	{
		throw std::runtime_error(GetErrorDescription());
	}

	if (waiting) {
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
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);
	
	std::string retValue = LPTSTR(lpMsgBuf);
	LocalFree(lpMsgBuf);

	return retValue;
}
