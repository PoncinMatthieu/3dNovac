
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

#ifndef NC_CORE_UTILS_VISITORMANAGER_H_
#define NC_CORE_UTILS_VISITORMANAGER_H_

#include "Utils.h"
#include "VTableManager.h"
#include "Visitable.h"

namespace Nc
{
    namespace Utils
    {
        /// This class encapsulate some usefull inner classes which can help you to use a Visitor pattern.
        /**
            The implemented visitor, is a "Cooperative Visitor", this one use some template method like a VTable to define with object should be visited.
            The Cooperative Visitor is a flexible and efficient version of the visitor design pattern that accommodates arbritrary argument and return types.
            It use a VTable system to avoid any dynamic_cast and resolve the class hierarchie.

            The Visitor Manager implement two visitor.
            The first one allow you to visit an object by using a vtable and resolving the type and calling the visit method if you want to visit the true type by initialize the type you want to visit with a typelist.
            And the second allow you like the first to visit an object but use a second vtable to call methods which should be used to recall the visitor itself to visit a structure like a graph or a tree.

            \todo Specialize the InvokableVisitor for (ReturnType == bool) to setup some Invokation method (for exemple to stop the visitation when result == false).
        */
        class VisitorManager
        {
            private:
                template<typename Visitable, typename Base>
                struct GetVisitMethodArgumentType
                {
                    typedef Visitable Type;
                };

                // specialize for const Base
                template<typename Visitable, typename Base>
                struct GetVisitMethodArgumentType<Visitable, const Base>
                {
                    typedef const Visitable Type;
                };

                /// To Create and initialize the static vtable.
                template<typename Visitor, typename VisitedList, typename Invoker, typename VTableType>
                struct VTableInitializer
                {
                    VTableInitializer ()
                    {
                        // add visit function for each type in VisitedList
                        Apply(VisitedList(), *this);
                    }

                    template<typename Visitable>
                    void Call()
                    {
                        vtable.template Add<Visitable>(&Visitor::template Thunk<Visitable,Invoker>);
                    }

                    VTableType  vtable;
                };

                /// To Create and initialize the static invokable vtable.
                template<typename Visitor, typename VisitedList, typename Invoker, typename VTableType>
                struct VTableInvokableInitializer
                {
                    VTableInvokableInitializer ()
                    {
                        // add visit function for each type in VisitedList
                        Apply(VisitedList(), *this);
                    }

                    template<typename Visitable>
                    void Call()
                    {
                        vtable.template Add<Visitable>(&Visitor::template ThunkInvokable<Visitable,Invoker>);
                    }

                    VTableType  vtable;
                };


            public:
                /** To define a vtable type to be used with the Visitor class. */
                class VTableVisitorType {};
                /** To define a vtable type to be used with the Invokable Visitor class. */
                class VTableInvokableVisitorType {};

            public:
                /// Define a cooperative visitor.
                template<class VisitorType, class Base, typename ReturnType>
                class Visitor
                {
                    public:
                        typedef ReturnType (Visitor::*Func)(Base&);
                        typedef VTableManager::VTable<VTableVisitorType, const Base, Func>      VTableType;
                        typedef Base                                                            BaseType;

                    public:
                        Visitor() : _vtable(0)      {}

                        /** Method used to invoke the visit method. */
                        template<typename Visitable, typename Invoker>
                        ReturnType Thunk(Base &b)
                        {
                            typedef typename GetVisitMethodArgumentType<Visitable, Base>::Type        VisitableType;
                            return Invoker::template Invoke<ReturnType, VisitorType, VisitableType>(static_cast<VisitorType&>(*this), static_cast<VisitableType&>(b));        // invoke visit method
                        }

                        /** Method used to invoke the visit method. */
                        template<typename Visitable, typename Invoker>
                        ReturnType ConstThunk(const Base &b)
                        {
                            typedef typename GetVisitMethodArgumentType<Visitable, const Base>::Type        VisitableType;
                            return Invoker::template Invoke<ReturnType, VisitorType, VisitableType>(static_cast<VisitorType&>(*this), static_cast<VisitableType&>(b));        // invoke visit method
                        }

