
/*-----------------------------------------------------------------------------

	3dNovac Editor
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef POPULOUS_EDITOR_DEFINE_H_
#define POPULOUS_DEITOR_DEFINE_H_

#define _DEBUG

#define LOG                 Nc::Utils::Logger::Log(0)
#define LOG_ERROR           Nc::Utils::Logger::Log(1)
#ifdef _DEBUG
	#define LOG_DEBUG       Nc::Utils::Logger::Log(2)
#else
	#define LOG_DEBUG       Nc::Utils::Logger::Log(-1)
#endif

#include <Nc/Core/Utils/Utils.h>

#endif
