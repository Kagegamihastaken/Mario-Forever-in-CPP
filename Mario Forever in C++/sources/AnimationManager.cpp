#include "../headers/Core/Animate/AnimationManager.hpp"

// Map definitions
std::map<std::string, sf::Clock> AnimationManager::m_timeRun;
std::map<std::string, sf::Texture*> AnimationManager::m_textures;
std::map<std::string, sf::Vector2i> AnimationManager::m_indicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_startingIndicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_endingIndicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_sheetSizes;
std::map<std::string, sf::Vector2i> AnimationManager::m_spriteSizes;
std::map<std::string, int> AnimationManager::m_frequencies;

/*
update(string animation, Sprite& sprite)

animation - This is the string key that corresponds to the animation we are updating

sprite - This sprite will have its texture changed to the next one in its
  animation sequence
*/
void AnimationManager::update(std::string animation, sf::Sprite& sprite) {
	// First, we want to locate our animation in the map
	// We look at the sheet size because that is the easiest indicator to tell if
	// there is actually an entry
	if (m_sheetSizes[animation] != sf::Vector2i(0, 0)) {
		// We want to do a few calculations to find the coordinates of the next frame

		sf::IntRect rect(m_indicies[animation].x * m_spriteSizes[animation].x + m_indicies[animation].x,
			m_indicies[animation].y * m_spriteSizes[animation].y + m_indicies[animation].y,
			m_spriteSizes[animation].x, m_spriteSizes[animation].y);

		sprite.setTexture(*m_textures[animation]);
		sprite.setTextureRect(rect);
		// Now we want to update the indicies based on the format of our sheet
		// If we are not at the bottom of a column, we just move down one in y
		if (m_frequencies[animation] != 0 && m_timeRun[animation].getElapsedTime().asMilliseconds() >= 3000.0f / m_frequencies[animation]) {
			m_timeRun[animation].restart().asMilliseconds();
			if (m_indicies[animation].y < m_endingIndicies[animation].y) {
				m_indicies[animation].y++;
			}
			else {
				// Otherwise, we move over one column and go back to the top
				m_indicies[animation].y = m_startingIndicies[animation].y;
				m_indicies[animation].x++;
				// And then reset the sheet if we are past the width of the sheet
				if (m_indicies[animation].x >= m_endingIndicies[animation].x)
					m_indicies[animation].x = m_startingIndicies[animation].x;
			}

			// Now we update the texture on our sprite reference
		}
	}
	else {
		// If we didn't find an entry
		std::cout << "No animation entry found for \"" << animation << "\"!" << "\n";
	}
}

/*
updateAll(map<string, Sprite> &map)

map - This should be a map of string and sprite pairs, all of which that are found
  in the local map will be updated

This is essentially just an extension of the previous method, in that it just
iterates through
*/
void AnimationManager::updateAll(std::map<std::string, sf::Sprite>& map) {
	// Really not much to do here, most is in the previous method
	for (auto element : map) {
		update(element.first, element.second);
	}
}

/*
ADDING & DELETING
The following methods are for adding and deleting entries and animations
*/

/*
addAnimation(string animation, Texture texture)

animation - The string key for the animation we are adding
texture - The spritesheet that will be associated with our animation
sheetSize - The vector containing the number of animation frames in our image
  - NOTE: all sheets must be rectangular
*/
void AnimationManager::addAnimation(std::string animation, sf::Texture* texture,
	sf::Vector2i sheetSize, sf::Vector2i spriteSize, sf::Vector2i index, int frequency,
	sf::Vector2i startingIndex, sf::Vector2i endingIndex) {
	// First, we want to make an entry in the texture map
	m_timeRun[animation] = sf::Clock();
	m_timeRun[animation].restart().asMilliseconds();
	m_textures[animation] = texture;
	// Next, we make sheet size entry
	m_sheetSizes[animation].x = sheetSize.x;
	m_sheetSizes[animation].y = sheetSize.y;

	// Next, we make sprite size entry
	m_spriteSizes[animation].x = spriteSize.x;
	m_spriteSizes[animation].y = spriteSize.y;

	// Our index vector
	m_indicies[animation].x = index.x;
	m_indicies[animation].y = index.y;

	// Our starting index vector
	m_startingIndicies[animation].x = startingIndex.x;
	m_startingIndicies[animation].y = startingIndex.y;

	// Our ending index vector
	if (endingIndex != sf::Vector2i(0, 0)) {
		m_endingIndicies[animation].x = endingIndex.x;
		m_endingIndicies[animation].y = endingIndex.y;
	}
	else {
		m_endingIndicies[animation].x = sheetSize.x;
		m_endingIndicies[animation].y = sheetSize.y;
	}

	// Our update rate (frequency)
	m_frequencies[animation] = frequency;
	// We don't need to initialize the times udpated because its default is 0
}

/*
deleteAnimation(string animation)

animation - The string key for the animation set we are removing

Essentially this method just removes all entries for the animation provided
If this is not called, there won't be a functional difference, but it *may*
speed up the time it takes to run certain methods
*/

void AnimationManager::deleteAnimation(std::string animation) {
	// We just erase each entry in every map
	m_textures.erase(animation);
	m_indicies.erase(animation);
	m_startingIndicies.erase(animation);
	m_sheetSizes.erase(animation);
	m_spriteSizes.erase(animation);
	m_frequencies.erase(animation);
	m_endingIndicies.erase(animation);
	m_timeRun.erase(animation);
	// Ez pz
}

/*
Here are a whole bunch of setters for any given animation instance
Nothing too special about any of them in particular, and they all follow the
same form
*/

void AnimationManager::setAnimationFrequency(std::string animation, int frequency) {
	m_frequencies[animation] = frequency;
}

void AnimationManager::setAnimationIndex(std::string animation, sf::Vector2i index) {
	m_indicies[animation].x = index.x;
	m_indicies[animation].y = index.y;
}

void AnimationManager::setAnimationSheetSize(std::string animation, sf::Vector2i size) {
	m_sheetSizes[animation].x = size.x;
	m_sheetSizes[animation].y = size.y;
}

void AnimationManager::setAnimationSpriteSize(std::string animation, sf::Vector2i size) {
	m_spriteSizes[animation].x = size.x;
	m_spriteSizes[animation].y = size.y;
}

void AnimationManager::setAnimationTexture(std::string animation, sf::Texture* texture) {
	m_textures[animation] = texture;
}

void AnimationManager::resetAnimationIndex(std::string animation) {
	m_timeRun[animation].restart().asMilliseconds();
	m_indicies[animation].x = m_startingIndicies[animation].x;
	m_indicies[animation].y = m_startingIndicies[animation].y;
}

void AnimationManager::setAnimationStartingIndex(std::string animation, sf::Vector2i index) {
	m_startingIndicies[animation].x = index.x;
	m_startingIndicies[animation].y = index.y;
}

void AnimationManager::setAnimationEndingIndex(std::string animation, sf::Vector2i index) {
	m_endingIndicies[animation].x = index.x;
	m_endingIndicies[animation].y = index.y;
}

bool AnimationManager::isReachedEnd(std::string animation) {
	if (m_indicies[animation].x == m_endingIndicies[animation].x - 1) {
		if (m_indicies[animation].y == m_endingIndicies[animation].y) {
			return true;
		}
	}
	return false;
}
bool AnimationManager::isBegin(std::string animation) {
	if (m_indicies[animation].x == m_startingIndicies[animation].x) {
		if (m_indicies[animation].y == m_startingIndicies[animation].y) {
			return true;
		}
	}
	return false;
}