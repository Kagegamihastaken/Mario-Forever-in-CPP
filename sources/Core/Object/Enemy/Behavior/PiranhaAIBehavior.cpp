#include "Core/Object/Enemy/Behavior/PiranhaAIBehavior.hpp"

#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"

sf::Vector2f PiranhaAIBehavior::PiranhaPositionAdjust(PiranhaDirection dir, const sf::Vector2f& pos) {
    sf::Vector2f output = pos;
    switch (dir) {
        case PIRANHA_UP:
            output += sf::Vector2f(0.f, 64.f);
            break;
        case PIRANHA_DOWN:
            output -= sf::Vector2f(0.f, 64.f);
            break;
        case PIRANHA_RIGHT:
            output += sf::Vector2f(64.f, 0.f);
            break;
        case PIRANHA_LEFT:
            output -= sf::Vector2f(64.f, 0.f);
            break;
        default: ;
    }
    return output;
}
PiranhaAIBehavior::PiranhaAIData PiranhaAIBehavior::PiranhaFireUpdate(bool& flag, PiranhaAIData data, const float deltaTime) {
	PiranhaAIData dataOutput = data;
	flag = false;
	if (dataOutput.fire_counting > 0) {
		if (dataOutput.fire_ticking < dataOutput.fire_interval)
			dataOutput.fire_ticking += deltaTime;
		else {
			flag = true;
			dataOutput.fire_ticking = 0.f;
			dataOutput.fire_counting--;
		}
	}
	return dataOutput;
}
PiranhaAIBehavior::PiranhaAIData PiranhaAIBehavior::PiranhaMovementUpdate(PiranhaAIData data, PiranhaDirection dir, const float deltaTime) {
	PiranhaAIData dataOutput = data;
	if (!dataOutput.stop) {
		if (!dataOutput.state) {
			if (dataOutput.pos_temp <= dataOutput.pos_limit) {
				switch (dir) {
					case PIRANHA_UP:
						dataOutput.pos.y += dataOutput.speed * deltaTime;
						break;
					case PIRANHA_DOWN:
						dataOutput.pos.y -= dataOutput.speed * deltaTime;
						break;
					case PIRANHA_RIGHT:
						dataOutput.pos.x += dataOutput.speed * deltaTime;
						break;
					case PIRANHA_LEFT:
						dataOutput.pos.x -= dataOutput.speed * deltaTime;
						break;
					default: ;
				}
				dataOutput.pos_temp += dataOutput.speed * deltaTime;
			}
			else {
				dataOutput.state = true;
				//Set back to original
				dataOutput.pos += sf::Vector2f((dataOutput.pos_limit - dataOutput.pos_temp) * static_cast<float>(dir == PIRANHA_RIGHT || dir == PIRANHA_LEFT), (dataOutput.pos_limit - dataOutput.pos_temp) * static_cast<float>(dir == PIRANHA_UP || dir == PIRANHA_DOWN));
				dataOutput.pos_temp = dataOutput.pos_limit;
				dataOutput.stop = true;
				dataOutput.stop_clock = 0.f;
			}
		}
		else {
			if (dataOutput.pos_temp >= 0.0f) {
				switch (dir) {
					case PIRANHA_UP:
						dataOutput.pos.y -= dataOutput.speed * deltaTime;
						break;
					case PIRANHA_DOWN:
						dataOutput.pos.y += dataOutput.speed * deltaTime;
						break;
					case PIRANHA_RIGHT:
						dataOutput.pos.x -= dataOutput.speed * deltaTime;
						break;
					case PIRANHA_LEFT:
						dataOutput.pos.x += dataOutput.speed * deltaTime;
						break;
					default: ;
				}
				dataOutput.pos_temp -= dataOutput.speed * deltaTime;
			}
			else {
				dataOutput.state = false;
				//Set back to normal
				dataOutput.fire_counting = dataOutput.fire_count;
				dataOutput.pos -= sf::Vector2f(dataOutput.pos_temp * static_cast<float>(dir == PIRANHA_RIGHT || dir == PIRANHA_LEFT), dataOutput.pos_temp * static_cast<float>(dir == PIRANHA_UP || dir == PIRANHA_DOWN));
				dataOutput.pos_temp = 0.f;
				dataOutput.stop = true;
				dataOutput.stop_clock = 0.f;
			}
		}
	}
	else {
		if (dataOutput.stop_clock <= dataOutput.stop_time)
			dataOutput.stop_clock += deltaTime;
		if (dataOutput.stop_clock > dataOutput.stop_time && f_abs(player.curr.x - dataOutput.pos.x) > dataOutput.distance_appear && dataOutput.state) dataOutput.stop = false;
		else if (dataOutput.stop_clock > dataOutput.stop_time && !dataOutput.state) dataOutput.stop = false;
	}
	return dataOutput;
}