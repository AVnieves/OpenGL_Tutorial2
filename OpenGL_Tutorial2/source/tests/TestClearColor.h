#pragma once
#include "Test.h"

namespace test {
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender();
		void OnImGuiRender();

	private:
		float m_ClearColor[4];
	};
}
