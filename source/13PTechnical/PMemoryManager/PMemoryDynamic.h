#pragma once

#include <13PTechnical/typedef.h>
#define _PMemoryDynamic_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryDynamic)
#define _PMemoryDynamic_Name "PMemoryDynamic"

#include <03Technical/MemoryManager/MemoryDynamic.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class PMemoryDynamic : public MemoryDynamic {
private:
//	CRITICAL_SECTION CriticalSection;
#ifdef _WIN32
	CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif

protected:
	void Lock();
	void UnLock();

public:
	PMemoryDynamic(
		unsigned nClassId = _PMemoryDynamic_Id,
		const char* pcClassName = _PMemoryDynamic_Name);
	~PMemoryDynamic() override;

	void Initialize(int szPage, int szSlotUnit);
	void Finalize() override;

	void* Malloc(size_t szObject, const char* sMessage);
	bool Free(void* pObject);
};