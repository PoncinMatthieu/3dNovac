
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


#ifndef NC_CORE_UTILS_XML_OBJECT_H_
#define NC_CORE_UTILS_XML_OBJECT_H_

#include <iostream>
#include <map>
#include <list>

#include "../SmartPointer.h"

namespace Nc
{
    namespace Utils
    {
        namespace Xml
        {
            /// Xml::Object, provide an element of a DOM Model.
			/**
                \todo Manage unicode xml file.
				\todo Now that we defined a graph structure, we should use it to define the XML object.
				\todo Now that we defined a visitor structure, we should use it on the XML object to allow the visitation of the XML object.
			*/
            class LCORE Object
            {
                public:
                    /** TYPE, to define the type of Xml object. */
                    enum TYPE {HEADER, INLINE, BLOCK};

                public:
                    Object(Object *parent = NULL);
                    Object(const std::string &name, TYPE type = INLINE, Object *parent = NULL);
                    Object(const std::string &name, TYPE type, const std::map<std::string, std::string> &params, Object *parent = NULL);
                    Object(const Object &o);
                    virtual ~Object();

                    Object						&operator = (const Object &o);
                    friend LCORE std::ostream	&operator << (std::ostream &out, const Object &o);

                    virtual void Write(std::ostream &out, unsigned int i) const;

                    //accesseurs
                    // values
                    /** \return the type of the object. */
                    inline TYPE                 Type() const                        {return _type;}
                    /** Set the name of the object. */
                    inline void                 Name(const std::string &s)                                      {_name = s;}
                    /** \return the name of the object. */
                    inline const std::string    &Name() const                                                   {return _name;}
                    /** Add a param with the key/value. */
                    inline void                 AddParam(const std::string &key, const std::string &value)      {_params[key] = value;}
                    /** \return the param with the given key. */
                    inline std::string          &Param(const std::string &key)                                  {return _params[key];}
                    /** \return true if the given key exist. */
                    inline bool                 ParamExist(const std::string &key)                              {return (_params.find(key) != _params.end());}
                    /** \return the map of param key/values. */
                    inline MapParam             &Params()                                                       {return _params;}
                    /** \return the data. */
                    inline std::string          &Data()                                                         {return _data;}
                    /** Set the data. */
                    inline void                 Data(const std::string &s)                                      {_data = s;}
                    /** \return the CDATA block statement, to know if the data is in a CBLOCK data. */
                    inline bool                 CData()                                                         {return _cdata;}
                    /** Set the CDATA block statement, to know if the data is in a CBLOCK data.  */
                    inline void                 CData(bool b)                                                   {_cdata = b;}

                    // parent
                    /** \return the parent of the object, Null if the object has no parents. */
                    inline Object               *Parent()                                                       {return _parent;}
                    /** \return the parent of the object, Null if the object has no parents. */
                    inline const Object         *Parent() const                                                 {return _parent;}
                    /** \return the number of parent in the hierarchy. */
                    unsigned int                GetNbParent() const;

                    // child
                    /** Create a new Block object in the Object. */
                    Object                      *NewBlock(const std::string &name)                              {return AddChild(new Object(name, BLOCK, this));}
                    /** Create a new Inline object in the Object. */
                    Object                      *NewLine(const std::string &name)                               {return AddChild(new Object(name, INLINE, this));}
                    /** \return the block with the good name, Throw if don't exist. */
                    Object                      *Block(const std::string &name);
                    /** \return true if the block with the good name exist. */
                    bool                        BlockExist(const std::string &name);
                    /** \return the line with the good name, Throw if don't exist. */
                    Object                      *Line(const std::string &name);
                    /** \return true if the line with the good name exist. */
                    bool                        LineExist(const std::string &name);
                    /** \return the chils list contained in the Object. */
                    inline ListObject           &ListChild()                                                    {return _content;}
                    /** Add a child in the list of child. */
                    inline Object               *AddChild(Object *o)                                            {_content.push_back(o); return *_content.rbegin();}


                protected:
                    inline void                 Type(TYPE t)                                                    {_type = t;}

                    std::string         _name;      ///< the name.
                    TYPE                _type;      ///< the type.
                    MapParam            _params;    ///< the map of params stored with (key/values).
                    Object              *_parent;   ///< the parent of the object.
                    ListObject          _content;   ///< the list of childs.
                    std::string         _data;      ///< the data.
                    bool                _cdata;     ///< a booleen used to know if we need to store the data in a CBLOCK data.

                    friend class Parser;
            };
        }
    }
}

#endif
