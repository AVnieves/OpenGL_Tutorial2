#include "TestTranslation.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test 
{

	TestTranslation::TestTranslation(glm::vec3 t, float w, float h)
		: m_Translation(t), m_Width(w), m_Height(h)
	{
		m_Proj = glm::ortho(0.f, m_Width, 0.f, m_Height, -1.f, 1.f);
		m_View = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
	}

	TestTranslation::~TestTranslation()
	{

	}

	void TestTranslation::OnUpdate(float deltaTime)
	{
		m_Model = glm::translate(glm::mat4(1.f), m_Translation);
	}

	void TestTranslation::OnRender(VertexArray& va, IndexBuffer& ib, Shader& s, Renderer& r)
	{
		glm::mat4 MVP = m_Proj * m_View * m_Model;
		s.Bind();
		s.SetUniformMat4f("u_MVP", MVP);
		r.Draw(va, ib, s);
	}
	
	void TestTranslation::OnImGuiRender()
	{
		ImGui::SliderFloat("Translation In X", &m_Translation.x, 0.0f, m_Width); 
		ImGui::SliderFloat("Translation In Y", &m_Translation.y, 0.0f, m_Height);

	}
}
