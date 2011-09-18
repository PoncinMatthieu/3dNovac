
/*--------------------------------------------------------------------------------

	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        06/12/2010

--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

        Define all common define for Nc-Libs

--------------------------------------------------------------------------------*/

#ifndef NC_COMMON_DEFINE_H_
#define NC_COMMON_DEFINE_H_

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

#endif
