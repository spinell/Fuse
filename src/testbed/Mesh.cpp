#include "Mesh.h"

namespace AttributeIndex {

constexpr GLint kPosition = 0;
constexpr GLint kNormal   = 1;
constexpr GLint kTangent  = 2;
constexpr GLint kUV       = 3;

} // namespace AttributeIndex


void Mesh::render() const {
    glEnableVertexAttribArray(AttributeIndex::kPosition);
    glEnableVertexAttribArray(AttributeIndex::kNormal);
    glEnableVertexAttribArray(AttributeIndex::kTangent);
    glEnableVertexAttribArray(AttributeIndex::kUV);

    glVertexAttribBinding(AttributeIndex::kPosition, 0);
    glVertexAttribBinding(AttributeIndex::kNormal, 0);
    glVertexAttribBinding(AttributeIndex::kTangent, 0);
    glVertexAttribBinding(AttributeIndex::kUV, 0);

    glVertexAttribFormat(AttributeIndex::kPosition,
                         3,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof(GeometryGenerator::Vertex, Position));
    glVertexAttribFormat(AttributeIndex::kNormal,
                         3,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof(GeometryGenerator::Vertex, Normal));
    glVertexAttribFormat(AttributeIndex::kTangent,
                         3,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof(GeometryGenerator::Vertex, TangentU));
    glVertexAttribFormat(AttributeIndex::kUV,
                         2,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof(GeometryGenerator::Vertex, TexC));

    glBindVertexBuffer(0, mVertexBuffer.getId(), 0, sizeof(GeometryGenerator::Vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer.getId());

    glDrawElements(GL_TRIANGLES, mNbIndices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::upload(const GeometryGenerator::MeshData& data, Mesh& mesh) {
    mesh.mVertexBuffer =
      Buffer((GLsizeiptr)(data.Vertices.size() * sizeof(GeometryGenerator::Vertex)),
             (void*)data.Vertices.data());
    mesh.mIndexBuffer =
      Buffer((GLsizeiptr)(data.Indices.size() * sizeof(unsigned)), (void*)data.Indices.data());
    mesh.mNbIndices = (GLsizei)data.Indices.size();
}


Mesh Mesh::CreateBox() {
    Mesh              mesh;
    GeometryGenerator geometryGenerator;
    const auto        data = geometryGenerator.createBox(1, 1, 1, 0);
    mesh.upload(data, mesh);
    return mesh;
}

Mesh Mesh::CreateGrid() {
    Mesh              mesh;
    GeometryGenerator geometryGenerator;
    const auto        data = geometryGenerator.createGrid(1, 1, 2, 2);
    mesh.upload(data, mesh);
    return mesh;
}
Mesh Mesh::CreateGeoSphere() {
    Mesh              mesh;
    GeometryGenerator geometryGenerator;
    const auto        data = geometryGenerator.createGeoSphere(1, 4);
    mesh.upload(data, mesh);
    return mesh;
}

Mesh Mesh::CreateSphere() {
    Mesh              mesh;
    GeometryGenerator geometryGenerator;
    const auto        data = geometryGenerator.createSphere(1, 25, 25);
    mesh.upload(data, mesh);
    return mesh;
}

Mesh Mesh::CreateCylinder() {
    Mesh              mesh;
    GeometryGenerator geometryGenerator;
    const auto        data = geometryGenerator.createCylinder(1, 1, 3, 10, 2);
    mesh.upload(data, mesh);
    return mesh;
}
