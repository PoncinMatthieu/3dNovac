
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

#ifndef NC_CORE_SYSTEM_CONFIG_H_
#define NC_CORE_SYSTEM_CONFIG_H_

#include "../Define.h"
#include "../Utils/Logger.h"
#include "../Utils/Xml/File.h"
#include "../Utils/Singleton.h"
#include "../System/API/API.h"

#define CONFIG      Nc::System::Config::Instance()      /// allow you to access to the Config class

namespace Nc
{
    namespace System
    {
        /// To save and load configs like ressources path in an Xml format
        /**
            <div class="title"> Sample of 3dNovac config file: </div>
            \p Samples/bin/3dNovac.conf
            \include bin/3dNovac.conf
        */
        class LCORE Config : public Utils::Singleton<Config>
        {
            public:
                /** Load the configuration file */
                void Load(const Utils::FileName &f);

                /** Save the configuration file */
                void Save();

                /** \return the Content of the configuration in Xml structure */
                inline Utils::Xml::Object *operator -> ()
                {
                    if (_content == NULL)
                        throw Utils::Exception("System::Config", "Config file Not Loaded !");
                    return _content;
                }

                /**
                    Check the state of the "GlobalConfig/ErrorTreatAsWarning/state" option,
                    if true, then Write a warning,
                    otherwise, throw an exception
                */
                static inline void Error(const std::string &s1, const std::string &s2)
                {
                    bool state;
                    if (Utils::Convert::StringTo(CONFIG->Block("GlobalConfig")->Line("ErrorTreatAsWarning")->Param("state"), state) && state)
                        Warning(s1, s2);
                    else
                        throw Utils::Exception(s1, s2);
                }

                /** Can be used to write a warning with the logger */
                static inline void Warning(const std::string &s1, const std::string &s2)
                {
                    LOG_ERROR << "Warning: " << s1 << ": " << s2 << std::endl;
                }

            protected:
                Config();
                virtual ~Config();

            protected:
                Utils::Xml::File        _file;          ///< xml file used to read and write the configs
                Utils::Xml::Object      *_content;      ///< xml object witch store the configs

                friend class Utils::Singleton<Config>;
        };
    }

    // explicit instantiation
    template class Utils::Singleton<System::Config>;
}

#endif
