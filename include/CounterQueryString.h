#pragma once

#define dfQUERY_TOTAL_CPU_USAGE				L"\\Processor(_Total)\\% Processor Time"	// CPU ��ü ����
											
#define dfQUERY_CORE0_CPU_USAGE				L"\\Processor(0)\\% Processor Time"	// CPU �ھ� ����
#define dfQUERY_CORE1_CPU_USAGE				L"\\Processor(1)\\% Processor Time"	// CPU �ھ� ����
#define dfQUERY_CORE2_CPU_USAGE				L"\\Processor(2)\\% Processor Time"	// CPU �ھ� ����
#define dfQUERY_CORE3_CPU_USAGE				L"\\Processor(3)\\% Processor Time"	// CPU �ھ� ����
											
#define dfQUERY_PROCESS_CPU_USER_USAGE		L"\\Process(@processName)\\% User Time"			// ���μ��� CPU ���� ����
#define dfQUERY_PROCESS_CPU_TOTAL_USAGE		L"\\Process(@processName)\\% Processor Time"		// ���μ��� CPU ��ü ����
#define dfQUERY_PRECESS_HANDLE_COUNT		L"\\Process(@processName)\\Handle Count"			// ���μ��� �ڵ� �� 
#define dfQUERY_PROCESS_THREAD_COUNT		L"\\Process(@processName)\\Thread Count"			// ���μ��� ������ ��
#define dfQUERY_PROCESS_VMEMORY_USAGE		L"\\Process(@processName)\\Virtual Bytes"			// ���μ��� ����޸� ���(����޸� ���̺��� �ּ� �뷮���� ���� ���޸𸮿ʹ� ����)
#define dfQUERY_PROCESS_USER_VMEMORY_USAGE	L"\\Process(@processName)\\Private Bytes"			// ���μ��� �����Ҵ� �޸�(���� ���� �޸� ��뷮, ���μ��� ����޸�/Ŀ�θ޸� ����)
#define dfQUERY_PROCESS_WORKING_SET			L"\\Process(@processName)\\Working Set"			// ���μ��� �۾� �޸�(���� �޸𸮿� ���Ǵ� ũ���� �� �Ҵ� �뷮�� �ƴ� �� ����)
#define dfQUERY_PROCESS_NON_PAGED_MEMORY	L"\\Process(@processName)\\Pool Nonpaged Bytes"	// ���μ��� �������� �޸�

#define dfQUERY_MEMORY_AVAILABLE			L"\\Memory\\Available MBytes"			// ��밡�� �޸�
#define dfQUERY_MEMORY_NON_PAGED			L"\\Memory\\Pool Nonpaged Bytes"		// ���������� �޸�
