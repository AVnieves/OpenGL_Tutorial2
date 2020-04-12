#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTranslation.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

// Tutorial 14 Notes
// Abstracting vertex array objects should enable tying vertex buffer with layout

// Tutorial 15 Notes
// Can create code that creates a shader program during run time
// Complex shader wrapper not necessary for openGL
// Basic funtionality, load file, bind unbind shaders, set all uniforms

// Tutorial 16 Notes
// Render will handle the draw functions, clear functions to handle objects dynamically
// Renderer should take in vertex buffer, index buffer, and material.
// material includes shader and all uniforms

// Tutorial 17 Notes
// Textures enables image texture to place on 3d object
// load -> create texture -> bind texture -> shader -> update draw
// Texture slot to sample on uses integer uniform

// Tutorial 18 Notes
// Blending helps with transparent portions of the image
// Blending determines how to combine output color with what is in target buffer
// Way to control blending
// 1. glEnable(GL_BLEND)
// 2. glBlendFunc(src, dest)
// src = how src RGBA is computed, weights for equation
// dest = how dest RGBA is computed, weights for equation
// 3. glBlendEquation(mode)
// default is added 
// src + dest
// GL_SRC_ALPHA
// GL_ONE_MINUS_SRC_ALPHA
// Src (1,1,1,0.5)
// Dest (1,0,1,1)
// Result (1, 0.5, 1, 0.75)

// Tutorial 19 Notes
// Math used for graphics are matrices and vectors
// Used for transformations
// Orthographic matrix means objects farther away do not get smaller usually used for 2d
// perspective matrix handles farther objects away usually used for 3d

// Tutorial 20 Notes
// Projection 3d world projected onto 2d screen
// positions are converted to normalized device coordinates x(-1,1) y(-1,1)
// Decide coordinate space on screen then perform projection math for screen

// Tutorial 21 Notes
// Model View Projection: transformation pipeline
// multiplied Projection * View * Model OpenGL specific
// Projection -> orthographic or perspective
// View -> postion(translation) rotation scale of camera
// Model -> position(translation) rotation scale of object

// Tutorial 22 Notes
// IM GUI Introduction used for quick debuggin purposes and input interface to graphics

// Tutorial 23 Notes
// Moving can be done by having two vertex buffer arrays
// or can be done by changing the postion (MVP matrix)

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //T 12: SET OPENGL CHANGES
    // CORE VAO does not exists
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // default is compatibility which creates a default

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Throttle???

    if (glewInit() != GLEW_OK)
        std::cout << " ERROR " << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float triPositions[6] =
        {
            -WINDOW_WIDTH/4, -WINDOW_HEIGHT/4,
            -WINDOW_WIDTH/4,  WINDOW_HEIGHT/4,
                          0,  WINDOW_HEIGHT /4
        };
        unsigned int triIndices[3] =
        {
            0,1,2
        };

        // T4: Vertex buffer with position and texture
        float positions[16] = {
             -50.f, -50.f, 0.0f, 0.0f,
              50.f, -50.f, 1.0f, 0.0f,
              50.f,  50.f, 1.0f, 1.0f,
             -50.f,  50.f, 0.0f, 1.0f
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;        
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
  
        IndexBuffer ib(indices, 6);
        
        // MVP movement
        glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);
        glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,0.f));
        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(200, 200, 0));
        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


        Texture texture("res/textures/bricks.jpg");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // IM GUI
        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        float r = 0.0f;
        float increment = 0.05;

        test::TestClearColor test;
        test::TestTranslation test2(translationA, WINDOW_WIDTH, WINDOW_HEIGHT);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //test.OnUpdate(0.f);
            //test.OnRender();

            test2.OnUpdate(0.f);
            test2.OnRender(va, ib, shader, renderer);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            //test.OnImGuiRender();
            {
                test2.OnImGuiRender();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // T9
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
      
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}