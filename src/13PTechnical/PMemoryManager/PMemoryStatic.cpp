#include <13PTechnical/PMemoryManager/PMemoryStatic.h>

void PMemoryStatic::Lock() {
#ifdef _WIN32
	EnterCriticalSection(&m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}
void PMemoryStatic::UnLock() {
#ifdef _WIN32
	LeaveCriticalSection(&m_mutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}

PMemoryStatic::PMemoryStatic(
	unsigned nClassId,
	const char* pcClassName)
	: MemoryStatic(nClassId, pcClassName)
{
#ifdef _WIN32
	InitializeCriticalSection(&m_mutex);
#else
	pthread_mutex_init(&m_mutex, nullptr);
#endif
}
PMemoryStatic::~PMemoryStatic() {
#ifdef _WIN32
	DeleteCriticalSection(&m_mutex);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void PMemoryStatic::Initialize() {
	MemoryStatic::Initialize();
}
void PMemoryStatic::Finalize() {
	MemoryStatic::Finalize();
}


void* PMemoryStatic::Malloc(size_t szObject, const char* sMessage)
{
    Lock();
    void* pObject = MemoryStatic::Malloc(szObject, sMessage);
    UnLock();
    return pObject;
}

bool PMemoryStatic::Free(void* pObject) {
    Lock();
    bool result = MemoryStatic::Free(pObject);
    UnLock();
    return result;
}