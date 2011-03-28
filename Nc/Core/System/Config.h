
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

    File Created At:        23/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

    Config class, too save and load configs like ressources path
    The Config Class is a Singleton

--------------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_CONFIG_H_
#define NC_CORE_SYSTEM_CONFIG_H_

#include "../Define.h"
#include "../Utils/Xml/File.h"
#include "../Utils/NonCopyable.h"
#include "../System/API/API.h"

#define CONFIG      Nc::System::Config::Instance()      /// allow you to access to the Config class

namespace Nc
{
    namespace System
    {
        class LCORE Config : Utils::NonCopyable
        {
            public:
                static Config &Instance();
                static void DeleteInstance();

                void Load(const Utils::FileName &f);
                void Save();

                inline Utils::Xml::Object *operator -> ()
                {
                    if (_content == NULL)
                        throw Utils::Exception("System::Config", "Config file Not Loaded !");
                    return _content;
                }

                // config error:
                static inline void Error(const std::string &s1, const std::string &s2)
                {
                    bool state;
                    if (Utils::Convert::StringTo(CONFIG->Block("GlobalConfig")->Line("ErrorTreatAsWarning")->Param("state"), state) && state)
                        LOG_ERROR << "Warning: " << s1 << ": " << s2 << std::endl;
                    else
                        throw Utils::Exception(s1, s2);
                }

            private:
                Config()    {_content = NULL;}
                ~Config();

                Utils::Xml::File        _file;
                Utils::Xml::Object      *_content;
                Mutex                   _mutex;
                static Config           *_instance;
        };
    }
}

#endif
