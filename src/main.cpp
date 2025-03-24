#include "config.h"
#include "sky.h"
#include "sea.h"
#include "shader_loader.h"


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

    int windowWidth = 1440;
    int windowHeight = 720;
    window = glfwCreateWindow(windowWidth, windowHeight, "My Renderer", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;

    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f); 
    

    //mesh initialisation
    SeaMesh* sea = new SeaMesh(100.0f, 100);
    SkyMesh* sky = new SkyMesh(100);

    //Camera set up
    float camera_speed = 0.015f;
    float fovy = 1.0f;
    glm::mat4 projection = glm::perspective(fovy, (float)windowWidth/windowHeight, 0.1f, 50.0f);
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

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


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float time = (float)glfwGetTime();

        //camera
        glm::vec3 cameraPos = {1.0f * cos(camera_speed * time), 3.0f, 1.0f * sin(camera_speed * time)};
        glm::vec3 cameraTarget = {-100.0f * cos(camera_speed * time), 0.0f, -100.0f * sin(camera_speed * time)};
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);

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
        glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		sea->draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
	delete sea;
    delete sky;
	return 0;
}
