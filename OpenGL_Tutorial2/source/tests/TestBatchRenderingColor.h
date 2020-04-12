#pragma once
// Tutorial 29 use vertex colors for batch rendering

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test {
	class TestBatchRenderingColor : public Test
	{
	public:
		TestBatchRenderingColor();
		~TestBatchRenderingColor();

		void OnUpdate(float deltaTime) override;
		void OnRender();
		void OnImGuiRender();

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr <IndexBuffer> m_IBO;
		std::unique_ptr <Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		glm::vec3 m_TranslationA;
		glm::vec3 m_ViewTrans;
		glm::mat4 m_Proj, m_View;
	};
}



