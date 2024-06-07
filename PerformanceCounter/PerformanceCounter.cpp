#include "PerformanceCounter.h"
#include <string>

void PerformanceCounter::SetCounter(const std::string& counterName, LPCWSTR counter) {
	// PDH ���ҽ� ī���� ����
	// ���� �ڵ鿡 ī���͸� ���ε��ϴ� ���
	// �ϳ��� ���� �ڵ鿡 ���� ī���͸� ���ε��� �� �ִ�.
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
//		// PDH ����
//		PdhCollectQueryData(perfCounter->m_hQuery);
//		for (size_t i = 0; i < perfCounter->m_hCounters.size(); i++) {
//			// ���� ������ ����
//			// PDH_FMT_COUNTERVALUE ����� ���� �ڷ����� �ִ�. �ڷ����� DOUBLE�� �����Ͽ� ȹ���ϵ��� �Ѵ�. 
//			// cf) lpdwType: ���� ��(NULL), ����, �ִ�/�ּ� �� �پ��� ������ ���� �� �ִ�. (���� ����Ϳ����� Ȯ�� ����)
//			PdhGetFormattedCounterValue(perfCounter->m_hCounters[i], PDH_FMT_DOUBLE, NULL, &perfCounter->m_CounterValues[i]);	
//		}
//
//		// CpuUsageCounter ����
//		perfCounter->m_CpuUsageCounter->UpdateCpuTime();
//
//		// 1�ʸ��� ����
//		Sleep(1000);
//	}
//
//	return 0;
//}
