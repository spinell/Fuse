#include "GeometryGenerator.h"

#include <cassert>
#include <numbers>


//------------------------------------------------------------------------
GeometryGenerator::MeshData GeometryGenerator::createBox(float width, float height, float pDepth,
                                                         unsigned numSubdivisions) {
    MeshData meshData;

    //
    // Create the vertices.
    //

    Vertex v[24];

    const float w2 = 0.5f * width;
    const float h2 = 0.5f * height;
    const float d2 = 0.5f * pDepth;

    // Fill in the front face vertex data.
    v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the back face vertex data.
    v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the top face vertex data.
    v[8]  = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[9]  = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the bottom face vertex data.
    v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the left face vertex data.
    v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

    // Fill in the right face vertex data.
    v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    meshData.Vertices.assign(&v[0], &v[24]);

    //
    // Create the indices.
    //

    unsigned int i[36];
    // clang-format off
    // Fill in the front face index data
    i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;
    // clang-format on
    meshData.Indices.assign(&i[0], &i[36]);

    // Put a cap on the number of subdivisions.
    numSubdivisions = std::min<unsigned>(numSubdivisions, 6u);
    for (unsigned j = 0; j < numSubdivisions; ++j) {
        subdivide(meshData);
    }

    return meshData;
}

//------------------------------------------------------------------------
GeometryGenerator::MeshData GeometryGenerator::createGrid(float gridWidth, float gridDepth,
                                                          unsigned int nbVertexWidth,
                                                          unsigned int nbVertexDepth) {
    assert(nbVertexWidth >= 2);
    assert(nbVertexDepth >= 2);

    MeshData meshData;

    const unsigned int vertexCount = nbVertexWidth * nbVertexDepth;
    const unsigned int faceCount   = (nbVertexWidth - 1) * (nbVertexDepth - 1) * 2;

    const float halfWidth = 0.5f * gridWidth;
    const float halfDepth = 0.5f * gridDepth;

    const float dx = gridWidth / float(nbVertexWidth - 1);
    const float dz = gridDepth / float(nbVertexDepth - 1);

    const float du = 1.0f / float(nbVertexWidth - 1);
    const float dv = 1.0f / float(nbVertexDepth - 1);

    //
    // Create Vertices
    //
    meshData.Vertices.resize(vertexCount);

    for (unsigned int i = 0; i < nbVertexDepth; i++) {
        float z = halfDepth - (float)i * dz;
        for (unsigned int j = 0; j < nbVertexWidth; j++) {
            float x = -halfWidth + (float)j * dx;

            unsigned int index                = i * nbVertexWidth + j;
            meshData.Vertices[index].Position = fuse::Vec3(x, 0, z);

            meshData.Vertices[index].Normal   = fuse::Vec3(0.f, 1.f, 0.f);
            meshData.Vertices[index].TangentU = fuse::Vec3(1.f, 0.f, 0.f);

            meshData.Vertices[index].TexC.x = (float)j * du;
            meshData.Vertices[index].TexC.y = (float)i * dv;
        }
    }

    //
    // Create Indices
    //
    meshData.Indices.resize(faceCount * 3);
    unsigned int k = 0;
    for (unsigned int i = 0; i < nbVertexDepth - 1; i++) {
        for (unsigned int j = 0; j < nbVertexWidth - 1; j++) {
            meshData.Indices[k]     = i * nbVertexWidth + j;
            meshData.Indices[k + 1] = i * nbVertexWidth + j + 1;
            meshData.Indices[k + 2] = (i + 1) * nbVertexWidth + j;
            meshData.Indices[k + 3] = (i + 1) * nbVertexWidth + j;
            meshData.Indices[k + 4] = i * nbVertexWidth + j + 1;
            meshData.Indices[k + 5] = (i + 1) * nbVertexWidth + j + 1;

            k += 6;
        }
    }

    return meshData;
}

