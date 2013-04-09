
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

#include <assert.h>
#include "../Logger.h"
#include "../PerThreadSingleton.h"

#if defined(_DEBUG)
    #ifndef CALLSTACK_INFO
        #define CALLSTACK_INFO()                Nc::Utils::CallStackInfo callStackInfo(__ASSERT_FUNCTION);
        #define CALLSTACK_INFO_BLOCK(info)      Nc::Utils::CallStackInfo callStackInfo(info);
        #define CALLSTACK_APPEND_INFO(info)     callStackInfo.AppendInfo(info);
        #define CALLSTACK_INFO_ARG(args)        CALLSTACK_INFO(); CALLSTACK_APPEND_INFO(args);
    #endif
#else
    #ifndef CALLSTACK_INFO
        #define CALLSTACK_INFO()
        #define CALLSTACK_INFO_BLOCK(info)
        #define CALLSTACK_APPEND_INFO(info)
        #define CALLSTACK_INFO_ARG(args)        CALLSTACK_INFO(); CALLSTACK_APPEND_INFO(args);
    #endif
#endif

namespace Nc
{
    namespace Utils
    {
        /// Help to register an info into the call stack using the RAII principle.
        struct CallStackInfo
        {
            CallStackInfo(const std::string &funcName);
            ~CallStackInfo();

            void    AppendInfo(const std::string &info);
        };

        /// Allow to add info on the current call stack.
        /**
            The CallStack can be reimplemented if needed.

            \todo add a method to track function calls. (adding strings to track, if the string show up, print the call. eventualy with some printing parameters).

            \warning The CallStack has one instance per thread.
        */
        class CallStack : public PerThreadSingleton<CallStack>
        {
            public:
                CallStack();
                virtual ~CallStack();

                /** Push an info stack. */
                void            Push(const std::string &info);
                /** Append an info to the current stack. */
                void            Append(const std::string &info);
                /** Pop the current info stack. */
                void            Pop();

                /** \return the current info on the stack. */
                const std::string &Back()               {return _stack.back();}

                /** \return true if the stack is empty. */
                bool            Empty() const           {return _stack.empty();}
                /** \return the size of the stack. */
                unsigned int    Size() const            {return _stack.size();}

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
