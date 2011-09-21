
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

    File Created At:        27/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_PLUGIN_H_
#define NC_CORE_SYSTEM_PLUGIN_H_

#include "../Define.h"

#ifdef SYSTEM_LINUX
	#include <dlfcn.h>
#endif

#include "../Utils/Exception.h"
#include "../Utils/FileName.h"

namespace Nc
{
    namespace System
    {
        /// Template class to load a plugin
        /**
            To load a plugin. Can load only one plugin by PluginLoader instance.
            A plugin should have this extern c symbol:
 \code
    T   *GetPlugin();
 \endcode

            The PluginLoader delete the instance at the next Load or at the destruction of the PluginLoader.
            If you have already delete the instance yourself, you can call the method `UnsetInstance` to avoid the delete. But the library will be unload in all case.
        */
        template<typename T>
        class PluginLoader
        {
            public:
                PluginLoader() : _handle(NULL), _instance(NULL)     {}
                virtual ~PluginLoader()                             {Close();}

                /** \return the instance of the plugin */
                T       *operator -> ()                             {return _instance;}
                /** \return the instance of the plugin */
                T       *operator * ()                              {return _instance;}

                /** \return the instance of the plugin */
                T       *GetInstance()                              {return _instance;}
                /** unset the instance of the plugin, to avoid the delete of the instance. But the library will be unload in all case.*/
                void    UnsetInstance()                             {_instance = NULL;}

                /** Load the plugin */
                void    Load(const Utils::FileName &file);

            protected:
                /** Open the plugin file */
                void    Open(const Utils::FileName &file);

                /** Close the plugin file */
                void    Close();

                /** Load the given symbol */
                template<typename SYM>
                SYM     LoadSymbol(const std::string &sym);

            private:
                typedef T *(*GetPluginFunc)();

				#ifdef SYSTEM_WINDOWS
					HMODULE	_handle;			///< The handle used to load the library on win32
				#else
					void    *_handle;			///< The handle used to load the library on linux
				#endif

				T       *_instance;				///< The plugin instance, loaded with the function "T *GetPlugin();" in the library
        };

        template<typename T>
        void    PluginLoader<T>::Load(const Utils::FileName &file)
        {
            Close();
            Open(file);
            GetPluginFunc GetPlugin = LoadSymbol<GetPluginFunc>("GetPlugin");
            _instance = GetPlugin();
        }

        template<typename T>
        void    PluginLoader<T>::Open(const Utils::FileName &file)
        {
			#ifdef SYSTEM_WINDOWS
				_handle = LoadLibrary(file.c_str());
				if(_handle == NULL)
				   throw Utils::Exception("Plugin", "Can't load the library " + file);
			#else
				_handle = dlopen(file.c_str(), RTLD_LAZY);
				if (!_handle)
					throw Utils::Exception("Plugin", dlerror());
				dlerror();    // Clear any existing error
			#endif
		}

        template<typename T>
        void    PluginLoader<T>::Close()
        {
            if (_instance)
            {
                delete _instance;
                _instance = NULL;
            }
            if (_handle)
            {
				#ifdef SYSTEM_WINDOWS
					FreeLibrary(_handle);
				#else
					dlclose(_handle);
				#endif
				_handle = NULL;
            }
        }

        template<typename T>
        template<typename SYM>
        SYM     PluginLoader<T>::LoadSymbol(const std::string &sym)
        {
            if (_handle == NULL)
                throw Utils::Exception("plugin", "Can't load the symbol `" + sym + "`, the library is not loaded.");

			#ifdef SYSTEM_WINDOWS
				FARPROC symbol = GetProcAddress(_handle, sym.c_str());
				if(symbol == NULL)
					throw Utils::Exception("Plugin", "Can't load the symbol.");
				return (SYM)symbol;
			#else
				char *error;
				SYM s = (SYM)dlsym(_handle, sym.c_str());
				if ((error = dlerror()) != NULL)
					throw Utils::Exception("Plugin", error);
				return s;
			#endif
        }
    }
}

#endif // NC_CORE_SYSTEM_PLUGIN_H_
