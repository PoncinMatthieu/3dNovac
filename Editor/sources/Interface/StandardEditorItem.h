
/*-----------------------------------------------------------------------------

	3dNovac Editor
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef EDITOR_STANDARDEDITORITEM_H_
#define EDITOR_STANDARDEDITORITEM_H_

#include <QStandardItem>
#include <QVariant>
#include <Nc/Core/System/Object.h>
#include <Nc/Core/Utils/Logger.h>

namespace Nc
{
  namespace Editor
  {
    namespace Interface
    {
      class IObjectProperties;

      class StandardEditorItem : public QStandardItem
      {
      public:
	enum Type
	  {
	    SysObject,
	    Node,
	    Scene,
	    Camera2d,
            Camera3d,
            StandardCamera3d,
            Object,
	    Effect,
	    GUIScene,
	    Widget
	  };

      public:
        StandardEditorItem(System::Object *obj = NULL, Type type = SysObject);
        StandardEditorItem(const StandardEditorItem &item);
        StandardEditorItem &operator == (const StandardEditorItem &item);
        virtual ~StandardEditorItem();

	// for drag and drop management
	virtual QStandardItem       *clone() const;
	static StandardEditorItem   *GetGlobalObjectDroped()                    {StandardEditorItem *obj = _globalObjectDroped; _globalObjectDroped = NULL; return obj;}
        static StandardEditorItem   *GetGlobalOldParentDroped()                 {StandardEditorItem *obj = _globalOldParentDroped; _globalOldParentDroped = NULL; return obj;}
        static int                  GetGlobalOldPosDroped()                     {int pos = _globalOldPosDroped; _globalOldPosDroped = -1; return pos;}

	static Type                 GetObjectType(Nc::System::Object *obj);
	inline Type                 GetType() const				{return _type;}

	template<typename T>
	T                           *GetData();
	System::Object              *GetData()					{return _object;}

        virtual int                 type() const				{return UserType;}
        IObjectProperties           *Properties()				{return _properties;}
        const IObjectProperties	    *Properties() const				{return _properties;}
	virtual QVariant            data(int role = Qt::UserRole + 1) const;
	virtual void                setData(const QVariant &value, int role = Qt::UserRole + 1);

      private:
        void                        CreateObjectProperties();

      private:
	Type                    _type;
        IObjectProperties       *_properties;
	System::Object		*_object;

	// for drag and drop management with the Qt clone system
	static StandardEditorItem           *_globalObjectDroped;
	static const StandardEditorItem     *_globalLastGetDataObject;
        static StandardEditorItem           *_globalOldParentDroped;
        static int                          _globalOldPosDroped;
      };
    }
  }
}

Q_DECLARE_METATYPE(Nc::Editor::Interface::StandardEditorItem);

namespace Nc
{
  namespace Editor
  {
    namespace Interface
    {
      template<typename T>
      T  *StandardEditorItem::GetData()
      {
	if (_object == NULL)
	  throw Utils::Exception("StandardEditorItem", "The data of the Item is null.");
	return dynamic_cast<T*>(_object);
      }
    }
  }
}

#endif
