
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

#ifndef NC_CORE_UTILS_TAGMANAGER_H_
#define NC_CORE_UTILS_TAGMANAGER_H_

#include "../Define.h"

namespace Nc
{
    namespace Utils
    {
#ifdef SYSTEM_WINDOWS
		/// Used to register every tag by using a class name
		/**
			On linux, we use a templated TagCounter.
			Unfortunately, on windows we have a copy of every static on every DLL, so we had different classes with the same tag.
			To Correct this, we created this tag register which register the class name to ensure that we create only one tag by classe type. This necessarily put down the perf but only during the loading of the .dll/.so.

			Damn you windows!!!
		*/
		struct LCORE TagRegister
		{
			typedef std::map<std::string, size_t>	MapClassNameTagValue;

			static size_t					counter;
			static MapClassNameTagValue		tagValues;
		};
#endif

		/// Define a class to manage a Tag system to associate a unique tag to a class for the given VTableType
        template<typename VTableType>
        struct TagManager
        {
            private:
            /// Define a tag counter to put a unique tag on each visitable class
#ifdef SYSTEM_LINUX
                template<typename Base>
                struct TagCounter
                {
                    static size_t       counter; // default 0
                };
#endif

            /// Define a class which store the tag for each Visitable associated to the given Base
                template<typename Visitable, typename Base>
                struct TagHolder
                {
                    static size_t       tag;
					static std::string	visitableClassName;
                };

            public:
                /** \return the tag of a visitable */
                template<typename Visitable, typename Base>
                static size_t GetTag()
                {
                    size_t &tag = TagHolder<const Visitable, const Base>::tag;
                    // first time : generate tag
                    if (tag == 0)
                    {
#ifdef SYSTEM_LINUX
						tag = ++TagCounter<const Base>::counter;
#else
						std::string className = typeid(Visitable).name();
						TagRegister::MapClassNameTagValue::iterator it = TagRegister::tagValues.find(className);
						if (it != TagRegister::tagValues.end())
						{
							tag = it->second;
						}
						else
						{
							tag = ++TagRegister::counter;
                            TagRegister::tagValues.insert(std::make_pair<std::string, size_t>(className, tag));
						}
                        //LOG << "Generate tag " << tag << "  with VtableType: " << typeid(VTableType).name() << " and base: " << typeid(Base).name() << "     for " << typeid(Visitable).name() << std::endl;
#endif
					}
                    return tag;
                }
        };

#ifdef SYSTEM_LINUX
        template<typename VTableType>
        template<typename Base>
        size_t TagManager<VTableType>::TagCounter<Base>::counter = 0;
#endif

        template<typename VTableType>
        template<typename Visitable, typename Base>
        size_t TagManager<VTableType>::TagHolder<Visitable, Base>::tag = TagManager<VTableType>::GetTag<Visitable, Base>();
	}
}

#endif
