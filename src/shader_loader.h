#include "config.h"

GLint make_module(const std::string& file_path, GLint module_type, const std::string& include_path = "");
GLint make_shader(const std::string& vert_path, const std::string& frag_path);