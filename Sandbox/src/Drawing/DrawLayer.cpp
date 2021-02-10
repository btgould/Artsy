#include "Drawing/DrawLayer.hpp"

DrawLayer::DrawLayer(int width, int height, std::string shaderPath)
	: m_Width(width), m_Height(height), shader(shaderPath) {
	float drawCorners[] = {
		LEFT_DRAW_BOUND,		 UPPER_DRAW_BOUND,			// 0
		LEFT_DRAW_BOUND + width, UPPER_DRAW_BOUND,			// 1
		LEFT_DRAW_BOUND + width, UPPER_DRAW_BOUND + height, // 2
		LEFT_DRAW_BOUND,		 UPPER_DRAW_BOUND + height	// 3
	};

	unsigned int drawOrder[] = {0, 1, 2, 2, 3, 0};
	indexBuffer = new IndexBuffer(drawOrder, 6);

	drawArea = new VertexBuffer(drawCorners, 4 * 2 * sizeof(float));

	// FIXME: I think this is a memory leak
	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->Push<float>(2);

	drawLayout = new VertexArray();
	drawLayout->addBuffer(*drawArea, *layout);
}

DrawLayer::~DrawLayer() {
	delete drawArea;
	delete drawLayout;
	delete indexBuffer;
}

void DrawLayer::OnUpdate() {
	renderer.Draw(*drawLayout, *indexBuffer, shader);
}
