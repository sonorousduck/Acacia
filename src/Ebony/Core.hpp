#pragma once


#ifdef EB_PLATFORM_WINDOWS
	#ifdef EB_BUILD_DLL
		#define EBONY_API __declspec(dllexport)
	#else
		#define EBONY_API __declspec(dllimport)
	#endif
#else
	#error Ebony only supports Windows for now
#endif


namespace Ebony {


}