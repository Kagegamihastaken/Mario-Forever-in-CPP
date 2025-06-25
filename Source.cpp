#include "Core/WindowFrame.hpp"
#include "Core/Sound.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/ExitGate.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/ExternalHeaders/Kairos.hpp"
#include "Object/Mario.hpp"
#include "Core/Game.hpp"
#include "config.h"
//#include "imgui.h"
//#include "imgui-SFML.h"
float alpha = 1.0f;
//kairos::Timestep render;
// TODO: Implement DEBUG in Engine
// TODO: ImGUI for better debug
#if DEVELOPMENT_BUILD
int WinMain() {
#else
int main() {
#endif
#if DEVELOPMENT_BUILD
	std::cout << "RELEASE!" << "\n";
#else
	std::cout << "DEVELOP!" << "\n";
#endif
	IOInit();
	GameObjectInit();
	//ImGui::SFML::Init(window);
	//sf::Clock deltaClock;
	GameTextInit();
	GameLoadLevel();

	sf::RectangleShape test(sf::Vector2f(32.0f, 32.0f));

	ImageManager::AddImage("TempTexImage", "data/resources/testImage.png");
	ImageManager::CreateTestImage("TempTexImage", "TempTexImageFixed");
	ImageManager::AddTexture("TempTexImageFixed", "TempTexTexture");

	tempTex.setSmooth(true);
	test.setTexture(ImageManager::GetReturnTexture("TempTexTexture"));

	test.setTextureRect(sf::IntRect({1, 1}, {32, 32}));

	//for (int i = 0; i <= 6; ++i) {
	//	AddSlope(128.0f + static_cast<float>(i) * 32.0f, 384.0f - static_cast<float>(i) * 32.0f);
	//}
	//render.setStep(1.0f / 300.0f);

	while (window.isOpen()) {
		bool Updated = false;
		while (const std::optional event = window.pollEvent()) {
			//ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				SoundManager::ClearUp();
				window.close();
				IODeinit();
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::Q) {
					Updated = true;
				}
			}
			GameObjectRetrieveEvent(event);
		}
		if (ExitGateClock.getElapsedTime().asSeconds() > 8.5f && !EffectActive) {
			ExitGateClock.reset();
			SoundManager::ClearUp();
			window.close();
			IODeinit();
		}
		GameObjectEditorUpdate();
		GameObjectEditText();
		fpsLite.update();
		timestep.addFrame();
		//if (Updated) {
		while (timestep.isUpdateRequired()) {
			GameObjectSetPrev();
			GameObjectDeltaMovement(timestep.getStepAsFloat() * 50.0f);
			GameObjectCollision();
			InvincibleStateUpdate();
		}
		if (isInterpolation) alpha = timestep.getInterpolationAlphaAsFloat();
		else alpha = 1.0f;
		GameObjectInterpolateMovement(alpha);
		GameObjectAnimation();

		test.setPosition(player.property.getPosition());
		//ImGui::SFML::Update(window, deltaClock.restart());
		//ImGui::ShowDemoWindow();

		//ImGui::Begin("Hello, world!");
		//ImGui::Text("This is some useful text.");

		//ImGui::Button("Look at this pretty button");
		//ImGui::End();
		GameObjectMiscUpdate();
		GameCleanUp();
		//draw
		//rObject.clear();
		//rObject.display();
		//sf::Sprite Renderer(rObject.getTexture());
		//render.addFrame();
		window.clear();
		GameObjectDraw();
		//window.draw(test);
		//if (render.isUpdateRequired()) {
		//	window.clear();
		//	GameObjectDraw();
		//}
		//window.draw(Renderer);
		//ImGui::SFML::Render(window);
		window.display();
	}
	//ImGui::SFML::Shutdown();
}