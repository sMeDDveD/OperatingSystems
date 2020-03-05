#pragma once

#include <sstream>
#include <string>
#include <windows.h>

//#ifdef __linux__
//
//#include <unistd.h>
//
//#elif _WIN32
//
//#include <windows.h>
//
//#endif

class Subprocess final
{
public:

	std::stringstream arguments;

	STARTUPINFO startupInfo{};
	PROCESS_INFORMATION processInfo{};

	char* argv[];

	explicit Subprocess(const std::string& processName);

	template <typename ...Args, typename T>
	void SetArgs(const T& head, const Args&... args)
	{
		arguments << head << " ";
		return SetArgs(args...);
	}

	void SetArrayArgs(char* argv[]);

	void CreateSubprocess(bool waiting = true);
	~Subprocess();
private:
	
	static void SetArgs()
	{
	}

	static std::string GetErrorDescription();
};
