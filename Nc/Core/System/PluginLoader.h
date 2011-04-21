
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

#include <dlfcn.h>
#include "../Define.h"
#include "../Utils/Exception.h"
#include "../Utils/FileName.h"

namespace Nc
{
    namespace System
    {
        /// template class to load a plugin
        /**
            A plugin should have this extern c symbol:
            T *GetPlugin();
        */
        template<typename T>
        class PluginLoader
        {
            public:
                PluginLoader() : _handle(NULL), _instance(NULL)     {}
                virtual ~PluginLoader()                             {Close();}

                /** return the instance of the plugin */
                T *operator -> ()                                   {return _instance;}

                /** return the instance of the plugin */
                T *GetInstance()                                    {return _instance;}

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

                void    *_handle;
                T       *_instance;
        };

        template<typename T>
        void    PluginLoader<T>::Load(const Utils::FileName &file)
        {
            Open(file);
            GetPluginFunc GetPlugin = LoadSymbol<GetPluginFunc>("GetPlugin");
            _instance = GetPlugin();
        }

        template<typename T>
        void    PluginLoader<T>::Open(const Utils::FileName &file)
        {
            _handle = dlopen(file.c_str(), RTLD_LAZY);
            if (!_handle)
                throw Utils::Exception("Plugin", dlerror());
            dlerror();    // Clear any existing error
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
                dlclose(_handle);
                _handle = NULL;
            }
        }

        template<typename T>
        template<typename SYM>
        SYM     PluginLoader<T>::LoadSymbol(const std::string &sym)
        {
            if (_handle == NULL)
                throw Utils::Exception("plugin", "Can't load the symbol `" + sym + "`, the library is not loaded.");

            char *error;
            SYM s = (SYM)dlsym(_handle, sym.c_str());

            if ((error = dlerror()) != NULL)
                throw Utils::Exception("Plugin", error);
            return s;
        }
    }
}

#endif // NC_CORE_SYSTEM_PLUGIN_H_
