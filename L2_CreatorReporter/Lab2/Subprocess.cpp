#include "Subprocess.h"
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

Subprocess::Subprocess(const std::string& processName)
{
	arguments << processName << " ";
	ZeroMemory(&startupInfo, sizeof startupInfo);
	startupInfo.cb = sizeof startupInfo;
	ZeroMemory(&processInfo, sizeof processInfo);
}

void Subprocess::SetArrayArgs(const std::initializer_list<std::string>& args)
{
	std::vector<std::string> lines(args);
	std::vector<const char*> starts;
	transform(lines.begin(), lines.end(), back_inserter(starts), mem_fn(&std::string::c_str));
	starts.emplace_back(NULL);
	argv = const_cast<char**>(&starts.front());
}

void Subprocess::CreateSubprocess(bool waiting)
{
	if (!CreateProcess(nullptr,						// No module name (use command line)
	                   const_cast<LPSTR>(arguments.str().c_str()), // Command line
	                   nullptr,					// Thread handle not inheritable
	                   nullptr,					// Process handle not inheritable
	                   FALSE,						// Set handle inheritance to FALSE
	                   CREATE_NEW_CONSOLE,			// No creation flags
	                   nullptr,						// Use parent's environment block
	                   nullptr,					// Use parent's starting directory 
	                   &startupInfo,							// Pointer to STARTUPINFO structure
	                   &processInfo)							// Pointer to PROCESS_INFORMATION structure
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
