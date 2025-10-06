#pragma once

namespace mesh_processing {
class IOManager {
 public:
  IOManager() = default;
  ~IOManager() = default;

 public:
  void LoadPolygonSoupOBJ(
      const std::string& filename,
      std::vector<std::array<double, 3>>& vertexPositionsOut,
      std::vector<std::vector<size_t>>& faceIndicesOut);


  // Load a mesh from a general file, detecting type from filename
  void LoadPolygonSoup(std::string filename,
                       std::vector<std::array<double, 3>>& vertexPositionsOut,
                       std::vector<std::vector<size_t>>& faceIndicesOut);
};
}  // namespace mesh_processing