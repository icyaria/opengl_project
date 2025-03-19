#include "../include/glad.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sphere.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

#include <iostream>
#include <fstream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Camera Settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Input handling
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// some variables for control
GLfloat scale = 0.1f;
bool sphere_moves = true;
GLfloat speed = 1.0f;
float radius = 14.0f * scale;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//for debugging
void checkShaderExists(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "ERROR: Shader file not found: " << path << std::endl;
    } else {
        std::cout << "Shader file found: " << path << std::endl;
    }
}

int main(int, char**){

    // check shaders are found correctly
    checkShaderExists("build/shaders/light_shader.vs");
    checkShaderExists("build/shaders/light_shader.fs");
    checkShaderExists("build/shaders/face_shader.vs");
    checkShaderExists("build/shaders/face_shader.fs");
    
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Graphics I", NULL, NULL);

    if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Shaders
    Shader sphere_shader("build/shaders/light_shader.vs", "build/shaders/light_shader.fs");
    Shader face_shader("build/shaders/face_shader.vs", "build/shaders/face_shader.fs");
        
    Sphere sphere(20, 20);
    Model Cece("build/assets/woman1.obj");

    // variables used in render loop
	GLfloat x, z;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Time management
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window);

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// projection
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		
        // draw sphere

        //activate shader
        sphere_shader.use();

        // set view and projection matrices
		sphere_shader.setMat4("projection", projection);
		sphere_shader.setMat4("view", view);

        sphere_shader.setVec3("lightPos", glm::vec3(x, 0.0, z));
        sphere_shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Άσπρο φως

		sphere_shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

        // sphere rotation around face:
        if (sphere_moves) {
            // calculate current angle
            float angle = glfwGetTime() * speed;
            x = radius * sin(angle);
            z = radius * cos(angle);
        }

        // Set model matrix
        glm::mat4 model_sphere = glm::mat4(1.0f);
		model_sphere = glm::translate(model_sphere, glm::vec3(x, 0.0, z));	// translate sphere for rotation using the x and y calculated for this frame
		model_sphere = glm::scale(model_sphere, glm::vec3(1.0f * scale));	// scale sphere so that it fit the window
		sphere_shader.setMat4("model", model_sphere);

        // Render the sphere
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
        sphere.Draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // return to fill mode
        
        // draw face

        //activate shader
		face_shader.use();

        // set view and projection matrices
		face_shader.setMat4("projection", projection);
		face_shader.setMat4("view", view);

        // Set model matrix
		glm::mat4 model_face = glm::mat4(1.0f);
		model_face = glm::rotate(model_face, glm::radians(-90.0f), glm::vec3(0,1,0));
        model_face = glm::translate(model_face, glm::vec3(0.0f, -0.2f, 0.0f)); 
		model_face = glm::scale(model_face, glm::vec3(0.05f * scale));
		face_shader.setMat4("model", model_face);

        // Set light properties
		face_shader.setVec3("light.position", glm::vec3(x, 0.0, z)); 
		face_shader.setVec3("viewPos", camera.Position);
		
        // Set light properties
        face_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        face_shader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        face_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        face_shader.setFloat("light.constant", 1.0f);
        face_shader.setFloat("light.linear", 0.09f);
        face_shader.setFloat("light.quadratic", 0.032f);
		
        // Set material properties
        face_shader.setFloat("material.shininess", 32.0f);
		
		Cece.Draw(face_shader);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Cleaning up OpenGL resources..." << std::endl;
    glDeleteVertexArrays(1, &sphere.VAO);
    glDeleteBuffers(1, &sphere.VBO);
    glDeleteBuffers(1, &sphere.EBO);

    // Delete loaded textures
    for (unsigned int i = 0; i < Cece.textures_loaded.size(); i++) {
        glDeleteTextures(1, &Cece.textures_loaded[i].id);
    }
    // Clean up and terminate
    glfwMakeContextCurrent(NULL);
    glfwTerminate();

    return 0;
}

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

    lastX = (float)xpos;
    lastY = (float)ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)		// close window with ESC
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)			// move forward with W
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)			// move backward with S
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)			// move left with A
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)			// move right with D
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)			// pause the sphere with P
		sphere_moves = false;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)			// unpause with U
		sphere_moves = true;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)			// increase speed of sphere with J
        speed += 0.05;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)			// decrease speed of sphere with H
	{
		speed -= 0.05;
		if (speed < 0.0) speed = 0.0;
	}

}

