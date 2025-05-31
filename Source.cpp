//include
#include <SFML/Graphics.hpp>
#include "headers/Core/WindowFrame.hpp"
#include "headers/Object/Coin.hpp"
#include "headers/Core/Sound.hpp"
#include "headers/Effect/MarioEffect.hpp"
#include "headers/Object/ExitGate.hpp"
#include "headers/Core/Interpolation.hpp"
#include "headers/Core/Loading/Loading.hpp"
#include "headers/Core/ExternalHeaders/Kairos.hpp"

#include "Object/GoombaAI.hpp"

#include "headers/Editor/Editor.hpp"

#include "Core/Hash.hpp"
#include "Core/Game.hpp"
#include "config.h"
//#include "imgui.h"
//#include "imgui-SFML.h"
#include <iostream>
float alpha = 1.0f;
// TODO: Implement DEBUG in Engine
// TODO: ImGUI for better debug
// TODO: Implement REGEX;
#ifdef DEVELOPMENT_BUILD
int main() {
#else
int WinMain() {
#endif
#ifdef DEVELOPMENT_BUILD
	std::cout << "DEVELOP!" << "\n";
#else
	std::cout << "RELEASE!" << "\n";
#endif
	IOInit();
	GameObjectInit();
	//ImGui::SFML::Init(window);
	//sf::Clock deltaClock;
	GameTextInit();
	GameLoadLevel();
	std::string fall;
	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			//ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				SoundManager::ClearUp();
				window.close();
				IODeinit();
			}
			else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left) {
					AddGoombaAI(GoombaAIType::MUSHROOM, 0, MouseX + view.getCenter().x - 320.0f, MouseY + view.getCenter().y - 240.0f, GoombaAIDirection::LEFT);
				}
			}
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
		while (timestep.isUpdateRequired()) {
			GameObjectSetPrev();
			GameObjectDeltaMovement(timestep.getStepAsFloat() * 50.0f);
			GameObjectCollisionNAnimation();
		}

		if (isInterpolation) alpha = timestep.getInterpolationAlphaAsFloat();
		else alpha = 1.0f;
		GameObjectInterpolateMovement(alpha);
		//ImGui::SFML::Update(window, deltaClock.restart());
		//ImGui::ShowDemoWindow();

		//ImGui::Begin("Hello, world!");
		//ImGui::Text("This is some useful text.");

		//ImGui::Button("Look at this pretty button");
		//ImGui::End();
		GameObjectMiscUpdate();
		//draw
		//rObject.clear();
		//rObject.display();
		//sf::Sprite Renderer(rObject.getTexture());
		window.clear();
		GameObjectDraw();
		//window.draw(Renderer);
		//ImGui::SFML::Render(window);
		window.display();
	}
	//ImGui::SFML::Shutdown();
}