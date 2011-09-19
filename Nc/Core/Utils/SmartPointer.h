
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

#ifndef NC_CORE_UTILS_SMART_POINTER_H_
#define NC_CORE_UTILS_SMART_POINTER_H_

#include <typeinfo>
#include "../Define.h"
#include "Exception.h"

namespace Nc
{
    namespace Utils
    {
        /// Provide a SmartPointer class with type T and bool IsArray
        /**
            Provide a SmartPointer class with type T, and the bool IsArray to inform if the operator delete[] needs to be called insteed of the delete operator

            <h3>To use the SmartPointer, use a typedef like this :  </h3>
                typedef SmartPointer<MyClass>    SPMyClass;     <br/> <br/>
                And next create the instance like this : <br/>
                SPMyClass sp = new MyClass();
        */
        template<typename T, bool IsArray = false>
        class SmartPointer
        {
            public:
                /**
                    Construct the smartPointer with the ptr, and if `destroyable` is true delete the pointer when the number of reference is null
                */
                SmartPointer(T *ptr = 0, bool destroyable = true);
                SmartPointer(const SmartPointer &sp);
                virtual ~SmartPointer();

                // operator
                SmartPointer    &operator = (const SmartPointer &sp);
                bool            operator == (const SmartPointer &sp) const;
                bool            operator != (const SmartPointer &sp) const;
                T               &operator * () const;
                T               *operator ->() const;

                friend LCORE std::ostream &operator << (std::ostream &os, const SmartPointer &sp)
                {
                    os << sp._ptr;
                    return os;
                }

                // Accesseurs
                /** Return the pointer */
                inline T        *Get() const                            {return _ptr;}
                /** Return true if the reference is unique (no another reference on the pointer) */
                inline bool     Unique() const                          {return ((*_nbRef) == 1);}

                // modificateurs
                /** Swap tow smartPointer */
                void                    Swap(SmartPointer &sp);
                /** Clone the SmartPointer in no destroyable (the pointer is never deleted) */
                inline SmartPointer     CloneNoDestroyable() const      {return SmartPointer<T,IsArray>(_ptr, false);}

            protected:
                void Release();
                virtual void AddRefPtr() const                          {}
                virtual void ReleasePtr()                               {if (IsArray) delete[] _ptr; else delete _ptr;}

                T               *_ptr;
                unsigned int    *_nbRef;
                bool            _destroyable;
        };

        template<typename T, bool isArray>
        SmartPointer<T,isArray>::SmartPointer(T *ptr, bool destroyable)
        {
            _nbRef = new unsigned int;
            *_nbRef = 1;
            _ptr = ptr;
            _destroyable = destroyable;
            //std::cout << "construction\tshared : " << _nbRef << "\tptr : " << _ptr << "\tNB Ref = " << *_nbRef << std::endl;
        }

        template<typename T, bool isArray>
        SmartPointer<T,isArray>::SmartPointer(const SmartPointer &sp)
        {
            _nbRef = sp._nbRef;
            if (_nbRef != NULL)
                ++(*_nbRef);
            _ptr = sp._ptr;
            _destroyable = sp._destroyable;
            //std::cout << "construction\tshared : " << _nbRef << "\tptr : " << _ptr << "\tNB Ref = " << *_nbRef << std::endl;
        }

        template<typename T, bool isArray>
        SmartPointer<T,isArray>::~SmartPointer()
        {
            Release();
        }

        template<typename T, bool isArray>
        SmartPointer<T,isArray>    &SmartPointer<T,isArray>::operator = (const SmartPointer &sp)
        {
            _destroyable = sp._destroyable;
            if (_nbRef != sp._nbRef)
            {
                Release();
                _ptr = sp._ptr;
                _nbRef = sp._nbRef;
                if (_nbRef != NULL)
                    ++(*_nbRef);
                if (_ptr != NULL)
                    AddRefPtr();
            }
            //std::cout << "construction\tshared : " << _nbRef << "\tptr : " << _ptr << "\tNB Ref = " << *_nbRef << std::endl;
            return *this;
        }

        template<typename T, bool isArray>
        void SmartPointer<T,isArray>::Release()
        {
            if (_nbRef != NULL)
            {
                --(*_nbRef);
                //std::cout << "destruction\tshared : " << _nbRef << "\tptr : " << _ptr << "\tNB Ref = " << *_nbRef << std::endl;
                if (*_nbRef == 0)
                {
                    delete _nbRef;
                    if (_ptr != NULL && _destroyable)
                        ReleasePtr();
                    _nbRef = NULL;
                    _ptr = NULL;
                }
            }
        }

        template<typename T, bool isArray>
        bool SmartPointer<T,isArray>::operator != (const SmartPointer<T,isArray> &sp) const
        {
            return (_ptr != sp._ptr);
        }

        template<typename T, bool isArray>
        bool SmartPointer<T,isArray>::operator == (const SmartPointer<T,isArray> &sp) const
        {
            return (_ptr == sp._ptr);
        }

        template<typename T, bool isArray>
        T    &SmartPointer<T,isArray>::operator * () const
        {
            if (_ptr == NULL)
                throw Exception("SmartPointer", "operator*: The pointer of type " + std::string(typeid(*_ptr).name()) + " is NULL");
            return *_ptr;
        }

        template<typename T, bool isArray>
        T    *SmartPointer<T,isArray>::operator ->() const
        {
            if (_ptr == NULL)
                throw Exception("SmartPointer", "operator->: The pointer of type " + std::string(typeid(*_ptr).name()) + " is NULL");
            return _ptr;
        }

        template<typename T, bool isArray>
        void SmartPointer<T,isArray>::Swap(SmartPointer &sp)
        {
            unsigned int    *tmpNbRef = sp._nbRef;
            T               *tmpPtr = sp._ptr;

            sp._nbRef = _nbRef;
            sp._ptr = _ptr;
            _nbRef = tmpNbRef;
            _ptr = tmpPtr;
        }
    }
}

#endif
