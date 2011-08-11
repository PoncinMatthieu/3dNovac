
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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <GL/glew.h>
#include <QtGui>
#include <propertyeditor.h>

#include "../Define.h"
#include "ui_MainWindow.h"
#include "MainTreeModel.h"
#include "Logger.h"

namespace Nc
{
  namespace Editor
  {
    class EditorManager;

    namespace Interface
    {
      class MainWindow : public QMainWindow, private Ui::MainWindow
      {
	Q_OBJECT;

      public:
        MainWindow(QWidget* parent = 0);
        virtual ~MainWindow();

        MainTreeModel       *TreeModel()        {return _treeModel;}

      public slots:
        void    SlotNewEngine(const std::string &name, Nc::Engine::IEngine *engine);
        void    SlotEngineDeleted(const std::string &name, Nc::Engine::IEngine *engine);
        void    SlotLoadSample();
        void    SlotLoadModel(QListWidgetItem *item);

        void    on_ActionRasterModePoints_triggered();
        void    on_ActionRasterModeWireframe_triggered();
        void    on_ActionRasterModeFill_triggered();
        void    on_ActionRasterModeFillWireframe_triggered();
        void    on_ActionRasterModeFillWireframeNoDepth_triggered();

        void    on_ActionRefreshTree_triggered();
        void    on_ActionAddScene_triggered();
        void    on_ActionAddObject_triggered();
        void    on_ActionAddLight_triggered();
        void    on_ActionAddEffect_triggered();
        void    on_ActionAddGUIObject_triggered();
        void    on_ActionAddGUIWidget_triggered();

        void    on__DWModal_visibilityChanged(bool state);
        void    on__TBConsolePromptOk_clicked();

        void    on_ActionDestroy_Current_Sample_triggered();

      signals:
        void    SignalLoadSample(const std::string &filename);

      private:
        void    GetSamples();
        void    ConnectAll();
        void    UncheckRasterModes(unsigned int no);

        template<typename T>
        T       *CreateModalFrame();

        Logger                          *_logger;
        QBoxLayout                      *_centralLayout;    // layout contenant le graphic engine
        QBoxLayout                      *_frameLayout;
        QFrame                          *_currentFrame;
        MainTreeModel                   *_treeModel;
        QTimer                          *_timerFPS;
        EditorManager                   *_editorManager;
        PropertyEditor::PropertyEditor  *_propertyEditor;
        std::list<QAction*>             _sampleLoadActions;
      };

      template<typename T>
      T    *MainWindow::CreateModalFrame()
      {
	on__DWModal_visibilityChanged(false);

	_DWModal->setVisible(true);
	_currentFrame = new QFrame;
	T  *ui = new T();
	ui->setupUi(_currentFrame);
	_frameLayout->addWidget(_currentFrame);
	return ui;
      }
    }
  }
}

#endif
