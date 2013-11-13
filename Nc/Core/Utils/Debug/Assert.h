
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

#ifndef NC_CORE_UTILS_DEBUG_ASSERT_H_
#define NC_CORE_UTILS_DEBUG_ASSERT_H_

#include <assert.h>
#include "../../Define.h"
#include "CallStack.h"
#include "CrashReporter.h"

#ifndef Assert
    #ifdef	NDEBUG
        #define Assert(expr)		(__ASSERT_VOID_CAST (0))
    #else
        #ifdef SYSTEM_WINDOWS
            #define Assert(expr)    ((expr) ? static_cast<void>(0) : Nc::Utils::AssertFailed(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))
        #else
            #define Assert(expr)    ((expr) ? static_cast<void>(0) : Nc::Utils::AssertFailed(__STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))
        #endif
    #endif
#endif

#ifndef AssertError
    #ifdef	NDEBUG
        #define Assert(expr)		(__ASSERT_VOID_CAST (0))
    #else
        #ifdef SYSTEM_WINDOWS
            #define AssertError(expr, errorString)  ((expr) ? static_cast<void>(0) : Nc::Utils::AssertFailedError(errorString, #expr, __FILE__, __LINE__, __ASSERT_FUNCTION))
        #else
            #define AssertError(expr, errorString)  ((expr) ? static_cast<void>(0) : Nc::Utils::AssertFailedError(errorString, __STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))
        #endif
    #endif
#endif

namespace Nc
{
    namespace Utils
    {
      LIB_NC_CORE void AssertFailed(const std::string &expr, const std::string &file, int line, const std::string &function);
      LIB_NC_CORE void AssertFailedError(const std::string &error, const std::string &expr, const std::string &file, int line, const std::string &function);
    }
}

#endif
