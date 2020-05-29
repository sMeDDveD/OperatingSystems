#pragma once
#include <string>
#include <Windows.h>


class Mutex final
{

	HANDLE hMutex = nullptr;
	bool owner = true;

	Mutex() = default;

public:
    explicit Mutex(const std::string& name);
	static Mutex Open(const std::string& name);


    Mutex(const Mutex& other) = delete;

    Mutex(Mutex&& other) noexcept;

    Mutex& operator=(Mutex other);

	void Wait() const;
	bool Release();

    ~Mutex();
};