//------------------------------------------------------------------------
GeometryGenerator::MeshData GeometryGenerator::createGeoSphere(float        radius,
                                                               unsigned int subdivisionCount) {
    MeshData meshData;

    // Put a cap on the number of subdivisions.
    subdivisionCount = std::min(subdivisionCount, 6u);

    // Approximate a sphere by tessellating an icosahedron.
    const float X = 0.525731f;
    const float Z = 0.850651f;

    const fuse::Vec3 pos[12] = {
      // clang-format off
      fuse::Vec3(-X, 0.0f, Z), fuse::Vec3(X, 0.0f, Z),
      fuse::Vec3(-X, 0.0f, -Z),fuse::Vec3(X, 0.0f, -Z),
      fuse::Vec3(0.0f, Z, X),  fuse::Vec3(0.0f, Z, -X),
      fuse::Vec3(0.0f, -Z, X), fuse::Vec3(0.0f, -Z, -X),
      fuse::Vec3(Z, X, 0.0f),  fuse::Vec3(-Z, X, 0.0f),
      fuse::Vec3(Z, -X, 0.0f), fuse::Vec3(-Z, -X, 0.0f)
      // clang-format on
    };

    const unsigned long k[60] = {1,  4,  0, 4,  9, 0, 4, 5,  9, 8, 5, 4,  1,  8, 4,
                                 1,  10, 8, 10, 3, 8, 8, 3,  5, 3, 2, 5,  3,  7, 2,
                                 3,  10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6,  1, 0,
                                 10, 1,  6, 11, 0, 9, 2, 11, 9, 5, 2, 9,  11, 2, 7};

    meshData.Vertices.resize(12);
    meshData.Indices.assign(&k[0], &k[60]);

    for (unsigned int i = 0; i < 12; ++i) {
        meshData.Vertices[i].Position = pos[i];
    }

    for (unsigned int i = 0; i < subdivisionCount; ++i) {
        subdivide(meshData);
    }

    // Project vertices onto sphere and scale.
    for (unsigned int i = 0; i < meshData.Vertices.size(); ++i) {
        // Project onto unit sphere.
        const fuse::Vec3 n = meshData.Vertices[i].Position.normalize();

        // Project onto sphere.
        const fuse::Vec3 p = radius * n;

        meshData.Vertices[i].Position = p;
        meshData.Vertices[i].Normal   = n;

        // Derive texture coordinates from spherical coordinates.
        float theta = atan2f(meshData.Vertices[i].Position.z, meshData.Vertices[i].Position.x);

        // Put in [0, 2pi].
        if (theta < 0.0f) {
            theta += std::numbers::pi_v<float>;
        }

        const float phi = acosf(meshData.Vertices[i].Position.y / radius);

        meshData.Vertices[i].TexC.x = theta / std::numbers::pi_v<float>;
        meshData.Vertices[i].TexC.y = phi / std::numbers::pi_v<float>;

        // Partial derivative of P with respect to theta
        meshData.Vertices[i].TangentU.x = -radius * sinf(phi) * sinf(theta);
        meshData.Vertices[i].TangentU.y = 0.0f;
        meshData.Vertices[i].TangentU.z = +radius * sinf(phi) * cosf(theta);

        meshData.Vertices[i].TangentU = meshData.Vertices[i].TangentU.normalize();
    }

    return meshData;
}

