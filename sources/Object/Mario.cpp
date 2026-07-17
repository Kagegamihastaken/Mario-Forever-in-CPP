#include "Object/Mario.hpp"
#include "Block/Brick.hpp"
#include "Core/AnimationSequenceManager.hpp"
#include "Core/AutoScroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Input.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Level.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/ExitGate.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Class/CollisionObjectClass.hpp"

#include "Core/Time.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Projectile/MarioBeetroot.hpp"
#include "Object/Projectile/MarioFireball.hpp"
#include "Core/Profiler.hpp"

sf::FloatRect Mario::m_hitboxFloor;
sf::FloatRect Mario::m_hitboxWall;

MFCPP::SingleAnimationObject Mario::m_MarioAnimation;
MFCPP::ActiveObject<float> Mario::m_player(sf::Vector2f(0.f, 0.f), sf::Vector2f(15.f, 61.f), sf::degrees(0.f));
sf::Vector2f Mario::m_velocity(0.f, 0.f);
bool Mario::m_FirstMarioDirection = false;
bool Mario::m_MarioDirection = Mario::m_FirstMarioDirection;
bool Mario::m_MarioCurrentFalling = true;
bool Mario::m_PreJump = false;
bool Mario::m_Holding;
bool Mario::m_MarioCrouchDown = false;
float Mario::m_player_speed;
int8_t Mario::m_MarioState = 0;
int8_t Mario::m_lastMarioState = -1;
int8_t Mario::m_PowerState = 0;
int8_t Mario::m_lastPowerState = 0;
uint16_t Mario::m_Lives = PLAYER_LIVES;
bool Mario::m_OverSpeed = false;
uint64_t Mario::m_Score = 0;
sf::Clock Mario::m_AppearingTimer;
sf::Clock Mario::m_InvincibleTimer;
bool Mario::m_Invincible = false;
bool Mario::m_InvincibleState = false;
bool Mario::m_MarioAppearing = false;
bool Mario::m_OutsideWallLeft = false;
bool Mario::m_OutsideWallRight = false;
float Mario::m_FireTime = 4.f;
float Mario::m_FireTimeCounting = m_FireTime;
bool Mario::m_isFireHolding = false;
bool Mario::m_CanControlMario = true;

//Storing for future
float Mario::m_temp_alpha = 0.f;

//texture loading
void Mario::UpdateSequenceAnimation() {
    switch (m_PowerState) {
        case 0:
            m_MarioAnimation.setAnimationSequence("SmallMario");
            break;
        case 1:
            m_MarioAnimation.setAnimationSequence("BigMario");
            break;
        case 2:
            m_MarioAnimation.setAnimationSequence("FireMario");
            break;
        case 3:
            m_MarioAnimation.setAnimationSequence("BeetrootMario");
            break;
        default: ;
    }
}

void Mario::MarioEffectActivate() {
    ActiveMarioEffect(m_temp_alpha);
    m_CanControlMario = false;
}

void Mario::SetPowerState(const int ps) {
    m_PowerState = ps;
    UpdateSequenceAnimation();
}

