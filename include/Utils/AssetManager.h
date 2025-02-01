#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
	class Texture;
}

class AssetManager
{
	public:

		struct Animation
		{
			sf::Texture* texture{ nullptr };
			int frameCount{ 0 };
			float frameTime{ 0.f };
			int frameWidth{ 0 };
			int frameHeight{ 0 };
		};

		static AssetManager* getInstance();

		~AssetManager();

		sf::Texture* getTexture(const std::string& assetPath);
		sf::Sprite* getSprite(const std::string& filePath);

	private:
		static AssetManager* s_instance;

		std::unordered_map<std::string, sf::Texture*> m_texturePathToTexture;
		std::unordered_map<std::string, sf::Sprite*> m_spritePathToTexture;

		sf::Texture* loadTexture(const std::string& assetPath);
		sf::Sprite* loadSprite(const std::string& assetPath);

		void unloadTexture(const std::string& assetPath);
		void unloadSprite(const std::string& filePath);

		void clear();
};