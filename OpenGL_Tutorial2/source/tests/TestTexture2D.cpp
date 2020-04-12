#include "TestTexture2D.h"
#include "GL/glew.h"
#include "Renderer.h"
#include "imgui/imgui.h"



namespace test {

	TestTexture2D::TestTexture2D()
        : m_Proj (glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f)),
          m_View (glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f))),
          m_TranslationA(200,200,0), m_TranslationB(400,200,0)
	{

        // T4: Vertex buffer with position and texture
        float positions[] = {
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


        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        //VertexBuffer m_VB(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VB, layout);
        m_IBO = std::make_unique<IndexBuffer>(indices , 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/bricks.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;
        m_Texture->Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationA);
            glm::mat4 MVP = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", MVP);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationB);
            glm::mat4 MVP = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", MVP);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }

	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0, 960.f);
        ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0, 960.f);
	}
}