                        /** To visit the given class, call the visit method if the class is visitable. */
                        ReturnType operator() (Base &b)
                        {
                            if (_vtable == 0)
                                throw Exception("Visitor", "The vtable of the visitor has not been initialized.");

                            // fetch thunk and call it
                            unsigned int    i = 0;
                            size_t          index = b.Tag();
                            Func            thunk = _vtable->GetFunc(index);
                            //std::cout << "tag: " << b.Tag() << " thunk " << thunk << " " << b.GetClassName() << std::endl;

                            while (thunk == NULL && index != 0)
                            {
                                index = b.ParentTag(i++);
                                if (index != 0)
                                    thunk = _vtable->GetFunc(index);
                            }

                            if (thunk != NULL)
                                return (static_cast<VisitorType*>(this)->*thunk)(b);
                            return ReturnType();
                        }

                        /** Initialize the vtable with the given invoker and visitable typelist. */
                        template<typename VisitedList, typename Invoker>
                        void InitVTable(const VisitedList&, const Invoker&)
                        {
                            // instantiate the static vtable and set the vtable pointer
                            _vtable = VTableManager::GetStaticVtable<VisitorType, VisitedList, Invoker, VTableType, VTableInitializer>();
                        }

                    private:
                        const VTableType    *_vtable; // vtable pointer
                };

                /// Define a cooperative visitor specialized to be invokable when a class is visited (especialy usefull to browse a graph structure).
                template<class VisitorType, class Base, typename ReturnType>
                class InvokableVisitor : public Visitor<VisitorType, Base, ReturnType>
                {
                    public:
                        typedef void (InvokableVisitor::*InvokableFunc)(Base&);
                        typedef VTableManager::VTable<VTableInvokableVisitorType, const Base, InvokableFunc>    VTableInvokableType;

                    public:
                        InvokableVisitor(bool postVisits = true) : _postVisit(postVisits), _vTableInvokable(0) {}

                        /** Method used to invoke the invokable method. */
                        template<typename Visitable, typename Invoker>
                        void ThunkInvokable(Base &b)
                        {
                            typedef typename GetVisitMethodArgumentType<Visitable, Base>::Type  VisitableType;
                            Invoker::template Invoke<void, VisitorType, VisitableType>(static_cast<VisitorType&>(*this), static_cast<VisitableType&>(b));  // invoke visit method
                        }

                        /** Visit the given class, call the visit method if the class is visitable and call the invokable method if the class is intrusive. */
                        void operator() (Base &b)
                        {
                            if (_postVisit)
                            {
                                //std::cout << "visit " << &b << std::endl;
                                Visitor<VisitorType, Base, ReturnType>::operator()(b);
                                if (_vTableInvokable != 0)
                                    Invoke(b);
                            }
                            else
                            {
                                if (_vTableInvokable != 0)
                                    Invoke(b);
                                Visitor<VisitorType, Base, ReturnType>::operator()(b);
                            }
                        }

                        /** Initialize the invokable vtable with the given invoker and visitable typelist. */
                        template<typename VisitedList, typename Invoker>
                        void InitVTableInvokable(const VisitedList&, const Invoker&)
                        {
                            _vTableInvokable = VTableManager::GetStaticVtable<VisitorType, VisitedList, Invoker, VTableInvokableType, VTableInvokableInitializer>();
                        }

                    private:
                        void Invoke(Base &b)
                        {
                            unsigned int    i = 0;
                            size_t          index = b.InvokableTag();
                            InvokableFunc   thunk = _vTableInvokable->GetFunc(index);
                            //std::cout << "tag: " << b.InvokableTag() << " thunk " << thunk << " " << b.GetClassName() << std::endl;

                            while (thunk == NULL && index != 0)
                            {
                                index = b.InvokableParentTag(i++);
                                if (index != 0)
                                    thunk = _vTableInvokable->GetFunc(index);
                            }

                            if (thunk != NULL)
                                (this->*thunk)(b);
                        }

                    private:
                        bool                        _postVisit;
                        const VTableInvokableType   *_vTableInvokable;      // vtable pointer for the nodes which should be internaly visited
                  };
        };
    }
}

#endif
