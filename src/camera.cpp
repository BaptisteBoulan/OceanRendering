#include "camera.h"

Camera::Camera(glm::vec3 startPosition) {
    position = startPosition;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 10.0f;
    sensitivity = 0.1f;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
    float velocity = speed * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
        position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        position += glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
        position += up * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
        position -= up * velocity;
}

void Camera::processMouse(float xOffset, float yOffset) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch -= yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}
