
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

    File Created At:        26/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                    Implementation de la classe "Object"

                     Classe mere de base pour un objet
                permet d'identifier chaque objet du programme

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_OBJECT_H_
#define NC_CORE_SYSTEM_OBJECT_H_

#include <string>
#include "../Define.h"

namespace Nc
{
    namespace System
    {
        class LCORE  Object
        {
            public:
                Object();
                Object(const std::string &name);
                virtual ~Object();

                inline void Name(std::string name)			{_name = name;}
                inline std::string Name()					{return _name;}
                inline unsigned int Id()					{return _id;}

                friend std::ostream& operator << (std::ostream& Out, const Object& o);

            protected:
                std::string         _name;

            public:
                unsigned int        _id;
                static unsigned int _nbObject;
        };
    }
}

#endif // OBJECT_H_INCLUDED
