
/**-------------------------------------------------------------------------------

	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        14/12/2010

--------------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------------

    Provide an implementation of WindowInput for a windows platform

--------------------------------------------------------------------------------*/

#ifndef NC_SYSTEM_WIN32_WINDOW_INPUT_H_
#define NC_SYSTEM_WIN32_WINDOW_INPUT_H_

#include "../Input/WindowInput.h"
#include <Windows.h>

#ifndef VK_OEM_PERIOD	
	#define VK_OEM_PERIOD	0xBE
#endif
#ifndef VK_OEM_COMMA
	#define VK_OEM_COMMA	0xBC
#endif
#ifndef VK_OEM_PLUS
	#define VK_OEM_PLUS		0xBB
#endif

namespace Nc
{
    namespace System
    {
        class WWindow;

        class LSYSTEM WWindowInput : public WindowInput
        {
            public:
                WWindowInput(WWindow *win);
                virtual ~WWindowInput();

                virtual void Create();
                virtual void CheckEvents();

                static LRESULT CALLBACK GlobalOnEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

            private:
                void ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam);
                Key::Code ConvertVirtualKey(WPARAM VirtualKey, LPARAM Flags);

                long    _callback;
                bool    _isCursorIn;
                bool    _keyRepeatEnabled;
        };
    }
}

#endif
