#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../Reciever/Event.h"
#include "../Reciever/Constants.h"
#include "../Reciever/Mutex.h"


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

	Mutex mtx = Mutex::Open(Constants::fileMutexName);

	std::string choice = "Y";
    while (true)
    {
		std::cout << "Do you want to enter the message? Y/N" << std::endl;
		std::cin >> choice;
		std::cin.ignore();

		if (choice == "Y") {
			mtx.Wait();

			char message[Constants::messageLength];
			std::cout << "Enter the message: " << std::endl;
			std::cin.getline(message, Constants::messageLength);

            if (!TryToWrite(filename, numberOfRecords, message))
            {
				std::cout << "File buffer is full" << std::endl;
				std::cout << "Waiting for some space..";

                // Semaphore?
				while (!TryToWrite(filename, numberOfRecords, message)) {
					mtx.Release();
					Sleep(Constants::relaxTime);
					std::cout << "..." << std::endl;
					mtx.Wait();
				}

				std::cout << "Message was sent" << std::endl;
            }
			cEvent.Set();
			mtx.Release();
		}
		else
		{
			break;
		}
    }
  
	return 0;
}
