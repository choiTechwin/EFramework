#pragma once

#include <19PApplication/typedef.h>
#define _PAppMain_Id _GET_CLASS_UID(_ELayer_PApplication::_ePAppMain)
#define _PAppMain_Name "PAppMain"

#include <09Application/AppMain/AppMain.h>

class PAppMain : public AppMain
{
public:
	static int main_ex();

public:
	PAppMain(unsigned uClassId = _PAppMain_Id,
		const char* pcClassName = _PAppMain_Name);
	virtual ~PAppMain();

	// as a Component
	void BootstrapSystem() override;
	void RunAsAMain() override;
	void ShutdownSystem() override;
};