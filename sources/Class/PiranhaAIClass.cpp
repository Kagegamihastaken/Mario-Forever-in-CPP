#include "Class/PiranhaAIClass.hpp"
namespace MFCPP {
	void PiranhaAI::setID(const PiranhaID ID) {
		m_ID = ID;
	}
	PiranhaID PiranhaAI::getID() const {
		return m_ID;
	}
	void PiranhaAI::setHitbox(const sf::FloatRect &hitbox) {
		m_hitbox = hitbox;
	}
	sf::FloatRect PiranhaAI::getHitbox() const {
		return m_hitbox;
	}
	void PiranhaAI::setDisabled(const bool disabled) {
		m_Disabled = disabled;
	}
	bool PiranhaAI::isDisabled() const {
		return m_Disabled;
	}
	void PiranhaAI::setSpeed(const float speed) {
		m_speed = speed;
	}
	float PiranhaAI::getSpeed() const {
		return m_speed;
	}
	void PiranhaAI::setPositionLimit(const float position_limit) {
		m_position_limit = position_limit;
	}
	float PiranhaAI::getPositionLimit() const {
		return m_position_limit;
	}
	void PiranhaAI::setPositionTemporary(const float position_temporary) {
		m_position_temporary = position_temporary;
	}
	float PiranhaAI::getPositionTemporary() const {
		return m_position_temporary;
	}
	void PiranhaAI::setState(const bool state) {
		m_state = state;
	}
	bool PiranhaAI::getState() const {
		return m_state;
	}
	void PiranhaAI::setStopTime(const float stop_time) {
		m_stop_time = stop_time;
	}
	float PiranhaAI::getStopTime() const {
		return m_stop_time;
	}
	void PiranhaAI::restartStopClock() {
		m_stop_clock.restart();
	}
	sf::Clock PiranhaAI::getStopClock() const {
		return m_stop_clock;
	}
	void PiranhaAI::setStop(const bool stop) {
		m_stop = stop;
	}
	bool PiranhaAI::getStop() const {
		return m_stop;
	}
	void PiranhaAI::setDistanceAppear(const float distance_appear) {
		m_distance_appear = distance_appear;
	}
	float PiranhaAI::getDistanceAppear() const {
		return m_distance_appear;
	}
}