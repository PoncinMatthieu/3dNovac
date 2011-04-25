
/*-----------------------------------------------------------------------------

	3dNovac Audio
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

    File Created At:        20/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NOVAC_ENGINE_AUDIO_H_
#define NOVAC_ENGINE_AUDIO_H_

#include <SFML/Audio.hpp>
#include <Nc/Core/Engine/IEngine.h>
#include <Nc/Core/Utils/FileName.h>
#include "Define.h"

namespace Nc
{
    /// Provide an AudioEngine to play sounds and musics
    namespace Audio
    {
        /// An Audio Engine to load/play sounds and musics
        /**
            Actually the Audio engine could load and play only one music at one time.
            But there is not limit for sounds. And the Audio engine use the sfml-audio librairy to play audio.  <br\>
            Nb: A Sounds is not encoded and a Music is
        */
        class LAUDIO Engine : public Nc::Engine::IEngine
        {
            public:
                /// The event command for the Audio::Engine
                enum CmdIndex
                {
                    LoadDescFile = 0,   ///< Load a .desc xml configuration file witch contain all the sounds to load
                    LoadMusic,          ///< Load a music file on memory
                    PlayMusic,          ///< Play the music loaded
                    StopMusic,          ///< Stop the music loaded
                    LoadSound,          ///< Load a sound file on memory
                    PlaySound           ///< Play the sound number
                };

            private:
                /// To link a sound and sound buffer with an index
                struct              SoundIndex
                {
                    unsigned int    index;
                    sf::SoundBuffer buffer;
                    sf::Sound       sound;
                };

            public:
                Engine(Nc::Engine::Manager *manager);
                virtual ~Engine();

                virtual inline void Execute(float runningTime) {}

            protected:
                virtual inline void ActiveContext() {};
                virtual inline void DisableContext() {};

                void LdDescFile(const Utils::FileName &file);
                void LdMusic(const Utils::FileName &file);
                void LdSound(const Utils::FileName &file);
                void PlSound(unsigned int no);

                void LoadDescFileCmd(Nc::Engine::IEvent *e);
                void LoadMusicCmd(Nc::Engine::IEvent *e);
                void PlMusic(Nc::Engine::IEvent *);
                void StpMusic(Nc::Engine::IEvent *);
                void LoadSoundCmd(Nc::Engine::IEvent *e);
                void PlaySoundCmd(Nc::Engine::IEvent *e);


                sf::Music                       _music;
                std::list<SoundIndex>           _listSounds;        ///< The list of sounds loaded
                bool                            _musicOpened;
        };
    }
}

#endif // AUDIOENGINE_H_INCLUDED
