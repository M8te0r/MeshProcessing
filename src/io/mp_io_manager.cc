#include "mp_io_manager.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "utils/mp_utils.h"
#include "mp_vtk_io.h"

namespace mesh_processing
{
  // Helpers for the OBJ reader
  // TODO factor this out in to something neat like happly
  namespace
  {

    class Index
    {
    public:
      Index() {}

      Index(int v, int vt, int vn) : position(v), uv(vt), normal(vn) {}

      bool operator<(const Index &i) const
      {
        if (position < i.position)
          return true;
        if (position > i.position)
          return false;
        if (uv < i.uv)
          return true;
        if (uv > i.uv)
          return false;
        if (normal < i.normal)
          return true;
        if (normal > i.normal)
          return false;

        return false;
      }

      int position;
      int uv;
      int normal;
    };

    Index parseFaceIndex(const std::string &token)
    {
      std::stringstream in(token);
      std::string indexString;
      int indices[3] = {1, 1, 1};

      int i = 0;
      while (std::getline(in, indexString, '/'))
      {
        if (indexString != "\\")
        {
          std::stringstream ss(indexString);
          ss >> indices[i++];
        }
      }

      // decrement since indices in OBJ files are 1-based
      return Index(indices[0] - 1, indices[1] - 1, indices[2] - 1);
    }
  } // namespace

  void IOManager::LoadPolygonSoupOBJ(
      const std::string &filename,
      std::vector<std::array<double, 3>> &vertexPositionsOut,
      std::vector<std::vector<size_t>> &faceIndicesOut)
  {
    faceIndicesOut.clear();
    vertexPositionsOut.clear();

    // Open the file
    std::ifstream in(filename);
    if (!in)
      throw std::invalid_argument("Could not open mesh file " + filename);

    // parse obj format
    std::string line;
    while (getline(in, line))
    {
      std::stringstream ss(line);
      std::string token;

      ss >> token;

      if (token == "v")
      {
        double x, y, z;
        ss >> x >> y >> z;

        vertexPositionsOut.push_back({{x, y, z}});
      }
      else if (token == "vt")
      {
        // Do nothing
      }
      else if (token == "vn")
      {
        // Do nothing
      }
      else if (token == "f")
      {
        std::vector<size_t> face;
        while (ss >> token)
        {
          Index index = parseFaceIndex(token);
          if (index.position < 0)
          {
            getline(in, line);
            size_t i = line.find_first_not_of("\t\n\v\f\r ");
            index = parseFaceIndex(line.substr(i));
          }

          face.push_back(index.position);
        }

        faceIndicesOut.push_back(face);
      }
    }
  }

  void IOManager::LoadPolygonSoup(
      std::string filename,
      std::vector<std::array<double, 3>> &vertexPositionsOut,
      std::vector<std::vector<size_t>> &faceIndicesOut)
  {
    // Check if file exists
    std::ifstream testStream(filename);
    if (!testStream)
    {
      throw std::runtime_error(
          "Could not load polygon soup; file does not exist: " + filename);
      return;
    }
    testStream.close();

    // Attempt to detect filename
    std::string::size_type sepInd = filename.rfind('.');
    std::string extension = "";
    if (sepInd != std::string::npos)
    {
      extension = filename.substr(sepInd + 1);

      // Convert to all lowercase
      std::transform(extension.begin(), extension.end(), extension.begin(),
                     ::tolower);
    }

    if (extension == "obj")
    {
      LoadPolygonSoupOBJ(filename, vertexPositionsOut, faceIndicesOut);
    }
    else
    {
      throw std::runtime_error("Could not detect file type to load mesh from " +
                               filename);
    }
  }

