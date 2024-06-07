#include "PerformanceCounter.h"
#include <string>

void PerformanceCounter::SetCounter(const std::string& counterName, LPCWSTR counter) {
	// PDH 리소스 카운터 생성
	// 쿼리 핸들에 카운터를 바인딩하는 방식
	// 하나의 쿼리 핸들에 여러 카운터를 바인딩할 수 있다.
	PDH_HCOUNTER hCounter;
	PdhAddCounter(m_hQuery, counter, NULL, &hCounter);
	PdhCollectQueryData(m_hQuery);

	AcquireSRWLockExclusive(&m_CounterMapSrwLock);
	m_CounterMap[counterName].hCounter = hCounter;
	m_CounterMap[counterName].counterValue = { 0 };
	ReleaseSRWLockExclusive(&m_CounterMapSrwLock);
}
void PerformanceCounter::SetProcessCounter(const std::string& counterName, LPCWSTR counter, LPCWSTR processName) {
	std::wstring counterStr(counter);
	std::wstring paramStr(processName);
	std::wstring placeholder = L"@param";

	size_t pos = counterStr.find(placeholder);
	if (pos != std::wstring::npos) {
		counterStr.replace(pos, placeholder.length(), paramStr);
	}

	PDH_HCOUNTER hCounter;
	PdhAddCounter(m_hQuery, counterStr.c_str(), NULL, &hCounter);
	PdhCollectQueryData(m_hQuery);

	AcquireSRWLockExclusive(&m_CounterMapSrwLock);
	m_CounterMap[counterName].hCounter = hCounter;
	m_CounterMap[counterName].counterValue = { 0 };
	ReleaseSRWLockExclusive(&m_CounterMapSrwLock);
}

void PerformanceCounter::SetThreadCounter(const std::string& counterName, LPCWSTR counter, LPCWSTR processName, DWORD threadID)
{
	std::wstring counterStr(counter);
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

	AcquireSRWLockExclusive(&m_CounterMapSrwLock);
	m_CounterMap[counterName].hCounter = hCounter;
	m_CounterMap[counterName].counterValue = { 0 };
	ReleaseSRWLockExclusive(&m_CounterMapSrwLock);
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

void PerformanceCounter::ResetPerfCounter() {
	AcquireSRWLockShared(&m_CounterMapSrwLock);
	for (auto& iter : m_CounterMap) {
		PdhGetFormattedCounterValue(iter.second.hCounter, PDH_FMT_DOUBLE, NULL, &iter.second.counterValue);
	}
	ReleaseSRWLockShared(&m_CounterMapSrwLock);

	if (m_CpuUsageCounter != NULL) {
		m_CpuUsageCounter->UpdateCpuTime();
	}
}


double PerformanceCounter::GetPerfCounter(const std::string& counterName) {
	double ret = 0.0;

	AcquireSRWLockShared(&m_CounterMapSrwLock);
	if (m_CounterMap.find(counterName) == m_CounterMap.end()) {
		DebugBreak();
	}
	else {
		ret = m_CounterMap[counterName].counterValue.doubleValue;
	}
	ReleaseSRWLockShared(&m_CounterMapSrwLock);

	return ret;
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