void Mario::loadMarioRes() {
    m_AppearingTimer.restart();
    // Resources Loader;
    m_MarioAnimation.setAnimation(0, 0, 0, true);
    MFCPP::AnimationSequenceManager::newData("SmallMario");
    MFCPP::AnimationSequenceManager::newData("BigMario");
    MFCPP::AnimationSequenceManager::newData("FireMario");
    MFCPP::AnimationSequenceManager::newData("BeetrootMario");
    for (int i = 0; i < MARIO_IMAGE_WIDTH / MARIO_WIDTH; ++i) {
        ImageManager::AddTexture(fmt::format("SmallMario_{}", i), "data/resources/SmallMario.png",
                                 sf::IntRect({MARIO_WIDTH * i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("SmallMario", fmt::format("SmallMario_{}", i));
        ImageManager::AddTexture(fmt::format("BigMario_{}", i), "data/resources/BigMario.png",
                                 sf::IntRect({MARIO_WIDTH * i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("BigMario", fmt::format("BigMario_{}", i));
        ImageManager::AddTexture(fmt::format("FireMario_{}", i), "data/resources/FireMario.png",
                                 sf::IntRect({MARIO_WIDTH * i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("FireMario", fmt::format("FireMario_{}", i));
        ImageManager::AddTexture(fmt::format("BeetrootMario_{}", i), "data/resources/BeetrootMario.png",
                                 sf::IntRect({MARIO_WIDTH * i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("BeetrootMario", fmt::format("BeetrootMario_{}", i));
    }
    UpdateSequenceAnimation();
}

//sprite function
void Mario::SetPrevMarioPos() {
    m_player.Update();
}

void Mario::MarioOutSideScreen() {
    float temp = 0;
    const auto [fst, snd] = QuickCheckSideCollision(
        MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), extendHitbox(m_hitboxWall, 1.f)),
        m_MarioDirection, temp, temp);
    if (m_player.getCurrentPosition().x <= m_player.getOrigin().x + Scroll::getViewPosition().x) {
        if (!snd)
            m_player.teleport({m_player.getOrigin().x + Scroll::getViewPosition().x, m_player.getCurrentPosition().y});
        //if (m_velocity.x > 0.f) m_velocity.x = 0.f;
        m_OutsideWallLeft = true;
    } else m_OutsideWallLeft = false;

    if (!LevelCompleteEffect) {
        if (m_player.getCurrentPosition().x > Scroll::getViewPosition().x - (MARIO_WIDTH - m_player.getOrigin().x) +
            WindowFrame::getGameSize().x) {
            if (!fst)
                m_player.teleport({
                    Scroll::getViewPosition().x - (MARIO_WIDTH - m_player.getOrigin().x) + WindowFrame::getGameSize().x,
                    m_player.getCurrentPosition().y
                });
            if (m_velocity.x > 0.f) m_velocity.x = 0.f;
            m_OutsideWallRight = true;
        } else m_OutsideWallRight = false;
    }

    if (Scroll::isOutOfScreenXLeft(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), m_hitboxWall), 0.f))
        MarioEffectActivate();
}

//false: right, true: left
void Mario::KeyboardMovement(const float deltaTime) {
    ZoneScopedNC("Mario::KeyboardMovement", 0xFF0000);
    const bool winFocus = WindowFrame::getWindow().hasFocus();
    if (m_CanControlMario && !LevelCompleteEffect) {
        if ((MFCPP::Input::isLeftKeyPressed() && !MFCPP::Input::isRightKeyPressed()) && (
                (!m_MarioCrouchDown && !m_MarioCurrentFalling) || m_MarioCurrentFalling) && winFocus) {
            if (m_velocity.x == 0) m_MarioDirection = true;
            else if (!m_MarioDirection) {
                m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.375f * deltaTime);
                m_velocity.x = m_velocity.x < 0.f ? 0.f : m_velocity.x;
            }
            //init speed
            if (m_velocity.x < 1.0f && m_MarioDirection) m_velocity.x = 1.0f;
            if (m_MarioDirection) {
                m_velocity.x += (m_velocity.x > m_player_speed ? 0.0f : 0.125f * deltaTime);
                m_velocity.x = m_velocity.x > 7.5f ? 7.5f : m_velocity.x;
            }
        } else if ((MFCPP::Input::isRightKeyPressed() && !
                    MFCPP::Input::isLeftKeyPressed()) && (
                       (!m_MarioCrouchDown && !m_MarioCurrentFalling) || m_MarioCurrentFalling) &&
                   winFocus) {
            if (m_velocity.x == 0) m_MarioDirection = false;
            else if (m_MarioDirection) {
                m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.375f * deltaTime);
                m_velocity.x = m_velocity.x < 0.f ? 0.f : m_velocity.x;
            }
            if (m_velocity.x < 1.0f && !m_MarioDirection) m_velocity.x = 1.0f;
            if (!m_MarioDirection) {
                m_velocity.x += (m_velocity.x > m_player_speed ? 0.0f : 0.125f * deltaTime);
                m_velocity.x = m_velocity.x > 7.5f ? 7.5f : m_velocity.x;
            }
        } else if (((!MFCPP::Input::isLeftKeyPressed() && !
                      MFCPP::Input::isRightKeyPressed()) || ((
                         MFCPP::Input::isLeftKeyPressed() &&
                         MFCPP::Input::isRightKeyPressed())) && (
                        !m_MarioCrouchDown || m_MarioCurrentFalling)) || (
                       m_MarioCrouchDown && !m_MarioCurrentFalling)) {
            if (!(m_MarioCrouchDown && !m_MarioCurrentFalling))
                m_velocity.x -= (m_velocity.x <= 0.0f
                                     ? 0.0f
                                     : 0.125f * deltaTime);
            else m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.25f * deltaTime);
        }
        m_velocity.x = m_velocity.x < 0.0f ? 0.0f : m_velocity.x;
        if (MFCPP::Input::isJumpKeyPressed() && !m_MarioCurrentFalling && winFocus) {
            if ((!m_PreJump && !m_Holding) || m_PreJump) {
                SoundManager::PlaySound(SoundID::GAME_JUMP);
                m_velocity.y = -13.0f;
                m_PreJump = (m_PreJump ? !m_PreJump : m_PreJump);
                m_Holding = true;
            }
        }
        if (MFCPP::Input::isDownKeyPressed() && !m_MarioCurrentFalling && m_PowerState > 0 &&
            winFocus) {
            m_MarioCrouchDown = true;
        } else if (!m_MarioCurrentFalling) m_MarioCrouchDown = false;
        if (MFCPP::Input::isJumpKeyPressed() && winFocus) {
            if (m_velocity.y < 0.f)
                m_velocity.y -= (m_velocity.x < 0.625f ? 0.4f : 0.5f) * deltaTime;
            else if (m_velocity.y >= 0 && !m_Holding) m_PreJump = true;
        }
        m_Holding = !(!MFCPP::Input::isJumpKeyPressed() && winFocus);
        if (MFCPP::Input::isFireKeyPressed() && !m_MarioCrouchDown && winFocus)
            m_player_speed = 7.5f;
        else m_player_speed = 4.375f;
        if ((!m_MarioCurrentFalling || m_velocity.y < 0.f) && m_PreJump) m_PreJump = false;
    } else if (LevelCompleteEffect) {
        if (m_MarioDirection) m_MarioDirection = false;
        if (m_MarioCrouchDown) m_MarioCrouchDown = false;
        m_velocity.x = 2.5f;
    } else if (!m_CanControlMario) {
        m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.625f * deltaTime);
        if (m_velocity.x < 0.0f) m_velocity.x = 0.0f;
    }

    m_FireTimeCounting += (m_FireTimeCounting < m_FireTime ? 1.f * deltaTime : 0.f);
    if (MFCPP::Input::isFireKeyPressed() && winFocus && !m_isFireHolding &&
        m_CanControlMario && !LevelCompleteEffect) {
        if (m_FireTimeCounting >= m_FireTime && m_PowerState > 1 && GameScene::projectileManager.
            getMarioProjectileList().size() < 2 && !m_MarioCrouchDown) {
            SoundManager::PlaySound(SoundID::GAME_FIREBALL);
            m_FireTimeCounting = 0.f;
            switch (m_PowerState) {
                case 2:
                    GameScene::projectileManager.addProjectile<MarioFireball>(
                        m_MarioDirection, m_player.getCurrentPosition() +
                        sf::Vector2f(4.f * (m_MarioDirection ? -1.f : 1.f), -23.f));
                    break;
                case 3:
                    GameScene::projectileManager.addProjectile<MarioBeetroot>(
                        m_MarioDirection, m_player.getCurrentPosition() +
                        sf::Vector2f((4.f * (m_MarioDirection ? -1.f : 1.f)),- 11.f));
                    break;
                default: ;
            }
        }
    }
    m_isFireHolding = MFCPP::Input::isFireKeyPressed();
}

void Mario::MarioPosXUpdate(const float deltaTime) {
    ZoneScopedNC("Mario::MarioPosXUpdate", 0x877951);
    if (m_CanControlMario) {
        if (float PlatDistance; PlatformXCollision(
            MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), PlatDistance,
            m_velocity.y)) {
            m_player.setCurrentPosition({
                m_player.getCurrentPosition().x + PlatDistance, m_player.getCurrentPosition().y
            });
        }
    }

    if (!EffectActive) {
        if (!m_MarioDirection)
            m_player.setCurrentPosition({
                m_player.getCurrentPosition().x + m_velocity.x * deltaTime, m_player.getCurrentPosition().y
            });
        else {
            if (!m_OutsideWallLeft && !m_OutsideWallRight)
                m_player.setCurrentPosition({
                    m_player.getCurrentPosition().x + (0 - m_velocity.x) * deltaTime, m_player.getCurrentPosition().y
                });
            else m_velocity.x = 0.f;
        }

        if (m_CanControlMario) {
            if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || m_MarioCrouchDown) && m_velocity.x > 4.375f) {
                m_OverSpeed = true;
            }
            if (m_velocity.x > m_player_speed && (
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || m_MarioCrouchDown)) {
                if (m_OverSpeed) {
                    m_velocity.x -= 0.125f * deltaTime;
                    if (m_velocity.x <= m_player_speed) m_OverSpeed = false;
                } else m_velocity.x = m_player_speed;
            }
            if (m_velocity.x > 7.5f && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || m_MarioCrouchDown))
                m_velocity.x = 7.5f;
        }
    }
}

