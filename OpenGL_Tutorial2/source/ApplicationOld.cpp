#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define RUN_THIS 0

#if RUN_THIS == 1
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Tutorial 4 Notes
// VertexBuffers
// occurs in vram of gpu
// define data to represent data 
// place in vram
// perform draw call
// can control how to interpret data and how to put on screen
// program gpu occurs through shaders
// openGL operates in a state machine

// Tutorial 5 Notes
// GPU needs to know the layout of the buffer data which determine the characteristics of the data
// This is done by vertex attribute pointer
// Vertex can contain more than just positions

// Tutorial 6 Notes
// Shader is code that runs on GPU
// set to openGL which then compiled, linked, and run on GPU
// Shaders tell how to use data
// draw call -> vertex shaders -> fragment shaders -> render
// vertex shader defines where each vertex should be, called based on number of vertices
// fragment shader fills in the pixel and determines the color, called based on the number of pixels defined by vertices
// can write a dynamic shaders

// Tutorial 9 Notes
// drawing a square requires 2 triangles
// by specifying all the points of the triangle, there is repeated data which is inefficient
// index buffers allows for reuse of different vertices
// index buffers must be unsigned integers

// Tutorial 10 Notes
// Tutorial 11 Notes
// Data defined in CPU can be sent to GPU by using uniforms and vertex buffers
// Uniforms are set per draw. They are set before the draw call

// Tutorial 12 Notes
// Vertex array vs vertex buffer (vertexAttribArray
// Vertex arrays are specific openGL
// A way to bind vertex attrib array pointer to buffer
// Avoids having to binding vertex buffer, binding index index, then drawing for every object
// Vertex array pointer will bound vertex array buffer slot
// bind shader -> bind vertex array -> bind index array -> draw call
// vertex attrib pointer will bound vertex object to buffer slot
// everytime vertex attrib pointer is called it will bind a new object to vertex attribute object

// Compatibility profile: default VAO
// create one VAO and leave it bound -> define vertex buffer for each object -> define vertex layout -> draw
// for every object create VAO -> bind vertex buffer by ID -> bind layout -> draw

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" <<
            function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    // T8
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    // T8
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

// Tutorial 7 Create Shader Source Code
// return a code that references shader

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // T8
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // TODO: ERROR HANDLING
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length*sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? " vertex " : " fragment " )
            <<  " shader " << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}
static unsigned int CreateShader(const std::string& vs, const std::string& fs)
{

    unsigned int program = glCreateProgram();
    unsigned int v_s = CompileShader(GL_VERTEX_SHADER, vs);
    unsigned int f_s = CompileShader(GL_FRAGMENT_SHADER, fs);
    GLCall(glAttachShader(program, v_s));
    GLCall(glAttachShader(program, f_s));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    GLCall(glDeleteShader(v_s));
    GLCall(glDeleteShader(f_s));
    return program;
}

int application(void)
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Throttle???

    if (glewInit() != GLEW_OK)
        std::cout <<" ERROR " << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    // T4: Define Vertex Buffer
    float positions[8] = {
        -0.5f, -0.5f, 
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] =
    {
        0,1,2,
        2,3,0
    };

    // T 12 Creating a vertex attribute object
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    unsigned int bufferTri; // buffer ID
    GLCall(glGenBuffers(1, &bufferTri));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferTri)); // selecting
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));

    // T5: glVertexAttribPointer tells gpu how the data is packaged
    // index of vertex attribute to be modified
    // size of components per generic vertex attribute
    // data type of vertex attribute
    // stride size of 1 vertex attribute
    // pointer offset of first vertex attributes
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    //T9
    unsigned int ibo; // buffer ID
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // selecting
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));


    // T8
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    // T7
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    // T11
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    std::cout << "Vertex Source" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment Source" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    // T12 unbind everything (specifying VAO per object)
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float increment = 0.05;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        // T3 Legacy gl
        //glBegin(GL_TRIANGLES);
        //glVertex2f(-0.5f, -0.5f);
        //glVertex2f(0.0f, 0.5f);
        //glVertex2f(0.5f, -0.5f);
        //glEnd();

        // T12 
        GLCall(glUseProgram(shader));
        //T 11 are per draw functions
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        // T12 Bind Square
        // Can delete vertex positions
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferTri));
        //GLCall(glEnableVertexAttribArray(0));
        //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        // Binding vertex array
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        // T4
        // Must select layer to output a draw function
        // similar to autocad where layers must be selected.
        // this selection occurs with the glBindBuffer
        // The item that is drawn by gpu is the one that is bound
        //glDrawArrays(GL_TRIANGLES, 0, 6); // used when there is no indexarray
  
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // T9

        // T10 
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr))

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    
    GLCall(glDeleteProgram(shader)); // T7 Delete shader

    GLCall(glfwTerminate());
    return 0;
}

#endif