#pragma once


#ifdef EB_BUILD_DLL
	#define EBONY_API __declspec(dllexport)
#else
	#define EBONY_API __declspec(dllimport)
#endif


#define BIT(x) (1 << x)


namespace Ebony {


}