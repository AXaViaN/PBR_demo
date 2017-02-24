/**
 *	File: AXengine/Tool/Time.h
 *	Purpose: Provide time info for movements, animations etc.
 */

#ifndef __AX__TOOL__TIME_H
#define __AX__TOOL__TIME_H

#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Window;
} }

namespace AX { namespace Tool {

class Time : public Singleton<Time> {
public:
	Time() : deltaTime(0) { }

	/**
	 * @returns Time between frames in seconds
	 */
	static const F32& GetDeltaTime() { return instance->deltaTime; }

protected:
	friend class Core::Window;
	F32 deltaTime;

};

} } // namespace AX::Tool

#endif // __AX__TOOL__TIME_H
