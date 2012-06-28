
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

    File Created At:        22/06/201é
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_INPUT_SUBWINDOWINPUT_H_
#define NC_GRAPHICS_WINDOW_INPUT_SUBWINDOWINPUT_H_

#include "WindowInput.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define an input to be used with a SubWindow
        /**
            \sa
                - WindowInput
                - SubWindow
        */
        class LGRAPHICS SubWindowInput : public WindowInput
        {
            public:
                SubWindowInput(Window *win, WindowInput *parent);
                virtual ~SubWindowInput();

                /** Create and init the input */
                virtual void Create();

            private:
                WindowInput     *_parent;       ///< parent of the SubWindowInput
        };
    }
}

#endif