void Mario::MarioVertXUpdate() {
    ZoneScopedNC("Mario::MarioVertXUpdate", 0xfcba03);
    if (m_CanControlMario) {
        float CurrPosXCollide = 0, CurrPosYCollide = 0;
        const auto [fst, snd] = QuickCheckSideCollision(
            MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxWall), m_MarioDirection,
            CurrPosXCollide, CurrPosYCollide);
        //snap back
        if (m_MarioDirection) {
            if (fst) {
                m_velocity.x = 0.0f;
                m_player.setCurrentPosition({
                    CurrPosXCollide + 28.f + m_player.getOrigin().x, m_player.getCurrentPosition().y
                });
            } else if (snd) {
                m_velocity.x = 0.0f;
                m_player.setCurrentPosition({
                    CurrPosXCollide - (0.0f + (27 - m_player.getOrigin().x)), m_player.getCurrentPosition().y
                });
            }
        } else {
            if (snd) {
                m_velocity.x = 0.0f;
                m_player.setCurrentPosition({
                    CurrPosXCollide - (0.0f + (27 - m_player.getOrigin().x)), m_player.getCurrentPosition().y
                });
            } else if (fst) {
                m_velocity.x = 0.0f;
                m_player.setCurrentPosition({
                    CurrPosXCollide + 28.f + m_player.getOrigin().x, m_player.getCurrentPosition().y
                });
            }
        }
    }
}