//------------------------------------------------------------------------
GeometryGenerator::MeshData GeometryGenerator::createCylinder(float bottomRadius, float topRadius,
                                                              float height, unsigned int sliceCount,
                                                              unsigned int stackCount) {
    MeshData meshData;

    const float stackHeight = height / (float)stackCount;

    // Amount to increment radius as we move up each stack level from bottom to top.
    const float radiusStep = (topRadius - bottomRadius) / (float)stackCount;

    const unsigned int ringCount = stackCount + 1;

    // Compute vertices for each stack ring starting at the bottom and moving up.
    for (unsigned int i = 0; i < ringCount; i++) {
        const float y = -0.5f * height + (float)i * stackHeight;
        const float r = bottomRadius + (float)i * radiusStep;

        // Vertices of ring
        const float dTheta = 2.0f * std::numbers::pi_v<float> / (float)sliceCount;
        for (unsigned int j = 0; j <= sliceCount; j++) {
            Vertex vertex;

            float c = std::cos((float)j * dTheta);
            float s = std::sin((float)j * dTheta);

            vertex.Position = fuse::Vec3(r * c, y, r * s);
            vertex.TexC.x   = (float)j / (float)sliceCount;
            vertex.TexC.y   = 1.0f - (float)i / (float)stackCount;

            // Cylinder can be parameterized as follows, where we introduce v
            // parameter that goes in the same direction as the v tex-coord
            // so that the bitangent goes in the same direction as the v tex-coord.
            //   Let r0 be the bottom radius and let r1 be the top radius.
            //   y(v) = h - hv for v in [0,1].
            //   r(v) = r1 + (r0-r1)v
            //
            //   x(t, v) = r(v)*cos(t)
            //   y(t, v) = h - hv
            //   z(t, v) = r(v)*sin(t)
            //
            //  dx/dt = -r(v)*sin(t)
            //  dy/dt = 0
            //  dz/dt = +r(v)*cos(t)
            //
            //  dx/dv = (r0-r1)*cos(t)
            //  dy/dv = -h
            //  dz/dv = (r0-r1)*sin(t)

            // This is unit length.
            vertex.TangentU = fuse::Vec3(-s, 0.0f, c);

            float      dr = bottomRadius - topRadius;
            fuse::Vec3 bitangent(dr * c, -height, dr * s);

            fuse::Vec3 T  = vertex.TangentU;
            fuse::Vec3 B  = bitangent;
            fuse::Vec3 N  = T.cross(B).normalize();
            vertex.Normal = N;

            meshData.Vertices.push_back(vertex);
        }
    }

    //
    // Index creation
    //
    // Add one because we duplicate the first and last vertex per ring
    // since the texture coordinates are different.
    const unsigned int lRingVertexCount = sliceCount + 1;
    for (unsigned int i = 0; i < stackCount; i++) {
        for (unsigned int j = 0; j < sliceCount; j++) {
            meshData.Indices.push_back(i * lRingVertexCount + j);
            meshData.Indices.push_back((i + 1) * lRingVertexCount + j);
            meshData.Indices.push_back((i + 1) * lRingVertexCount + j + 1);

            meshData.Indices.push_back(i * lRingVertexCount + j);
            meshData.Indices.push_back((i + 1) * lRingVertexCount + j + 1);
            meshData.Indices.push_back(i * lRingVertexCount + j + 1);
        }
    }

    buildCylinderTopCap(topRadius, height, sliceCount, meshData);
    buildCylinderBottomCap(bottomRadius, height, stackCount, meshData);

    return meshData;
}

