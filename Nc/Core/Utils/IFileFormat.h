
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        12/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_IFILEFORMAT_H_
#define NC_CORE_UTILS_IFILEFORMAT_H_

#include "FileName.h"

namespace Nc
{
    namespace Utils
    {
        /// Interface to manage file formats
        /**
            A file format define a way to manage a specific format to read and save.
            It just
        */
        template<typename FileFormat, typename T>
        class IFileFormat
        {
            public:
                IFileFormat()           {}
                virtual ~IFileFormat()  {}

                /** Use this static method to load one file format */
                static inline void Load(const Utils::FileName &file, T &content)
                {
                    FileFormat format;
                    format.LoadFile(file, content);
                }

                /** Use this static method to save one file format */
                static inline void Save(const Utils::FileName &file, const T &content)
                {
                    FileFormat format;
                    format.SaveFile(file, content);
                }

                /** Load a file to the given content */
                virtual void LoadFile(const Utils::FileName &file, T &content) = 0;

                /** Save a file by using the given content */
                virtual void SaveFile(const Utils::FileName &file, const T &content) = 0;
        };
    }
}

#endif
