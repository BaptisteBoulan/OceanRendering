#include "sky.h"
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <cmath>

#define PI 3.14159265

SkyMesh::SkyMesh(int resolution) {
    if (resolution < 3) resolution = 3; // Minimum resolution for a sphere

    std::vector<float> positions;
    std::vector<int> elements_indices;

    // Generate vertices for the sphere
    for (int y = 0; y <= resolution; ++y) {
        for (int x = 0; x <= resolution; ++x) {
            float theta = 2.0f * PI * x / resolution;
            float phi = PI * y / resolution;

            float xPos = sin(phi) * cos(theta);
            float yPos = cos(phi);
            float zPos = sin(phi) * sin(theta);

            positions.push_back(xPos);
            positions.push_back(yPos);
            positions.push_back(zPos);
        }
    }

    // Generate indices for the sphere
    for (int y = 0; y < resolution; ++y) {
        for (int x = 0; x < resolution; ++x) {
            int topLeft = (y * (resolution + 1) + x);
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
