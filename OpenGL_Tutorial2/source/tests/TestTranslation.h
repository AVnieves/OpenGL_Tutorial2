#pragma once
#include "Test.h"
#include "GL/glew.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"

namespace test {
	class TestTranslation : public Test
	{
	public:
		TestTranslation(glm::vec3 t, float w, float h);
		~TestTranslation();

		void OnUpdate(float deltaTime) override;
		void OnRender(VertexArray& va, IndexBuffer& ib, Shader& s, Renderer& r);
		void OnImGuiRender();

	private:
		glm::vec3 m_Translation;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		float m_Width;
		float m_Height;
	};
}

