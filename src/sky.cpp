#include "sky.h"
#include <glad/glad.h>
#include <vector>
#include <iostream>

SkyMesh::SkyMesh(int resolution) {
    if (resolution < 1) resolution = 1;

    std::vector<float> positions;
    std::vector<int> elements_indices;

    // Génération des sommets
    for (int y = 0; y <= resolution; ++y) {
        for (int x = 0; x <= resolution; ++x) {
            float px = -1.0f + 2.0f * x / resolution;
            float py = -1.0f + 2.0f * y / resolution;
            positions.push_back(px);
            positions.push_back(py);
            positions.push_back(0.0f);
        }
    }

    // Génération des indices des triangles
    for (int y = 0; y < resolution; ++y) {
        for (int x = 0; x < resolution; ++x) {
            int topLeft = y * (resolution + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (resolution + 1);
            int bottomRight = bottomLeft + 1;

            elements_indices.push_back(topLeft);
            elements_indices.push_back(bottomLeft);
            elements_indices.push_back(bottomRight);

            elements_indices.push_back(topLeft);
            elements_indices.push_back(bottomRight);
            elements_indices.push_back(topRight);
        }
    }

    vertex_count = elements_indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(1);
    glGenBuffers(1, VBOs.data());

    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Element buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_indices.size() * sizeof(int), elements_indices.data(), GL_STATIC_DRAW);
}

void SkyMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
}

SkyMesh::~SkyMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, VBOs.data());
    glDeleteBuffers(1, &EBO);
}