#pragma once

#include "Application/Layer.hpp"

#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/ApplicationEvent.hpp"

// forward declaration
class Application;

class ImGuiLayer : public Layer {
  public:
	ImGuiLayer();
	~ImGuiLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;

	void begin();
	void end();

  private:
	float m_Time = 0.0f;
	Application& app;
};
