
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

#include <Nc/GUI/SceneGraph.h>
#include <Nc/Graphics/Camera/Camera3d.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include <Nc/Graphics/Effect/Effect.h>
#include "StandardEditorItem.h"
#include "NodeProperties.h"

using namespace Nc::Editor::Interface;

StandardEditorItem          *StandardEditorItem::_globalObjectDroped = NULL;
const StandardEditorItem    *StandardEditorItem::_globalLastGetDataObject = NULL;
StandardEditorItem          *StandardEditorItem::_globalOldParentDroped = NULL;
int                         StandardEditorItem::_globalOldPosDroped = -1;

StandardEditorItem::StandardEditorItem(Nc::System::Object *obj, Type type)
  : QStandardItem(), _type(type), _properties(NULL), _object(obj)
{
    CreateObjectProperties();
    if (obj != NULL)
    {
        std::string s = obj->GetClassName();
        if (!obj->Name().empty())
            s += " " + obj->Name();
        setText(s.c_str());
    }
}

StandardEditorItem::StandardEditorItem(const StandardEditorItem &item)
    : QStandardItem(item), _type(item._type), _properties(NULL), _object(item._object)
{
    CreateObjectProperties();
}

StandardEditorItem &StandardEditorItem::operator == (const StandardEditorItem &item)
{
    delete _properties;
    _properties = NULL;
    _type = item._type;
    _object = item._object;
    CreateObjectProperties();
    return *this;
}


StandardEditorItem::~StandardEditorItem()
{
  delete _properties;
  if (_globalLastGetDataObject == this)
    _globalLastGetDataObject = NULL;
  if (_globalObjectDroped == this)
    _globalObjectDroped = NULL;
}

void             StandardEditorItem::CreateObjectProperties()
{
    switch (_type)
    {
        case Node:              _properties = new NodeProperties(this);                 break;
        case Scene:             _properties = new SceneProperties(this);                break;
        case Camera2d:          _properties = new Camera2dProperties(this);             break;
        case Camera3d:          _properties = new Camera3dProperties(this);             break;
        case StandardCamera3d:  _properties = new StandardCamera3dProperties(this);     break;
        case Object:            _properties = new ObjectProperties(this);               break;
        case Effect:            _properties = new EffectProperties(this);               break;
        case GUIScene:          _properties = new GUISceneProperties(this);             break;
        case Widget:            _properties = new WidgetProperties(this);               break;
        default:                _properties = new IObjectProperties(this);              break;
    }
}

QStandardItem   *StandardEditorItem::clone() const
{
    if (_globalLastGetDataObject != NULL)
    {
        StandardEditorItem *newObj = new StandardEditorItem(*_globalLastGetDataObject);
        _globalOldParentDroped = static_cast<StandardEditorItem*>(_globalLastGetDataObject->parent());
        LOG << _globalOldParentDroped << std::endl;
        _globalObjectDroped = newObj;
        return newObj;
    }
    else
        return new StandardEditorItem(*this);
}

QVariant    StandardEditorItem::data(int role) const
{
    // for drag and drop management with the Qt clone system
    if (role == 31)
    {
        _globalLastGetDataObject = this;
        _globalOldPosDroped = _globalLastGetDataObject->row();
    }
    return QStandardItem::data(role);
}

void StandardEditorItem::setData(const QVariant &value, int role)
{
    QStandardItem::setData(value, role);
}

StandardEditorItem::Type StandardEditorItem::GetObjectType(Nc::System::Object *obj)
{
    if (obj->AsWithoutThrow<Nc::GUI::Widget>() != NULL)
        return Widget;
    else if (obj->AsWithoutThrow<Nc::Graphic::StandardCamera3d>() != NULL)
        return StandardCamera3d;
    else if (obj->AsWithoutThrow<Nc::Graphic::Camera3d>() != NULL)
        return Camera3d;
    else if (obj->AsWithoutThrow<Nc::Graphic::Camera2d>() != NULL)
        return Camera2d;
    else if (obj->AsWithoutThrow<Nc::Graphic::Object>() != NULL)
        return Object;
    else if (obj->AsWithoutThrow<Nc::Graphic::Effect>() != NULL)
        return Effect;
    else if (obj->AsWithoutThrow<Nc::Graphic::SceneGraph>() != NULL)
        return Scene;
    else if (obj->AsWithoutThrow<Nc::GUI::SceneGraph>() != NULL)
        return GUIScene;
    else if (obj->AsWithoutThrow<Nc::Graphic::ISceneNode>() != NULL)
        return Node;
    return SysObject;
}
