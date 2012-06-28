
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
#include "Object.h"

#define NC_UTILS_XML_SIZE_BUFFER 4096        ///< buffer size used to parse an xml file

namespace Nc
{
    namespace Utils
    {
        namespace Xml
        {
            /// Provide a parser for reading an Xml format by using an std::istream
            class LCORE Parser
            {
                private:
                    enum TOKEN_TYPE {CDATA, DATA, TAG, END_TAG, COMMENT};

                    /// token used to parse Xml
                    struct Token
                    {
                        TOKEN_TYPE      type;           ///< the type of token
                        Object::TYPE    objectType;     ///< the type of the xml object to create
                        std::string     data;           ///< the data string of the token
                    };

                public:
                    /**
						Use this function to parse an xml stream <br/>
						\return a CONTENT Object witch contain all others object
					*/
					static Object *Parse(std::istream &in, bool parseHeader = true);

				private:
					Parser(std::istream &in, bool parseHeader = true);

					/** read the header and next call the Read function */
					void ReadAll();
					/** read all the file, call the GenereToken and next the CreateObject method */
					void Read();

					/** Generate a token */
					bool GenereToken(Token &newToken);
					// avis aux amateur, ne pas regarder le code de ces 2 fonctions, ca pique les yeaux :), elles fonctionnent tres bien et c'est tout ce qu'on leurs demandent :D
					/** search and fill a token, in the stream */
					bool SearchTokenInBuffer(Token &newToken, unsigned int &pos, const std::streamsize &size, bool lastEndData, char lastCaract);
                    unsigned int ChooseTypeToken(Token &newToken, unsigned int &pos, const std::streamsize &size, bool lastEndData, char lastCaract);

                    /** Create an Xml object with the given token */
                    void CreateObject(Token &token, Object *&newObject);
                    /** Return the params of the data of the given token <"name param1=value1 param2=value2"> */
                    void GetParamsAndName(Token &token, std::string &name, std::map<std::string, std::string> &param);

					std::istream    &_in;                               ///< the input stream to parse
					bool			_parseHeader;						///< if false, do not check the header
                    Object          *_lastObject;                       ///< the last parsed object
                    Object          *_content;                          ///< the content that has been parsed
                    char            _buffer[NC_UTILS_XML_SIZE_BUFFER];  ///< the buffer used to read the input stream
            };
        }
    }
}

#endif
