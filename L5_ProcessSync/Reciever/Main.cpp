#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <Windows.h>
#include <vector>

#include "Subprocess.h"
#include "Constants.h"
#include "Event.h"
#include "Mutex.h"


void ResetEvents(std::vector<Event>& events)
{
    for (auto& e : events)
    {
		e.Reset();
    }
}

void WaitForSenders(const std::vector<Event>& sendersEvents, bool all = true)
{
	const int n = sendersEvents.size();
	std::vector<HANDLE> eventsHandles(n);

	//mem_fn?
	std::transform(sendersEvents.begin(), sendersEvents.end(),
		eventsHandles.begin(), [](auto& e)
		{
			return e.GetHandle();
		});
	WaitForMultipleObjects(n, eventsHandles.data(), all, INFINITE);
}


void CreateFile(const std::string& filename)
{
	std::ofstream file(filename);
	file.close();
}

std::string CycleRead(const std::string& filename)
{
	std::fstream f(filename);

	std::string m;
	std::getline(f, m);

	std::vector<std::string> lines;
	std::string buffer;
    while(std::getline(f, buffer))
    {
		lines.push_back(buffer);
    }
	f.close();

	f.open(filename, std::ios::trunc | std::ios::out);
	for (auto& line : lines)
	{
		f << line << std::endl;
	}
	f.close();

	return m;
}

bool TryToRead(const std::string& filename, std::string& message)
{
	std::ifstream fin(filename);
    if (fin.peek() != std::ifstream::traits_type::eof())
    {
		message = CycleRead(filename);
		return true;
    }
	return false;
}

int main()
{
	try {
		unsigned int numberOfRecords;
		unsigned int numberOfSenders;
		std::string filename;

		std::cout << "Enter the number of records: " << std::endl;
		std::cin >> numberOfRecords;

		std::cout << "Enter the filename: " << std::endl;
		std::cin >> filename;

		std::cout << "Enter the number of senders: " << std::endl;
		std::cin >> numberOfSenders;

		std::vector<Subprocess> senders;
		std::vector<Event> sendersEvents;
		Mutex mtx(Constants::fileMutexName);
		CreateFile(filename);

        for (auto i = 0; i < numberOfSenders; ++i)
        {
			senders.emplace_back(Subprocess("Sender"));
			senders[i].SetArgs(filename, numberOfRecords);
        }

        for (auto& s : senders)
        {
			s.CreateSubprocess(false);
			const auto id = s.processInfo.dwProcessId;
			sendersEvents.emplace_back(Event(
				true,
				false,
				Constants::senderEventPrefix + std::to_string(id)));
        }

		WaitForSenders(sendersEvents);
		ResetEvents(sendersEvents);

		std::string choice = "Y";
		while (true)
		{
			std::cout << "Do you want to read the message? Y/N" << std::endl;
			std::cin >> choice;
			std::cin.ignore();

			if (choice == "Y") {
				mtx.Wait();
				std::string message;
				if (TryToRead(filename, message))
				{
					std::cout << "Message: " << std::endl;
					std::cout << message << std::endl;
				}
				else {
					std::cout << "Buffer is empty" << std::endl;
					std::cout << "Waiting for some messages.." << std::endl;
					mtx.Release();
					WaitForSenders(sendersEvents, false);

					mtx.Wait();

					std::cout << "Message: " << std::endl;
					std::cout << CycleRead(filename) << std::endl;
				}
				ResetEvents(sendersEvents);
				mtx.Release();
			}
			else
			{
				break;
			}
		}
	}
    catch (const std::exception& e)
	{
		std::cerr << "Error occured: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}