#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

PMemoryDynamic::PMemoryDynamic(
	unsigned nClassId,
	const char* pcClassName)
	: MemoryDynamic(nClassId, pcClassName)
{
#ifdef _WIN32
	InitializeCriticalSection(&m_mutex);
#else
	pthread_mutex_init(&m_mutex, nullptr);
#endif
}
PMemoryDynamic::~PMemoryDynamic() {
#ifdef _WIN32
	DeleteCriticalSection(&m_mutex);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void PMemoryDynamic::Initialize(int szPage, int szSlotUnit) {
	MemoryDynamic::Initialize(szPage, szSlotUnit);
}
void PMemoryDynamic::Finalize() {
	MemoryDynamic::Finalize();
}

void PMemoryDynamic::Lock() {
#ifdef _WIN32
	EnterCriticalSection(&m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}
void PMemoryDynamic::UnLock() {
#ifdef _WIN32
	LeaveCriticalSection(&m_mutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}

void* PMemoryDynamic::Malloc(size_t szObject, const char* sMessage)
{
    Lock();
    void* pObject = MemoryDynamic::Malloc(szObject, sMessage);
    UnLock();
    return pObject;
}
bool PMemoryDynamic::Free(void* pObject) {
    Lock();
    bool result = MemoryDynamic::Free(pObject);
    UnLock();
    return result;
}