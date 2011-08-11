
#ifndef QSCENEGRAPHMANAGER_H
#define QSCENEGRAPHMANAGER_H

#include "QGraphicEngine.h"
#include <Nc/Graphics/Scene/SceneGraphManager.h>

namespace Nc
{
    namespace Editor
    {
        class QSceneGraphManager : public QObject, public Graphic::SceneGraphManager
        {
            Q_OBJECT;

        public:
            QSceneGraphManager();
            virtual ~QSceneGraphManager();

            inline void     EmitChanges(bool state)        {_emitChanges = state;}

        public slots:

        signals:
            void            SignalScenesChanged(Nc::Graphic::ListPScene*);
            void            SignalSceneDeleted(Graphic::SceneGraph *);

        public:
            virtual void    AddScene(Graphic::SceneGraph *scene);
            virtual void    RemoveScene(Graphic::SceneGraph *scene);

        private:
            bool            _emitChanges;
        };
    }
}

#endif
