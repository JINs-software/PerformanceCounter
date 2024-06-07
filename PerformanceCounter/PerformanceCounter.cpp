#include "PerformanceCounter.h"
#include <string>
#include <assert.h>

void PerformanceCounter::SetCounter(BYTE counterNo, LPCWSTR counterQuery) {
	// PDH 리소스 카운터 생성
	// 쿼리 핸들에 카운터를 바인딩하는 방식
	// 하나의 쿼리 핸들에 여러 카운터를 바인딩할 수 있다.
	PDH_HCOUNTER hCounter;
	PdhAddCounter(m_hQuery, counterQuery, NULL, &hCounter);
	PdhCollectQueryData(m_hQuery);

	//AcquireSRWLockExclusive(&m_CounterMapSrwLock);
	//m_CounterMap[counterName].hCounter = hCounter;
	//m_CounterMap[counterName].counterValue = { 0 };
	//ReleaseSRWLockExclusive(&m_CounterMapSrwLock);

	assert(counterNo < m_CounterVec.size());
	if (m_CounterVec[counterNo].hCounter != NULL) {
		PdhRemoveCounter(m_CounterVec[counterNo].hCounter);
	}
	m_CounterVec[counterNo].hCounter = hCounter;
	m_CounterVec[counterNo].counterValue = { 0 };
}
void PerformanceCounter::SetProcessCounter(BYTE counterNo, LPCWSTR counterQuery, LPCWSTR processName) {
	std::wstring counterStr(counterQuery);
	std::wstring paramStr(processName);
	std::wstring placeholder = L"@param";

	size_t pos = counterStr.find(placeholder);
	if (pos != std::wstring::npos) {
		counterStr.replace(pos, placeholder.length(), paramStr);
	}

	PDH_HCOUNTER hCounter;
	PdhAddCounter(m_hQuery, counterStr.c_str(), NULL, &hCounter);
	PdhCollectQueryData(m_hQuery);

	//AcquireSRWLockExclusive(&m_CounterMapSrwLock);
	//m_CounterMap[counterName].hCounter = hCounter;
	//m_CounterMap[counterName].counterValue = { 0 };
	//ReleaseSRWLockExclusive(&m_CounterMapSrwLock);

	assert(counterNo < m_CounterVec.size());
	if (m_CounterVec[counterNo].hCounter != NULL) {
		PdhRemoveCounter(m_CounterVec[counterNo].hCounter);
	}
	m_CounterVec[counterNo].hCounter = hCounter;
	m_CounterVec[counterNo].counterValue = { 0 };
}

void PerformanceCounter::SetThreadCounter(BYTE counterNo, LPCWSTR counterQuery, LPCWSTR processName, DWORD threadID)
{
	std::wstring counterStr(counterQuery);
	std::wstring threadIDWStr = std::to_wstring(threadID);
	std::wstring paramStr(processName);
	paramStr += L"/";
	paramStr += threadIDWStr;

	std::wstring placeholder = L"@param";

	size_t pos = counterStr.find(placeholder);
	if (pos != std::wstring::npos) {
		counterStr.replace(pos, placeholder.length(), paramStr);
	}

	PDH_HCOUNTER hCounter;
	PdhAddCounter(m_hQuery, counterStr.c_str(), NULL, &hCounter);
	PdhCollectQueryData(m_hQuery);

	//AcquireSRWLockExclusive(&m_CounterMapSrwLock);
	//m_CounterMap[counterName].hCounter = hCounter;
	//m_CounterMap[counterName].counterValue = { 0 };
	//ReleaseSRWLockExclusive(&m_CounterMapSrwLock);

	assert(counterNo < m_CounterVec.size());
	if (m_CounterVec[counterNo].hCounter != NULL) {
		PdhRemoveCounter(m_CounterVec[counterNo].hCounter);
	}
	m_CounterVec[counterNo].hCounter = hCounter;
	m_CounterVec[counterNo].counterValue = { 0 };
}

