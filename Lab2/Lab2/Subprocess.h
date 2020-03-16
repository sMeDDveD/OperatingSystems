#pragma once

#include <sstream>
#include <string>

#include <cstdarg>
#include <vector>

#ifdef __linux__

#include <unistd.h>

#elif _WIN32

#include <windows.h>

#endif

class Subprocess final {
public:
    std::stringstream arguments;

    std::string processName;
    std::vector<char *> argv;

    explicit Subprocess(std::string processName);

    void SetArrayArgs(const std::initializer_list<std::string> &list);

    template<typename ...Args, typename T>
    void SetArgs(const T &head, const Args &... args) {
        arguments << head << " ";
        return SetArgs(args...);
    }


    void CreateSubprocess(bool waiting = true);

    ~Subprocess() = default;

private:

	static void SetArgs()
	{
	}

    static std::string GetErrorDescription();
};
