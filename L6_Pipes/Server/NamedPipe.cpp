#include "NamedPipe.h"
#include <iostream>

NamedPipe::NamedPipe(const std::string& name, int size)
{
    hPipe = CreateNamedPipe(name.c_str(),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                            PIPE_UNLIMITED_INSTANCES,
                            size,
                            size,
                            10000,
                            nullptr);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        throw std::exception("Unable to create named pipe");
    }
}

NamedPipe NamedPipe::OpenPipe(const std::string& name)
{
    NamedPipe pipe;

    pipe.hPipe = CreateFile(name.c_str(),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            nullptr,
                            OPEN_EXISTING,
                            0,
                            nullptr);

    if (pipe.hPipe == INVALID_HANDLE_VALUE)
    {
        throw std::exception("Unable to open pipe");
    }
    pipe.owner = false;
    return pipe;
}

NamedPipe::NamedPipe(NamedPipe&& other) noexcept: hPipe(other.hPipe),
                                                  connected(other.connected),
                                                  owner(other.owner)
{
    other.owner = false;
}

NamedPipe& NamedPipe::operator=(NamedPipe&& other) noexcept
{
    using std::swap;
    swap(other.hPipe, hPipe);
    swap(other.connected, connected);
    swap(other.owner, owner);

    other.owner = false;
    return *this;
}

bool NamedPipe::Connect() const
{
    return ConnectNamedPipe(hPipe, nullptr);
}

bool NamedPipe::Disconnect() const
{
    return DisconnectNamedPipe(hPipe);
}

HANDLE NamedPipe::GetHandle() const
{
    return hPipe;
}

NamedPipe::~NamedPipe()
{
    if (owner && hPipe != nullptr)
    {
        CloseHandle(hPipe);
    }
}
