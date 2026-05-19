#include <19PApplication/PAppMain/PAppMain.h>

#include <19PApplication/PAppMain/Config.h>
#include <13PTechnical/PMemoryManager/PMemoryManager.h>
#include <19PApplication/PLifecycleManager/PLifecycleManager.h>
#include <12PPlatform/PEventQueue/PEventQueue.h>

#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

PAppMain::PAppMain(unsigned uClassId,
	const char* pcClassName)
	: AppMain(new("PMain::PEventQueue") PEventQueue(this), uClassId, pcClassName)
{
}
PAppMain::~PAppMain() {
	delete this->GetPEventQueue();
}

void PAppMain::BootstrapSystem() {
	LOG_HEADER("PMain::BootstrapSystem");
	// Lifecycle Manager
	PLifecycleManager *pPLifecycleManager = new("PLifecycleManager") PLifecycleManager();
	this->SetPLifecycleManager(pPLifecycleManager);
	this->AllocateAComponent(pPLifecycleManager);
	// root event
	this->SendNoReplyEvent(this->GetUId(), (unsigned)IAppMain::EEventType::eInitializeAsAMain, 0, nullptr);

	LOG_FOOTER("PMain::BootstrapSystem");
}

void PAppMain::RunAsAMain() {
	AppMain::RunAsAMain();
}

void PAppMain::ShutdownSystem() {
	delete this->GetPLifecycleManager();
}

///////////////////////////////////////////////
int PAppMain::main_ex() {
	PMemoryManager::Allocate(SIZE_MEMORY_APPLICATION, SIZE_PAGE, SIZE_SLOT_UNIT);

	PAppMain* pPAppMain = new("PAppMain") PAppMain();
	pPAppMain->BootstrapSystem();
	pPAppMain->RunAsAMain();
	pPAppMain->ShutdownSystem();
	delete pPAppMain;

	PMemoryManager::Delocate();
	return 0;
}
///////////////////////////////////////////////
