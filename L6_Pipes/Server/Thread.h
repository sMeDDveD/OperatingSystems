#pragma once
#include  <Windows.h>
#include <stdexcept>

class Thread final
{
	HANDLE hThread{};
	DWORD dwThreadID{};

	static std::string GetErrorDescription();
public:
	using Function = LPTHREAD_START_ROUTINE;


    Thread(const Thread& other) = delete;

    Thread(Thread&& other) noexcept;

    Thread& operator=(Thread&& other) noexcept;

	template <typename In>
	struct Data
	{
		In arg;
	};

	explicit Thread(Function f, void* pArgs);

	void Start() const;
	void Join() const;


    HANDLE GetHandle() const;
	DWORD GetDwThreadId() const;
	~Thread();
};