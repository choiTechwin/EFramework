#pragma once

#include <13PTechnical/typedef.h>
#define _PMemoryStatic_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryStatic)
#define _PMemoryStatic_Name "PMemoryStatic"

#include <03Technical/MemoryManager/MemoryStatic.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class PMemoryStatic : public MemoryStatic {
private:
//	CRITICAL_SECTION CriticalSection;
#ifdef _WIN32
	CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif

private:
	void Lock();
	void UnLock();

public:
	PMemoryStatic(
		unsigned nClassId = _PMemoryStatic_Id,
		const char* pcClassName = _PMemoryStatic_Name);
	~PMemoryStatic() override;

	void Initialize() override;
	void Finalize() override;

	void* Malloc(size_t szObject, const char* sMessage);
	bool Free(void* pObject);
};