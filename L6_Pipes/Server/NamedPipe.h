#pragma once
#include <Windows.h>
#include <string>

class NamedPipe final
{
	HANDLE hPipe = nullptr;
	bool connected = false;
	bool owner = true;

	
public:
	NamedPipe() = default;
    explicit NamedPipe(const std::string& name, int size);
	static NamedPipe OpenPipe(const std::string& name);

    NamedPipe(const NamedPipe& other) = default;

    NamedPipe(NamedPipe&& other) noexcept;
    NamedPipe& operator=(NamedPipe&& other) noexcept;

	bool Connect() const;
	bool Disconnect() const;
	HANDLE GetHandle() const;

    template<typename MessageType>
    void Write(MessageType message) const
    {
		WriteFile(hPipe, &message, sizeof(MessageType),
			nullptr, nullptr);
    }

    template<typename MessageType>
    MessageType Read() const
    {
		MessageType message;
		if (ReadFile(hPipe, &message, sizeof(MessageType),
			nullptr, nullptr)) {
			return message;
		}
		std::cout << GetLastError() << std::endl;
		throw std::exception("Unable to read");

    }

	~NamedPipe();
};

