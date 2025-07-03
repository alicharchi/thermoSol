#pragma once

#ifdef _WIN32
#   ifdef PHASEEQUIL_EXPORTS
#		define  PHASEEQUIL_API __declspec(dllexport)
#	else
#		define  PHASEEQUIL_API __declspec(dllimport)
#	endif
#elif __GNUC__ >= 4
#   define PHASEEQUIL_API __attribute__((visibility("default")))
#else
#   define PHASEEQUIL_API
#endif