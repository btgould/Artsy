#pragma once

#include "Events/Event.hpp"

class Layer {
  public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {};
	virtual void OnDetach() {};
	virtual void OnUpdate() {};
	virtual void OnEvent(Event& e) {};

	virtual void OnImGuiRender() {};

	inline const std::string getName() {
		return m_DebugName;
	}

  private:
	std::string m_DebugName;
};