  void IOManager::ReadMESH(const std::string &filename,
                           std::vector<std::vector<double>> &verts,
                           std::vector<std::vector<unsigned int>> &polys,
                           std::vector<int> &vert_labels,
                           std::vector<int> &poly_labels)
  {
    verts.clear();
    polys.clear();
    vert_labels.clear();
    poly_labels.clear();

    setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator

    FILE *f = fopen(filename.c_str(), "r");
    if (!f)
    {
      std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : load_MESH() : couldn't open input file " << filename << std::endl;
      exit(-1);
    }
    std::unordered_set<int> v_unique_labels;
    std::unordered_set<int> p_unique_labels;

    // read header
    int ver, dim, nv, nc;
    if (!seek_keyword(f, "MeshVersionFormatted"))
      assert(false && "could not find keyword MESHVERSIONFORMATTED");
    eat_int(f, ver);
    if (!seek_keyword(f, "Dimension"))
      assert(false && "could not find keyword DIMENSION");
    eat_int(f, dim);

    // read verts
    if (!seek_keyword(f, "Vertices"))
      assert(false && "could not find keyword VERTICES");
    eat_int(f, nv);
    for (int i = 0; i < nv; ++i)
    {
      double x = 0, y = 0, z = 0;
      int l;
      if (!eat_double(f, x) ||
          !eat_double(f, y) ||
          !eat_double(f, z) ||
          !eat_int(f, l))
        assert(false && "failed reading vert");
      verts.push_back(std::vector<double>{x, y, z});
      vert_labels.push_back(l);
      v_unique_labels.insert(l);
    }

    // read cells
    char cell_type[50];
    while (eat_word(f, cell_type))
    {
      if (strcmp(cell_type, "End") == 0)
      {
        fclose(f);
        if (v_unique_labels.size() < 2)
          vert_labels.clear();
        if (p_unique_labels.size() < 2)
          poly_labels.clear();
        return;
      }
      else if (strcmp(cell_type, "#") == 0)
      {
        // comment, ignore whole line up to next \n
        char line[1024];
        fgets(line, 1024, f);
      }
      else if (strcmp(cell_type, "Tetrahedra") == 0)
      {
        if (!eat_int(f, nc))
          assert(false && "failed reading num tets");
        for (int i = 0; i < nc; ++i)
        {
          int l;
          std::vector<unsigned int> tet(4);
          if (!eat_uint(f, tet[0]) ||
              !eat_uint(f, tet[1]) ||
              !eat_uint(f, tet[2]) ||
              !eat_uint(f, tet[3]) ||
              !eat_int(f, l))
            assert(false && "failed reading tet");

          for (unsigned int &vid : tet)
            vid -= 1;
          polys.push_back(tet);
          poly_labels.push_back(l);
          p_unique_labels.insert(l);
        }
      }
      else if (strcmp(cell_type, "Hexahedra") == 0)
      {
        if (!eat_int(f, nc))
          assert(false && "failed reading num hexa");
        for (int i = 0; i < nc; ++i)
        {
          int l;
          std::vector<unsigned int> hex(8);
          if (!eat_uint(f, hex[0]) ||
              !eat_uint(f, hex[1]) ||
              !eat_uint(f, hex[2]) ||
              !eat_uint(f, hex[3]) ||
              !eat_uint(f, hex[4]) ||
              !eat_uint(f, hex[5]) ||
              !eat_uint(f, hex[6]) ||
              !eat_uint(f, hex[7]) ||
              !eat_int(f, l))
            assert(false && "failed reading hexa");

          for (unsigned int &vid : hex)
            vid -= 1;
          polys.push_back(hex);
          poly_labels.push_back(l);
          p_unique_labels.insert(l);
        }
      }
      else if (strcmp(cell_type, "Triangles") == 0)
      {
        if (!eat_int(f, nc))
          assert(false && "failed reading num tris");
        for (int i = 0; i < nc; ++i)
        {
          int l;
          std::vector<unsigned int> tri(4);
          if (!eat_uint(f, tri[0]) ||
              !eat_uint(f, tri[1]) ||
              !eat_uint(f, tri[2]) ||
              !eat_int(f, l))
            assert(false && "failed reading tri");
          // discard these elements
        }
      }
      else if (strcmp(cell_type, "Quadrilaterals") == 0)
      {
        if (!eat_int(f, nc))
          assert(false && "failed reading num quads");
        for (int i = 0; i < nc; ++i)
        {
          int l;
          std::vector<unsigned int> quad(4);
          if (!eat_uint(f, quad[0]) ||
              !eat_uint(f, quad[1]) ||
              !eat_uint(f, quad[2]) ||
              !eat_uint(f, quad[3]) ||
              !eat_int(f, l))
            assert(false && "failed reading quad");
          // discard these elements
        }
      }
      else if (strcmp(cell_type, "Edges") == 0)
      {
        if (!eat_int(f, nc))
          assert(false && "failed reading num edges");
        for (int i = 0; i < nc; ++i)
        {
          int l;
          std::vector<unsigned int> edge(4);
          if (!eat_uint(f, edge[0]) ||
              !eat_uint(f, edge[1]) ||
              !eat_int(f, l))
            assert(false && "failed reading edge");
          // discard these elements
        }
      }
      else if (strcmp(cell_type, "Corners") == 0)
      {
        if (!eat_int(f, nc))
          assert(false && "failed reading corners");
        for (int i = 0; i < nc; ++i)
        {
          std::vector<unsigned int> corner(4);
          if (!eat_uint(f, corner[0]))
            assert(false && "failed reading corner");
          // discard these elements
        }
      }
    }
  }


  void IOManager::ReadQHEX(const std::string &filename,
                           std::vector<std::vector<double>> &verts,
                           std::vector<std::vector<unsigned int>> &hexs)
  {
    verts.clear();
    hexs.clear();

    std::ifstream fin(filename);
    if (!fin.is_open())
    {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return;
    }

    std::string line;
    while (std::getline(fin, line))
    {
      // 跳过空行
      if (line.empty())
        continue;

      std::istringstream iss(line);
      std::string keyword;
      iss >> keyword;

      if (keyword == "Vertex")
      {
        unsigned int vid;
        double x, y, z;
        iss >> vid >> x >> y >> z;

        // 确保verts大小足够
        if (verts.size() < vid)
          verts.resize(vid);
        verts[vid - 1] = {x, y, z};
      }
      else if (keyword == "Hex")
      {
        unsigned int hid;
        std::vector<unsigned int> h(8);
        iss >> hid;
        for (int i = 0; i < 8; ++i)
          iss >> h[i];

        hexs.push_back(h);
      }
      // 其他关键字不处理
    }

    fin.close();
  }


  bool IOManager::Ovm2Vtk(const std::string &ovm_file, const std::string &vtk_file)
  {
  }
} // namespace mesh_processing