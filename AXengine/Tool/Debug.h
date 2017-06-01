/**
 *	File: AXengine/Tool/Debug.h
 *	Purpose: Logger
 *	
 *	You can log info messages with LogInfo(message, ...)
 *	and warning messages with LogWarning(message, ...).
 *	They are both macros but in the AX::Tool::Debug namespace.
 *	
 *	Warning messages will always be logged. If you define
 *	__AX_DEBUG_INFO_MODE__ as 1, info messages will be logged too.
 *	
 *	You can redirect your logs to a file if you call SetLogTarget().
 *	But then you must call CloseLogTarget() after you finished.
 */

#ifndef __AX__TOOL__DEBUG_H
#define __AX__TOOL__DEBUG_H

#define __AX_DEBUG_INFO_MODE__ 1

#include "AXengine/Tool/Utility.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cstring>
#include <SDL2/SDL.h>

namespace AX { namespace Tool {

#ifdef _MSC_VER
	#define __LOG_ARGS__ __FUNCTION__, __LINE__
#else
	#define __LOG_ARGS__ __PRETTY_FUNCTION__, __LINE__
#endif

#if (__AX_DEBUG_INFO_MODE__ == 1)
	#define LogInfo(message, ...) __log(__LOG_ARGS__, "Info = " message, ##__VA_ARGS__)
#else
	#define LogInfo(message, ...) __dont_log()
#endif

#define LogWarning(message, ...) __log(__LOG_ARGS__, "Warning = " message, ##__VA_ARGS__)

class Debug {
public:
	/**
	 * Redirect logs to a file
	 * 
	 * @param filePath Path to log file
	 * @param startMessage Message to write with date info
	 */
	static void SetLogTarget(const CHR* filePath, const CHR* startMessage)
	{
		std::freopen(filePath, "a", stderr);

		fseek(stderr, 0, SEEK_END);
		if(std::ftell(stderr) > 10000000)
			std::freopen(filePath, "w", stderr);
		
		std::fprintf(stderr, "\n------------------------------------------------------\n\n");
		
		auto time = std::time(nullptr);
		auto tm_info = std::localtime(&time);

		CHR date[26];
		std::strftime(date, 26, "%d-%m-%Y %H:%M:%S", tm_info);

		std::fprintf(stderr, "%s - %s\n", startMessage, date);
	}
	static void CloseLogTarget()
	{
		std::fclose(stderr);
	}
	
	static void __log(const CHR* function, U32 line, const CHR* message, ...)
	{
		std::va_list argv;
		va_start(argv, message);
		std::vfprintf(stderr, message, argv);
		va_end(argv);
		
		#ifdef _MSC_VER
			std::fprintf(stderr, " {%s::%d} [%ums]\n", function, line, SDL_GetTicks());
		#else
			U32 lastIndex = std::strlen(function) - 1;
			while(function[lastIndex--] != '(');
			lastIndex++;
			
			U32 firstIndex = lastIndex;
			while(function[firstIndex--] != ' ');
			firstIndex+=2;
			
			CHR func[lastIndex - firstIndex + 1];
			std::strncpy(func, function+firstIndex, lastIndex - firstIndex);
			func[lastIndex - firstIndex] = '\0';
			
			std::fprintf(stderr, " {%s::%d} [%ums]\n", func, line, SDL_GetTicks());
		#endif

		std::fflush(stderr);
	}
	static void __dont_log() { }
	
};

} } // namespace AX::Tool

#endif // __AX__TOOL__DEBUG_H
