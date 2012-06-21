
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


#include <vector>

#include <Nc/Core/Utils/Xml/File.h>
#include <Nc/Core/System/Config.h>
#include "Engine.h"

using namespace std;
using namespace Nc::Utils;
using namespace Nc::Audio;

Engine::Engine(Nc::Engine::Manager *manager)
    : Nc::Engine::IEngine(manager, 0, 1, 1, 1)
{
    _musicOpened = false;

    AddNewCmd(LoadDescFile, (Nc::Engine::CmdFunction)&Engine::LoadDescFileCmd);
    AddNewCmd(LoadMusic,    (Nc::Engine::CmdFunction)&Engine::LoadMusicCmd);
    AddNewCmd(PlayMusic,    (Nc::Engine::CmdFunction)&Engine::PlMusic);
    AddNewCmd(StopMusic,    (Nc::Engine::CmdFunction)&Engine::StpMusic);
    AddNewCmd(LoadSound,    (Nc::Engine::CmdFunction)&Engine::LoadSoundCmd);
    AddNewCmd(PlaySound,    (Nc::Engine::CmdFunction)&Engine::PlaySoundCmd);
}

Engine::~Engine()
{
}

void Engine::LdDescFile(const FileName &file)
{
    // load the desc file
    Xml::File   descFile(file);
    Xml::Object *content = descFile.Read();

    // Load Sounds
	bool playMusic;
	if (Convert::StringTo(CONFIG->Block("GameConf")->Line("AudioMusic")->Param("state"), playMusic) && playMusic)
        LdMusic(content->Line("Music")->Param("path"));

	bool playSound;
	if (Convert::StringTo(CONFIG->Block("GameConf")->Line("AudioSound")->Param("state"), playSound) && playSound)
	{
// \todo segv on visual 2010
        Xml::ListObject &sounds = content->Block("Sounds")->ListChild();
        for (Xml::ListObject::iterator it = sounds.begin(); it != sounds.end(); ++it)
            LdSound((*it)->Param("path"));
	}
    delete content;
}

void Engine::LdMusic(const FileName &file)
{
    _musicOpened = false;
    if (!_music.OpenFromFile(file))
        throw Utils::Exception("AudioEngine", "Can't open music `" + file + "`");
    _musicOpened = true;
}

void Engine::PlMusic(Nc::Engine::IEvent*)
{
    if (!_musicOpened)
        throw Utils::Exception("AudioEngine", "Can't play the music, it's not load !");
    _music.Play();
    _music.SetLoop(true);
}

void Engine::StpMusic(Nc::Engine::IEvent*)
{
    if (_musicOpened)
        _music.Stop();
}

void Engine::LdSound(const FileName &file)
{
    SoundIndex sound;

    if (!sound.buffer.LoadFromFile(file))
        throw Utils::Exception("AudioEngine", "Can't open sound `" + file + "`");
    sound.index = _listSounds.size();
    _listSounds.push_back(sound);
}

void Engine::PlSound(unsigned int no)
{
    for (list<SoundIndex>::iterator it = _listSounds.begin(); it != _listSounds.end(); it++)
        if (it->index == no)
        {
            it->sound.SetBuffer(it->buffer);
            it->sound.Play();
            return;
        }
    throw Utils::Exception("AudioEngine", "Bad Argument " + Utils::Convert::ToString(no));
}

void Engine::LoadDescFileCmd(Nc::Engine::IEvent *e)
{
    Nc::Engine::Event<Utils::FileName> *es = dynamic_cast<Nc::Engine::Event<Utils::FileName>*>(e);
    if (es == NULL)
        throw Utils::Exception("AudioEngine", "Function LoadDescFileCmd: bad argument. Event<Utils::FileName> is expecting");

    LdDescFile(es->Data);
}

void Engine::LoadMusicCmd(Nc::Engine::IEvent *e)
{
    const Utils::FileName &f = e->GetData<Utils::FileName>();
    LdMusic(f);
}

void Engine::LoadSoundCmd(Nc::Engine::IEvent *e)
{
	const Utils::FileName &f = e->GetData<Utils::FileName>();
    LdSound(f);
}

void Engine::PlaySoundCmd(Nc::Engine::IEvent *e)
{
	unsigned int id = e->GetData<unsigned int>();
    PlSound(id);
}

