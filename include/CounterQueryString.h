#pragma once

#define dfQUERY_TOTAL_CPU_USAGE				L"\\Processor(_Total)\\% Processor Time"	// CPU 전체 사용률
											
#define dfQUERY_CORE0_CPU_USAGE				L"\\Processor(0)\\% Processor Time"	// CPU 코어 사용률
#define dfQUERY_CORE1_CPU_USAGE				L"\\Processor(1)\\% Processor Time"	// CPU 코어 사용률
#define dfQUERY_CORE2_CPU_USAGE				L"\\Processor(2)\\% Processor Time"	// CPU 코어 사용률
#define dfQUERY_CORE3_CPU_USAGE				L"\\Processor(3)\\% Processor Time"	// CPU 코어 사용률
											
#define dfQUERY_PROCESS_CPU_USER_USAGE		L"\\Process(@processName)\\% User Time"			// 프로세스 CPU 유저 사용률
#define dfQUERY_PROCESS_CPU_TOTAL_USAGE		L"\\Process(@processName)\\% Processor Time"		// 프로세스 CPU 전체 사용률
#define dfQUERY_PRECESS_HANDLE_COUNT		L"\\Process(@processName)\\Handle Count"			// 프로세스 핸들 수 
#define dfQUERY_PROCESS_THREAD_COUNT		L"\\Process(@processName)\\Thread Count"			// 프로세스 스레드 수
#define dfQUERY_PROCESS_VMEMORY_USAGE		L"\\Process(@processName)\\Virtual Bytes"			// 프로세스 가상메모리 사용(가상메모리 테이블의 주소 용량으로 실제 사용메모리와는 무관)
#define dfQUERY_PROCESS_USER_VMEMORY_USAGE	L"\\Process(@processName)\\Private Bytes"			// 프로세스 유저할당 메모리(실제 유저 메모리 사용량, 프로세스 공용메모리/커널메모리 제외)
#define dfQUERY_PROCESS_WORKING_SET			L"\\Process(@processName)\\Working Set"			// 프로세스 작업 메모리(물리 메모리에 사용되는 크기일 뿐 할당 용량은 아닐 수 있음)
#define dfQUERY_PROCESS_NON_PAGED_MEMORY	L"\\Process(@processName)\\Pool Nonpaged Bytes"	// 프로세스 논페이지 메모리

#define dfQUERY_MEMORY_AVAILABLE			L"\\Memory\\Available MBytes"			// 사용가능 메모리
#define dfQUERY_MEMORY_NON_PAGED			L"\\Memory\\Pool Nonpaged Bytes"		// 논페이지드 메모리
