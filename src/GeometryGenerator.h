#pragma once
#include "math/Vec2.h"
#include "math/Vec3.h"

#include <vector>

class GeometryGenerator {
public:
    struct Vertex {
        Vertex() {}

        Vertex(fuse::Vec3& position, fuse::Vec3& normal, fuse::Vec3& tangentU, fuse::Vec2& uv)
            : Position(position)
            , Normal(normal)
            , TangentU(tangentU)
            , TexC(uv) {}

        Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty,
               float tz, float u, float v)
            : Position(x, y, z)
            , Normal(nx, ny, nz)
            , TangentU(tx, ty, tz)
            , TexC(u, v) {}

        fuse::Vec3 Position;
        fuse::Vec3 Normal;
        fuse::Vec3 TangentU;
        fuse::Vec2 TexC;
    };

    struct MeshData {
        std::vector<Vertex>       Vertices;
        std::vector<unsigned int> Indices;

        std::vector<unsigned short>& GetIndices16() {
            if (mIndices16.empty()) {
                mIndices16.resize(Indices.size());
                for (size_t i = 0; i < Indices.size(); ++i) {
                    mIndices16[i] = static_cast<unsigned short>(Indices[i]);
                }
            }

            return mIndices16;
        }

    private:
        std::vector<unsigned short> mIndices16;
    };

public:
    GeometryGenerator() {}
    ~GeometryGenerator() {}

    /// @brief Creates a box centered at the origin with the given dimensions.
    /// @param width,height,depth Dimension of the box.
    /// @param numSubdivisions The number of subdivision of each faces.
    MeshData createBox(float width, float height, float depth, unsigned numSubdivisions = 0);

    /// @brief creates a sphere centered at the origin with the given radius.
    /// The slices and stacks parameters control the degree of tessellation.
    /// @param radius
    /// @param sliceCount
    /// @param stackCount
    MeshData createSphere(float radius, unsigned int sliceCount, unsigned int stackCount);

    /// @brief Creates a geosphere centered at the origin with the given radius.
    /// The depth controls the level of tessellation.
    /// @param radius
    /// @param subdivisionCount
    MeshData createGeoSphere(float radius, unsigned int subdivisionCount);


    /// @brief Creates a cylinder parallel to the y-axis, and centered about the origin.
    ///
    /// The bottom and top radius can vary to form various cone shapes rather than true cylinders.
    /// The slices and stacks parameters control the degree of tessellation.
    /// @param bottomRadius The bottom radius world unit.
    /// @param topRadius The top radius world unit.
    /// @param height The height of the cylinder in world unit.
    /// @param sliceCount
    /// @param stackCount The number of division along the height.
    MeshData createCylinder(float bottomRadius, float topRadius, float height,
                            unsigned int sliceCount, unsigned int stackCount);

    /// @brief Creates an grid in the xz-plane, centered at the origin with the
    ///        specified width and depth.
    /// @param width The width of the grid in world unit.
    /// @param depth The depth of the grid in world unit.
    /// @param nbVertexWidth The number of vertices on the x-axis.
    /// @param nbVertexDepth The number of vertices on the z-axis.
    /// @pre nbVertexWidth and nbVertexDepth yst be greater or equals to 2.
    MeshData createGrid(float width, float depth, unsigned int nbVertexWidth,
                        unsigned int nbVertexDepth);

    /// @brief Creates a quad covering the screen in NDC coordinates.
    /// This is useful for postprocessing effects.
    MeshData createFullscreenQuad();

    /// @brief Creates a quad aligned with the screen.
    /// This is useful for postprocessing and screen effects.
    MeshData CreateQuad(float x, float y, float w, float h, float depth);

private:
    void   subdivide(MeshData& meshData);
    Vertex midPoint(const Vertex& v0, const Vertex& v1);
    void   buildCylinderTopCap(float topRadius, float height, unsigned int sliceCount,
                               MeshData& meshData);
    void   buildCylinderBottomCap(float bottomRadius, float height, unsigned int sliceCount,
                                  MeshData& meshData);
};
