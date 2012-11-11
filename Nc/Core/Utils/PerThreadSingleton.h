
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

    File Created At:        10/11/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_PERTHREADSINGLETON_H_
#define NC_CORE_UTILS_PERTHREADSINGLETON_H_

#include "../Define.h"
#include "../System/API/API.h"

namespace Nc
{
    namespace Utils
    {
        /// Implement a template singleton class that has one instance for each thread.
        /**
            The class use the thread id to identify the thread.

            \sa Singleton
        */
        template<class T>
        class PerThreadSingleton
        {
            public:
                typedef std::map<unsigned int, T*>      MapInstance;

            public:
                /**
                    Static function which return the instance of the singleton T.
                    \return the instance of the singleton of type T.
                */
                static T &Instance()
                {
                    System::Locker l(&_mutex);
                    unsigned int id = System::ThreadId();
                    typename MapInstance::iterator it = _instances.find(id);
                    if (it == _instances.end())
                    {
                        T *instance = new T();
                        _instances[id] = instance;
                        return *instance;
                    }
                    return *it->second;
                }

                /**
                    Delete the instance.
                    If this function is not called before the program exit, the data will be not released and appears in valgrind.
                    It's totally normal and not ugly with a static which exist during the all execution of the program.
                */
                static void DeleteInstance()
                {
                    System::Locker l(&_mutex);
                    typename MapInstance::iterator it = _instances.find(System::ThreadId());
                    if (it == _instances.end())
                    {
                        delete it->second;
                        _instances.erase(it);
                    }
                }

                /** \return true if the instance is already created. */
                static bool Exist()
                {
                    System::Locker l(&_mutex);
                    typename MapInstance::iterator it = _instances.find(System::ThreadId());
                    return (it != _instances.end());
                }

                /**
                    Allow to setup your own instance.
                    if an instance is already set, the instance will be deleted.
                    \warning Be aware that the instance will be delete by the class if the method "DeleteInstance" is call.
                */
                template<class I>
                static void SetInstance(I *instance)
                {
                    DeleteInstance();
                    System::Locker l(&_mutex);
                    _instances[System::ThreadId()] = instance;
                }

                /**
                    \return every instance of the singleton.
                    \sa Instance
                */
                static const MapInstance        &Instances()                {return _instances;}

                /**
                    Delete Every Instance of the singleton.
                    \sa DeleteInstance
                */
                static void                     DeleteInstances()
                {
                    System::Locker l(&_mutex);
                    for (typename MapInstance::iterator it = _instances.begin(); it != _instances.end(); ++it)
                        delete it->second;
                    _instances.clear();
                }

            protected:
                PerThreadSingleton()                 {}
                virtual ~PerThreadSingleton()        {}

                static MapInstance          _instances;     ///< instances of type T.
                static System::Mutex        _mutex;         ///< protect the instance for multithread context.
        };

        template<typename T>
        typename PerThreadSingleton<T>::MapInstance     PerThreadSingleton<T>::_instances;

        template<typename T>
        System::Mutex                                   PerThreadSingleton<T>::_mutex;
    }
}

#endif
