#pragma once

#include <sstream>
#include <string>
#include <windows.h>
#include <cstdarg>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

class Subprocess final
{
public:

	std::stringstream arguments;

	STARTUPINFO startupInfo{};
	PROCESS_INFORMATION processInfo{};


	explicit Subprocess(const std::string& processName);


	Subprocess(const Subprocess& other) = delete;

	Subprocess(Subprocess&& other) noexcept;

	Subprocess& operator=(Subprocess other);

	template <typename ...Args, typename T>
	void SetArgs(const T& head, const Args&... args)
	{
		arguments << head << " ";
		return SetArgs(args...);
	}

	HANDLE GetProcessHandle() const;


	void CreateSubprocess(bool waiting = true);
	~Subprocess();
private:

	static void SetArgs()
	{
	}

	static std::string GetErrorDescription();
};