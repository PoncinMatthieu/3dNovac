
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

#ifndef NC_CORE_UTILS_LOG_H_
#define NC_CORE_UTILS_LOG_H_

#ifndef LOG
    #if defined(_DEBUG) && defined(_DEBUG_LOG)
        #define _DEBUG_LOG_DEF  __FILE__, __LINE__,
    #else
        #define _DEBUG_LOG_DEF
    #endif

    #define LOG             Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 0)
    #define LOG_ERROR       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 1)
    #ifdef _DEBUG
        #define LOG_DEBUG       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 2)
    #else
        #define LOG_DEBUG       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF -1)
    #endif
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Singleton.h"
#include "FileName.h"

namespace Nc
{
    namespace Utils
    {
      /// Provide a logging system
      /**
        Logger base, redefine the methode write in heritance to have your own logging system
      */
      class LCORE Logger : public Singleton<Logger>, NonCopyable
      {
      public:
        /**
            Prototype a function pointer to have a logging function
        */
        typedef void (*LogFunction)(const std::string, bool);

        /** Return the instance of the logger, and permit to call operator << to log
            The Logger is thread safe.
            By default, status correspond to <br/>
                0 -> simple log    <br/>
                1 -> error log     <br/>
                2 -> debug log

            <p>exemple:</p>
                Logger::Log(0) << "plop this is a log" << std::endl;
            <p>or use a define : </p>
                #define Logger::Log()   LOG     <br/>
                LOG << "plop this is a log" << std::endl;
        */
        static Logger	&Log(int status);
        static Logger	&Log(const char* aFile, int aLine, int status);

        /**
            Change the logging filename, the file is open in the write method, to force the reopen, call CloseFile()
        */
        inline void SetLogFilename(const FileName &f)   {_filename = f;}

        /**
            Set an additionnel Logging function (function pointer)
            show the typedef LogFunction to see the prototype
        */
        inline void SetLoggingFunction(LogFunction f)   {_loggingfunction = f;}

        /** Close the file, with this you can change the conf file and relog, the function log automatically open the file */
        inline void CloseFile()                         {_file.close();}

        // surcharge de l'operateur << permettant d'appeler la fonction
        // Write sur n'importe quel objet ayant defini la surcharge <<
        template<class T>
        Logger &operator << (const T &ToLog);
    #ifdef SYSTEM_WINDOWS
        Logger &operator << (std::ostream &(APIENTRY *f)(std::ostream &)); // permet de gerer le std::endl
        Logger &operator << (std::ostream &(__cdecl *f)(std::ostream &)); // permet de gerer le std::endl
    #else
        Logger &operator << (std::ostream &(*f)(std::ostream &)); // permet de gerer le std::endl
    #endif

      protected:
        Logger();
        virtual ~Logger();

        /**
            To redefine in your own logger class <br/>
            By default, <br/>
            Write the msg, and flush the buffer if `flush` == true <br/>
            Call the CheckFile method to open the file, if it's not open
        */
        virtual void Write(const std::string msg, bool flush);
        virtual void CheckFile();					            /** open the file, if it isn't already yet */

        LogFunction     _loggingfunction;
        std::ofstream	_file;
        FileName		_filename;
        System::Mutex   _mutex;
        static int		_status;		// status d'ecriture du logger (default = 0; error = 1; debug = 2)

        friend class Singleton<Logger>; // pour avoir acces a l'instance du singleton
    };

        template<class T>
        Logger &Logger::operator << (const T& ToLog)
        {
            std::ostringstream s;

            s << ToLog;
            _mutex.Lock();
            Write(s.str(), false);
            _mutex.Unlock();
            return *this;
        }

        // explicit instanciation
        //static template class Singleton<Logger>;
    }
}

#endif // LOG_H_INCLUDED
