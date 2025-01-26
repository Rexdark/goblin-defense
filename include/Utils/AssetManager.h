#pragma once

#include <string>
#include <unordered_map>

#include <Utils/Constants.h>

namespace sf
{
	class Texture;
}

class AssetManager
{
	public:

		static AssetManager* getInstance();

		~AssetManager();

		sf::Texture* getTexture(const std::string& assetPath);

	private:

		static AssetManager* s_instance;

		std::unordered_map<std::string, sf::Texture*> m_texturePathToTexture;

		sf::Texture* loadTexture(const std::string& assetPath);

		void unloadTexture(const std::string& assetPath);

		void clear();
};