#include "TileGrid.h"
#include "Tile.h"
#include <algorithm>

TileGrid::TileGrid() = default;

TileGrid::~TileGrid() = default;

void TileGrid::init(int width, int height)
{
    m_width = width;
    m_height = height;
    m_grid.assign(width * height, nullptr);
    m_tiles.clear();
}

Tile* TileGrid::addTile(std::unique_ptr<Tile> newTile) {
    return placeTile(std::move(newTile));
}

Tile* TileGrid::copyTile(const Tile* tileTemplate) {
    if (!tileTemplate) return nullptr;
    return placeTile(std::make_unique<Tile>(*tileTemplate));
}

Tile* TileGrid::placeTile(std::unique_ptr<Tile> tile) {
    if (!tile) return nullptr;

    int startX = tile->getPosX();
    int startY = tile->getPosY();
    int width = static_cast<int>(tile->getTexWidth());
    int height = static_cast<int>(tile->getTexHeight());

    // Bounds check
    if (startX < 0 || startY < 0 ||
        startX + width > m_width || startY + height > m_height) {
        return nullptr;
    }

    // Overlap validation
    for (int y = startY; y < startY + height; ++y) {
        for (int x = startX; x < startX + width; ++x) {
            if (m_grid[y * m_width + x] != nullptr) {
                return nullptr;
            }
        }
    }

    Tile* rawTilePtr = tile.get();

    // Save to master container
    m_tiles.push_back(std::move(tile));

    // Map spatial lookup elements
    for (int y = startY; y < startY + height; ++y) {
        for (int x = startX; x < startX + width; ++x) {
            m_grid[y * m_width + x] = rawTilePtr;
        }
    }

    return rawTilePtr;
}

void TileGrid::deleteTile(Tile* tile)
{
    if (!tile) return;

    int startX = tile->getPosX();
    int startY = tile->getPosY();
    int width = static_cast<int>(tile->getTexWidth());
    int height = static_cast<int>(tile->getTexHeight());

    for (int y = startY; y < startY + height; ++y) {
        for (int x = startX; x < startX + width; ++x) {
            if (m_grid[y * m_width + x] == tile) {
                m_grid[y * m_width + x] = nullptr;
            }
        }
    }

    m_tiles.erase(
        std::remove_if(m_tiles.begin(), m_tiles.end(),
            [tile](const std::unique_ptr<Tile>& ptr) { return ptr.get() == tile; }),
        m_tiles.end()
    );
}

void TileGrid::clear()
{
    m_tiles.clear();
    std::fill(m_grid.begin(), m_grid.end(), nullptr);
}

Tile* TileGrid::getTile(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return nullptr;
    }
    return m_grid[y * m_width + x];
}

Tile* TileGrid::getTile(glm::vec2 worldPos) const {
    return getTile(static_cast<int>(worldPos.x / TILESIZE), static_cast<int>(worldPos.y / TILESIZE));
}

float TileGrid::getWorldWidth() const { return m_width * TILESIZE; }
float TileGrid::getWorldHeight() const { return m_height * TILESIZE; }
