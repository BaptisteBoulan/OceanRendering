#pragma once
#include <vector>

class SkyMesh {
    public:
        SkyMesh(int resolution);
        void draw();
        ~SkyMesh();

    private:
        unsigned int EBO, VAO, vertex_count;
        std::vector<unsigned int> VBOs;
    };