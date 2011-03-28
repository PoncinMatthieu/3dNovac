
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/


#ifndef NC_CORE_UTILS_XML_PARSER_H_
#define NC_CORE_UTILS_XML_PARSER_H_

#include <iostream>
#include <fstream>
#include <list>

#include "../../Define.h"
#include "XmlObject.h"

#define SIZE_BUFFER 4096

namespace Nc
{
    namespace Utils
    {
        namespace Xml
        {
            /// Provide a parsing a the format Xml using a stream std::istream
            class LCORE Parser
            {
                private:
                    enum TOKEN_TYPE {CDATA, DATA, TAG, END_TAG, COMMENT};

                    struct Token
                    {
                        TOKEN_TYPE      type;
                        Object::TYPE    objectType;
                        std::string     data;
                    };

                public:
                    /**
                        Use this function to parse to parse an xml stream <br/>
                        Return a CONTENT Object witch contain all others object
                    */
                    static Object *Parse(std::istream &in);

                private:
                    Parser(std::istream &in);

                    void ReadAll();
                    void Read();

                    bool GenereToken(Token &newToken);
                    // avis aux amateur, ne pas regarder le code de ces 2 fonctions, ca pique les yeaux :), elles fonctionnent tres bien et c'est tout ce qu'on lui demande
                    bool SearchTokenInBuffer(Token &newToken, unsigned int &pos, const std::streamsize &size, bool lastEndData, char lastCaract); // search and fill a token, in the stream
                    unsigned int ChooseTypeToken(Token &newToken, unsigned int &pos, const std::streamsize &size, bool lastEndData, char lastCaract);

                    void CreateObject(Token &token, Object *&newObject);
                    void GetParamsAndName(Token &token, std::string &name, std::map<std::string, std::string> &param);

                    std::istream    &_in;
                    Object          *_lastObject;
                    Object          *_content;
                    char            _buffer[SIZE_BUFFER];
            };
        }
    }
}

#endif
