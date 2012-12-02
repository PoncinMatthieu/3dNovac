
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

#ifndef NC_CORE_UTILS_ASSERT_H_
#define NC_CORE_UTILS_ASSERT_H_

#include <assert.h>
#include "CallStack.h"
#include "CrashReporter.h"

#ifndef Assert
#define Assert(expr)    ((expr) ? static_cast<void>(0) : Nc::Utils::AssertFailed(__STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))
#endif

#ifndef AssertError
#define AssertError(expr, errorString)  ((expr) ? static_cast<void>(0) : Nc::Utils::AssertFailedError(errorString, __STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))
#endif

namespace Nc
{
    namespace Utils
    {
        static void AssertFailed(const std::string &expr, const std::string &file, int line, const std::string &function)
        {
            CALLSTACK_INFO("Assertion Failed: " + file + ":" + Convert::ToString(line) + ": " + function + ": (" + expr + ")");
            CrashReporter::Abort();
        }

        static void AssertFailedError(const std::string &error, const std::string &expr, const std::string &file, int line, const std::string &function)
        {
            CALLSTACK_INFO("Assertion Failed: " + file + ":" + Convert::ToString(line) + ": " + function + ": (" + expr + ")");
            LOG_ERROR << "Fatal error: " << error << std::endl;
            CrashReporter::Abort();
        }
    }
}

#endif