void Mario::MarioPosYUpdate(float deltaTime) {
    ZoneScopedNC("Mario::MarioPosYUpdate", 0x422F00);
    if (m_CanControlMario) {
        m_MarioCurrentFalling = true;
        m_player.setCurrentPosition({
            m_player.getCurrentPosition().x, m_player.getCurrentPosition().y + m_velocity.y * deltaTime
        });
        m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : 1.f * deltaTime);
        if (m_velocity.y > 10.0f) m_velocity.y = 10.0f;
    }
}

void Mario::MarioVertYBottomUpdate() {
    ZoneScopedNC("Mario::MarioVertYBottomUpdate", 0x001FB0);
    if (m_CanControlMario) {
        float CurrPosYCollide, CurrPosXCollide;
        //Collision With Platform
        if (float PlatPosY; PlatformYCollision(
            MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), PlatPosY,
            m_velocity.y, true)) {
            m_player.setCurrentPosition({m_player.getCurrentPosition().x, PlatPosY});
            m_MarioCurrentFalling = false;
            m_velocity.y = 0.f;
        }
        //Collision With Obstacles
        const float offset = std::min(m_velocity.x + 1.f, 3.f);
        if (QuickCheckBotCollision(
            MFCPP::CollisionObject({m_player.getCurrentPosition().x, m_player.getCurrentPosition().y},
                                   m_player.getOrigin(), m_hitboxFloor), offset, CurrPosXCollide, CurrPosYCollide)) {
            if (m_velocity.y >= -m_velocity.x) {
                const float floorY = GetCurrFloorY(m_player.getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
                if (m_player.getCurrentPosition().y < CurrPosYCollide + floorY - offset) return;
                if (m_MarioCurrentFalling) m_MarioCurrentFalling = false;
                m_player.setCurrentPosition({
                    m_player.getCurrentPosition().x,
                    CurrPosYCollide + floorY - (MARIO_HEIGHT - 1.f - m_player.getOrigin().y)
                });
                m_velocity.y = 0.f;
            }
        }
    }
}

