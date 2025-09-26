#include "Core/WindowFrame.hpp"
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
#include "Core/MusicManager.hpp"
#include "Core/SoundManager.hpp"

#include "Object/Platform.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

class TestValue final {
public:
	TestValue() = default;
	~TestValue() = default;
	void AddValue() {
		++value;
	}
	void SubValue() {
		--value;
	}
	[[nodiscard]] int GetValue() const {
		return value;
	}
private:
	int value = 0;
};

float alpha = 1.0f;
TestValue tvalue;
// TODO: Implement DEBUG in Engine
// TODO: ImGUI for better debug

// TODO: Add encrypt protection for value can be easily cheat
/*
 * To archive this, I need to use encryption library
 * Then create a static class called "Protected Value"
 * This class can took "int", "float", "string", etc. With std::variant
 * Create a std::map with REAL value to actually store variable
 * Then create another std::map to "queue" value to store that REAL value (this map will use encrypted)
 * But this will guarantee hurt fps if check every value EVERY FRAME.
 * To Prevent this, It just needs to check value between 5 seconds to not put stress to CPU
 * And tada, I now have a very own "Protected Value"
 *
 * Why build this? Because I was aware how easily cheat a CTF Game, it just flip value from positive to negative and so on.
*/
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
		if (!ImGui::SFML::Init(window)) throw std::runtime_error("Cannot Load ImGui");
		sf::Clock deltaClock;
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
		//render.setStep(1.0f / 300.0f);
		//AddPlatform(sf::Vector2f(0.f, 352.f), sf::Vector2f(192.f - 32.f, 352.f - 96.f));
		AudioEnginePlay();
		while (window.isOpen()) {
			while (const std::optional event = window.pollEvent()) {
				ImGui::SFML::ProcessEvent(window, *event);
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
					}
					else if (keyPressed->code == sf::Keyboard::Key::S) {
						tvalue.AddValue();
					}
					else if (keyPressed->code == sf::Keyboard::Key::A) {
						tvalue.SubValue();
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
				GameObjectRetrieveEvent(event);
			}
			//MFCPP::Log::SuccessPrint(fmt::format("{}", tvalue.GetValue()));
			if (ExitGateClock.getElapsedTime().asSeconds() > 8.5f && !EffectActive) {
				ExitGateClock.reset();
				window.close();
			}
			GameObjectEditText();
			MarioOutSideScreen();
			fpsLite.update();
			timestep.addFrame();
			while (timestep.isUpdateRequired()) {
				GameObjectSetPrev();
				GameObjectDeltaMovement(timestep.getStepAsFloat() * 50.0f);
				GameObjectCollision();
				InvincibleStateUpdate();
			}
			if (isInterpolation) alpha = timestep.getInterpolationAlphaAsFloat();
			else alpha = 1.0f;
			GameObjectInterpolateMovement(alpha);
			//ImGui::ShowDemoWindow();
			GameObjectMiscUpdate();
			GameCleanUp();

			ImGui::SFML::Update(window, deltaClock.restart());
			//draw
			window.clear(sf::Color::Transparent);
			GameObjectDraw();
			ImGui::SFML::Render(window);
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
	MusicManager::CleanUp();
	ImGui::SFML::Shutdown();
	std::exit(EXIT_SUCCESS);

}