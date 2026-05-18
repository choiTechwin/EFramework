#include <92TestApplication/PTestMain/PTestMain.h>
#include <91TestPlatform/TestAspect/TestException.h>

#include <stdio.h>

int main() {
	try {
		PTestMain *pPTestMain = new("PTestMain") PTestMain();
		pPTestMain->BeforeInitialize();
		pPTestMain->Initialize();
		pPTestMain->BeforeRun();
		pPTestMain->Run();
		pPTestMain->AfterRun();
		pPTestMain->Finalize();
		pPTestMain->AfterFinalize();
		delete pPTestMain;
	}
	catch (TestException& exception) {
		exception.Println();
	}
	printf("Hello World!\n");
	return 0;
}