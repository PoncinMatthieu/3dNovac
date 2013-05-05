
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        28/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_ENUM_H_
#define NC_GUI_ENUM_H_

namespace Nc
{
    namespace GUI
    {
        /// enum used to describe alignment. It contains horizontal and vertical flags that can be combined to produce the required effect.
        enum Alignment
        {
            Left        = 1 << 1,               ///< Alignment with the left edge.
            CenterH     = 1 << 2,               ///< Centers horizontally in the available space.
            Right       = 1 << 3,               ///< Alignment with the right edge.
            Bottom      = 1 << 4,               ///< Alignment with the bottom.
            CenterV     = 1 << 5,               ///< Centers vertically in the available space.
            Top         = 1 << 6,               ///< Alignment with the top.
            Center      = CenterH | CenterV     ///< Centers horizontally and vertically.
        };

        /// typedef to use a mask of alignment.
        typedef Utils::Mask<Alignment>     AlignmentMask;

        /// used to define an object orientation.
        enum Orientation
        {
            Horizontal  = 1 << 0,
            Vertical    = 1 << 1
        };

        /// typdef to use a mask of orientation.
        typedef Utils::Mask<Orientation>   OrientationMask;

        /// Allow to define edge sizes in a box.
        struct LIB_NC_GUI BoxEdges
        {
            BoxEdges()
                : left(0), right(0), top(0), bottom(0)
            {}
            BoxEdges(int l, int r, int t, int b)
                : left(l), right(r), top(t), bottom(b)
            {}

            bool operator != (const BoxEdges &b)
            {
                return (left != b.left || right != b.right || top != b.top || bottom != b.bottom);
            }

            void Init(int left, int right, int top, int bottom)
            {
                this->left = left;
                this->right = right;
                this->top = top;
                this->bottom = bottom;
            }

            int     left;
            int     right;
            int     top;
            int     bottom;
        };

        namespace Event
        {
            enum EventId
            {
                EnterFocus = 0,
                LeaveFocus,
                Toggled,
                ItemChanged,
                EventIdMax,
                ReturnPressed,
                TextEntered,
            };
        }
    }
}

#endif
