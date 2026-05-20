#include <09Application/AppMain/AppMain.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

//----------------------------------------------------------------------------
void AppMain::RegisterEventTypes() {
	Directory::s_dirEvents[(int)IAppMain::EEventType::eInitializeAsAMain] = "eInitializeAsAMain";
	Directory::s_dirEvents[(int)IAppMain::EEventType::eRunAsAMain] = "eRunAsAMain";
	Directory::s_dirEvents[(int)IAppMain::EEventType::eFinalizeAsAMain] = "eFinalizeAsAMain";
}

void  AppMain::RegisterExceptions() {
//	REGISTER_EVENT(IMain, eEventTypeError);
}
//----------------------------------------------------------------------------

AppMain::AppMain(
	EventQueue *pEventQueue,
	unsigned uClassId,
	const char* pcClassName)
	: Scheduler(pEventQueue, uClassId, pcClassName)
	, m_pLifecycleManager(nullptr)
{
	this->RegisterEventTypes();
	this->RegisterExceptions();
}
AppMain::~AppMain() {
	Directory::s_dirComponents.Clear();
	Directory::s_dirEvents.Clear();
	Directory::s_dirExceptions.Clear();
}

void AppMain::RunAsAMain() {
	Scheduler::RunAsAScheduler();
}

void AppMain::InitializeAsAMain(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->GetReplyType() == (int)ILifecycleManager::EEventType::eInitializeAsALifecycleManager) {
			LOG_FOOTER("Main::InitializeAsAMain", "Send->m_pLifecycleManager(eInitializeAsALifecycleManager)");
			this->SendReplyEvent(this->m_pLifecycleManager->GetUId(), (int)ILifecycleManager::EEventType::eStartSystem);
			LOG_HEADER("Main::InitializeAsAMain", "Send->m_pLifecycleManager(eStartSystem)");
		} else if (pEvent->GetReplyType() == (int)ILifecycleManager::EEventType::eStartSystem) {
			this->SendNoReplyEvent(this->GetUId(), (int)IAppMain::EEventType::eFinalizeAsAMain);
			LOG_FOOTER("Main::InitializeAsAMain", "Send->Main(eFinalizeAsAMain)");
		} else {
			throw Exception ((unsigned)IComponent::EException::eEventNotSupported, "Main", "InitializeAsAMain", pEvent->GetType());
		}
	}
	else {
		LOG_HEADER("Main::InitializeAsAMain", "Send->m_pLifecycleManager(eInitializeAsALifecycleManager)");
		// send a trigger event
		ILifecycleManager::ParamInitializeAsALifecycleManager* pParamParamInitializeAsALifecycleManager
			= new("ParamInitializeAsALifecycleManager") ILifecycleManager::ParamInitializeAsALifecycleManager(this);
		this->SendReplyEvent(
			this->m_pLifecycleManager->GetUId()
			, (int)ILifecycleManager::EEventType::eInitializeAsALifecycleManager
			, undefined
			, pParamParamInitializeAsALifecycleManager
		);
	}

}

void AppMain::FinalizeAsAMain(Event* pEvent) {
	if (pEvent->IsReply()) {
		Scheduler::StopAsAScheduler();
		LOG_FOOTER("Main::FinalizeAsAMain");
	}
	else {
		LOG_HEADER("Main::FinalizeAsAMain");
		this->SendReplyEvent(this->m_pLifecycleManager->GetUId(),
			(unsigned)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager);
	}
}

void AppMain::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (unsigned)IAppMain::EEventType::eInitializeAsAMain:
		this->InitializeAsAMain(pEvent);
		break;
	case (unsigned)IAppMain::EEventType::eFinalizeAsAMain:
		this->FinalizeAsAMain(pEvent);
		break;
	default:
		Scheduler::ProcessAEvent(pEvent);
		break;
	}
}

