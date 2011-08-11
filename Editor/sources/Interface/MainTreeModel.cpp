
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

#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/Graphics/Effect/Effect.h>
#include "MainTreeModel.h"
#include "NodeProperties.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Editor::Interface;

MainTreeModel::MainTreeModel(QObject *parent, QTreeView *view, PropertyEditor::PropertyEditor  *propertyEditor)
    : QStandardItemModel(parent), _view(view), _propertyEditor(propertyEditor), _currentContextMenuItem(NULL),
      _updateItemStatus(false)
{
    _view->setModel(this);
    _view->setContextMenuPolicy(Qt::CustomContextMenu);
    _view->setDragDropMode(QAbstractItemView::InternalMove);

    // creation des actions des menu contextuel
    _actionDelete = new QAction("Delete", _view);
    _actionRename = new QAction("Rename", _view);
    _actionStandardCameraPatternTurntable = new QAction("Turntable", _view);
    _actionStandardCameraPatternTrackball = new QAction("Trackball", _view);
    _actionStandardCameraPatternTurntable->setCheckable(true);
    _actionStandardCameraPatternTrackball->setCheckable(true);

    connect(_actionDelete, SIGNAL(triggered()), this, SLOT(SlotActionDelete()));
    connect(_actionRename, SIGNAL(triggered()), this, SLOT(SlotActionRename()));
    connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(SlotItemChanged(QStandardItem*)));
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(SlotDropItem(QModelIndex,int,int)));
    _updateItemStatus = true;

    StandardEditorItem *itemProto = new StandardEditorItem(NULL, StandardEditorItem::SysObject);
    setItemPrototype(itemProto);
}

MainTreeModel::~MainTreeModel()
{
}

void    MainTreeModel::SlotRefreshScenes(ListPScene *listScene)
{
    _updateItemStatus = false;
    _propertyEditor->setObject(NULL);
    clear();
    for (ListPScene::iterator it = listScene->begin(); it != listScene->end(); ++it)
    {
        StandardEditorItem *item = new StandardEditorItem(*it, StandardEditorItem::Scene);
        item->setCheckState(((*it)->Enabled() ? Qt::Checked : Qt::Unchecked));
        item->setCheckable(true);
        item->setEditable(false);
        item->setDragEnabled(false);
        AddSceneItem(item, *it);
        appendRow(item);
    }
    _updateItemStatus = true;
}

void    MainTreeModel::SlotChangeCurrentScene(Nc::Graphic::SceneGraph *currentScene)
{
    if (currentScene == NULL)
        return;

    _updateItemStatus = false;
    for (int i = 0; i < rowCount(); i++)
    {
        StandardEditorItem *ourItem = dynamic_cast<StandardEditorItem*>(item(i));
        if (ourItem != NULL && ourItem->GetType() == StandardEditorItem::Scene && ourItem->GetData<Graphic::SceneGraph>() != currentScene)
        {
            QFont font;
            font.setBold(false);
            font.setUnderline(false);
            ourItem->setFont(font);
        }
    }

    StandardEditorItem *item = GetSceneItem(currentScene);
    if (item != NULL)
    {
        // set bold
        QFont font;
        font.setBold(true);
        font.setUnderline(true);
        item->setFont(font);

        // expand
        _view->expand(indexFromItem(item));
    }
    _updateItemStatus = true;
}

StandardEditorItem  *MainTreeModel::GetSceneItem(Nc::Graphic::SceneGraph *scene)
{
    for (int i = 0; i < rowCount(); i++)
    {
        StandardEditorItem *ourItem = dynamic_cast<StandardEditorItem*>(item(i));
        if (ourItem != NULL && ourItem->GetData<Graphic::SceneGraph>() == scene)
            return ourItem;
    }
    return NULL;
}

QStandardItem *MainTreeModel::GetItemWithText(QStandardItem *item, const QString &name)
{
    for (int i = 0; i < item->rowCount(); i++)
    {
        QStandardItem *ourItem = item->child(i);
        if (ourItem->text() == name)
            return ourItem;
    }
    return NULL;
}

void MainTreeModel::SlotRefreshScene(Graphic::SceneGraph *scene)
{
    if (scene == NULL)
        return;

    _updateItemStatus = false;
    StandardEditorItem *item = GetSceneItem(scene);
    _propertyEditor->setObject(NULL);
    item->removeRows(0, item->rowCount());
    AddSceneItem(item, scene);

    QModelIndex index = indexFromItem(item);
    _view->expand(index);
    _updateItemStatus = true;
}

void    MainTreeModel::AddSceneItem(QStandardItem *parent, SceneGraph *scene)
{
    AddNodesItem(parent, scene->Childs());
}

void    MainTreeModel::AddNodesItem(QStandardItem *parent, SceneGraph::ContainerType &nodes)
{
    for (SceneGraph::ContainerType::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        AddNodeItem(parent, *it);
    }
}

