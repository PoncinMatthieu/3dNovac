
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

    File Created At:        14/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                Implementation de la classe "GameEngine"

                    Moteur permettant la gestion du jeu
                en étroite relation avec le moteur de ressource

Herite de Engine

-----------------------------------------------------------------------------*/


#ifndef NC_CORE_ENGINE_GAMEENGINE_H_
#define NC_CORE_ENGINE_GAMEENGINE_H_

#include "IEngine.h"
#include "../System/Input/InputManager.h"

namespace Nc
{
    namespace Engine
    {
        class LCORE MainEngine : public IEngine, public System::InputManager
        {
            public:
                MainEngine(Manager *manager, unsigned int pattern = HasAContext | WaitingLoadContentsOfOthersEngines,
                           unsigned char deletePriority = 2, unsigned char loadingContextPriority = 2, unsigned int loadingPriority = 2);
                virtual ~MainEngine();

                void            ManageWindowEvents(float runningTime);

            protected:
                void            Quit();
                virtual bool    ReleaseContent()    {return true;}   /// can be redefined by heritance, to make something before exiting, no exit if return false

                virtual void    Update(float runningTime) = 0;
                virtual void    ManageWindowEvent(System::Event &event, float runningTime);
                virtual void    KeyboardEvent(System::Event &event, float runningTime) = 0;
                virtual void    MouseButtonEvent(System::Event &event, float runningTime) = 0;
                virtual void    MouseMotionEvent(System::Event &event, float runningTime) = 0;

            private:
                void Execute(float runningTime);
        };
    }
}

#endif
