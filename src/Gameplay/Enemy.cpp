#include <queue>
#include <unordered_set>

#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Gameplay/Enemy.h>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor, MapLayer* mapLayer)
{
	m_sprite = enemyDescriptor.sprite;

	m_sprite->setPosition(enemyDescriptor.position);

	m_tileWidth = enemyDescriptor.Width;
	m_tileHeight = enemyDescriptor.Height;
	m_frameCount = enemyDescriptor.frameCount;
	m_frameTime = enemyDescriptor.frameTime;
    m_speed = enemyDescriptor.speed;

    m_mapLayer = mapLayer;

	return true;
}

sf::Vector2f Enemy::searchNextTileBasedOnPath(sf::Vector2f destinationTile)
{
    const std::vector<sf::Vector2f> directions = {
        { 0, -1 }, 
        { 0, 1 },  
        { -1, 0 }, 
        { 1, 0 }   
    };

    auto heuristic = [](const sf::Vector2f& a, const sf::Vector2f& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);  
        };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::unordered_map<std::string, Node*> openSet;
    std::unordered_map<std::string, bool> closedSet;

    Node* startNode = new Node();
    startNode->position = m_position;  
    startNode->gCost = 0;
    startNode->hCost = heuristic(m_position, destinationTile);
    openList.push(*startNode);
    openSet[std::to_string(startNode->position.x) + "," + std::to_string(startNode->position.y)] = startNode;

    while (!openList.empty())
    {
        Node* currentNode = new Node(openList.top());
        openList.pop();

        
        if (currentNode->position == destinationTile)
        {
            std::vector<sf::Vector2f> path;
            Node* node = currentNode;

            while (node != nullptr)
            {
                path.push_back(node->position);
                node = node->parent;
            }

            std::reverse(path.begin(), path.end());

            
            if (path.size() > 1)
                return path[1]; 
            else
                return path[0]; 
        }

        closedSet[std::to_string(currentNode->position.x) + "," + std::to_string(currentNode->position.y)] = true;

        for (const auto& direction : directions)
        {
            sf::Vector2f neighbor = currentNode->position + direction;

            if (m_mapLayer->getTile(neighbor.x, neighbor.y).ID != 0 || closedSet[std::to_string(neighbor.x) + "," + std::to_string(neighbor.y)])
                continue;

            float tentativeGCost = currentNode->gCost + 1;

            auto neighborNode = new Node();
            neighborNode->position = neighbor;
            neighborNode->gCost = tentativeGCost;
            neighborNode->hCost = heuristic(neighbor, destinationTile);
            neighborNode->parent = currentNode;

            if (openSet.find(std::to_string(neighborNode->position.x) + "," + std::to_string(neighborNode->position.y)) == openSet.end())
            {
                openList.push(*neighborNode);
                openSet[std::to_string(neighborNode->position.x) + "," + std::to_string(neighborNode->position.y)] = neighborNode;
            }
        }
    }

    return m_position;
}

void Enemy::update(float deltaMilliseconds)
{
	m_sprite->setPosition(m_position);
	
	m_elapsedTime += deltaMilliseconds;
	if (m_elapsedTime >= m_frameTime)
	{
		m_elapsedTime = 0.f;
		m_currentFrame = (m_currentFrame + 1) % m_frameCount;
	}
}

void Enemy::render(sf::RenderWindow& window)
{
	int frameX = m_currentFrame * static_cast<int>(m_tileWidth);
	m_sprite->setTextureRect(sf::IntRect(frameX, 0, static_cast<int>(m_tileWidth), static_cast<int>(m_tileHeight)));

	window.draw(*m_sprite);

	// Debug bounding box
	//const sf::FloatRect spriteBounds = m_sprite->getGlobalBounds();
	//sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	//boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	//boundsRect.setOutlineColor(sf::Color::Red);
	//boundsRect.setOutlineThickness(2.f);
	//boundsRect.setFillColor(sf::Color::Transparent);
	//window.draw(boundsRect);
}

void Enemy::moveEnemy(float deltaMilliseconds)
{
    m_direction = m_nextTileCoordinates - m_position;

    float length = std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
    if (length > 0)
    {
        m_direction /= length;
    }

    sf::Vector2f movement;
    movement.x = m_direction.x * m_speed.x * (deltaMilliseconds / 1000.0f);
    movement.y = m_direction.y * m_speed.y * (deltaMilliseconds / 1000.0f);

    if (std::abs(m_position.x - m_nextTileCoordinates.x) < 0.001f &&
        std::abs(m_position.y - m_nextTileCoordinates.y) < 0.001f)
    {
        m_position = m_nextTileCoordinates;
    }
    else
    {
        m_position += movement;
    }
}
