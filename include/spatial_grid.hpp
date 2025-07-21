#ifndef SPATIAL_GRID_HPP
#define SPATIAL_GRID_HPP

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

//Hash function for glm::ivec3 to use in unordered_map
struct IVec3Hash {
    size_t operator()(const glm::ivec3& v) const {
        size_t hx = std::hash<int>()(v.x);
        size_t hy = std::hash<int>()(v.y);
        size_t hz = std::hash<int>()(v.z);
        return hx ^ (hy << 1) * (hz << 2);
    }
};

class Spatial_Grid {
    private:
        float cellSize;
        std::unordered_map<glm::ivec3, std::vector<uint32_t>, IVec3Hash> grid;
    public:
        Spatial_Grid(float cellSize);

        void clear();
        void insert(const glm::vec3& pos, uint32_t particleIndex);

        std::vector<glm::ivec3> getNeighborCells(const glm::vec3& pos) const;
        const std::vector<uint32_t>& getCell(const glm::ivec3& coord) const;
        glm::ivec3 getCellCoord(const glm::vec3& pos) const;
};

#endif