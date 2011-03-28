
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

#ifndef NC_CORE_UTILS_SYSTEN_ITHREAD_H_
#define NC_CORE_UTILS_SYSTEN_ITHREAD_H_

#include "../../Utils/NonCopyable.h"

namespace Nc
{
    namespace System
    {
        /// Interface of for using a Thread in a subclass
        /**
            <h3> Herite to the class Thread to create a thread like this : </h3>
            class Engine : public Utils::System::Thread                         <br/>
            {                                                                   <br/>
                void Run() {while(1);}                                          <br/>
            }                                                                   <br/>

            <h3>To Create the thread :  </h3>
            Engine e;                   <br/>
            e.Start();                  <br/>
            // do something             <br/>
            e.Wait();                   <br/>
        */
        class LCORE IThread : public Utils::NonCopyable
        {
            public:
                IThread()            {}
                virtual ~IThread()   {};

            /**
                Create and immediatly start the thread. <br/>
                The virtual function Run() will be called in the thread after that
            */
                virtual void Start() = 0;

            /**
                Wait a thread (pause the current thread until the end of the execution of the thread).
            */
                virtual void Wait() = 0;

            protected:
            /**
                virtal pure methode, to redefine in a subclass. <br/>
                After that the Start methode is call, the methode Run will be called in the thread
            */
                virtual void Run() = 0;

            /** Exit the current thread */
                virtual void Exit() = 0;
        };
    }
}

#endif
