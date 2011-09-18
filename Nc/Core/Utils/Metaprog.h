
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

    File Created At:        13/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_METAPROG_H_
#define NC_CORE_UTILS_METAPROG_H_

#include "../Define.h"

namespace Nc
{
    namespace Utils
    {
        /// Define some usefull metaprogramming templates
        namespace Metaprog
        {
            /// NullType
            struct NullType {};

            /// Typelist
            template<typename T1, typename T2>
            struct TypeList
            {
                typedef T1  Head;
                typedef T2  Tail;
            };

            template<typename TypeList, typename Action>
            struct TypeListApply
            {
                void operator () (TypeList, Action &action)
                {
                    action.template Call<typename TypeList::Head>();
                    Apply(typename TypeList::Tail(), action);
                }
            };

            template<typename Action>
            struct TypeListApply<NullType, Action>
            {
                void operator () (NullType, Action &)     {}
            };

            template<typename TypeList, typename Action>
            void    Apply(TypeList list, Action &action)
            {
                TypeListApply<TypeList, Action> app;
                app(list, action);
            }



            #define NC_UTILS_METAPROG_TYPELIST_1(t1)                            TypeList<t1, NullType>
            #define NC_UTILS_METAPROG_TYPELIST_2(t1, t2)                        TypeList<t1, NC_UTILS_METAPROG_TYPELIST_1(t2) >
            #define NC_UTILS_METAPROG_TYPELIST_3(t1, t2, t3)                    TypeList<t1, NC_UTILS_METAPROG_TYPELIST_2(t2, t3) >
            #define NC_UTILS_METAPROG_TYPELIST_4(t1, t2, t3, t4)                TypeList<t1, NC_UTILS_METAPROG_TYPELIST_3(t2, t3, t4) >
            #define NC_UTILS_METAPROG_TYPELIST_5(t1, t2, t3, t4, t5)            TypeList<t1, NC_UTILS_METAPROG_TYPELIST_4(t2, t3, t4, t5) >
            #define NC_UTILS_METAPROG_TYPELIST_6(t1, t2, t3, t4, t5, t6)        TypeList<t1, NC_UTILS_METAPROG_TYPELIST_5(t2, t3, t4, t5, t6) >
            #define NC_UTILS_METAPROG_TYPELIST_7(t1, t2, t3, t4, t5, t6, t7)    TypeList<t1, NC_UTILS_METAPROG_TYPELIST_6(t2, t3, t4, t5, t6, t7) >

            template<typename T1, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType>
            struct Seq;

            template<typename T1>
            struct Seq<T1, NullType, NullType, NullType, NullType, NullType, NullType>
            {
                typedef NC_UTILS_METAPROG_TYPELIST_1(T1)    Type;
            };

            template<typename T1, typename T2>
            struct Seq<T1, T2, NullType, NullType, NullType, NullType, NullType>
            {
                typedef NC_UTILS_METAPROG_TYPELIST_2(T1, T2)    Type;
            };

            template<typename T1, typename T2, typename T3>
            struct Seq<T1, T2, T3, NullType, NullType, NullType, NullType>
            {
                typedef NC_UTILS_METAPROG_TYPELIST_3(T1, T2, T3)    Type;
            };

            template<typename T1, typename T2, typename T3, typename T4>
            struct Seq<T1, T2, T3, T4, NullType, NullType, NullType>
            {
                typedef NC_UTILS_METAPROG_TYPELIST_4(T1, T2, T3, T4)    Type;
            };

            template<typename T1, typename T2, typename T3, typename T4, typename T5>
            struct Seq<T1, T2, T3, T4, T5, NullType, NullType>
            {
                typedef NC_UTILS_METAPROG_TYPELIST_5(T1, T2, T3, T4, T5)    Type;
            };

            template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
            struct Seq<T1, T2, T3, T4, T5, T6, NullType>
            {
                typedef NC_UTILS_METAPROG_TYPELIST_6(T1, T2, T3, T4, T5, T6)    Type;
            };

            template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
            struct Seq
            {
                typedef NC_UTILS_METAPROG_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7)    Type;
            };

            #undef NC_UTILS_METAPROG_TYPELIST_1
            #undef NC_UTILS_METAPROG_TYPELIST_2
            #undef NC_UTILS_METAPROG_TYPELIST_3
            #undef NC_UTILS_METAPROG_TYPELIST_4
            #undef NC_UTILS_METAPROG_TYPELIST_5
            #undef NC_UTILS_METAPROG_TYPELIST_6
            #undef NC_UTILS_METAPROG_TYPELIST_7


            /// conditional inheritance
            template<bool T, typename TRUE_CLASS, typename FALSE_CLASS>
            struct If : TRUE_CLASS {};

            /// specialisation of the struct IF, to make the inheritance with the conditional to false
            template<typename TRUE_CLASS, typename FALSE_CLASS>
            struct If<false, TRUE_CLASS, FALSE_CLASS> : FALSE_CLASS {};

            /// container of type T
            template<typename T>
            struct Container    { T   Data; };

            /// Just a struct witch do and store nothing
            struct Nop {};

            /// A Classic allocator witch allocate and destroy objects
            template<typename T>
            struct Allocator
            {
                /** Allocate the object with the operator new by using the default constructor  */
                inline T       *Allocate()                      {return new T();}
                /** Allocate the object with the operator new by using the copy constructor */
                inline T       *Allocate(const T &toCopy)       {return new T(toCopy);}
                /** Deallocate the object with the operator delete */
                inline void    Deallocate(T *p)                 {delete p;}
            };
        }
    }
}

#endif // NC_CORE_UTILS_METAPROG_H_
