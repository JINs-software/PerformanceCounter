#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>

#include <Pdh.h>
#pragma comment(lib, "Pdh.lib")

#include "CounterQueryString.h"

#include "CpuUsageCounter.h"

class PerformanceCounter
{
private:
	CpuUsageCounter*					m_CpuUsageCounter;
	PDH_HQUERY							m_hQuery;
	//std::vector<PDH_HCOUNTER>			m_hCounters;
	//std::vector<PDH_FMT_COUNTERVALUE>	m_CounterValues;

	struct stPerfCounter {
		PDH_HCOUNTER			hCounter;
		PDH_FMT_COUNTERVALUE	counterValue;
	};
	std::map<std::string, stPerfCounter>	m_CounterMap;
	SRWLOCK									m_CounterMapSrwLock;

	HANDLE								m_hCounterThread;
	bool								m_Stop;

public:
	PerformanceCounter() 
		: m_CpuUsageCounter(NULL), m_Stop(false)
	{
		InitializeSRWLock(&m_CounterMapSrwLock);

		// PDH 쿼리 핸들 생성
		// 첫 번째, 두 번째 인자는 활용안함. 무시 (실시간 데이터를 얻을 것이기 때문에)
		PdhOpenQuery(NULL, NULL, &m_hQuery);
	}

	void SetCounter(const std::string& counterName, LPCWSTR counter);
	void SetCpuUsageCounter();
	void ResetPerfCounter();
	double GetPerfCounter(const std::string& counterName);

	inline float ProcessorTotal(void) { 
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ProcessorTotal();
		}
		return 0.0;
	}
	inline float ProcessorUser(void) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ProcessorUser();
		}
		return 0.0;
	}
	inline float ProcessorKernel(void) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ProcessorKernel();
		}
		return 0.0;
	}
	inline float ProcessTotal(void) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ProcessTotal();
		}
		return 0.0;
	}
	inline float ProcessUser(void) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ProcessUser();
		}
		return 0.0;
	}
	inline float ProcessKernel(void) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ProcessKernel();
		}
		return 0.0;
	}

	//void ConsolePrint() {
	//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//	COORD coord;
	//	coord.X = 0;
	//	coord.Y = 0;
	//	SetConsoleCursorPosition(hConsole, coord);
	//
	//	// ...
	//}





///////////////////////////////////////////////////
// Counter Thread
///////////////////////////////////////////////////
	void BeginCounterThread() {
		m_hCounterThread = (HANDLE)_beginthreadex(NULL, 0, PerformanceCounterFunc, this, 0, NULL);
	}
	void StopCounterThread() {
		m_Stop = true;
	}

private:
	static UINT __stdcall PerformanceCounterFunc(void* arg);
};

