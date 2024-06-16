#pragma once

#include <Windows.h>
#include <map>

class CpuUsageCounter
{
public:
	//---------------------------------------------------------------------- 
	// 생성자, 확인대상 프로세스 핸들. 미입력시 자기 자신. 
	//---------------------------------------------------------------------- 
	CpuUsageCounter(HANDLE hProcess = INVALID_HANDLE_VALUE);

	void UpdateCpuTime(void);

	inline float ProcessorTotal(void) { return _fProcessorTotal; }
	inline float ProcessorUser(void) { return _fProcessorUser; }
	inline float ProcessorKernel(void) { return _fProcessorKernel; }

	inline float ProcessTotal(void) { return _fProcessTotal; }
	inline float ProcessUser(void) { return _fProcessUser; }
	inline float ProcessKernel(void) { return _fProcessKernel; }

private:

	HANDLE _hProcess;
	int _iNumberOfProcessors;

	float    _fProcessorTotal;
	float    _fProcessorUser;
	float    _fProcessorKernel;

	float _fProcessTotal;
	float _fProcessUser;
	float _fProcessKernel;

	ULARGE_INTEGER _ftProcessor_LastKernel;
	ULARGE_INTEGER _ftProcessor_LastUser;
	ULARGE_INTEGER _ftProcessor_LastIdle;

	ULARGE_INTEGER _ftProcess_LastKernel;
	ULARGE_INTEGER _ftProcess_LastUser;
	ULARGE_INTEGER _ftProcess_LastTime;

//	////////////////////////////////////////////////
//	// Thread
//	////////////////////////////////////////////////
//public:
//	void AddThreadHandle() {
//		HANDLE hTargetThreadHandle;
//		DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &hTargetThreadHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
//		_hThreads.insert({ GetCurrentThreadId(), stThreadInfo(hTargetThreadHandle)});
//	}
//	void DeleteThreadHandle() {
//		if (_hThreads.find(GetCurrentThreadId()) != _hThreads.end()) {
//			_hThreads.erase(GetCurrentThreadId());
//		}
//	}
//
//	inline float ThreadTotal(DWORD thID) { return _hThreads[thID]._fThreadTotal; }
//	inline float ThreadUser(DWORD thID) { return _hThreads[thID]._fThreadUser; }
//	inline float ThreadKernel(DWORD thID) { return _hThreads[thID]._fThreadKernel; }
//
//
//private:
//	struct stThreadInfo {
//		HANDLE _hThread;
//
//		float _fThreadTotal = 0;
//		float _fThreadUser = 0;
//		float _fThreadKernel = 0;
//
//		ULARGE_INTEGER _ftThread_LastKernel = { 0 };
//		ULARGE_INTEGER _ftThread_LastUser = { 0 };
//		ULARGE_INTEGER _ftThread_LastTime = { 0 };
//
//		stThreadInfo() {}
//		stThreadInfo(HANDLE hThrd) : _hThread(hThrd) {}
//	};
//	std::map<DWORD, stThreadInfo> _hThreads;
};

