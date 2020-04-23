#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../Reciever/Event.h"
#include "../Reciever/Constants.h"

HANDLE GetMutex()
{
    HANDLE hMutex = OpenMutex(
		SYNCHRONIZE,
		false,
		Constants::fileMutexName.c_str());

    if (hMutex == nullptr)
    {
		throw std::exception("Unable to open Mutex");
    }

	return hMutex;
}

int GetNumberOfRecords(const std::string& filename)
{
	std::ifstream f(filename);
	return std::count(std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>(), '\n');
}

bool TryToWrite(const std::string& filename, int max, const std::string& message)
{
	if (GetNumberOfRecords(filename) < max)
	{
		std::ofstream fout(filename, std::ios::app);
		fout << message << std::endl;
		fout.close();

		return true;
	}
    return false;
}

int main(int argc, char* argv[])
{
	std::cout << "I am alive" << std::endl;
    const std::string filename = argv[1];
	const int numberOfRecords = std::stoi(argv[2]);

	Event cEvent = Event::Open(Constants::senderEventPrefix + 
		std::to_string(GetCurrentProcessId()));
	cEvent.Set();

	HANDLE hMutex = GetMutex();

	std::string choice = "Y";
    while (true)
    {
		std::cout << "Do you want to enter the message? Y/N" << std::endl;
		std::cin >> choice;
		std::cin.ignore();

		if (choice == "Y") {
			WaitForSingleObject(hMutex, INFINITE);

			char message[Constants::messageLength];
			std::cout << "Enter the message: " << std::endl;
			std::cin.getline(message, Constants::messageLength);

            if (!TryToWrite(filename, numberOfRecords, message))
            {
				std::cout << "File buffer is full" << std::endl;
				std::cout << "Waiting for some space..";

                // Semaphore?
				while (!TryToWrite(filename, numberOfRecords, message)) {
					ReleaseMutex(hMutex);
					Sleep(Constants::relaxTime);
					std::cout << "..." << std::endl;
					WaitForSingleObject(hMutex, INFINITE);
				}

				std::cout << "Message was sent" << std::endl;
            }
			cEvent.Set();
			ReleaseMutex(hMutex);
		}
		else
		{
			break;
		}
    }
  
	return 0;
}
