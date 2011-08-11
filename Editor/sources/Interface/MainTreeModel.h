
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

#ifndef EDITOR_MAINTREEMODEL_H_
#define EDITOR_MAINTREEMODEL_H_

#include <QStandardItemModel>
#include <QTreeView>
#include <propertyeditor.h>
#include <Nc/Graphics/Scene/SceneGraph.h>
#include <Nc/GUI/SceneGraph.h>
#include "StandardEditorItem.h"

namespace Nc
{
  namespace Editor
  {
    namespace Interface
    {
      class MainTreeModel : public QStandardItemModel
      {
	Q_OBJECT;

      public:
	typedef std::list<StandardEditorItem*>      ListStandardEditorItem;

      public:
        MainTreeModel(QObject *parent, QTreeView *view, PropertyEditor::PropertyEditor  *propertyEditor);
	virtual ~MainTreeModel();

	virtual Qt::DropActions 	supportedDropActions () const           {return Qt::MoveAction;}
        virtual Qt::ItemFlags           flags(const QModelIndex &index) const;

      public slots:
	// refresh tree
        void    SlotRefreshScenes(Nc::Graphic::ListPScene *listScene);
	void    SlotRefreshScene(Nc::Graphic::SceneGraph *scene);

	// view
	void    SlotDropItem(QModelIndex parent, int start, int end);

	// change tree data (font/text)
	void    SlotChangeCurrentScene(Nc::Graphic::SceneGraph *currentScene);

	// change selected state
	void    SlotItemSelected(const QModelIndex &index);
	void    SlotItemDoubleClicked(const QModelIndex &index);
	void    SlotItemChanged(QStandardItem *item);

	// menu contextuel
	void    SlotShowTreeContextMenu(const QPoint &point);

      signals:
        void    SignalChangedCurrentScene(Nc::Graphic::SceneGraph*);
	void    SignalRemoveScene(Nc::Graphic::SceneGraph*);
	void    SignalRemoveNode(Nc::Graphic::ISceneNode*);
	void    SignalSelectedNode(Nc::Graphic::ISceneNode*);
        void    SignalMoveNode(Nc::Graphic::ISceneNode*, int, Nc::Graphic::ISceneNode*, Nc::Graphic::ISceneNode*, int);

      private slots:
	void    SlotActionDelete();
	void    SlotActionRename();

      private:
	// refresh items
        void    AddSceneItem(QStandardItem *parent, Graphic::SceneGraph *scene);
        void    AddNodesItem(QStandardItem *parent, Graphic::SceneGraph::ContainerType &nodes);
        void    AddNodeItem(QStandardItem *parent, Graphic::ISceneNode *node);

	// menu contextuel
	void    ShowMenuObject(Nc::Graphic::Object *obj, const QPoint &point);

	StandardEditorItem  *GetSceneItem(Nc::Graphic::SceneGraph *scene);
	QStandardItem       *GetItemWithText(QStandardItem *item, const QString &name);

      private:
        QTreeView                       *_view;
        PropertyEditor::PropertyEditor  *_propertyEditor;

	StandardEditorItem      *_currentContextMenuItem;
	QAction                 *_actionDelete;
	QAction                 *_actionRename;
	QAction                 *_actionStandardCameraPatternTurntable;
	QAction                 *_actionStandardCameraPatternTrackball;
	bool                    _updateItemStatus;
      };
    }
  }
}

#endif
