#include "Core/Class/PiranhaAIClass.hpp"

namespace MFCPP {
	PiranhaAI::PiranhaAI(const PiranhaID id, const PiranhaDirection dir, const float speed, const float stop_time, const unsigned fire_count, const float fire_interval, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin)
	: m_ID(id), m_direction(dir), m_hitbox(hitbox), m_speed(speed), m_stop_time(stop_time), m_fire_count(fire_count), m_fire_interval(fire_interval) {
		m_Disabled = true;
		switch (dir) {
			case PIRANHA_UP:
				setCurrentPosition(pos + sf::Vector2f(0.f, 64.f));
				break;
			case PIRANHA_DOWN:
				setCurrentPosition(pos - sf::Vector2f(0.f, 64.f));
				break;
			case PIRANHA_RIGHT:
				setCurrentPosition(pos + sf::Vector2f(64.f, 0.f));
				break;
			case PIRANHA_LEFT:
				setCurrentPosition(pos - sf::Vector2f(64.f, 0.f));
				break;
			default: ;
		}
		setPreviousPosition(getCurrentPosition());
		setInterpolatedPosition(getCurrentPosition());
		setOrigin(origin);

		m_position_limit = 64.f;
		m_position_temporary = 64.f;
		m_state = true;
		m_stop = false;
		m_distance_appear = 80.f;
		m_fire_counting = 0;
		m_fire_ticking = 0.f;
	}
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
	PiranhaDirection PiranhaAI::getDirection() const {
		return m_direction;
	}
	void PiranhaAI::setFireCount(const unsigned val) {
		m_fire_count = val;
	}
	unsigned PiranhaAI::getFireCount() const {
		return m_fire_count;
	}
	void PiranhaAI::setFireCounting(const unsigned val) {
		m_fire_counting = val;
	}
	unsigned PiranhaAI::getFireCounting() const {
		return m_fire_counting;
	}
	void PiranhaAI::setFireInterval(const float val) {
		m_fire_interval = val;
	}
	float PiranhaAI::getFireInterval() const {
		return m_fire_interval;
	}
	void PiranhaAI::setFireTicking(const float val) {
		m_fire_ticking = val;
	}
	float PiranhaAI::getFireTicking() const {
		return m_fire_ticking;
	}
}