
#include <stdio.h>
#include <Exception.h>
#include <19PApplication/PAppMain/PAppMain.h>

int main() {

	try {
		PAppMain *pPAppMain = new("PAppMain") PAppMain();
		pPAppMain->main_ex();
		delete pPAppMain;
	}
	catch (Exception& exception) {
		exception.Println();
	}

	printf("Hello World1\n");
	printf("Hello World2\n");
	printf("Hello World3\n");
	printf("Hello World4\n");
	printf("Hello World5\n");		

	return 0;
}