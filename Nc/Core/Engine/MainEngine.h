
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

#ifndef NC_CORE_ENGINE_MAINENGINE_H_
#define NC_CORE_ENGINE_MAINENGINE_H_

#include "IEngine.h"
#include "../System/Input/InputListener.h"

namespace Nc
{
    namespace Engine
    {
        /// Abstract class to help the implementation of a Main Engine witch receive events from an InputListener
        class LCORE MainEngine : public IEngine, public System::InputListener
        {
            public:
                MainEngine(const char *className, Manager *manager, unsigned int pattern = HasAContext | WaitingLoadContentsOfOthersEngines,
                           unsigned char deletePriority = 2, unsigned char loadingContextPriority = 2, unsigned int loadingPriority = 2);
                MainEngine(Manager *manager, unsigned int pattern = HasAContext | WaitingLoadContentsOfOthersEngines,
                           unsigned char deletePriority = 2, unsigned char loadingContextPriority = 2, unsigned int loadingPriority = 2);
                virtual ~MainEngine();

                static const char *ClassName()          {return "MainEngine";}

            protected:
                /**
                    Use this function to stop the engines normally.
                    Call the function `ReleaseContent` to release many contents and check if we really want to stop the execution of all engines.
                    So if the function `ReleaseContent` return false, do not stop the engines.
                */
                void            Quit();

                /**
                    Can be redefined, to make something before exiting
                    \return true if we want to stop the engines, otherwise the engines will not be powered off
                */
                virtual bool    ExitEvent()			{return true;}

                /**
                    Called at each execution of the MainEngine. To redefine
                    \param runningTime in second
                 */
                virtual void    Update(float runningTime) = 0;

                /**
                    Called at each input evenement received, and call the associated funciton `KeyboardEvent`, `MouseButtonEvent`, `MouseMotionEvent`
                */
                virtual void    ManageWindowEvent(System::Event &event);

                /**
                    Called at each keyboard input received. To redefine
                 */
                virtual void    KeyboardEvent(System::Event &event) = 0;

                /**
                    Called at each mouse button input received. To redefine
                 */
                virtual void    MouseButtonEvent(System::Event &event) = 0;

                /**
                    Called at each mouse motion input received. To redefine
                 */
                virtual void    MouseMotionEvent(System::Event &event) = 0;

            private:
                /** Retreive the events and call the function `ManageWindowEvent` */
                void            ManageWindowEvents();

                /**
                    Manage the event and Update the engine
                    \param runningTime in second
                */
                void Execute(float runningTime);
        };
    }
}

#endif

