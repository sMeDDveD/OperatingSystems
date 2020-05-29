#include "Mutex.h"



Mutex::Mutex(const std::string& name)
{
	hMutex = CreateMutex(
		nullptr,
		FALSE,
		name.c_str());
	if (hMutex == nullptr)
	{
		throw std::runtime_error("Unable to get mutex");
	}
}

Mutex Mutex::Open(const std::string& name)
{
	Mutex m{};

	m.owner = false;
	m.hMutex = OpenMutex(
		SYNCHRONIZE,
		false,
		name.c_str());

	if (m.hMutex == nullptr)
	{
		throw std::exception("Unable to open Mutex");
	}

	return m;
}

Mutex::Mutex(Mutex&& other) noexcept: hMutex(other.hMutex),
                                      owner(other.owner)
{
    std::swap(hMutex, other.hMutex);
    std::swap(owner, other.owner);
}

Mutex& Mutex::operator=(Mutex other)
{
    using std::swap;
    swap(*this, other);
    return *this;
}

void Mutex::Wait() const
{
	WaitForSingleObject(hMutex, INFINITE);
}

bool Mutex::Release()
{
	return ReleaseMutex(hMutex);
}

Mutex::~Mutex()
{
    if (owner && hMutex != nullptr)
    {
		CloseHandle(hMutex);
    }
}
