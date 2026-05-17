#pragma once

#include <12PPlatform/typedef.h>
#define _PEventQueue_Id _GET_CLASS_UID(_EPPlatform::_ePEventQueue)
#define _PEventQueue_Name "PEventQueue"

#include <02Platform/EventQueue/EventQueue.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <pthread.h>
#include <semaphore.h>

#define MAXLENGTH_EVENTQUEUE 20

class Scheduler;

class PEventQueue: public EventQueue {
private:
	pthread_mutex_t m_mutex;
	sem_t m_semaphoreFull;
	sem_t m_semaphoreEmpty;

protected:
	void PushLock() override;
	void PushUnlock() override;
	void PopLock() override;
	void PopUnlock() override;
public:
	PEventQueue(
		Scheduler *pScheduler = nullptr,
		int nClassId = _PEventQueue_Id, 
		const char* pcClassName = _PEventQueue_Name);
	virtual ~PEventQueue();
};
