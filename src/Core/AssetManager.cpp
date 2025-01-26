#include <Core/AssetManager.h>

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
	for (auto it : m_texturePathToTexture)
	{
		delete it.second;
	}
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

	const bool loadOk = newTexture->loadFromFile(assetPath);

	if (!loadOk)
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

void AssetManager::clear()
{
	for (auto& it : m_texturePathToTexture)
	{
		delete it.second;
	}
	m_texturePathToTexture.clear();
}