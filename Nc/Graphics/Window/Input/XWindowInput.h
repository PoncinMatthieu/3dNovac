
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

    File Created At:        07/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Provide an implementation of WindowInput for a server X

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_X_WINDOW_INPUT_H_
#define NC_GRAPHICS_WINDOW_X_WINDOW_INPUT_H_

#include "WindowInput.h"
#include <X11/Xlib.h>

namespace Nc
{
    namespace Graphic
    {
        class XWindow;

        class LCORE XWindowInput : public WindowInput
        {
            public:
                XWindowInput(XWindow *win);
                virtual ~XWindowInput();

                void Create();
                void CheckEvents();
                void ProcessEvent(XEvent &event);

            private:
                System::Key::Code   ConvertKeySym(KeySym Sym);

                XIM         _inputMethod;
                XIC         _inputContext;
                Atom        _atomClose; // used to identify the close event
                XEvent      _lastKeyReleaseEvent;
        };
    }
}

#endif