void Mario::MarioVertYTopUpdate() {
    if (m_CanControlMario) {
        float CurrPosYCollide, CurrPosXCollide;
        if (QuickCheckTopCollision(
            MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), CurrPosXCollide,
            CurrPosYCollide)) {
            //snap back
            float ceilY = (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide, CurrPosYCollide) == 3
                               ? 32.0f
                               : MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide).second);
            if (m_PowerState > 0 && !m_MarioCrouchDown)
                m_player.setCurrentPosition({
                    m_player.getCurrentPosition().x, CurrPosYCollide + (ceilY + m_player.getOrigin().y - 10.f)
                });
            else if ((m_PowerState > 0 && m_MarioCrouchDown) || (m_PowerState == 0 && m_MarioAppearing) || (
                         m_PowerState == 0 && !m_MarioCrouchDown))
                m_player.setCurrentPosition({
                    m_player.getCurrentPosition().x, CurrPosYCollide + (ceilY + m_player.getOrigin().y - 10.f - 23.0f)
                });
            m_velocity.y = 0.0f;
        }
    }
}

void Mario::MarioUpdateHitbox() {
    ZoneScopedNC("Mario::MarioUpdateHitbox", 0x805e00);
    if (m_PowerState > 0 && !m_MarioCrouchDown) {
        m_hitboxWall = sf::FloatRect({0.0f + 4.f, 0.f + 10.f}, {23.0f, 46.0f});
        m_hitboxFloor = sf::FloatRect({0.0f + 4.f, 0.f + 10.f}, {23.0f, 52.0f});
    } else if ((m_PowerState > 0 && m_MarioCrouchDown) || (m_PowerState == 0 && m_MarioAppearing) || (
                   m_PowerState == 0 && !m_MarioCrouchDown)) {
        m_hitboxWall = sf::FloatRect({0.0f + 4.f, 0.0f + 10.f + 23.0f}, {23.0f, 23.0f});
        m_hitboxFloor = sf::FloatRect({0.0f + 4.f, 0.0f + 10.f + 23.0f}, {23.0f, 29.0f}); // 30
    }
}

