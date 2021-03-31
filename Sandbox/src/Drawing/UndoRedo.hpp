#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Color.hpp"

enum OperationType { OPTYPE_DRAW, OPTYPE_MOUSE_DOWN, OPTYPE_MOUSE_UP, OPTYPE_NULL };

class Operation {
	friend class OperationStack;

  private:
	OperationType m_type;
	glm::vec2 m_pixelChanged;
	Color m_diff;

  public:
	Operation();
	Operation(OperationType optype, glm::vec2 pixelsChanged, Color diff);
	~Operation();

	inline const OperationType getType() const {
		return m_type;
	}

	inline const glm::vec2 getPxChanged() const {
		return m_pixelChanged;
	}

	inline const Color getColorDiff() const {
		return m_diff;
	}
};

class OperationStack {
  private:
	int curr = 0;
	std::vector<Operation> stack;

  public:
	Operation* popUndo();
	void pushUndo(Operation* ops);

	Operation* popRedo();
	void invalidateRedo();
};
