#include "config.h"
#include "sky.h"
#include "sea.h"
#include "shader_loader.h"
#include "camera.h"

// Window setup
int windowWidth = 640;
int windowHeight = 480;

//Camera setup
Camera camera(glm::vec3(0.0f, 15.0f, 10.0f));
float lastX = (float)windowWidth/2, lastY = (float)windowHeight/2; // Center of the window
bool firstMouse = true;

float fovy = 1.0f;
glm::mat4 projection = glm::perspective(fovy, (float)windowWidth/windowHeight, 0.1f, 300.0f);
glm::vec3 up = {0.0f, 1.0f, 0.0f};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xOffset = (float)xpos - lastX;
    float yOffset = (float)ypos - lastY;
    lastX = (float)xpos;
    lastY = (float)ypos;

    camera.processMouse(xOffset, yOffset);
}

// For debugging
void printMatrix(const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[j][i] << "  ";
        }
        std::cout << std::endl;
    }
}


int main() {
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout<<"GLFW couldn't start"<<std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window = glfwCreateWindow(windowWidth, windowHeight, "My Renderer", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;

    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f); 
    

    //mesh initialisation
    SeaMesh* sea = new SeaMesh(500.0f, 1000);
    SkyMesh* sky = new SkyMesh(20); // like an HDRI

    // Sea shaders
    GLint seaShader = make_shader(
        "./shaders/seaVertShader.glsl",
        "./shaders/seaFragShader.glsl"
    );
    glUseProgram(seaShader);

    //shader uniform variables
    GLint cameraPosLoc = glGetUniformLocation(seaShader, "cameraPos");
    GLint iResolutionLoc = glGetUniformLocation(seaShader, "iResolution");
    GLint iTimeLoc = glGetUniformLocation(seaShader, "iTime");
    GLint projectionLoc = glGetUniformLocation(seaShader, "projection");
    unsigned int viewLoc = glGetUniformLocation(seaShader, "view");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //transform matrix
    glm::mat4 model = glm::mat4(1.0f);
    GLint model_location = glGetUniformLocation(seaShader, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));


    // Sky Shader
    GLint skyShader = make_shader(
        "./shaders/skyVertShader.glsl",
        "./shaders/skyFragShader.glsl"
    );
    glUseProgram(skyShader);

    //shader uniform variables
    GLint skyITimeLoc = glGetUniformLocation(skyShader, "iTime");
    GLint skyViewLoc = glGetUniformLocation(skyShader, "view");
    GLint fovyLoc = glGetUniformLocation(skyShader, "fovy");
    GLint skyProjectionLoc = glGetUniformLocation(skyShader, "projection");

    glUniformMatrix4fv(skyProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(fovyLoc, fovy);


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwPollEvents();

        float time = (float)glfwGetTime();

        //camera
        static float lastFrame = time;
        float deltaTime = time - lastFrame;
        lastFrame = time;

        camera.processKeyboard(window, deltaTime);
        glm::mat4 view = camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT);

        // SKY
        glUseProgram(skyShader);

        //sky shader uniform variables
        glUniform1f(skyITimeLoc, time);
        glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(view));

        sky->draw();


        glUseProgram(seaShader);
        //sea shader uniform variables
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform1f(iTimeLoc, time);
        glUniform3f(cameraPosLoc, camera.position.x, camera.position.y, camera.position.z);

		sea->draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
	delete sea;
    // delete sky;
	return 0;
}
