
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

#include <iostream>
#include <QDir>
#include "ui_FrameAddObject.h"
#include "MainWindow.h"
#include "../Editor/EditorManager.h"
#include "../Engines/QManager.h"
#include "../Engines/QSceneGraphManager.h"


using namespace std;
using namespace Nc;
using namespace Nc::Editor;
using namespace Nc::Editor::Interface;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _currentFrame(NULL)
{
    setupUi(this);	// chargement de la fenetre cree avec QtDesigner
    _DWModal->setVisible(false);

    _logger = new Logger(_TEConsole);
    Utils::Logger::SetLogger(_logger);
    CONFIG.Load("3dNovac.conf");

    // creation de l'editorManager
    _editorManager = new EditorManager(this);
    for (Engine::Manager::MapEngine::const_iterator it = _editorManager->ThreadManager()->GetEngines().begin(); it != _editorManager->ThreadManager()->GetEngines().end(); ++it)
        SlotNewEngine(it->first, it->second.engine);

    // creation des actions pour load les samples
    GetSamples();

    // creation du layout permettant le redimentionnement de l'ecran du graphic engine
    _centralLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    _centralLayout->addWidget(_editorManager->Graphic(), 0);
    _WCenter->setLayout(_centralLayout);

    // layout du modal
    _frameLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    _DWModalContent->setLayout(_frameLayout);

    // creation du timer qui permetra d'afficher la scene a 60 FPS
    _timerFPS = new QTimer(this);
    _timerFPS->start(17);       // 60 frame par second

    // creation du property editor
    QBoxLayout *propertyLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    _DWPropertiesContent->setLayout(propertyLayout);
    _propertyEditor = new PropertyEditor::PropertyEditor(_DWPropertiesContent);
    propertyLayout->addWidget(_propertyEditor, 0);

    // creation du model correspondant a l'arbre principal
    _treeModel = new MainTreeModel(this, _TVProject, _propertyEditor);

    // creation des connection
    ConnectAll();

    // lancement des engines
    _editorManager->Start();
}

MainWindow::~MainWindow()
{
    _logger->LogIntoTextEdit(false);

    // avant de delete l'editorManager, on doit supprimer le GraphicEngine de sont layout
    // pour qu'il soit delete uniquement avec les autres engine
    _editorManager->Graphic()->setParent(NULL);

    // delete l'editor manager
    static_cast<QSceneGraphManager*>(_editorManager->Graphic()->GetSceneManager())->EmitChanges(false);
    delete _editorManager;
    CONFIG.DeleteInstance(); // delete l'instance de config
    LOG << "Destroy window" << std::endl;
}

