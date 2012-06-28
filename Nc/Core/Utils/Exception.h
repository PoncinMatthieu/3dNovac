
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

#ifndef NC_CORE_UTILS_EXCEPTION_H_
#define NC_CORE_UTILS_EXCEPTION_H_

#include <exception>
#include <string>

#include "../Define.h"
#include "Convert.h"

namespace Nc
{
    namespace Utils
    {
        /// Propose an exception class base
        /**
            provide Exception class in which you can pass a message.
			
            In windows the std::exception implementation can use an std::string in the constructor but not in linux,
            so this class assure you to have the same implementation for each OS.
        */
        class LCORE Exception : public std::exception
        {
            public:
                Exception(const std::string &msg) throw()
                {_msg = msg;}

                /**
                    Format the msg like this :
                    <p> "`name`: `msg`" </p>
                */
                Exception(const std::string &name, const std::string &msg) throw()
                {_msg = name + ": " + msg;}

                /**
                    Format the msg like this :
                    <p> "`file:line` \t `name`: `msg`" </p>
                */
                Exception(const std::string &file, int line, const std::string &name, const std::string &msg) throw()
                {_msg = file + ":" + Utils::Convert::ToString(line) + "\t" + name + ": " + msg;}

                virtual ~Exception() throw()                                        {};


                /**
                    Redefinition of the function what to return the msg, after catching any std::exception
                */
                virtual const char *what() const throw();

            private:
                std::string	_msg;   ///< the message of the exception
        };
    }
}

#if defined(_DEBUG) && defined(_DEBUG_EXCEPTION)
    #define Exception(name, msg)     Exception(__FILE__, __LINE__, name, msg)
#endif

#endif
