#include "config.h"

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        
        float yaw;
        float pitch;
        float speed;
        float sensitivity;
    
        Camera(glm::vec3 startPosition);
    
        glm::mat4 getViewMatrix();
        void processKeyboard(GLFWwindow* window, float deltaTime);
        void processMouse(float xOffset, float yOffset);
    };