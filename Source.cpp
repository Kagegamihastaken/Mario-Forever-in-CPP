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
#include "Core/Logging.hpp"
#include "Core/Loading/Handler.hpp"
#include <cpptrace/from_current.hpp>
#include "Core/AudioEngine.hpp"
#include "Core/Level.hpp"
#include "Core/MusicManager.hpp"
#include "Core/SoundManager.hpp"

#include "Block/BulletLauncher.hpp"
//#include "imgui.h"
//#include "imgui-SFML.h"
float alpha = 1.0f;
//kairos::Timestep render;
// TODO: Implement DEBUG in Engine
// TODO: ImGUI for better debug
#if DEVELOPMENT_BUILD
int main() {
#else
int WinMain() {
#endif
	MFCPP::HandlerInit();
	CPPTRACE_TRY {
#if DEVELOPMENT_BUILD
		MFCPP::Log::InfoPrint("Current build in Development Mode");
#else
		MFCPP::Log::InfoPrint("Current build in Release Mode");
#endif
		MFCPP::Log::InfoPrint(fmt::format("Version: {}", PROJECT_VERSION));
		AudioEngineInit();
		SoundManager::SoundManagerInit();
		IOInit();
		GameObjectInit();
		//ImGui::SFML::Init(window);
		//sf::Clock deltaClock;
		GameTextInit();
		GameLoadLevel();
		MusicManager::ForceLoadMusic("MarioDeath");
		MusicManager::ForceLoadMusic("LevelComplete");

		//MFCPP::SimpleSprite test;

		//sf::RectangleShape test(sf::Vector2f(32.f, 32.f));
		//test.setPointCount(4);
		//test.setPoint(0, {0.f, 0.f});
		//test.setPoint(1, {32.f, 0.f});
		//test.setPoint(2, {32.f, 32.f});
		//test.setPoint(3, {0.f, 32.f});
		//sf::RectangleShape test(sf::Vector2f(32.0f, 32.0f));

		//ImageManager::AddImage("TempTexImage", "data/resources/testImage.png");
		//ImageManager::CreateTestImage("TempTexImage", "TempTexImageFixed");
		//ImageManager::AddTexture("TempTexImage", "TempTexTexture");

		//test.setTexture(ImageManager::GetReturnTexture("TempTexTexture"), true);
		//test.setTextureRect(sf::IntRect({1, 1}, {32, 32}));

		//for (int i = 0; i <= 6; ++i) {
		//	AddSlope(128.0f + static_cast<float>(i) * 32.0f, 384.0f - static_cast<float>(i) * 32.0f);
		//}
		//render.setStep(1.0f / 300.0f);
		AudioEnginePlay();

		AddBulletLauncher(BULLET_NORMAL, 256.f + 32.f, 128.f);
		while (window.isOpen()) {
			//MFCPP::Log::InfoPrint(fmt::format("Active Voice Count: {}",audio_engine.getActiveVoiceCount()));
			//bool Updated = false;
			while (const std::optional event = window.pollEvent()) {
				//ImGui::SFML::ProcessEvent(window, *event);
				if (event->is<sf::Event::Closed>()) {
					MFCPP::Log::InfoPrint("Closing...");
					window.close();
				}
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Q) {
						MFCPP::Log::InfoPrint("Pressed Q");
						if (SoundManager::GetEnvironment() == OVERWORLD) {
							SoundManager::SetEnvironment(UNDERGROUND);
						}
						else {
							SoundManager::SetEnvironment(OVERWORLD);
						}
						//Updated = true;
					}
					else if (keyPressed->code == sf::Keyboard::Key::P) {
						MusicManager::PauseMusic(getMusicLevelName());
					}
					else if (keyPressed->code == sf::Keyboard::Key::L) {
						MusicManager::PlayMusic(getMusicLevelName());
					}
				}
				GameObjectRetrieveEvent(event);
			}
			if (ExitGateClock.getElapsedTime().asSeconds() > 8.5f && !EffectActive) {
				ExitGateClock.reset();
				window.close();
			}
			GameObjectEditorUpdate();
			GameObjectEditText();
			MarioOutSideScreen();
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
			//test.setPosition(player.property.getPosition());
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
			//render.addFrame()
			window.clear(sf::Color::Transparent);
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
	} CPPTRACE_CATCH (std::exception& e) {
		MFCPP::Log::ExceptionPrint(&e);
		MFCPP::Log::printCurrentTrace();
		std::exit(EXIT_FAILURE);
	}
	IODeinit();
	AudioEngineDeInit();
	SoundManager::CleanUp();
	ImageManager::Cleanup();
	std::exit(EXIT_SUCCESS);
	//ImGui::SFML::Shutdown();
}