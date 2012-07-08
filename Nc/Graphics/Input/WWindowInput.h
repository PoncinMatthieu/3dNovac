
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        14/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

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
    namespace Graphic
    {
        class Window;

        /// Implementation of a WindowInput for a Win32 System
        /**
            For more detail show WindowInput
        */
        class LGRAPHICS WWindowInput : public WindowInput
        {
            public:
                WWindowInput(Graphic::Window *win);
                virtual ~WWindowInput();

                virtual void Create();
				virtual void Destroy();
				virtual void CheckEvents();

                static LRESULT CALLBACK GlobalOnEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

            private:
                void ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam);
                System::Key::Code ConvertVirtualKey(WPARAM VirtualKey, LPARAM Flags);

                long    _callback;
                bool    _isCursorIn;
                bool    _keyRepeatEnabled;
        };
    }
}

#endif
