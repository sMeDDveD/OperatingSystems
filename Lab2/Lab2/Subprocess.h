#pragma once

#include <sstream>
#include <string>
#include <windows.h>

class Subprocess final
{
public:

	std::stringstream arguments;

	STARTUPINFO startupInfo{};
	PROCESS_INFORMATION processInfo{};

	explicit Subprocess(const std::string& processName);

	template <typename ...Args, typename T>
	void SetArgs(const T& head, const Args&... args)
	{
		arguments << head << " ";
		return SetArgs(args...);
	}

	void CreateSubprocess(bool waiting = true);
	~Subprocess();
private:

	static void SetArgs()
	{
	}

	static std::string GetErrorDescription();
};
