
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

#ifndef NC_CORE_SYSTEM_OBJECT_H_
#define NC_CORE_SYSTEM_OBJECT_H_

#include <string>
#include <typeinfo>
#include "../Define.h"
#include "../Utils/Exception.h"
#include "../Graph/INodePolitic.h"

#define STR_EXPAND(tok) #tok

#define NC_SYSTEM_DEFINE_OBJECT(ParentClass, FullClassObjectName)               \
    NC_UTILS_DEFINE_PARENT_CLASS(ParentClass);                                  \
    static const char     *ClassName()    {return #FullClassObjectName;}        \
    virtual const char    *ResolvedClassName() const    {return ClassName();}

#define NC_SYSTEM_DEFINE_OBJECT_VISITABLE(ParentClass, VisitableObject, FullClassObjectName)    \
    NC_UTILS_DEFINE_PARENT_CLASS(ParentClass);                                  \
    NC_UTILS_DEFINE_VISITABLE(VisitableObject);                                 \
    static const char     *ClassName()    {return #FullClassObjectName;}        \
    virtual const char    *ResolvedClassName() const    {return ClassName();}

#define NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(ParentClass, VisitableObject, InvokableObject, FullClassObjectName)    \
    NC_UTILS_DEFINE_PARENT_CLASS(ParentClass);                                  \
    NC_UTILS_DEFINE_VISITABLE(VisitableObject);                                 \
    NC_UTILS_DEFINE_INVOKABLE(InvokableObject);                                 \
    static const char     *ClassName()    {return #FullClassObjectName;}        \
    virtual const char    *ResolvedClassName() const    {return ClassName();}

namespace Nc
{
    namespace System
    {
        /// base class, with an Id and a Name, the Id is unique
        class LCORE  Object : public Utils::VisitableBase<Object>
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(Utils::VisitableBase<Object>, Nc::System::Object, Nc::System::Object, Nc::System::Object);

            public:
                Object();
                Object(const std::string &name);
                Object(const Object &obj);
                Object &operator = (const Object &obj);
                virtual ~Object();

                /** Set the name of the object */
                inline void                 Name(const std::string &name)       {_name = name;}
                /** \return the name of the object */
                inline const std::string    &Name() const		                {return _name;}
                /** \return the Id of the object */
                inline unsigned int         Id() const			                {return _id;}

                /**
                    Allow you to move up the SceneNode in the inheritance hierarchy, without any dynamic_cast witch could incredibly decrease the performances.
                    \return the SceneNode static_cast into the given type if the ClassName is corresponding to the given type.
                */
                template<typename T>
                inline T    *AsWithoutThrow()
                {
                    //if ((void*)T::ClassName() == (void*)_className)
                    //    return static_cast<T*>(this);
                    //return NULL;
                    return (this != NULL) ? dynamic_cast<T*>(this) : NULL;
                }

                /**
                    Allow you to move up the SceneNode in the inheritance hierarchy, without any dynamic_cast witch could incredibly decrease the performances.
                    \return the SceneNode static_cast into the given type if the ClassName is corresponding to the given type.
                */
                template<typename T>
                inline const T    *AsWithoutThrow() const
                {
                    //if ((const void*)T::ClassName() == (const void*)_className)
                    //    return static_cast<const T*>(this);
                    //return NULL;
                    return (this != NULL) ? dynamic_cast<const T*>(this) : NULL;
                }

                /**
                    Allow you to move up the SceneNode in the inheritance hierarchy, without any dynamic_cast witch could incredibly decrease the performances.
                    Throw an exception if the given node is not to the good type.
                    \return the SceneNode static_cast into the given type if the ClassName is corresponding to the given type.
                */
                template<typename T>
                inline T    *As()
                {
                    //if ((void*)T::ClassName() == (void*)_className)
                    //    return static_cast<T*>(this);
                    T *o = (this != NULL) ? dynamic_cast<T*>(this) : NULL;
                    if (o != NULL)
                        return o;
                    throw Utils::Exception("System::Object", "Cannot convert the object `" + std::string(ResolvedClassName()) + "` into a `" + std::string(T::ClassName()) + "`");
                }

                /**
                    Allow you to move up the SceneNode in the inheritance hierarchy, without any dynamic_cast witch could incredibly decrease the performances.
                    Throw an exception if the given node is not to the good type.
                    \return the SceneNode static_cast into the given type if the ClassName is corresponding to the given type.
                */
                template<typename T>
                inline const T    *As() const
                {
                    //if ((const void*)T::ClassName() == (const void*)_className)
                    //    return static_cast<const T*>(this);
                    T *o = (this != NULL) ? dynamic_cast<T*>(this) : NULL;
                    if (o != NULL)
                        return o;
                    throw Utils::Exception("System::Object", "Cannot convert the object `" + std::string(ResolvedClassName()) + "` into a `" + std::string(T::ClassName()) + "`");
                }

                /**
                    Fill the given ostream used with the operator << <br\>
                    Could be redefine in your own object.
                */
                virtual void                ToString(std::ostream &oss) const;

                /** Fill the given string */
                void                        ToString(std::string &str) const;

                friend LCORE std::ostream	&operator << (std::ostream &oss, const Object &o);

            protected:
                std::string         _name;          ///< the name of the object
                unsigned int        _id;            ///< the unique id of the object

            private:
                static unsigned int _nbObject;      ///< the current number of object
        };

		// explicit instanciation
		//template class Utils::VisitableBase<Object>;
	}
}

#endif // OBJECT_H_INCLUDED
