#include "Core/Background/BgGradient.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Level.hpp"
#include "Core/Profiler.hpp"

// NOTE: Cannot change to auto, will found way to work
sf::VertexArray bgGradient(sf::PrimitiveType::TriangleStrip, 4);
void BgGradientSetColor(const sf::Color& firstC, const sf::Color& secondC) {
	bgGradient[0].color = bgGradient[1].color = firstC;
	bgGradient[2].color = bgGradient[3].color = secondC;
}
void BgGradientInitPos(const float Width, const float Height) {
	bgGradient[0].position = sf::Vector2f(-32.f, -32.f);
	bgGradient[1].position = sf::Vector2f(Width + 32.f, -32.f);
	bgGradient[2].position = sf::Vector2f(-32.f, Height + 32.f);
	bgGradient[3].position = sf::Vector2f(Width + 32.f, Height + 32.f);
}
void BgGradientDraw() {
	ZoneScopedNC("BgGradientDraw", 0x1EAEE3);
	WindowFrame::getWindow().draw(bgGradient);
}