void PerformanceCounter::UnserCounter(BYTE counterNo)
{
	assert(counterNo < m_CounterVec.size());
	if (m_CounterVec[counterNo].hCounter != NULL) {
		PdhRemoveCounter(m_CounterVec[counterNo].hCounter);

		m_CounterVec[counterNo].hCounter = NULL;
		m_CounterVec[counterNo].counterValue = { 0 };
	}
}

void PerformanceCounter::SetCpuUsageCounter() {
	if (m_CpuUsageCounter == NULL) {
		m_CpuUsageCounter = new CpuUsageCounter();
		m_CpuUsageCounter->UpdateCpuTime();
	}
}

void PerformanceCounter::SetCpuUsageCounterThread()
{
	if (m_CpuUsageCounter == NULL) {
		SetCpuUsageCounter();
	}

	m_CpuUsageCounter->AddThreadHandle();
	m_CpuUsageCounter->UpdateCpuTime();
}

void PerformanceCounter::UnsetCpuUsageCounter()
{
	if (m_CpuUsageCounter != NULL) {
		delete m_CpuUsageCounter;
		m_CpuUsageCounter = NULL;
	}
}

void PerformanceCounter::ResetPerfCounterItems() {
	//AcquireSRWLockShared(&m_CounterMapSrwLock);
	//for (auto& iter : m_CounterMap) {
	//	PdhGetFormattedCounterValue(iter.second.hCounter, PDH_FMT_DOUBLE, NULL, &iter.second.counterValue);
	//}
	//ReleaseSRWLockShared(&m_CounterMapSrwLock);

	for (int i = 0; i < m_CounterVec.size(); i++) {
		if (m_CounterVec[i].hCounter != NULL) {
			PdhGetFormattedCounterValue(m_CounterVec[i].hCounter, PDH_FMT_DOUBLE, NULL, &m_CounterVec[i].counterValue);
		}
	}

	if (m_CpuUsageCounter != NULL) {
		m_CpuUsageCounter->UpdateCpuTime();
	}
}


double PerformanceCounter::GetPerfCounterItem(BYTE counterNo) {
	double ret = 0.0;
	//
	//AcquireSRWLockShared(&m_CounterMapSrwLock);
	//if (m_CounterMap.find(counterName) == m_CounterMap.end()) {
	//	DebugBreak();
	//}
	//else {
	//	ret = m_CounterMap[counterName].counterValue.doubleValue;
	//}
	//ReleaseSRWLockShared(&m_CounterMapSrwLock);
	//
	//return ret;

	assert(counterNo < m_CounterVec.size());
	return m_CounterVec[counterNo].counterValue.doubleValue;
}

//UINT __stdcall PerformanceCounter::PerformanceCounterFunc(void* arg)
//{
//	PerformanceCounter* perfCounter = (PerformanceCounter*)arg;
//
//	while (!perfCounter->m_Stop) {
//		// PDH 갱신
//		PdhCollectQueryData(perfCounter->m_hQuery);
//		for (size_t i = 0; i < perfCounter->m_hCounters.size(); i++) {
//			// 갱신 데이터 얻음
//			// PDH_FMT_COUNTERVALUE 멤버에 여러 자료형이 있다. 자료형은 DOUBLE로 통일하여 획득하도록 한다. 
//			// cf) lpdwType: 현재 값(NULL), 차이, 최대/최소 등 다양한 값으로 얻을 수 있다. (성능 모니터에서도 확인 가능)
//			PdhGetFormattedCounterValue(perfCounter->m_hCounters[i], PDH_FMT_DOUBLE, NULL, &perfCounter->m_CounterValues[i]);	
//		}
//
//		// CpuUsageCounter 갱신
//		perfCounter->m_CpuUsageCounter->UpdateCpuTime();
//
//		// 1초마다 갱신
//		Sleep(1000);
//	}
//
//	return 0;
//}
