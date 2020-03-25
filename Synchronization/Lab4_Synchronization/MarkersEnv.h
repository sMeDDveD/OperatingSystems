#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Event.h"
#include "Marker.h"

namespace Markers
{
	const int timeToSleep = 5;

	DWORD WINAPI MarkerFunction(LPVOID lpParam);

	void CreateMarkers(int number);
	bool ResumeMarkers();
	void StartMarkers();
	bool StopMarker(int idx);
	
	void CreateEvents(int number);

	void PrintArray();
	int Main();
}
