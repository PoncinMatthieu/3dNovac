
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

    File Created At:        11/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_VISITABLE_H_
#define NC_CORE_UTILS_VISITABLE_H_

#include "../Define.h"
#include "VisitorManager.h"

#define NC_UTILS_DEFINE_PARENT_CLASS(parent)                            \
    typedef parent  ParentType;

/// helps defining a virtual function in every Visitable class to return its tag.
#define NC_UTILS_DEFINE_VISITABLE(base)                                 \
    virtual size_t Tag() const                                          \
    {                                                                   \
        return base::GetTagHelper(Nc::Utils::VisitorManager::VTableVisitorType(), this); \
    }                                                                   \
                                                                        \
    virtual size_t ParentTag(unsigned int i) const                      \
    {                                                                   \
        if (i > 0)                                                      \
            return ParentType::ParentTag(i - 1);                            \
        return ParentType::Tag();                                           \
    }

#define NC_UTILS_DEFINE_INVOKABLE(base)                                 \
    virtual size_t InvokableTag() const                                 \
    {                                                                   \
        return base::GetTagHelper(Nc::Utils::VisitorManager::VTableInvokableVisitorType(), this); \
    }                                                                   \
                                                                        \
    virtual size_t InvokableParentTag(unsigned int i) const             \
    {                                                                   \
        if (i > 0)                                                      \
            return ParentType::InvokableParentTag(i - 1);                   \
        return ParentType::InvokableTag();                                  \
    }

/// check_member_function is a compile time assertion to be sure that the function name exist.
#define NC_UTILS_DEFINE_VISIT_INVOKER(structName, name)                 \
  struct structName {                                                   \
    template<typename ReturnTypeInvoker, typename VisitorImplInvoker, typename VisitableInvoker> \
    static ReturnTypeInvoker Invoke(VisitorImplInvoker &visitor, VisitableInvoker &visitable) \
    {                                                                   \
      return visitor.name(visitable);                                   \
    }                                                                   \
  }
//check_member_function<ReturnType, VisitorImpl, Visitable>(&Visitor::name);

namespace Nc
{
    namespace Utils
    {
        /// Visitable base, every visitable base should inherite to this class.
        template<typename Base>
        struct VisitableBase
        {
            virtual size_t Tag() const                                  {return 0;}
            virtual size_t ParentTag(unsigned int) const                {return 0;}
            virtual size_t InvokableTag() const                         {return 0;}
            virtual size_t InvokableParentTag(unsigned int) const       {return 0;}

            template<typename VTableType, typename Visitable>
            size_t GetTagHelper(const VTableType &, const Visitable*) const       {return TagManager<VTableType>::template GetTag<Visitable, Base>();}
        };
    }
}

#endif