void MainWindow::ConnectAll()
{
    // quiter l'application
    connect(ActionExit,                 SIGNAL(triggered()),
            this,                       SLOT(close()));
    connect(_editorManager->Editor(),   SIGNAL(SignalStop()),
            this,                       SLOT(close()));

    // from this
    connect(this,                       SIGNAL(SignalLoadSample(const std::string&)),
            _editorManager,             SLOT(SlotLoadSample(const std::string&)));

    // on TreeModel
    connect(_editorManager->Editor(),   SIGNAL(SignalScenesChanged(Nc::Graphic::ListPScene*)),
            _treeModel,                 SLOT(SlotRefreshScenes(Nc::Graphic::ListPScene*)));
    connect(static_cast<QSceneGraphManager*>(_editorManager->Graphic()->GetSceneManager()),   SIGNAL(SignalScenesChanged(Nc::Graphic::ListPScene*)),
            _treeModel,                 SLOT(SlotRefreshScenes(Nc::Graphic::ListPScene*)));
    connect(_editorManager->Editor(),   SIGNAL(SignalCurrentSceneChanged(Nc::Graphic::SceneGraph*)),
            _treeModel,                 SLOT(SlotChangeCurrentScene(Nc::Graphic::SceneGraph*)));
    connect(_editorManager->Editor(),   SIGNAL(SignalSceneChanged(Nc::Graphic::SceneGraph*)),
            _treeModel,                 SLOT(SlotRefreshScene(Nc::Graphic::SceneGraph*)));
    connect(_TVProject,                 SIGNAL(clicked(const QModelIndex&)),
            _treeModel,                 SLOT(SlotItemSelected(const QModelIndex&)));
    connect(_TVProject,                 SIGNAL(doubleClicked(const QModelIndex&)),
            _treeModel,                 SLOT(SlotItemDoubleClicked(const QModelIndex&)));
    connect(_TVProject,                 SIGNAL(customContextMenuRequested(const QPoint&)),
            _treeModel,                 SLOT(SlotShowTreeContextMenu(const QPoint&)));

    // To the EditorManager
    connect(_treeModel,         SIGNAL(SignalSelectedNode(Nc::Graphic::ISceneNode*)),
            _editorManager,     SLOT(SlotSelectNode(Nc::Graphic::ISceneNode*)));
    connect(_treeModel,         SIGNAL(SignalChangedCurrentScene(Nc::Graphic::SceneGraph*)),
            _editorManager,     SLOT(SlotChangeCurrentScene(Nc::Graphic::SceneGraph*)));
    connect(_treeModel,         SIGNAL(SignalRemoveNode(Nc::Graphic::ISceneNode*)),
            _editorManager,     SLOT(SlotRemoveNode(Nc::Graphic::ISceneNode*)));
    connect(_treeModel,         SIGNAL(SignalMoveNode(Nc::Graphic::ISceneNode*,int,Nc::Graphic::ISceneNode*,Nc::Graphic::ISceneNode*, int)),
            _editorManager,     SLOT(SlotMoveNode(Nc::Graphic::ISceneNode*,int,Nc::Graphic::ISceneNode*,Nc::Graphic::ISceneNode*, int)));

    // from QManager
    connect(_editorManager->ThreadManager(),    SIGNAL(SignalNewEngine(const std::string&, Nc::Engine::IEngine*)),
            this,                               SLOT(SlotNewEngine(const std::string&, Nc::Engine::IEngine*)));
    connect(_editorManager->ThreadManager(),    SIGNAL(SignalEngineDeleted(const std::string&, Nc::Engine::IEngine*)),
            this,                               SLOT(SlotEngineDeleted(const std::string&, Nc::Engine::IEngine*)));

    // update du graphic engine a chaque timeout du timer
    connect(_timerFPS,                  SIGNAL(timeout()),
            _editorManager->Graphic(),  SLOT(update()));
}

void    MainWindow::GetSamples()
{
    // Fetch plugin names
    Utils::Xml::Object *gameConf = CONFIG->Block("Samples");
    for (Utils::Xml::ListObject::iterator it = gameConf->ListChild().begin(); it != gameConf->ListChild().end(); ++it)
    {
        if ((*it)->Name() == "Sample")
        {
            const std::string &name = (*it)->Param("name");
            QAction *act = menuPlugins->addAction(name.c_str());
            connect(act, SIGNAL(triggered()), this, SLOT(SlotLoadSample()));
        }
    }
}

void    MainWindow::SlotLoadSample()
{
    QAction *emetteur = dynamic_cast<QAction*>(sender());
    if (emetteur != NULL)
    {
        emit SignalLoadSample(emetteur->text().toLocal8Bit().data());
    }
}

void    MainWindow::on_ActionDestroy_Current_Sample_triggered()
{
    _editorManager->DestroyCurrentSample();
}

void    MainWindow::on_ActionRefreshTree_triggered()
{
    LOG << "refresh Tree" << std::endl;
    _treeModel->SlotRefreshScenes(&_editorManager->Editor()->Scenes());
    _treeModel->SlotChangeCurrentScene(_editorManager->Editor()->CurrentScene());
}

void    MainWindow::on__TBConsolePromptOk_clicked()
{
    Engine::Manager::PushEvent(_CBConsoleEngine->currentText().toLocal8Bit().data(), _LEConsolePrompt->text().toLocal8Bit().data());
}

void    MainWindow::SlotNewEngine(const std::string &name, Nc::Engine::IEngine *engine)
{
    LOG << "New Engine: " << name << std::endl;
    QVariant var;
    var.setValue(engine);
    _CBConsoleEngine->addItem(name.c_str(), var);
}

void MainWindow::SlotEngineDeleted(const std::string &name, Nc::Engine::IEngine *engine)
{
    LOG << "Engine Deleted: " << name << std::endl;
    QVariant var;
    var.setValue(engine);
    int index = _CBConsoleEngine->findData(var);
    _CBConsoleEngine->removeItem(index);
}

void MainWindow::SlotLoadModel(QListWidgetItem *item)
{
    _editorManager->LoadNode(item->data(3).toString().toLocal8Bit().data()); // demande a l'editor manager de loader le model
}

