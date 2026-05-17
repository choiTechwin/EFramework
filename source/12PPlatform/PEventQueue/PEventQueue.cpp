#include <12PPlatform/PEventQueue/PEventQueue.h>
#include <02Platform/Scheduler/Scheduler.h>

PEventQueue::PEventQueue(
    Scheduler *pScheduler,
    int nClassId, 
    const char* pcClassName)
    : EventQueue(pScheduler, nClassId, pcClassName)
{
    int result = pthread_mutex_init(&m_mutex, nullptr);
    if (result != 0) {
        throw Exception(-1, "PEventQueue::PEventQueue");
    }
    result = sem_init(&m_semaphoreFull, 0, MAXLENGTH_EVENTQUEUE);
    if (result != 0) {
        pthread_mutex_destroy(&m_mutex);
        throw Exception(-1, "PEventQueue::PEventQueue");
    }
    result = sem_init(&m_semaphoreEmpty, 0, 0);
    if (result != 0) {
        sem_destroy(&m_semaphoreFull);
        pthread_mutex_destroy(&m_mutex);
        throw Exception(-1, "PEventQueue::PEventQueue");
    }
}

void PEventQueue::PushLock() {
    sem_wait(&m_semaphoreFull);
    pthread_mutex_lock(&m_mutex);
}

void PEventQueue::PushUnlock() {
    pthread_mutex_unlock(&m_mutex);
    sem_post(&m_semaphoreEmpty);
}

void PEventQueue::PopLock() {
    sem_wait(&m_semaphoreEmpty);
    pthread_mutex_lock(&m_mutex);
}

void PEventQueue::PopUnlock() {
    pthread_mutex_unlock(&m_mutex);
    sem_post(&m_semaphoreFull);
}

PEventQueue::~PEventQueue() 
{
    sem_destroy(&m_semaphoreEmpty);
    sem_destroy(&m_semaphoreFull);
    pthread_mutex_destroy(&m_mutex);
}
