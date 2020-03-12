#pragma once
#include  <Windows.h>
#include <exception>
#include <stdexcept>

class Thread final
{
	HANDLE hThread{};
	DWORD dwThreadID{};
public:
	using Function = LPTHREAD_START_ROUTINE;
	
	explicit Thread(Function f, void* pArgs);
	
	void Start() const;
	void Join() const;


	DWORD GetDwThreadId() const;
	~Thread();
	
	static std::string GetErrorDescription();
};

