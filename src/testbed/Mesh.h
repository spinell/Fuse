#pragma once
#include "Buffer.h"
#include "GeometryGenerator.h"


/// @brief
class Mesh {
public:
    Mesh() = default;

    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&)                 = default;
    Mesh& operator=(Mesh&&)      = default;

    void render() const;

    static Mesh CreateBox();
    static Mesh CreateGrid();
    static Mesh CreateGeoSphere();
    static Mesh CreateSphere();
    static Mesh CreateCylinder();

private:
    void upload(const GeometryGenerator::MeshData& data, Mesh& mesh);

    Buffer  mVertexBuffer;
    Buffer  mIndexBuffer;
    GLsizei mNbIndices;
};
