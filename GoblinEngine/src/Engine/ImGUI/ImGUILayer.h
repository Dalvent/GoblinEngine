#pragma once

namespace GoblinEngine
{
	class ImGUILayer : public Layer
	{
	public:
		ImGUILayer() : Layer("ImGUILayer") {}

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Start();
		void End();
	private:
		bool isShow;
		float _lastFrameTime = 0.0f;
	};
}