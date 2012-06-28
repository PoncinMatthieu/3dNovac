
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

    File Created At:        10/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_VTABLEMANAGER_H_
#define NC_CORE_UTILS_VTABLEMANAGER_H_

#include "TagManager.h"

namespace Nc
{
    namespace Utils
    {
        // Manage a vtable, to index function to a base class
        class VTableManager
        {
            public:
                /// Define V-Table to indexing new functions to a base class
                template<typename VTableType, typename Base, typename Func>
                class VTable
                {
                    public:
                        /** Add the given function for the given visitable */
                        template<typename Visitable>
                        void Add(Func f)
                        {
                            size_t    index = TagManager<VTableType>::template GetTag<Visitable, Base>(); // find the slot

                            //LOG << "add " << index << " " << Visitable::ClassName() << "    vtableType: " << typeid(VTableType).name() << std::endl;
                            if (index >= _table.size())
                            {
                                // find function for Base if it exists?
                                const size_t baseTag = TagManager<VTableType>::template GetTag<Base, Base>();
                                Func defaultFunction = (baseTag >= _table.size()) ? NULL : _table[baseTag];
                                _table.resize(index + 1, defaultFunction);
                            }
                            _table[index] = f;
                        }

                        /** \return the function which correspond to the given index, null if the function doesn't exist */
                        Func GetFunc(size_t index) const
                        {
                            if(index >= _table.size())
                                return NULL;
                            return _table[index];
                        }

                    private:
                        std::vector<Func>       _table; ///< function storage
                };

                /// To Create and store the static vtable
                template<typename Visitor, typename VisitedList, typename Invoker, typename VTableType,
                template<typename Visitor, typename VisitedList, typename Invoker, typename VTableType> class Initializer>
                class GetStaticVtable
                {
                    public:
                        // provide conversion operator
                        operator const VTableType* () const       {return &table.vtable;}

                    private:
                        // declare static instanceof vtable
                        static Initializer<Visitor, VisitedList, Invoker, VTableType>     table;
                };
        };

        template<typename Visitor, typename VisitedList, typename Invoker, typename VTableType, template<typename Visitor, typename VisitedList, typename Invoker, typename VTableType> class Initializer>
        Initializer<Visitor, VisitedList, Invoker, VTableType>          VTableManager::GetStaticVtable<Visitor, VisitedList, Invoker, VTableType, Initializer>::table;
    }
}

#endif
