
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

    File Created At:        25/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_ICONTEXT_H_
#define NC_CORE_ENGINE_ICONTEXT_H_

#include "../Define.h"

namespace Nc
{
    namespace Engine
    {
        /// Interface to define a Context to be used with an Engine
        /**
            The concept of concept follow the concept used by opengl contexts.

            A context must follow the folowing rule:
                - A context can be used in more than one thread but cannot be active in more than one thread at a time.
        */
        class IContext : public Utils::NonCopyable
        {
			public:
				typedef std::map<unsigned int, IContext*>   MapPIDContext;

            public:
                IContext() : _currentThreadId(0)        {}
                virtual ~IContext()                     {}

                /** Active the context, should set the currentThreadId */
                virtual void        Active() = 0;

                /** Disable the context, should unset the currentThreadId to 0 */
                virtual void        Disable() = 0;

                /** \return the current thread id in which the context is currently active. 0 means that the context is disable. */
                inline unsigned int CurrentThreadId() const             {return _currentThreadId;}

            protected:
				unsigned int	_currentThreadId;	///< Register the thread id in which the context is currently active
        };
    }
}

#endif
