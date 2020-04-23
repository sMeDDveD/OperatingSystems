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

	template <typename In, typename Out>
	struct Data
	{
		In arg;
		Out ret{};
	};

	explicit Thread(Function f, void* pArgs);

	void Start() const;
	void Join() const;

	DWORD GetDwThreadId() const;
	~Thread();
};
