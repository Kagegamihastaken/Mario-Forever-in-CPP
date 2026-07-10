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
#include <thread>

#include "Core/Input.hpp"
#include "Core/Profiler.hpp"
float alpha = 1.0f;
// TODO: Implement DEBUG in Engine
// TODO: ImGUI for better debug
#if DEVELOPMENT_BUILD
int main() {
#else
int WinMain() {
#endif
	MFCPP::Handler::HandlerInit();
	CPPTRACE_TRY {
#if DEVELOPMENT_BUILD
		MFCPP::Log::InfoPrint("Current build in Development Mode");
#else
		MFCPP::Log::InfoPrint("Current build in Release Mode");
#endif
		MFCPP::Log::InfoPrint(fmt::format("Version: {}", PROJECT_VERSION));
		AudioEngine::Init();
		SoundManager::SoundManagerInit();
		MFCPP::IO::Init();
		Game::Init();
		if (!ImGui::SFML::Init(WindowFrame::getWindow())) throw std::runtime_error("Cannot Load ImGui");
		MusicManager::ForceLoadMusic(MusicID::MARIO_DEATH);
		MusicManager::ForceLoadMusic(MusicID::LEVEL_COMPLETE);

		AudioEngine::SoundManagerParamsInit();
		//ImGui Things
		sf::Clock deltaClock;
		//hard coded thread
		std::thread inputThread(MFCPP::Input::inputHandling);

		while (WindowFrame::getWindow().isOpen()) {
			FrameMark;
			while (const std::optional event = WindowFrame::getWindow().pollEvent()) {
				ImGui::SFML::ProcessEvent(WindowFrame::getWindow(), *event);
				if (event->is<sf::Event::Closed>()) {
					MFCPP::Log::InfoPrint("Closing Game...");
					WindowFrame::running = false;
					WindowFrame::getWindow().close();
				}
				Game::RetrieveEvent(event);
				WindowFrame::Window::WindowEventUpdate(event);
			}
			Game::EditText();
			WindowFrame::getFpsLite().update();
			WindowFrame::getTimestep().addFrame();
			while (WindowFrame::getTimestep().isUpdateRequired()) {
				Game::SetPrev();
				Game::DeltaMovement(WindowFrame::getTimestep().getStepAsFloat() * 50);
				Game::Collision();
				Mario::InvincibleStateUpdate();
			}
			alpha = (MFCPP::Interpolation::isInterpolation ? WindowFrame::getTimestep().getInterpolationAlphaAsFloat() : 1.f);
			Game::InterpolateMovement(alpha);
			Game::Cleanup();
			Game::MiscUpdate();
			//ImGui::SFML::Update(WindowFrame::getWindow(), deltaClock.restart());
			WindowFrame::getWindow().clear(sf::Color::Black);
			Game::Draw(alpha);
			//ImGui::ShowDemoWindow();
			//ImGui::SFML::Render(WindowFrame::getWindow());
			WindowFrame::getWindow().display();
		}
		if (inputThread.joinable()) {
			inputThread.join();
		}
	} CPPTRACE_CATCH (std::exception& e) {
		MFCPP::Log::ExceptionPrint(&e);
		MFCPP::Log::printCurrentTrace();
		std::exit(EXIT_FAILURE);
	}
	MFCPP::IO::Deinit();
	AudioEngine::DeInit();
	SoundManager::CleanUp();
	ImageManager::Cleanup();
	MusicManager::CleanUp();
	ImGui::SFML::Shutdown();
	std::exit(EXIT_SUCCESS);

}