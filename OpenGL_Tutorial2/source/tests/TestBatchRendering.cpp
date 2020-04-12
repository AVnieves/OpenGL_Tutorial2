#include "TestBatchRendering.h"
#include "GL/glew.h"
#include "Renderer.h"
#include "imgui/imgui.h"



namespace test {

    TestBatchRendering::TestBatchRendering()
        : m_Proj(glm::ortho(0.f, 960.f, 0.f, 540.f, -100.f, 100.f)),
        m_View(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f))),
        m_TranslationA(200, 200, -0.5)
    {

        float positions[] = {
             -150.f, -50.f, 0.f, 0.0f, 0.0f,
              -50.f, -50.f, 0.f, 1.0f, 0.0f,
              -50.f,  50.f, 0.f, 1.0f, 1.0f,
             -150.f,  50.f, 0.f, 0.0f, 1.0f,

               50.f, -50.f, 0.f, 0.0f, 0.0f,
              150.f, -50.f, 0.f, 1.0f, 0.0f,
              150.f,  50.f, 0.f, 1.0f, 1.0f,
               50.f,  50.f, 0.f, 0.0f, 1.0f
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0,
            4,5,6,
            6,7,4

        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(positions, 5 * 8 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VB, layout);
        m_IBO = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/bricks.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    TestBatchRendering::~TestBatchRendering()
    {

    }

    void TestBatchRendering::OnUpdate(float deltaTime)
    {

    }

    void TestBatchRendering::OnRender()
    {
        GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;
        m_Texture->Bind();
        {
            glm::mat4 view = glm::translate(glm::mat4(1.f), m_ViewTrans);
            glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationA);
            glm::mat4 MVP = m_Proj * view * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", MVP);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
    }

    void TestBatchRendering::OnImGuiRender()
    {
        ImGui::SliderFloat2("TranslationA", &m_TranslationA.x, 0.0, 960.f);
        ImGui::SliderFloat("TranslationA", &m_ViewTrans.z, -100.f, 100.f);
        //ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0, 960.f);
    }
}