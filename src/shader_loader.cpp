#include "shader_loader.h"

GLint make_module(const std::string& file_path, GLint module_type, const std::string& include_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << file_path << std::endl;
        return 0;
    }

    std::stringstream main_shader;
    std::string line;
    int line_number = 0;
    
    // Read the first shader file
    while (std::getline(file, line)) {
        main_shader << line << '\n';
        if (line_number == 20 && !include_path.empty()) {
            std::ifstream include_file(include_path);
            if (include_file.is_open()) {
                std::stringstream include_shader;
                include_shader << include_file.rdbuf();
                main_shader << include_shader.str() << '\n';
                include_file.close();
            } else {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier d'inclusion " << include_path << std::endl;
            }
        }
        line_number += 1 ;
    }
    file.close();


    std::string shaderSource = main_shader.str();
    const char* shaderSrc = shaderSource.c_str();


    GLint shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cerr << "Erreur de compilation du shader : " << errorLog << std::endl;
    }

    return shaderModule;
}

GLint make_shader(const std::string& vert_path, const std::string& frag_path) {
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vert_path,GL_VERTEX_SHADER));
    // modules.push_back(make_module(frag_path,GL_FRAGMENT_SHADER));
    modules.push_back(make_module(frag_path,GL_FRAGMENT_SHADER,"./shaders/skyCOlor.glsl"));

    unsigned int shader = glCreateProgram();
    for (unsigned int module : modules) {
        glAttachShader(shader, module);
    }
    glLinkProgram(shader);

    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char errorLog[1024]; //size of the error log
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout<<"Shader linking error: "<< errorLog << std::endl;
    }

    for (unsigned int module : modules) {
        glDeleteShader(module);
    }

    return shader;
}
