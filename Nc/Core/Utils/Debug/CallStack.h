
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

    File Created At:        010/11/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_DEBUG_CALLSTACK_H_
#define NC_CORE_UTILS_DEBUG_CALLSTACK_H_

#include "../Logger.h"
#include "../PerThreadSingleton.h"

#ifdef _DEBUG
    #ifndef CALLSTACK_INFO
        #define CALLSTACK_INFO(info)        Nc::Utils::CallStackInfo callStackInfo(info);
    #endif
#endif

namespace Nc
{
    namespace Utils
    {
        /// Help to register an info into the call stack using the RAII principle.
        struct CallStackInfo
        {
            CallStackInfo(const std::string &info);
            ~CallStackInfo();
        };

        /// Allow to add info on the current call stack.
        /**
            The CallStack can be reimplemented if needed.

            \warning The CallStack has one instance per thread.
        */
        class CallStack : public PerThreadSingleton<CallStack>
        {
            public:
                CallStack();
                virtual ~CallStack();

                /** Push an info stack. */
                void            Push(const std::string &info);
                /** Pop the current info stack. */
                void            Pop();

                /** \return true if the stack is empty. */
                bool            Empty() const           {return _stack.empty();}

                /** Print the current info stack. */
                virtual void    Print() const;

            private:
                std::vector<std::string>    _stack;     ///< info stack

            protected:
                friend /*LCORE*/ class PerThreadSingleton<CallStack>; // pour avoir acces a l'instance du singleton
        };

        // explicit instanciation
        template class PerThreadSingleton<CallStack>;
    }
}

#endif
