#include "shader_loader.h"

unsigned int make_module(const std::string file_path, unsigned int module_type) {
    std::ifstream file;
    std::string line;

    std::stringstream bufferedlines;

    file.open(file_path);

    while (std::getline(file, line)) {
        bufferedlines << line << '\n';
    }

    std::string shaderSource = bufferedlines.str();

    const char* shaderSrc = shaderSource.c_str();
    bufferedlines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type); 
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success) {
        char errorLog[1024]; //size of the error log
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout<<"Shader Module error"<< errorLog << std::endl;
    }

    return shaderModule;
}

unsigned int make_shader(const std::string& vert_path, const std::string& frag_path) {
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vert_path,GL_VERTEX_SHADER));
    modules.push_back(make_module(frag_path,GL_FRAGMENT_SHADER));

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
