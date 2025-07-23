#pragma once
#include <glm/fwd.hpp>
#include  <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(std::string vertexPath, std::string fragmentPath);
    // use/activate the shader
    void bind();
    // utility uniform functions
    void set_bool(const std::string &name, bool value) const;  
    void set_int(const std::string &name, int value) const;   
    void set_float(const std::string &name, float value) const;
    void set_mat4(const std::string &name, glm::mat4& matrix) const;
    void set_vec3(const std::string &name, glm::vec3& vec) const;
    void set_vec3(const std::string &name, float x, float y, float z) const;
};