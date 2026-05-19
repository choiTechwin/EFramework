#pragma once

#include <09Application/typedef.h>
#define _AppMain_Id _GET_CLASS_UID(_ELayer_Application::_eAppMain)
#define _AppMain_Name "AppMain"


class IAppMain {
public:
	enum class EEventType {
		eBegin = _AppMain_Id,
		eInitializeAsAMain,
		eRunAsAMain,
		eFinalizeAsAMain,
		eEnd
	};
};