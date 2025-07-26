#include "camera.h"
#include <glm/fwd.hpp>

Camera::Camera(glm::vec3 pos, glm::vec2 rot_in) : pos(pos) {
    glm::vec3 front;
    front.x = cos(glm::radians(rot_in.x)) * cos(glm::radians(rot_in.y));
    front.y = sin(glm::radians(rot_in.y));
    front.z = sin(glm::radians(rot_in.x)) * cos(glm::radians(rot_in.y));
    rot = glm::normalize(front);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir_in) : pos(pos), rot(dir_in) {

}

void Camera::set_pos(glm::vec3 pos_in){
    pos = pos_in;
}

void Camera::set_rot(glm::vec2 rot_in){
    glm::vec3 front;
    front.x = cos(glm::radians(rot_in.x)) * cos(glm::radians(rot_in.y));
    front.y = sin(glm::radians(rot_in.y));
    front.z = sin(glm::radians(rot_in.x)) * cos(glm::radians(rot_in.y));
    rot = glm::normalize(front);  
}

void Camera::set_direction(glm::vec3 direction){
    rot = direction;
}

glm::mat4 Camera::get_view(){
    glm::vec3 right = glm::normalize(glm::cross(rot, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    glm::vec3 up    = glm::normalize(glm::cross(right, rot));

    return glm::lookAt(pos, rot-pos, up);
}

glm::vec3 Camera::get_pos(){
    return pos;
}