//------------------------------------------------------------------------
GeometryGenerator::MeshData GeometryGenerator::createSphere(float radius, unsigned int sliceCount,
                                                            unsigned int stackCount) {
    MeshData meshData;

    //
    // Compute the vertices stating at the top pole and moving down the stacks.
    //

    // Poles: note that there will be texture coordinate distortion as there is
    // not a unique point on the texture map to assign to the pole when mapping
    // a rectangular texture onto a sphere.
    const Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    const Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    meshData.Vertices.push_back(topVertex);

    const float phiStep   = std::numbers::pi_v<float> / (float)stackCount;
    const float thetaStep = 2.0f * std::numbers::pi_v<float> / (float)sliceCount;

    // Compute vertices for each stack ring (do not count the poles as rings).
    for (unsigned int i = 1; i <= stackCount - 1; ++i) {
        float phi = (float)i * phiStep;

        // Vertices of ring.
        for (unsigned int j = 0; j <= sliceCount; ++j) {
            const float theta = (float)j * thetaStep;

            Vertex v;

            // spherical to cartesian
            v.Position.x = radius * sinf(phi) * cosf(theta);
            v.Position.y = radius * cosf(phi);
            v.Position.z = radius * sinf(phi) * sinf(theta);

            // Partial derivative of P with respect to theta
            v.TangentU.x = -radius * sinf(phi) * sinf(theta);
            v.TangentU.y = 0.0f;
            v.TangentU.z = +radius * sinf(phi) * cosf(theta);

            v.TangentU = v.TangentU.normalize();

            v.Normal = v.Position.normalize();

            v.TexC.x = theta / std::numbers::pi_v<float>;
            v.TexC.y = phi / std::numbers::pi_v<float>;

            meshData.Vertices.push_back(v);
        }
    }

    meshData.Vertices.push_back(bottomVertex);

    //
    // Compute indices for top stack.  The top stack was written first to the vertex buffer
    // and connects the top pole to the first ring.
    //

    for (unsigned int i = 1; i <= sliceCount; ++i) {
        meshData.Indices.push_back(0);
        meshData.Indices.push_back(i + 1);
        meshData.Indices.push_back(i);
    }

    //
    // Compute indices for inner stacks (not connected to poles).
    //

    // Offset the indices to the index of the first vertex in the first ring.
    // This is just skipping the top pole vertex.
    unsigned int       baseIndex       = 1;
    const unsigned int ringVertexCount = sliceCount + 1;
    for (unsigned int i = 0; i < sliceCount - 2; ++i) {
        for (unsigned int j = 0; j < sliceCount; ++j) {
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    //
    // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
    // and connects the bottom pole to the bottom ring.
    //

    // South pole vertex was added last.
    const unsigned int southPoleIndex = (unsigned int)meshData.Vertices.size() - 1;

    // Offset the indices to the index of the first vertex in the last ring.
    baseIndex = southPoleIndex - ringVertexCount;

    for (unsigned int i = 0; i < sliceCount; ++i) {
        meshData.Indices.push_back(southPoleIndex);
        meshData.Indices.push_back(baseIndex + i);
        meshData.Indices.push_back(baseIndex + i + 1);
    }

    return meshData;
}

//------------------------------------------------------------------------
GeometryGenerator::MeshData GeometryGenerator::createFullscreenQuad() {
    MeshData meshData;
    meshData.Vertices.resize(4);
    meshData.Indices.resize(6);

    // Position coordinates specified in NDC space.
    meshData.Vertices[0] =
      Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    meshData.Vertices[1] =
      Vertex(-1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    meshData.Vertices[2] =
      Vertex(+1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    meshData.Vertices[3] =
      Vertex(+1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    meshData.Indices[0] = 0;
    meshData.Indices[1] = 1;
    meshData.Indices[2] = 2;

    meshData.Indices[3] = 0;
    meshData.Indices[4] = 2;
    meshData.Indices[5] = 3;

    return meshData;
}


GeometryGenerator::MeshData GeometryGenerator::CreateQuad(float x, float y, float w, float h,
                                                          float depth) {
    MeshData meshData;

    meshData.Vertices.resize(4);
    meshData.Indices.resize(6);

    // Position coordinates specified in NDC space.
    meshData.Vertices[0] = Vertex(x, y - h, depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    meshData.Vertices[1] = Vertex(x, y, depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    meshData.Vertices[2] = Vertex(x + w, y, depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    meshData.Vertices[3] =
      Vertex(x + w, y - h, depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    meshData.Indices[0] = 0;
    meshData.Indices[1] = 1;
    meshData.Indices[2] = 2;

    meshData.Indices[3] = 0;
    meshData.Indices[4] = 2;
    meshData.Indices[5] = 3;

    return meshData;
}

//------------------------------------------------------------------------
void GeometryGenerator::subdivide(MeshData& meshData) {
    // Save a copy of the input geometry.
    MeshData inputCopy = meshData;

    meshData.Vertices.resize(0);
    meshData.Indices.resize(0);

    //       v1
    //       *
    //      / \
    //     /   \
    //  m0*-----*m1
    //   / \   / \
    //  /   \ /   \
    // *-----*-----*
    // v0    m2     v2

    unsigned int numTris = (unsigned int)inputCopy.Indices.size() / 3;
    for (unsigned int i = 0; i < numTris; ++i) {
        const Vertex v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
        const Vertex v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
        const Vertex v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

        //
        // Generate the midpoints.
        //
        const Vertex m0 = midPoint(v0, v1);
        const Vertex m1 = midPoint(v1, v2);
        const Vertex m2 = midPoint(v0, v2);

        //
        // Add new geometry.
        //
        meshData.Vertices.push_back(v0); // 0
        meshData.Vertices.push_back(v1); // 1
        meshData.Vertices.push_back(v2); // 2
        meshData.Vertices.push_back(m0); // 3
        meshData.Vertices.push_back(m1); // 4
        meshData.Vertices.push_back(m2); // 5

        meshData.Indices.push_back(i * 6 + 0);
        meshData.Indices.push_back(i * 6 + 3);
        meshData.Indices.push_back(i * 6 + 5);

        meshData.Indices.push_back(i * 6 + 3);
        meshData.Indices.push_back(i * 6 + 4);
        meshData.Indices.push_back(i * 6 + 5);

        meshData.Indices.push_back(i * 6 + 5);
        meshData.Indices.push_back(i * 6 + 4);
        meshData.Indices.push_back(i * 6 + 2);

        meshData.Indices.push_back(i * 6 + 3);
        meshData.Indices.push_back(i * 6 + 1);
        meshData.Indices.push_back(i * 6 + 4);
    }
}

//------------------------------------------------------------------------
void GeometryGenerator::buildCylinderTopCap(float topRadius, float height, unsigned int sliceCount,
                                            MeshData& meshData) {
    const unsigned int baseIndex = (unsigned int)meshData.Vertices.size();

    const float y      = 0.5f * height;
    const float dTheta = 2.0f * std::numbers::pi_v<float> / (float)sliceCount;

    // Duplicate cap ring vertices because the texture coordinates and normals differ.
    for (unsigned int i = 0; i <= sliceCount; ++i) {
        const float x = topRadius * cosf((float)i * dTheta);
        const float z = topRadius * sinf((float)i * dTheta);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        const float u = x / height + 0.5f;
        const float v = z / height + 0.5f;

        meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
    }

    // Cap center vertex.
    meshData.Vertices.push_back(
      Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

    // Index of center vertex.
    unsigned int centerIndex = (unsigned int)meshData.Vertices.size() - 1;

    for (unsigned int i = 0; i < sliceCount; ++i) {
        meshData.Indices.push_back(centerIndex);
        meshData.Indices.push_back(baseIndex + i + 1);
        meshData.Indices.push_back(baseIndex + i);
    }
}

//------------------------------------------------------------------------
void GeometryGenerator::buildCylinderBottomCap(float bottomRadius, float height,
                                               unsigned int sliceCount, MeshData& meshData) {
    //
    // Build bottom cap.
    //

    const unsigned int baseIndex = (unsigned int)meshData.Vertices.size();
    const float        y         = -0.5f * height;

    // vertices of ring
    const float dTheta = 2.0f * std::numbers::pi_v<float> / (float)sliceCount;
    for (unsigned int i = 0; i <= sliceCount; ++i) {
        const float x = bottomRadius * cosf((float)i * dTheta);
        const float z = bottomRadius * sinf((float)i * dTheta);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        const float u = x / height + 0.5f;
        const float v = z / height + 0.5f;

        meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
    }

    // Cap center vertex.
    meshData.Vertices.push_back(
      Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

    // Cache the index of center vertex.
    const unsigned int centerIndex = (unsigned int)meshData.Vertices.size() - 1;

    for (unsigned int i = 0; i < sliceCount; ++i) {
        meshData.Indices.push_back(centerIndex);
        meshData.Indices.push_back(baseIndex + i);
        meshData.Indices.push_back(baseIndex + i + 1);
    }
}

GeometryGenerator::Vertex GeometryGenerator::midPoint(const Vertex& v0, const Vertex& v1) {
    // Compute the midpoints of all the attributes.
    /// Vectors need to be normalized since linear interpolating can make them not unit length.
    Vertex v;
    v.Position = 0.5f * (v0.Position + v1.Position);
    v.Normal   = (0.5f * (v0.Normal + v1.Normal)).normalize();
    v.TangentU = (0.5f * (v0.TangentU + v1.TangentU)).normalize();
    v.TexC     = 0.5f * (v0.TexC + v1.TexC);
    return v;
}
