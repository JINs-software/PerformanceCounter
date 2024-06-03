#pragma once

#include <Windows.h>

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
};

