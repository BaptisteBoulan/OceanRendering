#pragma once
#include <vector>

class SeaMesh {
    public:
        SeaMesh(float size, int resolution);
        void draw();
        ~SeaMesh();

    private:
        unsigned int EBO, VAO, vertex_count;
        std::vector<unsigned int> VBOs;
    };