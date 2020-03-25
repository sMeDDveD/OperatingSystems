#include "MarkersEnv.h"

#include <iterator>

namespace Markers
{
	static std::vector<int> arr;

	static std::vector<Marker> markers;

	static std::vector<Event> finishEvents;
	static std::vector<Event> stopEvents;
	static Event resumeEvent = Event();

	// TODO: Wrapper
	static CRITICAL_SECTION cs;
	
	DWORD __stdcall markerFunction(LPVOID lpParam)
	{
		const int currentMarker = reinterpret_cast<int>(lpParam);

		std::srand(currentMarker);
		std::vector<int> marked;

		while (true)
		{
			const int idx = std::rand() % arr.size();

			EnterCriticalSection(&cs);
			if (arr[idx] == 0)
			{
				Sleep(timeToSleep);
				arr[idx] = currentMarker + 1;
				Sleep(timeToSleep);

				LeaveCriticalSection(&cs);

				marked.push_back(idx);
			}
			else
			{
				LeaveCriticalSection(&cs);
				std::cout << "Thread: " << currentMarker << std::endl <<
					"Marked elements: " << marked.size() << std::endl <<
					"Unable to mark: " << idx << std::endl;

				finishEvents[currentMarker].Set();

				HANDLE eventsToWait[] = { stopEvents[currentMarker].GetHandle(), resumeEvent.GetHandle() };

				const int result = WaitForMultipleObjects(2, eventsToWait, false, INFINITE);

				if (result == WAIT_OBJECT_0)
				{
					for (auto mark : marked)
					{
						arr[mark] = 0;
					}

					return 0;
				}
			}
		}
	}

	void CreateArray(int size)
	{
		arr.resize(size, 0);
	}

	void CreateMarkers(int number)
	{
		markers.resize(number);

		for (int i = 0; i < number; i++)
		{
			markers[i] = Marker(markerFunction, i);
		}
	}

	void StartMarkers()
	{
		for (auto& marker : markers)
		{
			marker.Start();
		}
	}

	void CreateEvents(int number)
	{
		finishEvents.resize(number);
		stopEvents.resize(number);

		for (int i = 0; i < number; i++)
		{
			finishEvents[i] = Event();
			stopEvents[i] = Event();
		}
	}

	bool StopMarker(int idx)
	{
		if (markers[idx].IsStopped())
		{
			return false;
		}

		markers[idx].Stop();

		return true;
	}

	bool ResumeMarkers()
	{
		for (unsigned i = 0; i < markers.size(); ++i) {
			if (!markers[i].IsStopped()) {
				finishEvents[i].Reset();
			}
		}
		return resumeEvent.Pulse();
	}

	void PrintArray()
	{
		std::copy(arr.cbegin(), arr.cend(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}

	int Main()
	{
		int numberOfMarkers;
		int n;
		
		try
		{
			std::cout << "Enter the number of elements: ";
			std::cin >> n;

			arr.resize(n, 0);

			std::cout << "Enter the number of markers: ";
			std::cin >> numberOfMarkers;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what();
			return -1;
		}

		CreateMarkers(numberOfMarkers);
		CreateEvents(numberOfMarkers);

		InitializeCriticalSection(&cs);
		StartMarkers();

		int markersLeft = numberOfMarkers;
		std::vector<HANDLE> handles(numberOfMarkers);
		while (markersLeft)
		{
			// TODO 
			//std::transform(finishEvents.begin(), finishEvents.end(),
			//	std::back_inserter(handles), [](const auto& e) {return e.GetHandle(); });
			//	
			for (int i = 0; i < numberOfMarkers; i++)
			{
				handles[i] = finishEvents[i].GetHandle();
			}
			WaitForMultipleObjects(numberOfMarkers, handles.data(), true, INFINITE);
			
			PrintArray();

			int markerToStop;
			std::cin >> markerToStop;

			if (!StopMarker(markerToStop))
			{
				std::cout << "Unable to stop this marker" << std::endl;
			}
			else
			{
				markersLeft--;
				stopEvents[markerToStop].Set();
				markers[markerToStop].Join();
			}
			
			PrintArray();
			
			ResumeMarkers();
		}
		DeleteCriticalSection(&cs);
		return 0;
	}
}