void Mario::MarioUpdateAnimation() {
    //animation update

    //mariostate:
    // 0: idle, 1: run, 2: jump = fall, 3: crouch, 4: appear
    if (m_CanControlMario) {
        if (!m_MarioAppearing) {
            if (m_MarioCurrentFalling && !m_MarioCrouchDown) {
                m_MarioState = 2;
                if (m_lastMarioState != m_MarioState) {
                    m_MarioAnimation.setAnimation(3, 3, 100, true);
                    m_lastMarioState = m_MarioState;
                }
                m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
            } else if (m_MarioCrouchDown && m_PowerState > 0) {
                m_MarioState = 3;
                if (m_lastMarioState != m_MarioState) {
                    m_MarioAnimation.setAnimation(4, 4, 50, true);
                    m_lastMarioState = m_MarioState;
                }
                m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
            } else if (m_velocity.y == 0.f && !(!m_MarioCurrentFalling && m_MarioCrouchDown && m_PowerState > 0)) {
                if (m_velocity.x == 0.f && m_FireTimeCounting >= m_FireTime) {
                    m_MarioState = 0;
                    if (m_lastMarioState != m_MarioState) {
                        m_MarioAnimation.setAnimation(2, 2, 0, true);
                        m_lastMarioState = m_MarioState;
                    }
                    m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
                } else if (m_velocity.x != 0.f && m_FireTimeCounting >= m_FireTime) {
                    m_MarioState = 1;
                    if (m_lastMarioState != m_MarioState) {
                        m_MarioAnimation.setAnimation(0, 2, 50, true);
                        m_lastMarioState = m_MarioState;
                    }
                    m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
                    m_MarioAnimation.setFrequencyAnimation(
                        std::max(12.0f, std::min(m_velocity.x * 8.0f, 45.0f)));
                    //MarioAnimation.setAnimationFrequency("RunSmallLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
                } else if (m_FireTimeCounting < m_FireTime && m_PowerState > 1) {
                    m_MarioState = 5;
                    if (m_lastMarioState != m_MarioState) {
                        m_MarioAnimation.setAnimation(9, 9, 0, true);
                        m_lastMarioState = m_MarioState;
                    }
                    m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
                }
            }
        } else {
            m_MarioState = 4;
            if (m_lastMarioState != m_MarioState) {
                m_MarioAnimation.setAnimation(5, 7 + (m_PowerState > 1 ? 1 : 0), 100, true);
                m_lastMarioState = m_MarioState;
            }
            m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
        }
    }
}

void Mario::PowerDown() {
    //return; //Skip death, remove later
    if (LevelCompleteEffect || m_Invincible) return;

    if (!m_Invincible) {
        if (m_PowerState > 1) {
            SoundManager::PlaySound(SoundID::GAME_PIPE);
            SetPowerState(1);
            m_Invincible = true;
            m_InvincibleTimer.restart();
            m_InvincibleState = false;
        } else if (m_PowerState == 1) {
            SoundManager::PlaySound(SoundID::GAME_PIPE);
            SetPowerState(0);
            m_Invincible = true;
            m_InvincibleTimer.restart();
            m_InvincibleState = false;
        } else if (m_PowerState == 0 && m_CanControlMario && !LevelCompleteEffect) {
            MarioEffectActivate();
        }
    }
}

void Mario::Death() {
    if (m_Lives <= 0) {
        WindowFrame::getWindow().close();
        WindowFrame::running = false;
    }
    else --m_Lives;
    Objectbuilding();
    m_velocity = {0.f, 0.f};
    SetPowerState(0);
    m_lastPowerState = 0;
    LevelCompleteEffect = false;
    m_MarioDirection = m_FirstMarioDirection;
    ExitGateForeActive = true;
    if (m_MarioCrouchDown) m_MarioCrouchDown = false;
    TimeReset();
    ExitGateEffectReset();
}

void Mario::CheckForDeath() {
    if (Scroll::isOutOfScreenYBottom(
        MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), 16.f)) {
        m_CanControlMario = false;
        ActiveMarioEffect(m_temp_alpha);
    }
}

