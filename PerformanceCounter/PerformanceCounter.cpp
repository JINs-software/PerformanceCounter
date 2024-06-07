#include "PerformanceCounter.h"
#include <string>
#include <assert.h>

void PerformanceCounter::SetCounter(BYTE counterNo, LPCWSTR counterQuery) {
	// PDH ���ҽ� ī���� ����
	// ���� �ڵ鿡 ī���͸� ���ε��ϴ� ���
	// �ϳ��� ���� �ڵ鿡 ���� ī���͸� ���ε��� �� �ִ�.
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
