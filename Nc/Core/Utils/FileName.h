
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
        /// manipulate your filename path
        /** Parse filename to access the path, filename, extention, change filename, etc... */
        class LCORE FileName
        {
            public:
                FileName()  {};
                FileName(const std::string &path);
                FileName(const char *path);

                bool	    operator == (const FileName& f) const;

                friend std::ostream& operator << (std::ostream& os, const FileName& f)
                {
                    os << "Filename : `" << f._path << "`";
                    return os;
                }

            //Accesseurs
            // nom complet
                inline operator const std::string & () const		                {return _path;}	    ///< cast operateur in std::string
                inline void		            Fullname(const std::string& aPath)	    {_path = aPath;}    ///< set the Fullpath
                inline const std::string    &Fullname() const			            {return _path;}     ///< access the Fullpath passed to the fileName class

            // partie du nom
                std::string		Path() const;			        ///< path file
                std::string		Filename() const;		        ///< name + extension
                void		    SetFilename(std::string s);     ///< set the name
                std::string		ShortFilename() const;		    ///< only the name
                std::string		Extension() const;		        ///< only the extension

            // Accessibilite du fichier
                bool		    IsReadable() const;

            protected:
                std::string		_path;  ///< Full path (path+name+extention)
        };
    }
}

#endif // UFILENAME_H_INCLUDED
