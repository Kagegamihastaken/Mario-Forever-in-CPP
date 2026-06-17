#include "Core/WindowFrame.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Loading/Loading.hpp"
#include "Object/Mario.hpp"
#include "Core/Game.hpp"
#include <config.h>
#include "Core/Logging.hpp"
#include "Core/Loading/Handler.hpp"
#include <cpptrace/from_current.hpp>
#include "Core/AudioEngine.hpp"
#include "Core/MusicManager.hpp"
#include "Core/SoundManager.hpp"
#include <imgui-SFML.h>
float alpha = 1.0f;
// TODO: Implement DEBUG in Engine
// TODO: ImGUI for better debug

// TODO: Add encrypt protection for value can be easily cheat
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
		AudioEngine::Init();
		SoundManager::SoundManagerInit();
		IOInit();
		Game::Init();
		if (!ImGui::SFML::Init(WindowFrame::getWindow())) throw std::runtime_error("Cannot Load ImGui");
		sf::Clock deltaClock;
		MusicManager::ForceLoadMusic("MarioDeath");
		MusicManager::ForceLoadMusic("LevelComplete");

		// MFCPP::SimpleSprite test;
		// ImageManager::AddTexture("TempTexImage", "data/resources/testImage.png");
		// test.setTexture(ImageManager::GetReturnTexture("TempTexImage"), true);
		// test.setTextureRect(sf::IntRect({16, 16}, {32, 32}));

		AudioEngine::SoundManagerParamsInit();
		while (WindowFrame::getWindow().isOpen()) {
			while (const std::optional event = WindowFrame::getWindow().pollEvent()) {
				ImGui::SFML::ProcessEvent(WindowFrame::getWindow(), *event);
				if (event->is<sf::Event::Closed>()) {
					MFCPP::Log::InfoPrint("Closing...");
					WindowFrame::getWindow().close();
				}
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Q) {
						// MFCPP::Log::InfoPrint("Pressed Q");
						// if (SoundManager::GetEnvironment() == OVERWORLD) {
						// 	SoundManager::SetEnvironment(UNDERGROUND);
						// }
						// else {
						// 	SoundManager::SetEnvironment(OVERWORLD);
						// }
					}
					/*
					else if (keyPressed->code == sf::Keyboard::Key::P) {
						MusicManager::PauseMusic(getMusicLevelName());
					}
					else if (keyPressed->code == sf::Keyboard::Key::L) {
						MusicManager::PlayMusic(getMusicLevelName());
					}
					*/
				}
				Game::RetrieveEvent(event);
			}
			//MFCPP::Log::SuccessPrint(fmt::format("{}", tvalue.GetValue()));
			ImageManager::ClearAllVertex();
			Game::EditText();
			WindowFrame::getFpsLite().update();
			WindowFrame::getTimestep().addFrame();
			while (WindowFrame::getTimestep().isUpdateRequired()) {
				Game::SetPrev();
				Game::DeltaMovement(WindowFrame::getTimestep().getStepAsFloat() * 50);
				Game::Collision();
				// test.setPosition(Mario::getCurrentPosition());
				Mario::InvincibleStateUpdate();
			}
			if (isInterpolation) alpha = WindowFrame::getTimestep().getInterpolationAlphaAsFloat();
			else alpha = 1.0f;
			Game::InterpolateMovement(alpha);
			//ImGui::ShowDemoWindow();
			Game::MiscUpdate();
			Game::Cleanup();

			ImGui::SFML::Update(WindowFrame::getWindow(), deltaClock.restart());
			//draw
			WindowFrame::getWindow().clear(sf::Color::Black);
			Game::Draw();
			ImGui::SFML::Render(WindowFrame::getWindow());
			// window.draw(test);
			WindowFrame::getWindow().display();
			//ImageManager::printDrawCount();
		}
	} CPPTRACE_CATCH (std::exception& e) {
		MFCPP::Log::ExceptionPrint(&e);
		MFCPP::Log::printCurrentTrace();
		std::exit(EXIT_FAILURE);
	}
	IODeinit();
	AudioEngine::DeInit();
	SoundManager::CleanUp();
	ImageManager::Cleanup();
	MusicManager::CleanUp();
	ImGui::SFML::Shutdown();
	std::exit(EXIT_SUCCESS);

}