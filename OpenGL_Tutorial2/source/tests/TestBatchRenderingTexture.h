#pragma once
// texture sprites for different textures within a batch
// assign id to each quad in vertex buffer (32 slots)

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test {
	class TestBatchRenderingTexture : public Test
	{
	public:
		TestBatchRenderingTexture();
		~TestBatchRenderingTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender();
		void OnImGuiRender();

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr <IndexBuffer> m_IBO;
		std::unique_ptr <Shader> m_Shader;
		//std::unique_ptr <Texture> m_Texture;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;
		glm::vec3 m_TranslationA, m_TranslationB;
		glm::mat4 m_Proj, m_View;
	};
}
