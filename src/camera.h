#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:    
    Camera(glm::vec3 pos, glm::vec2 rot_in);
    Camera(glm::vec3 pos, glm::vec3 dir_in);
    void set_pos(glm::vec3 pos_in);
    void set_rot(glm::vec2 rot_in);
    void set_direction(glm::vec3 direction);
    glm::mat4 get_view();
    glm::vec3 get_pos();
private:
    glm::vec3 pos;
    glm::vec3 rot;
};