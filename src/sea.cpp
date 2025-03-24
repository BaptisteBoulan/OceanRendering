#include "sea.h"
#include <glad/glad.h>
#include <iostream>
#include <vector>

SeaMesh::SeaMesh(float size, int resolution) {
    int vertCount = (resolution + 1) * (resolution + 1);
    int indexCount = 6 * resolution * resolution;
    std::vector<float> positions(vertCount * 3);
    std::vector<int> elements_indices(indexCount);

    float step = size / resolution;
    int index = 0;
    
    for (int z = 0; z <= resolution; ++z) {
        for (int x = 0; x <= resolution; ++x) {
            positions[index * 3] = x * step - size / 2.0f;
            positions[index * 3 + 1] = 0.0f;
            positions[index * 3 + 2] = z * step - size / 2.0f;
            index++;
        }
    }
    
    index = 0;
    for (int z = 0; z < resolution; ++z) {
        for (int x = 0; x < resolution; ++x) {
            int topLeft = z * (resolution + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (resolution + 1) + x;
            int bottomRight = bottomLeft + 1;

            elements_indices[index++] = topLeft;
            elements_indices[index++] = bottomLeft;
            elements_indices[index++] = topRight;

            elements_indices[index++] = topRight;
            elements_indices[index++] = bottomLeft;
            elements_indices[index++] = bottomRight;
        }
    }

    vertex_count = indexCount;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(1);
    glGenBuffers(1, VBOs.data());

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_indices.size() * sizeof(int), elements_indices.data(), GL_STATIC_DRAW);
}

void SeaMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
}

SeaMesh::~SeaMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, VBOs.data());
    glDeleteBuffers(1, &EBO);
}
