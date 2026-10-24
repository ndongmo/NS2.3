/*!
* \file TileGrid.h
* \brief Tile grid data structure.
*	It represents a grid of tiles.
* \author Ndongmo Silatsa Fabrice
* \date 15-06-2018
* \version 2.0
*/

#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Tile;

class TileGrid {
public:
	TileGrid();
	~TileGrid();

    void init(int width, int height);

	/*!
	* \brief Adds a new tile to the grid at its specified position.
	* \param newTile: Pointer to the tile to be added. The tile's position should be set before calling this function.
    * \return Pointer to the added tile, or nullptr if the operation failed.
    */
	Tile* addTile(std::unique_ptr<Tile> newTile);

	/*!
	 * \brief Creates a copy of the given tile and adds it to the grid.
	 * \param tileTemplate: Pointer to the tile to be copied.
	 * \return Pointer to the newly created tile, or nullptr if the operation failed.
	 */
	Tile* copyTile(const Tile* tileTemplate);

	/*!
	 * \brief Deletes a tile and frees its associated memory.
	 * \param tile: Pointer to the tile to be deleted.
	 */
	void deleteTile(Tile* tile);

	/*!
	 * \brief Clears the entire grid, removing all tiles and freeing associated memory.
	 */
    void clear();

    /*!
	 * \brief Retrieves the tile at the given grid coordinates.
	 * \param x: The x-coordinate in the grid.
	 * \param y: The y-coordinate in the grid.
	 * \return Pointer to the tile at the specified coordinates. Throws an exception if out of bounds or empty.
     */
    Tile* getTile(int x, int y) const;

    /*!
	 * \brief Retrieves the tile at the given world position.
	 * \param worldPos: The world position to query.
	 * \return Pointer to the tile at the specified position, or nullptr if out of bounds or empty.
     */
    Tile* getTile(glm::vec2 worldPos) const;

    /*GETTERS*/
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getWorldWidth() const;
    float getWorldHeight() const;
	const std::vector<std::unique_ptr<Tile>>& getTiles() const { return m_tiles; }

private:
	/*!
	 * \brief Places a tile in the grid, updating the grid references and managing ownership.
	 * \param tile: Unique pointer to the tile to be placed.
	 * \return Pointer to the placed tile, or nullptr if placement failed (e.g., out of bounds or overlap).
	 */
	Tile* placeTile(std::unique_ptr<Tile> tile);

    int m_width = 0;
    int m_height = 0;

	std::vector<Tile*> m_grid; // Grid of tile pointers, nullptr indicates empty space
	std::vector<std::unique_ptr<Tile>> m_tiles; // Owning storage for tiles to manage their lifetimes
};

