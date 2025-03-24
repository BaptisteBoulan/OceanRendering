#include "triangle_mesh.h"
#include <glad/glad.h>
#include <iostream>

TriangleMesh::TriangleMesh() {
    std::vector<float> positions = {
        -1.0f, -1.0f, -.0f,
         1.0f, -1.0f, -.0f,
        -1.0f, 1.0f, .0f,
        1.0f, 1.0f, .0f,
    };

    std::vector<int> elements_indices = {
        1, 2, 3,
        0, 1, 2,
    };

    vertex_count = 6;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(1);

    glGenBuffers(1, VBOs.data());

    //position
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), 
            positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    //element buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_indices.size() * sizeof(int), 
            elements_indices.data(), GL_STATIC_DRAW);
}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, VBOs.data());
    glDeleteBuffers(1, &EBO);
}