//include
#include <SFML/Graphics.hpp>
#include "headers/Core/WindowFrame.hpp"
#include "headers/Object/Coin.hpp"
#include "headers/Core/Sound.hpp"
#include "headers/Effect/MarioEffect.hpp"
#include "headers/Object/ExitGate.hpp"
#include "headers/Core/Interpolation.hpp"
#include "headers/Core/Loading/Loading.hpp"
#include "headers/Core/Hash.hpp"

#include "headers/Editor/RenderTile.hpp"

#include "headers/Core/ExternalHeaders/Kairos.hpp"
#include "Core/Game.hpp"
#include "config.h"
#include <unordered_set>
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

	/*std::unordered_set<RenderTile, RenderTileHash, RenderTileEqual> sfFloatSet;
	sfFloatSet.insert(RenderTile(*TextureManager::GetTexture("Tileset"), sf::Vector2f({128, 128}), sf::IntRect({{0, 0}, {32, 32}})));
	sfFloatSet.insert(RenderTile(*TextureManager::GetTexture("Tileset"), sf::Vector2f({160, 128}), sf::IntRect({{32, 0}, {32, 32}})));
	sfFloatSet.insert(RenderTile(*TextureManager::GetTexture("Tileset"), sf::Vector2f({192, 128}), sf::IntRect({{64, 0}, {32, 32}})));*/
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
			/*else if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (keyPressed->button == sf::Mouse::Button::Right)
					if (sfFloatSet.contains(sf::Vector2f(std::floor(MouseX / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f))) {
						std::cout << "Found!\n";
						sfFloatSet.erase(sf::Vector2f(std::floor(MouseX / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f));
					}
					else std::cout << "Not Found!\n";
				if (keyPressed->button == sf::Mouse::Button::Left) {
					if (!sfFloatSet.contains(sf::Vector2f(std::floor(MouseX / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f))) {
						std::cout << "Placed\n";
						sfFloatSet.insert(RenderTile(*TextureManager::GetTexture("Tileset"), sf::Vector2f({std::floor(MouseX / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f}), sf::IntRect({{96, 0}, {32, 32}})));
					}
					else std::cout << "Already Placed\n";
				}
			}*/
		}
		if (ExitGateClock.getElapsedTime().asSeconds() > 8.5f && !EffectActive) {
			ExitGateClock.reset();
			SoundManager::ClearUp();
			window.close();
			IODeinit();
		}
		GameObjectEditText();
		fpsLite.update();
		timestep.addFrame();
		//timestep.setTimeSpeed(0.5f);
		while (timestep.isUpdateRequired()) {
			const float dt{ timestep.getStepAsFloat() * 50.0f };
			GameObjectSetPrev();
			GameObjectDeltaMovement(dt);
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
		GameObjectCollisionNAnimation();
		GameObjectMiscUpdate();
		//draw
		rObject.clear();
		/*for (const auto &i : sfFloatSet) {
			rObject.draw(i);
		}*/
		GameObjectDraw();
		rObject.display();
		sf::Sprite Renderer(rObject.getTexture());
		window.clear();
		window.draw(Renderer);
		//ImGui::SFML::Render(window);
		window.display();
	}
	//ImGui::SFML::Shutdown();
}