void Mario::MarioAnimationUpdate(const float deltaTime) {
    m_MarioAnimation.frameTimeAccumulate(deltaTime);
}

void Mario::MarioDraw(float alpha) {
    // check power state
    HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(m_hitboxFloor,
                                                          m_player.getCurrentPosition() - m_player.getOrigin(),
                                                          sf::Color::Blue));
    if (static_cast<float>(m_AppearingTimer.getElapsedTime().asMilliseconds()) > (3000.0f / 91.0f) * 15.0f)
        m_MarioAppearing = false;
    if (m_PowerState != m_lastPowerState) {
        m_MarioAppearing = true;
        m_AppearingTimer.restart();
        m_lastPowerState = m_PowerState;
    }
    //draw
    m_MarioAnimation.animationUpdate(m_player.getInterpolatedPosition(alpha), m_player.getOrigin());
    if (m_InvincibleTimer.getElapsedTime().asSeconds() > 2.0f) m_Invincible = false;
    if (!m_Invincible) {
        if (m_CanControlMario) m_MarioAnimation.animationDraw();
    } else {
        if (!m_InvincibleState)
            if (m_CanControlMario) m_MarioAnimation.animationDraw();
    }
}

void Mario::InvincibleStateUpdate() {
    if (m_Invincible) {
        if (!m_InvincibleState) m_InvincibleState = true;
        else m_InvincibleState = false;
    }
}

void Mario::setXvelocity(float val) {
    m_velocity.x = val;
}

void Mario::setYvelocity(float val) {
    m_velocity.y = val;
}

float Mario::getXvelocity() {
    return m_velocity.x;
}

float Mario::getYvelocity() {
    return m_velocity.y;
}

sf::FloatRect Mario::getHitbox() {
    return m_hitboxFloor;
}

sf::Vector2f Mario::getOrigin() {
    return m_player.getOrigin();
}

void Mario::setCurrentPosition(const sf::Vector2f &val) {
    m_player.setCurrentPosition(val);
}

sf::Vector2f Mario::getCurrentPosition() {
    return m_player.getCurrentPosition();
}

void Mario::resetPreviousPosition() {
    m_player.Update();
}

void Mario::forceSetPosition(const sf::Vector2f &val) {
    m_player.teleport(val);
}

void Mario::setDirection(bool val) {
    m_MarioDirection = val;
}

void Mario::setFirstDirection(bool val) {
    m_FirstMarioDirection = val;
}

bool Mario::getDirection() {
    return m_MarioDirection;
}

bool Mario::getFirstDirection() {
    return m_FirstMarioDirection;
}

void Mario::setLives(uint16_t val) {
    m_Lives = val;
}

void Mario::setScore(uint64_t val) {
    m_Score = val;
}

uint16_t Mario::getLives() {
    return m_Lives;
}

uint64_t Mario::getScore() {
    return m_Score;
}

bool Mario::isFalling() {
    return m_MarioCurrentFalling;
}

bool Mario::isPrejump() {
    return m_PreJump;
}

int Mario::getPowerState() {
    return m_PowerState;
}

sf::FloatRect Mario::getHitboxWall() {
    return m_hitboxWall;
}

sf::FloatRect Mario::getHitboxFloor() {
    return m_hitboxFloor;
}

void Mario::setCanControl(bool val) {
    m_CanControlMario = val;
}

bool Mario::getCanControl() {
    return m_CanControlMario;
}

sf::Vector2f Mario::getInterpolatedPosition() {
    return m_player.getInterpolatedPosition(m_temp_alpha);
}

void Mario::setHolding(bool val) {
    m_Holding = val;
}

bool Mario::getHolding() {
    return m_Holding;
}

bool Mario::getCurrentFalling() {
    return m_MarioCurrentFalling;
}

void Mario::storeAlpha(float alpha) {
    m_temp_alpha = alpha;
}
