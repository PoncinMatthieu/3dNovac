
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        05/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_MASK_H_
#define NC_CORE_UTILS_MASK_H_

#include "../Define.h"

namespace Nc
{
    namespace Utils
    {
        /// to manipulate a bit mask easily
        /**
            The bit field used a type T wich should be an enum and D to specify the sizeof of the mask

            <div class="title">Sample code to used a Mask:</div>
 \code
    // enum to define the possible values of the mask
    enum Values
    {
        Bit1 = 1 << 0,
        Bit2 = 1 << 1,
        Bit3 = 1 << 2,
        Bit4 = 1 << 3
    };

    // create and use a mask:
    Mask<Values>    mask = Bit1 | Bit2;     // create the mask with the Bit1 and Bit2 enabled

    if (mask.Enabled(Bit2))     // if the Bit2 is enabled, then disable it
    {
        mask.Disable(Bit2);
    }
 \endcode
        */
        template<typename T, typename D = int>
        class Mask
        {
            public:
                Mask() : _mask(0)       {}
                Mask(D m) : _mask(m)    {}
                Mask(T v) : _mask(v)    {}

                /** Enable the bit `v` in the mask */
                void    Enable(T v)             {_mask |= v;}

                /** Disable the bit `v` in the mask */
                void    Disable(T v)            {_mask &= ~v;}

                /** Trigger the bit `v` in the mask (if == true, then false, else true) */
                void    Trigger(T v)            {(Enabled(v)) ? Disable(v) : Enable(v);}

                /** \return true if the bit `v` is enabled in the bit mask  */
                bool    Enabled(T v) const      {return ((_mask & v) != 0);}

                /** \return true if the bit `v` is disabled in the bit mask */
                bool    Disabled(T v) const     {return ((_mask & v) == 0);}

            private:
                D       _mask;
        };
    }
}

#endif
