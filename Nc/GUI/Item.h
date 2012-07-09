
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        22/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_ITEM_H_
#define NC_GUI_ITEM_H_

#include "Define.h"

namespace Nc
{
    namespace GUI
    {
        /// Store a data text.
        class LGUI Item : public System::Object
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(System::Object, Nc::System::Object, Nc::GUI::Item);

            public:
                Item();
                Item(const std::string &data);
                ~Item();

                /** \return a clone of the data... this clone must be deleted further. */
                virtual Item                *Clone() const                          {return new Item(*this);}

                /** \return the data. */
                inline const std::string    &Data() const                           {return _data;}
                /** Set the data. */
                inline void                 Data(const std::string &data)           {_data = data;}

            private:
                std::string     _data;
        };
    }
}

#endif
