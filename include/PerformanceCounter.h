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

#include "PerfCounterConfig.h"

class PerformanceCounter
{
	struct st_ETHERNET {
		bool _bUse;
		WCHAR _szName[128];
		PDH_HCOUNTER _pdh_Counter_Network_RecvBytes;
		PDH_HCOUNTER _pdh_Counter_Network_SendBytes;
	};

private:
	CpuUsageCounter*					m_CpuUsageCounter;
	PDH_HQUERY							m_hQuery;

	struct stPerfCounter {
		PDH_HCOUNTER			hCounter;
		PDH_FMT_COUNTERVALUE	counterValue;
	};
	std::vector<stPerfCounter>			m_CounterVec;

	// 이더넷 모니터링
	bool								m_EthernetMontFlag;
	st_ETHERNET							m_EthernetStruct[df_PDH_ETHERNET_MAX]; // 랜카드 별 PDH 정보 
	double								m_pdh_value_Network_RecvBytes;  // 총 Recv Bytes  모든 이더넷의 Recv 수치 합산 
	double								m_pdh_value_Network_SendBytes;  // 총 Send Bytes 모든 이더넷의 Send 수치 합산 

	HANDLE								m_hCounterThread;
	bool								m_Stop;

public:
	PerformanceCounter() 
		: m_CpuUsageCounter(NULL), m_Stop(false), m_EthernetMontFlag(false)
	{
		//InitializeSRWLock(&m_CounterMapSrwLock);
		m_CounterVec.resize(dfMAXIMUM_NUM_OF_COUNTER_ITEMS, { 0 });

		// PDH 쿼리 핸들 생성
		// 첫 번째, 두 번째 인자는 활용안함. 무시 (실시간 데이터를 얻을 것이기 때문에)
		PdhOpenQuery(NULL, NULL, &m_hQuery);
	}

	void SetCounter(BYTE counterNo, LPCWSTR counterQuery);
	void SetProcessCounter(BYTE counterNo, LPCWSTR counterQuery, LPCWSTR processName);
	void SetThreadCounter(BYTE counterNo, LPCWSTR counterQuery, LPCWSTR processName, DWORD threadID);
	void UnserCounter(BYTE counterNo);

	bool SetEthernetCounter();

	void SetCpuUsageCounter();
	void SetCpuUsageCounterThread();
	void UnsetCpuUsageCounter();

	void ResetPerfCounterItems();
	double GetPerfCounterItem(BYTE counterNo);
	double GetPerfEthernetRecvBytes();
	double GetPerfEthernetSendBytes();

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

	inline float ThreadTotal() {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ThreadTotal(GetCurrentThreadId());
		}
		return 0.0;
	}
	inline float ThreadUser() {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ThreadUser(GetCurrentThreadId());
		}
		return 0.0;
	}
	inline float ThreadKernel() {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ThreadKernel(GetCurrentThreadId());
		}
		return 0.0;
	}
	inline float ThreadTotal(DWORD thID) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ThreadTotal(thID);
		}
		return 0.0;
	}
	inline float ThreadUser(DWORD thID) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ThreadUser(thID);
		}
		return 0.0;
	}
	inline float ThreadKernel(DWORD thID) {
		if (m_CpuUsageCounter != NULL) {
			return m_CpuUsageCounter->ThreadKernel(thID);
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

