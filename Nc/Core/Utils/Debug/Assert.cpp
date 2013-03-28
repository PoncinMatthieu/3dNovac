
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

#include "Assert.h"

using namespace Nc;
using namespace Nc::Utils;

void Nc::Utils::AssertFailed(const std::string &expr, const std::string &file, int line, const std::string &function)
{
    CALLSTACK_INFO_ARG("At: " + file + ":" + Convert::ToString(line) + ": " + function + ": (" + expr + ")")
    CrashReporter::Abort();
}

void Nc::Utils::AssertFailedError(const std::string &error, const std::string &expr, const std::string &file, int line, const std::string &function)
{
    CALLSTACK_INFO_ARG("At: " + file + ":" + Convert::ToString(line) + ": " + function + ": (" + expr + ")");
    LOG_ERROR << "Fatal error: " << error << std::endl;
    CrashReporter::Abort();
}
