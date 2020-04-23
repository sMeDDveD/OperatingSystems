#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <exception>

class Subprocess
{
public:

	std::string processPath;
	std::string arguments;

	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;



	Subprocess(const std::string& processPath);

	void SetArguments(const std::string& args);
	void CreateSubprocess(bool waiting);
	virtual ~Subprocess();
private:

	static std::string GetErrorDescription();
};

