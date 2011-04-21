
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


#ifndef NC_CORE_UTILS_XML_FILE_H_
#define NC_CORE_UTILS_XML_FILE_H_

#include "../../Define.h"
#include "../FileName.h"
#include "Parser.h"

namespace Nc
{
    namespace Utils
    {
        ///this namespace provide some classes to manage an Xml Format
        namespace Xml
        {
            /// Provide a Xml::File managing
            class LCORE File
            {
                public:
                    File() : _content(NULL) {}
                    File(const FileName &filename);
                    File(const FileName &filename, Object *content);

                    Object  *Read(const FileName &f);           ///< read the given file
                    Object  *Read();                            ///< read the file and return content
                    void    Save() const;                       ///< save the file with the propertie _content
                    void    Save(Object *content);              ///< save the file with the content passed in param

                    // accesseurs
                    inline const Object *Content() const        {return _content;}  ///< Return the content of the file
                    inline Object       *Content()              {return _content;}  ///< Return the content of the file

                private:
                    FileName    _filename;      ///< The name of the file to read
                    Object      *_content;      ///< Content of the XML file
            };
        }
    }
}

#endif
