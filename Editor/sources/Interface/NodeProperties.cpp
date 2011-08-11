
#include <Nc/Graphics/Object/Object.h>
#include <Nc/GUI/Widget.h>
#include "NodeProperties.h"

using namespace Nc;
using namespace Nc::Editor::Interface;

IObjectProperties::IObjectProperties(StandardEditorItem *item)
    : _item(item)
{
    if (item == NULL)
        throw Utils::Exception("IObjectProperties", "Can't create the properties with an item null.");

    if (item->GetData() != NULL)
    {
        std::string s = item->GetData()->GetClassName();
        setObjectName(s.c_str());
    }
}

IObjectProperties::~IObjectProperties()
{
}

QString   IObjectProperties::Name() const
{
    if (_item->GetData() != NULL)
      return _item->GetData()->Name().c_str();
    return "";
}

int IObjectProperties::Id() const
{
    if (_item->GetData() != NULL)
      return _item->GetData()->Id();
    return -1;
}

NodeProperties::NodeProperties(StandardEditorItem *item)
    : IObjectProperties(item)
{
}

bool	NodeProperties::Enable() const
{
    Graphic::ISceneNode *node = static_cast<Graphic::ISceneNode*>(_item->GetData());
    if (node != NULL)
      return node->Enabled();
    return true;
}

void	NodeProperties::Enable(bool state)
{
  Graphic::ISceneNode *node = static_cast<Graphic::ISceneNode*>(_item->GetData());
  if (node != NULL)
    node->Enable(state);
}

SceneProperties::SceneProperties(StandardEditorItem *item)
    : NodeProperties(item)
{
}

CameraProperties::CameraProperties(StandardEditorItem *item)
    : NodeProperties(item)
{
}

bool    CameraProperties::UseWindowSizeViewport() const
{
    Graphic::Camera *cam = static_cast<Graphic::Camera*>(_item->GetData());
    if (cam != NULL)
      return cam->UseWindowSizeForViewport();
    return true;
}

void    CameraProperties::UseWindowSizeViewport(bool state)
{
    if (state == true)
    {
        Graphic::Camera *cam = static_cast<Graphic::Camera*>(_item->GetData());
        if (cam != NULL)
            cam->ResetViewport();
    }
}

QRect   CameraProperties::Viewport() const
{
    Graphic::Camera *cam = static_cast<Graphic::Camera*>(_item->GetData());
    if (cam != NULL)
      return QRect(cam->ViewportX(), cam->ViewportY(), cam->ViewportWidth(), cam->ViewportHeight());
    return QRect();
}

void          CameraProperties::Viewport(const QRect &rect)
{
    Graphic::Camera *cam = static_cast<Graphic::Camera*>(_item->GetData());
    if (cam != NULL)
        cam->SetViewport(rect.x(), rect.y(), rect.width(), rect.height());
}

Camera2dProperties::Camera2dProperties(StandardEditorItem *item)
    : CameraProperties(item)
{
}

Camera3dProperties::Camera3dProperties(StandardEditorItem *item)
    : CameraProperties(item)
{
}

StandardCamera3dProperties::StandardCamera3dProperties(StandardEditorItem *item)
    : Camera3dProperties(item)
{
}

StandardCamera3dProperties::Pattern StandardCamera3dProperties::GetPattern() const
{
    Graphic::StandardCamera3d *camera = static_cast<Graphic::StandardCamera3d*>(_item->GetData());
    if (camera != NULL)
        return static_cast<Pattern>(camera->GetPattern());
    return Trackball;
}

void    StandardCamera3dProperties::SetPattern(Pattern p)
{
    Graphic::StandardCamera3d *camera = static_cast<Graphic::StandardCamera3d*>(_item->GetData());
    if (camera != NULL)
        camera->SetPattern(static_cast<Graphic::StandardCamera3d::Pattern>(p));
}

ObjectProperties::ObjectProperties(StandardEditorItem *item)
    : NodeProperties(item)
{
}

bool	ObjectProperties::DisplayBox() const
{
    Graphic::Object *obj = static_cast<Graphic::Object*>(_item->GetData());
    if (obj != NULL)
        return obj->DisplayBox();
    return false;
}

void	ObjectProperties::DisplayBox(bool state)
{
    Graphic::Object *obj = static_cast<Graphic::Object*>(_item->GetData());
    if (obj != NULL)
        obj->DisplayBox(state);
}

EffectProperties::EffectProperties(StandardEditorItem *item)
    : NodeProperties(item)
{
}

bool	EffectProperties::Activated() const
{
    Graphic::Effect *eff = static_cast<Graphic::Effect*>(_item->GetData());
    if (eff != NULL)
        return eff->Activated();
    return true;
}

void	EffectProperties::Activated(bool state)
{
    Graphic::Effect *eff = static_cast<Graphic::Effect*>(_item->GetData());
    if (eff != NULL)
        eff->Activated(state);
}

GUISceneProperties::GUISceneProperties(StandardEditorItem *item)
    : SceneProperties(item)
{
}

WidgetProperties::WidgetProperties(StandardEditorItem *item)
    : ObjectProperties(item)
{
}

bool  WidgetProperties::Inhibit() const
{
    GUI::Widget *w = static_cast<GUI::Widget*>(_item->GetData());
    if (w != NULL)
        return w->Inhibited();
    return false;
}

void  WidgetProperties::Inhibit(bool state)
{
    GUI::Widget *w = static_cast<GUI::Widget*>(_item->GetData());
    if (w != NULL)
        w->Inhibit(state);
}