void    MainWindow::on__DWModal_visibilityChanged(bool state)
{
    if (_currentFrame != NULL && state == false)
    {
        _frameLayout->removeWidget(_currentFrame);
        delete _currentFrame;
        _currentFrame = NULL;
        _DWModal->setVisible(false);
    }
}

void    MainWindow::on_ActionAddScene_triggered()
{
    bool ok = true;
    while (ok)
    {
        QString name = QInputDialog::getText(this, "Scene name", "The name of the scene ?", QLineEdit::Normal, "Default-Scene", &ok);
        if (ok)
        {
            if (name.size() == 0)
                QMessageBox::critical(this, "Wrong Scene name", "Please choose a name !");
            else
            {
                ok = false;
                _editorManager->Editor()->PushEvent(EditorEngine::IndexCreateNewScene, string(name.toLocal8Bit().data()));
            }
        }
    }
}

void    MainWindow::on_ActionAddObject_triggered()
{
    Ui::FrameAddObject *ui = CreateModalFrame<Ui::FrameAddObject>();
    connect(ui->LWLibrary, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(SlotLoadModel(QListWidgetItem*)));

    // set la liste presente dans la library
    std::string path = _editorManager->CurrentProject()->LibraryPath() + "Object/";
    QDir d(path.c_str());

    QStringList filters;
    filters << "*.png";
    QStringList listString = d.entryList(filters, QDir::Files);
    for (QStringList::iterator it = listString.begin(); it != listString.end(); ++it)
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        std::string iconePath = path + it->toLocal8Bit().data();
        newItem->setIcon(QIcon(iconePath.c_str()));

        Utils::FileName f((*it).toLocal8Bit());
        std::string name = f.ShortFilename();
        std::string ext = ".dae";       ///\todo pour le moment on load en dur des .obj

        QDir dir(std::string(path + name).c_str());
        if (dir.exists())
        {
/*
            QStringList listStringIcone = dir.entryList(filters, QDir::Files);
            for (QStringList::iterator it = listStringIcone.begin(); it != listStringIcone.end(); ++it)
            {}
*/
            newItem->setData(3, QString(dir.path() + "/" + QString(name.c_str()) + QString(ext.c_str())));
            ui->LWLibrary->addItem(newItem);
        }
    }
}

void    MainWindow::on_ActionAddLight_triggered()
{
    LOG << "Not Implemented" << std::endl;
}

void    MainWindow::on_ActionAddEffect_triggered()
{
    LOG << "Not Implemented" << std::endl;
}

void    MainWindow::on_ActionAddGUIObject_triggered()
{
    LOG << "Not Implemented" << std::endl;
}

void    MainWindow::on_ActionAddGUIWidget_triggered()
{
    LOG << "Not Implemented" << std::endl;
}

void    MainWindow::UncheckRasterModes(unsigned int no)
{
    if (no != 1)
        ActionRasterModePoints->setChecked(false);
    if (no != 2)
        ActionRasterModeWireframe->setChecked(false);
    if (no != 3)
        ActionRasterModeFill->setChecked(false);
    if (no != 4)
        ActionRasterModeFillWireframe->setChecked(false);
    if (no != 5)
        ActionRasterModeFillWireframeNoDepth->setChecked(false);
}

void    MainWindow::on_ActionRasterModePoints_triggered()
{
    UncheckRasterModes(1);
    _editorManager->SlotChangeRasterMode(Nc::Graphic::RasterEffect::Points);
}

void    MainWindow::on_ActionRasterModeWireframe_triggered()
{
    UncheckRasterModes(2);
    _editorManager->SlotChangeRasterMode(Nc::Graphic::RasterEffect::Wireframe);
}

void    MainWindow::on_ActionRasterModeFill_triggered()
{
    UncheckRasterModes(3);
    _editorManager->SlotChangeRasterMode(Nc::Graphic::RasterEffect::Fill);
}

void    MainWindow::on_ActionRasterModeFillWireframe_triggered()
{
    UncheckRasterModes(4);
    _editorManager->SlotChangeRasterMode(Nc::Graphic::RasterEffect::FillWireframe);
}

void    MainWindow::on_ActionRasterModeFillWireframeNoDepth_triggered()
{
    UncheckRasterModes(5);
    _editorManager->SlotChangeRasterMode(Nc::Graphic::RasterEffect::FillWireframeNoDepth);
}