void    MainTreeModel::AddNodeItem(QStandardItem *parent, ISceneNode *node)
{
    _updateItemStatus = false;
    StandardEditorItem *objItem = new StandardEditorItem(node, StandardEditorItem::GetObjectType(node));
    objItem->setEditable(false);
    parent->appendRow(objItem);

    Entity *e = node->AsWithoutThrow<Entity>();
    if (e != NULL)
    {
        AddNodesItem(objItem, e->Childs());
        objItem->setCheckState((e->Enabled() ? Qt::Checked : Qt::Unchecked));
        objItem->setCheckable(true);
    }
    _updateItemStatus = true;
}

void    MainTreeModel::SlotItemSelected(const QModelIndex &index)
{
    QStandardItem   *ourItem = itemFromIndex(index);

    if (ourItem != NULL)
    {
        StandardEditorItem  *editorItem = dynamic_cast<StandardEditorItem*>(ourItem);
        if (editorItem != NULL)
        {
            _propertyEditor->setObject(editorItem->Properties());
            Graphic::ISceneNode *n = editorItem->GetData<Graphic::ISceneNode>();
            if (n != NULL)
                emit SignalSelectedNode(n);
        }
    }
}

void    MainTreeModel::SlotItemDoubleClicked(const QModelIndex &index)
{
    QStandardItem   *ourItem = itemFromIndex(index);

    if (ourItem != NULL)
    {
        StandardEditorItem  *item = dynamic_cast<StandardEditorItem*>(ourItem);
        if (item != NULL)
        {
            switch (item->GetType())
            {
                case StandardEditorItem::Scene:
                    emit SignalChangedCurrentScene(item->GetData<Graphic::SceneGraph>());
                    break;
                default:
                    break;
            }
        }
    }
}

void    MainTreeModel::SlotShowTreeContextMenu(const QPoint &point)
{
    QModelIndex index = _view->indexAt(point);
    if (index.isValid())
    {
        StandardEditorItem   *item = dynamic_cast<StandardEditorItem*>(itemFromIndex(index));

        if (item != NULL)
        {
            _currentContextMenuItem = item;
            switch (item->GetType())
            {
                /*
                case StandardEditorItem::Scene:
                case StandardEditorItem::Camera3d:
                case StandardEditorItem::Object:
                */
                default:
                    ShowMenuObject(item->GetData<Graphic::Object>(), point);
                    break;
            }
        }
    }
}

void    MainTreeModel::ShowMenuObject(Graphic::Object *, const QPoint &point)
{
    QList<QAction*> actions;
    actions.append(_actionRename);
    actions.append(_actionDelete);

    QMenu::exec(actions, _view->mapToGlobal(point));
}

void    MainTreeModel::SlotActionDelete()
{
    _propertyEditor->setObject(NULL);
    emit SignalRemoveNode(_currentContextMenuItem->GetData<Graphic::ISceneNode>());
}

void    MainTreeModel::SlotActionRename()
{
    // choose a name
    QString     name;
    bool        ok = true;
    while (ok)
    {
        name = QInputDialog::getText(_view, "Scene name", "Choose a name:", QLineEdit::Normal, _currentContextMenuItem->GetData()->Name().c_str(), &ok);
        if (ok)
        {
            if (name.size() == 0)
                QMessageBox::critical(_view, "Wrong name", "Please choose a name !");
            else
            {
                ok = false;
                // change the name
                _currentContextMenuItem->GetData()->Name(name.toLocal8Bit().data());
                _currentContextMenuItem->setText(name);
            }
        }
    }
}

void    MainTreeModel::SlotItemChanged(QStandardItem *item)
{
    StandardEditorItem *ourItem = dynamic_cast<StandardEditorItem*>(item);

    // MAJ le check status
    if (_updateItemStatus && ourItem != NULL)
    {
        NodeProperties *p = dynamic_cast<NodeProperties*>(ourItem->Properties());
        if (p != NULL)
            p->Enable(item->checkState());
    }
}

void    MainTreeModel::SlotDropItem(QModelIndex parentIndex, int start, int)
{
    if (parentIndex.isValid())
    {
        StandardEditorItem  *itemDroped = StandardEditorItem::GetGlobalObjectDroped();
        StandardEditorItem  *parentObjDroped = StandardEditorItem::GetGlobalOldParentDroped();
        int                 oldPos = StandardEditorItem::GetGlobalOldPosDroped();
        if (itemDroped != NULL && parentObjDroped != NULL)
        {
            StandardEditorItem  *parent = static_cast<StandardEditorItem*>(itemFromIndex(parentIndex));
            emit SignalMoveNode(parent->GetData<Graphic::ISceneNode>(), start, itemDroped->GetData<Graphic::ISceneNode>(), parentObjDroped->GetData<Graphic::ISceneNode>(), oldPos);
        }
    }
}

Qt::ItemFlags MainTreeModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

  if (index.isValid())
      return Qt::ItemIsDropEnabled | defaultFlags;
  return defaultFlags & ~Qt::ItemIsDropEnabled;
}
