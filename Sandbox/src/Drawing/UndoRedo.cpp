#include "Drawing/UndoRedo.hpp"

#include <string.h>

Operation::Operation() : m_type(OPTYPE_NULL) {}

Operation::Operation(OperationType optype, glm::vec2 pixelChanged, Color prevColor)
	: m_type(optype), m_pixelChanged(pixelChanged), m_diff(prevColor) {}

Operation::~Operation() {}

Operation* OperationStack::popUndo() {
	// check if stack is empty before any processing
	if (curr == 0)
		return nullptr;

	// set curr back to beginning of previous operation
	curr--;

	while (stack[curr].m_type != OPTYPE_MOUSE_DOWN && curr > 0) {
		curr--;
	}

	return &stack[curr];
}

void OperationStack::pushUndo(Operation* ops) {
	invalidateRedo();

	// add full operation to stack
	int i = 0;
	bool opFinished = false;
	while (!opFinished) {
		if (ops[i].m_type == OPTYPE_MOUSE_UP)
			opFinished = true;

		stack.push_back(ops[i]);
		i++;
	}

	// set curr to front of stack
	curr = stack.size();
}

Operation* OperationStack::popRedo() {
	// check if redo stack is empty before any processing
	if (curr == stack.size())
		return nullptr;

	// save address to return
	Operation* op = &stack[curr];

	// set curr forward to beginning of next operation
	curr++;

	while (stack[curr].m_type != OPTYPE_MOUSE_DOWN && curr < stack.size()) {
		curr++;
	}

	return op;
}

void OperationStack::invalidateRedo() {
	while (stack.size() > curr) {
		stack.pop_back();
	}
}
