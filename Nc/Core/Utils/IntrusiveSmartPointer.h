
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_INTRUSIVESMART_POINTER_H_
#define NC_CORE_UTILS_INTRUSIVESMART_POINTER_H_

#include "../Define.h"
#include "SmartPointer.h"

namespace Nc
{
    namespace Utils
    {
        /// Like SmartPointer, but call the methods SPAddRef()
        template<typename T, bool IsArray = false>
        class IntrusiveSmartPointer : public SmartPointer<T, IsArray>
        {
            public:
                IntrusiveSmartPointer(T *ptr = 0, bool destroyable = true) : SmartPointer<T,IsArray>::SmartPointer(ptr, destroyable)
                {
                    if (SmartPointer<T,IsArray>::_ptr != NULL)
                        AddRefPtr();
                }
                IntrusiveSmartPointer(const IntrusiveSmartPointer &sp) : SmartPointer<T,IsArray>::SmartPointer(sp)
                {
                    if (SmartPointer<T,IsArray>::_ptr != NULL)
                        AddRefPtr();
                }

                inline IntrusiveSmartPointer   CloneNoDestroyable() const  {return IntrusiveSmartPointer(SmartPointer<T,IsArray>::_ptr, false);}

            protected:
                virtual void AddRefPtr()    {SmartPointer<T>::_ptr->SPAddRef(this);}
        };
    }
}

#endif
