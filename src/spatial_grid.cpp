#include "spatial_grid.hpp"

Spatial_Grid::Spatial_Grid(float cellSize) {
    this->cellSize = cellSize;
}

void Spatial_Grid::clear() {
    grid.clear();
}

void Spatial_Grid::insert(const glm::vec3& pos, uint32_t particleIndex) {
    glm::ivec3 cell = glm::floor(pos / cellSize);
    grid[cell].push_back(particleIndex);
}

std::vector<glm::ivec3> Spatial_Grid::getNeighborCells(const glm::vec3& pos) const {
    glm::ivec3 base = glm::floor(pos / cellSize);
    std::vector<glm::ivec3> neighbors;

    for(int dx = -1; dx <= 1; dx++)
    for(int dy = -1; dy <= 1; dy++)
    for(int dz = -1; dz <= 1; dz++)
        neighbors.emplace_back(base + glm::ivec3(dx,dy,dz));

    return neighbors;
}

const std::vector<uint32_t>& Spatial_Grid::getCell(const glm::ivec3& coord) const {
    static const std::vector<uint32_t> empty;
    auto it = grid.find(coord);
    return (it != grid.end()) ? it->second : empty;
}

glm::ivec3 Spatial_Grid::getCellCoord(const glm::vec3& pos) const {
    return glm::floor(pos / cellSize);
}
