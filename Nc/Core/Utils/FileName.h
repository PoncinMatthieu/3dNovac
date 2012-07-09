
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

#ifndef NC_CORE_UTILS_FILENAME_H_
#define NC_CORE_UTILS_FILENAME_H_

#include <sstream>
#include <string>
#include "../Define.h"

namespace Nc
{
    namespace Utils
    {
        /// manipulate your filename path.
        /**
            Parse filename to access the path, filename, extention, change filename, etc...

            For transparency and simplicity, FileName inherite of std::string.

            If the given path begin with "Nc:Type:", then the beginning of the filename will be set to the "RessourcePath/Type" of the 3dNovac Config file.

			\sa
				- System::Config
        */
        class LCORE FileName : public std::string
        {
            public:
                FileName()  {};
                FileName(const std::string &path);
                FileName(const char *path);

                FileName &operator = (const std::string &path);
                FileName &operator = (const char *path);

                /** Print the filename. */
                friend LCORE std::ostream& operator << (std::ostream& os, const FileName& f);

            //Accesseurs
            // partie du nom
                /** \return the path of the file. */
                std::string		Path() const;
                /** \return the name + extension. */
                std::string		Filename() const;
                /** Set the name + extension. */
                void		    SetFilename(const std::string &s);
                /** \return Only the name. */
                std::string		ShortFilename() const;
                /** Set only the name. */
                void		    SetShortFilename(const std::string &s);
                /** \return Only the extension. */
                std::string		Extension() const;

            // Accessibilite du fichier
                /** \return true if the file is readable. */
                bool		    IsReadable() const;

            private:
                void            SetFullname(const char *name);
        };
    }
}

#endif // UFILENAME_H_INCLUDED
