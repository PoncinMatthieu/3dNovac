
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
            class Object;
            typedef std::list<Object*>                  ListObject;     ///< typedef to define a list of Xml::Object
            typedef std::map<std::string, std::string>  MapParam;       ///< typedef to define an std::map of double std::string (key/value)

            /// Xml::Object, provide an element of a DOM Model
            class LCORE Object
            {
                public:
                    /** TYPE, to create an Object Xml */
                    enum TYPE {HEADER, INLINE, BLOCK};

                public:
                    Object(Object *parent = NULL);
                    Object(const std::string &name, TYPE type = INLINE, Object *parent = NULL);
                    Object(const std::string &name, TYPE type, const std::map<std::string, std::string> &params, Object *parent = NULL);
                    Object(const Object &o);
                    virtual ~Object();

                    Object                  &operator = (const Object &o);
                    friend std::ostream     &operator << (std::ostream &out, const Object &o) {o.Write(out, 0); return out;}

                    virtual void Write(std::ostream &out, unsigned int i) const;

                    //accesseurs
                    // values
                    inline TYPE                 Type() const                        {return _type;}
                    inline void                 Name(const std::string &s)                                      {_name = s;}
                    inline const std::string    &Name() const                                                   {return _name;}
                    inline void                 AddParam(const std::string &key, const std::string &value)      {_params[key] = value;}
                    inline const std::string    &Param(const std::string &key)                                  {return _params[key];}
                    inline bool                 ParamExist(const std::string &key)                              {return (_params.find(key) != _params.end());}
                    inline MapParam             &Params()                                                       {return _params;}
                    inline std::string          &Data()                                                         {return _data;}
                    inline void                 Data(const std::string &s)                                      {_data = s;}
                    inline void                 CData(bool b)                                                   {_cdata = b;}
                    inline bool                 CData()                                                         {return _cdata;}

                    // parent
                    inline Object               *Parent()                           {return _parent;}
                    inline const Object         *Parent() const                     {return _parent;}
                    unsigned int                GetNbParent() const;

                    // child
                    Object                      *NewBlock(const std::string &name)  {return AddChild(new Object(name, BLOCK, this));}       /** Create a new Block object in the Object */
                    Object                      *NewLine(const std::string &name)   {return AddChild(new Object(name, INLINE, this));}      /** Create a new Inline object in the Object */
                    Object                      *Block(const std::string &name);        /** Return the block with the good name, Throw if don't exist */
                    bool                        BlockExist(const std::string &name);    /** Return true if the block with the good name exist */
                    Object                      *Line(const std::string &name);         /** Return the line with the good name, Throw if don't exist */
                    bool                        LineExist(const std::string &name);     /** Return true if the line with the good name exist */
                    inline ListObject           &ListChild()                        {return _content;}  /** Return the chils list contained in the Object */
                    inline Object               *AddChild(Object *o)        {_content.push_back(o); return *_content.rbegin();} /** Add a child in the list of child */


                protected:
                    inline void                 Type(TYPE t)                        {_type = t;}

                    std::string     _name;
                    TYPE            _type;
                    MapParam        _params;
                    Object          *_parent;
                    ListObject      _content;
                    std::string     _data;
                    bool            _cdata;

                    friend class Parser;
            };
        }
    }
}

#endif
