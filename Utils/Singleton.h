
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

#ifndef NC_CORE_UTILS_SINGLETON_H_
#define NC_CORE_UTILS_SINGLETON_H_

#include "../Define.h"
//#include "../System/API/API.h"
#if defined(SYSTEM_WINDOWS)
    #include "../System/API/Windows/Mutex.h"
#elif defined(SYSTEM_LINUX)
    #include "../System/API/Unix/Mutex.h"
#else
    #error "Thread API not implemented for this system."
#endif

namespace Nc
{
    namespace Utils
    {
        /// Implement a template singleton class.
        /**
            The singleton class is thread safe.

            <div class="title"> To create a singleton class, create a public heritance like this: </div>
 \code
    class MySingleton : public Utils::Singleton<MySingleton>
    {
        void function()
        {
            cout << "hello" << endl;
        }
    }
 \endcode

            <div class="title"> To access to MySingleton instance anywere in your code: </div>
 \code
    MySingleton::Instance()->function();
 \endcode


            \todo [Feature] Add a ISingleton class to register every instances of Singleton<T> and be able to delete every singleton at exit to clean the memory.
        */
        template <typename T>
        class   Singleton
        {
            public:
                /**
                    Static function which return the instance of the singleton T.
                    \return the instance of the singleton of type T.
                */
                static T &Instance()
                {
                    if (_instance == NULL)
                    {
                        // synchronize the threads access, only once for creation, so in a second if != NULL
                        System::Locker l(&_mutex);
                        if (_instance == NULL)
                            _instance = new T();
                    }
                    return *_instance;
                }

                /**
                    Delete the instance.
                    If this function is not called before the program exit, the data will be not released and appears in valgrind.
                    It's totally normal and not ugly with a static which exist during the all execution of the program.
                */
                static void DeleteInstance()
                {
                    System::Locker l(&_mutex);
                    if (_instance != NULL)
                        delete _instance;
                    _instance = NULL;
                }

                /** \return true if the instance is already created. */
                static bool Exist()
                {
                    System::Locker l(&_mutex);
                    return (_instance != NULL);
                }

                /**
                    Allow to setup your own instance.
                    if an instance is already set, the instance will be deleted.
                    \warning Be aware that the instance will be delete by the class if the method "DeleteInstance" is call.
                */
                static void SetInstance(T *instance)
                {
                    DeleteInstance();
                    System::Locker l(&_mutex);
                    _instance = instance;
                }

            protected:
                Singleton()                 {_instance = NULL;}
                virtual ~Singleton()        {_instance = NULL;}

                static T                *_instance;     ///< instance of type T.
                static System::Mutex    _mutex;         ///< protect the instance for multithread context.
        };

        template<typename T>
        T               *Singleton<T>::_instance = NULL;

        template<typename T>
        System::Mutex	Singleton<T>::_mutex;
    }
}

#endif
