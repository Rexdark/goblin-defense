#include <Utils/AssetManager.h>
#include <Utils/Constants.h>

#include <SFML/Graphics/Texture.hpp>


AssetManager* AssetManager::s_instance{ nullptr };

AssetManager* AssetManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new AssetManager();
	}
	return s_instance;
}

AssetManager::~AssetManager()
{
	clear();
	delete s_instance;
}

sf::Texture* AssetManager::getTexture(const std::string& filePath)
{
	std::string assetPath = TEXURES_PATH + filePath;

	auto it = m_texturePathToTexture.find(assetPath);

	if (it != m_texturePathToTexture.end())
	{
		return it->second;
	}
	else
	{
		return loadTexture(assetPath);
	}
}


sf::Texture* AssetManager::loadTexture(const std::string& assetPath)
{
	sf::Texture* newTexture = new sf::Texture();

	if (!newTexture->loadFromFile(assetPath))
	{
		printf("There was an error loading the texture '%s'.\n", assetPath.c_str());
		delete newTexture;
		return nullptr;
	}

	m_texturePathToTexture[assetPath] = newTexture;

	return newTexture;
}

void AssetManager::unloadTexture(const std::string& filePath)
{
	std::string assetPath = TEXURES_PATH + filePath;

	auto it = m_texturePathToTexture.find(assetPath);
	if (it != m_texturePathToTexture.end())
	{
		delete it->second;
		m_texturePathToTexture.erase(it);
	}
	else
	{
		printf("Texture at path '%s' did not exist!\n", assetPath.c_str());
	}
}


sf::Sprite* AssetManager::getSprite(const std::string& filePath)
{
	std::string assetPath = TEXURES_PATH + filePath;

	auto is = m_spritePathToTexture.find(assetPath);

	if (is != m_spritePathToTexture.end())
	{
		return is->second;
	}
	else
	{
		return loadSprite(filePath); //We use only the file path to avoid duplicaning directory path on loadTexture
	}
}

sf::Sprite* AssetManager::loadSprite(const std::string& assetPath)
{
	sf::Sprite* newSprite = new sf::Sprite();
	sf::Texture* textureToLoad = getTexture(assetPath);

	newSprite->setTexture(*textureToLoad);

	m_spritePathToTexture[assetPath] = newSprite;

	return newSprite;
}

void AssetManager::unloadSprite(const std::string& filePath)
{
	std::string assetPath = TEXURES_PATH + filePath;

	auto is = m_spritePathToTexture.find(assetPath);

	if (is != m_spritePathToTexture.end())
	{
		delete is->second;
		m_spritePathToTexture.erase(is);
	}
	else
	{
		printf("Sprite with the texture at path '%s' did not exist!\n", assetPath.c_str());
	}
}

void AssetManager::clear()
{
	for (auto& it : m_texturePathToTexture)
	{
		delete it.second;
	}
	m_texturePathToTexture.clear();

	for (auto& it : m_spritePathToTexture)
	{
		delete it.second;
	}
	m_spritePathToTexture.clear();
}