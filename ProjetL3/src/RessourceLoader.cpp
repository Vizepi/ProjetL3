/*

	Platformer Game - Made for the 3rd year of undergraduated project.
    Copyright (C) 2015  Corbat Lisa, Kieffer Joseph

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "../header/RessourceLoader.h"


/*static*/ void RessourceLoader::LoadTextures(const char* dataFile)
{
	std::ifstream file(dataFile, std::ios::in); // = FILE* file = fopen(dataFile, "r");
	if(!file.good())
	{
		std::cerr << "Error while opening textures file \"" << dataFile << "\"." << std::endl;
		exit(1);
	}
	std::string name;
	std::string path;
	while(NextItem(file, name, path))
	{
		texture_set * ts = new texture_set;
		ts->name.assign(name);
		ts->texture = new sf::Texture();
		if(!(ts->texture->loadFromFile(path.c_str())))
		{
			std::cerr << "Unable to open texture file \"" << path << "\"" << std::endl;
		}
		s_textureArray.push_back(ts);
	}
	file.close();
}

/*static*/ sf::Texture* RessourceLoader::GetTexture(const std::string& name)
{
	int l = s_textureArray.size();
	for(int i=0;i<l;i++)
		if(name == s_textureArray[i]->name)
			return s_textureArray[i]->texture;
	return s_textureArray[0]->texture;
}

/*static*/ sf::Texture* RessourceLoader::GetTexture(unsigned int index)
{
	if(index >= 0 && index < s_textureArray.size())
	{
		return s_textureArray[index]->texture;
	}
	return s_textureArray[0]->texture;
}

/*static*/ void RessourceLoader::ClearTextures()
{
	while(s_textureArray.size() > 0)
	{
		delete(s_textureArray[0]->texture);
		delete(s_textureArray[0]);
		s_textureArray.erase(s_textureArray.begin());
	}
}

/*static*/ void RessourceLoader::LoadSounds(const char* dataFile)
{
	std::ifstream file(dataFile, std::ios::in);
	if(!file.good())
	{
		std::cerr << "Error while opening sounds file \"" << dataFile << "\"." << std::endl;
		exit(1);
	}
	std::string name;
	std::string path;
	while(NextItem(file, name, path))
	{
		sound_set * ss = new sound_set;
		ss->name.assign(name);
		ss->buffer = new(sf::SoundBuffer);
		if(!(ss->buffer->loadFromFile(path.c_str())))
		{
			std::cerr << "Unable to open sound file \"" << path << "\"" << std::endl;
		}
		ss->sound = new sf::Sound(*(ss->buffer));
		s_soundArray.push_back(ss);
	}
	file.close();
}

/*static*/ sf::Sound* RessourceLoader::GetSound(const std::string& name)
{
	int l = s_soundArray.size();
	for(int i=0;i<l;i++)
		if(name == s_soundArray[i]->name)
			return s_soundArray[i]->sound;
	return s_soundArray[0]->sound;
}

/*static*/ sf::Sound* RessourceLoader::GetSound(unsigned int index)
{
	if(index >= 0 && index < s_soundArray.size())
	{
		return s_soundArray[index]->sound;
	}
	return s_soundArray[0]->sound;
}

/*static*/ void RessourceLoader::ClearSounds()
{
	while(s_soundArray.size() > 0)
	{
		delete(s_soundArray[0]->buffer);
		delete(s_soundArray[0]->sound);
		delete(s_soundArray[0]);
		s_soundArray.erase(s_soundArray.begin());
	}
}

/*static*/ void RessourceLoader::LoadMusics(const char* dataFile)
{
	std::ifstream file(dataFile, std::ios::in);
	if(!file.good())
	{
		std::cerr << "Error while opening musics file \"" << dataFile << "\"." << std::endl;
		exit(1);
	}
	std::string name;
	std::string path;
	while(NextItem(file, name, path))
	{
		music_set * ms = new music_set;
		ms->name.assign(name);
		ms->music = new sf::Music();
		if(!(ms->music->openFromFile(path.c_str())))
		{
			std::cerr << "Unable to open music file \"" << path << "\"" << std::endl;
		}
		s_musicArray.push_back(ms);
	}
	file.close();
}

/*static*/ sf::Music* RessourceLoader::GetMusic(const std::string& name)
{
	int l = s_musicArray.size();
	for(int i=0;i<l;i++)
		if(name == s_musicArray[i]->name)
			return s_musicArray[i]->music;
	return s_musicArray[0]->music;
}

/*static*/ sf::Music* RessourceLoader::GetMusic(unsigned int index)
{
	if(index >= 0 && index < s_musicArray.size())
	{
		return s_musicArray[index]->music;
	}
	return s_musicArray[0]->music;
}

/*static*/ void RessourceLoader::ClearMusics()
{
	while(s_musicArray.size() > 0)
	{
		delete(s_musicArray[0]->music);
		delete(s_musicArray[0]);
		s_musicArray.erase(s_musicArray.begin());
	}
}

/*static*/ void RessourceLoader::LoadFonts(const char* dataFile)
{
	std::ifstream file(dataFile, std::ios::in);
	if(!file.good())
	{
		std::cerr << "Error while opening fonts file \"" << dataFile << "\"." << std::endl;
		exit(1);
	}
	std::string name;
	std::string path;
	while(NextItem(file, name, path))
	{
		font_set * fs = new font_set;
		fs->name.assign(name);
		fs->font = new sf::Font();
		if(!(fs->font->loadFromFile(path.c_str())))
		{
			std::cerr << "Unable to open font file \"" << path << "\"" << std::endl;
		}
		s_fontArray.push_back(fs);
	}
	file.close();
}

/*static*/ sf::Font* RessourceLoader::GetFont(const std::string& name)
{
	int l = s_fontArray.size();
	for(int i=0;i<l;i++)
		if(name == s_fontArray[i]->name)
			return s_fontArray[i]->font;
	return s_fontArray[0]->font;
}

/*static*/ sf::Font* RessourceLoader::GetFont(unsigned int index)
{
	if(index >= 0 && index < s_fontArray.size())
	{
		return s_fontArray[index]->font;
	}
	return s_fontArray[0]->font;
}

/*static*/ void RessourceLoader::ClearFonts()
{
	while(s_fontArray.size() > 0)
	{
		delete(s_fontArray[0]->font);
		delete(s_fontArray[0]);
		s_fontArray.erase(s_fontArray.begin());
	}
}

/*static*/ bool RessourceLoader::NextItem(std::ifstream& file, std::string& name, std::string& path)
{
	char buffer[BUFFER_SIZE];
	file.getline(buffer, BUFFER_SIZE, ':');
	if(file.eof())
		return false;
	if(file.fail())
	{
		std::cerr << "Buffer overflow while reading name of a ressource." << std::endl;
		exit(1);
	}
	name.assign(buffer);
	file.getline(buffer, BUFFER_SIZE, '\n');
	if(file.eof())
	{
		std::cerr << "No backspace at the end of the ressource file." << std::endl;
		exit(1);
	}
	if(file.fail())
	{
		std::cerr << "Buffer overflow while reading path of a ressource." << std::endl;
		exit(1);
	}
	path.assign(buffer);
	return true;
}

std::vector<texture_set*> RessourceLoader::s_textureArray;
std::vector<sound_set*> RessourceLoader::s_soundArray;
std::vector<music_set*> RessourceLoader::s_musicArray;
std::vector<font_set*> RessourceLoader::s_fontArray;

