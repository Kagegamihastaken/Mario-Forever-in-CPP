#ifndef BRICKPARTICLE_HPP
#define BRICKPARTICLE_HPP

extern std::vector<sf::Sprite> BrickParticleList;
extern void AddBrickParticle(BrickID id, BrickParticleDirection direction, float x, float y, float vx, float vy);
extern void DeleteBrickParticle(float x, float y);
extern void BrickParticleStatusUpdate();
extern void BrickParticleUpdate();

#endif // BRICKPARTICLE_HPP