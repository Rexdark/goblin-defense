#include <queue>
#include <unordered_map>

#include <Core/Level.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Gameplay/Enemy.h>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor, std::vector<std::vector<uint32_t>> pathMapVector, Level* level)
{
    id = enemyDescriptor.id;

	m_sprite = enemyDescriptor.sprite;

	m_sprite->setPosition(enemyDescriptor.position);

	m_tileWidth = enemyDescriptor.Width;
	m_tileHeight = enemyDescriptor.Height;
	m_frameCount = enemyDescriptor.frameCount;
	m_frameTime = enemyDescriptor.frameTime;
    m_speed = enemyDescriptor.speed;

    m_pathMapVector = pathMapVector;

    m_level = level;

	return true;
}

sf::Vector2i Enemy::searchNextTileCoordinates() {

    sf::Vector2i currentTile = getCurrentTile();
    sf::Vector2i destinationTile = getDestinationTile();

    //printf("Current tile is [%d][%d]\n", currentTile.x, currentTile.y);
    //printf("Destination tile is [%d][%d]\n", destinationTile.x, destinationTile.y);

    const std::vector<sf::Vector2i> directions = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    auto heuristic = [](const sf::Vector2i& a, const sf::Vector2i& b) {
        return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
        };

    sf::Vector2i start = { currentTile.y, currentTile.x };
    sf::Vector2i goal = { destinationTile.y, destinationTile.x };

    auto compare = [](const Node* a, const Node* b) { return a->fCost() > b->fCost(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openList(compare);

    std::unordered_map<sf::Vector2i, Node*, Vector2iHash> openSet;
    std::unordered_map<sf::Vector2i, bool, Vector2iHash> closedSet;

    Node* startNode = new Node{ start, 0.0f, heuristic(start, goal), nullptr };
    openList.push(startNode);
    openSet[start] = startNode;

    while (!openList.empty()) {
        Node* currentNode = openList.top();
        openList.pop();
        openSet.erase(currentNode->position);

        if (currentNode->position == goal) {

            std::vector<sf::Vector2i> path;
            for (Node* node = currentNode; node != nullptr; node = node->parent) {
                path.emplace_back(static_cast<float>(node->position.x), static_cast<float>(node->position.y));
            }

            for (auto& [key, node] : openSet) delete node;

            std::reverse(path.begin(), path.end());
            return path.size() > 1 ? path[1] : path[0];
        }

        closedSet[currentNode->position] = true;

        for (const auto& dir : directions) {
            sf::Vector2i neighbor = currentNode->position + dir;

            if (neighbor.y < 0 || neighbor.x < 0 || neighbor.y >= m_pathMapVector.size() || neighbor.x >= m_pathMapVector[0].size()) {
                continue;
            }

            if (m_impassableTiles.count(m_pathMapVector[neighbor.y][neighbor.x])
                || closedSet.count(neighbor)) {
                continue;
            }

            float tentativeGCost = currentNode->gCost + 1;

            if (!openSet.count(neighbor) || tentativeGCost < openSet[neighbor]->gCost) {

                Node* neighborNode = new Node{ neighbor, tentativeGCost, heuristic(neighbor, goal), currentNode };
                openList.push(neighborNode);
                openSet[neighbor] = neighborNode;
            }
        }
    }

    for (auto& [key, node] : openSet) delete node;
    return currentTile;
}

sf::Vector2i Enemy::getCurrentTile()
{
    sf::Vector2i currentTile = m_level->getTilebyCoordinates(m_position.x, m_position.y);

    return currentTile;
}

sf::Vector2i Enemy::getDestinationTile()
{

    sf::Vector2i currentTile = m_level->getTilebyCoordinates(m_destination.x, m_destination.y);

    return currentTile;
}

void Enemy::setDestination(sf::Vector2f destination)
{
    m_destination = destination;
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

    if (getCurrentTile() == getDestinationTile())
    {
        atDestination = true;
    }
}

void Enemy::render(sf::RenderWindow& window)
{
	int frameX = m_currentFrame * static_cast<int>(m_tileWidth);
	m_sprite->setTextureRect(sf::IntRect(frameX, 0, static_cast<int>(m_tileWidth), static_cast<int>(m_tileHeight)));

	window.draw(*m_sprite);
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

    setPosition(m_position);
}

bool Enemy::getCompleteStatus()
{
    return atDestination;
}
