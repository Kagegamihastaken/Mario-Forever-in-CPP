#include "Editor/Editor.hpp"

#include <imgui.h>
#include "Editor/SelectTile.hpp"
#include "Editor/TabButton.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Hash.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/SimpleSprite.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Editor/UI/EditorImgui.hpp"

sf::Vector2f getEditorPosOffset() {
    return sf::Vector2f((640.f - WindowFrame::getGameSize().x) / 2.f, (480.f - WindowFrame::getGameSize().y) / 2.f);
}
void SetPrevEditor() {
    EditorPrevPos = EditorPos;
}
void InterpolateEditorPos(const float alpha) {
    EditorInterpolatedPos = MFCPP::Interpolation::linearInterpolation(EditorPrevPos, EditorPos, alpha);
}

void AlphaUpdate(float& alpha, bool& state, const float min, const float max, const float change, const float dt) {
    if (state) {
        if (alpha < max) alpha += change * dt;
        if (alpha >= max) {
            alpha = max;
            state = false;
        }
    }
    else {
        if (alpha > min) alpha -= change * dt;
        if (alpha <= min) {
            alpha = min;
            state = true;
        }
    }
}

void EditorInit() {
    SelectBox.setTexture(ImageManager::getReturnTexture("EDITOR_SELECT_BOX"), true);
    if (ImageManager::getReturnTexture(TilePage[LevelTab][0].name) == nullptr) throw std::runtime_error("NULLPTR");
    EDITOR_Mario.setPosition(sf::Vector2f(128, 320));
    EDITOR_Mario.setTexture(ImageManager::getReturnTexture(TilePage[LevelTab][0].name), true);
    EDITOR_Mario.setOrigin(sf::Vector2f(0.0f, static_cast<float>(ImageManager::getReturnTexture(TilePage[LevelTab][0].name)->getSize().y) - 32.0f));

    EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(256, 320));
    EDITOR_ExitGateIndicator.setTexture(ImageManager::getReturnTexture(TilePage[LevelTab][1].name), true);
    EDITOR_ExitGateIndicator.setOrigin(sf::Vector2f(0.0f, static_cast<float>(ImageManager::getReturnTexture(TilePage[LevelTab][1].name)->getSize().y) - 32.0f));

    EDITOR_ExitGate.setPosition(sf::Vector2f(384, 320));
    EDITOR_ExitGate.setTexture(ImageManager::getReturnTexture(TilePage[LevelTab][2].name), true);
    EDITOR_ExitGate.setOrigin(sf::Vector2f(0.0f, static_cast<float>(ImageManager::getReturnTexture(TilePage[LevelTab][2].name)->getSize().y) - 32.0f));

    BgGradientSetColor(BgColor.getProperty<ColorProps>("First Background Color")->val.ColorNormalize(), BgColor.getProperty<ColorProps>("Second Background Color")->val.ColorNormalize());
    BgGradientInitPos(LevelSize.getProperty<FloatProps>("Level Width")->val, LevelSize.getProperty<FloatProps>("Level Height")->val);
}
void SelectedTilePosUpdate() {
    SelectedBlock.setPosition(sf::Vector2f(EditorInterpolatedPos.x + 16, EditorInterpolatedPos.y + 16));

    Grid[0].texCoords = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y);
    Grid[1].texCoords = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y);
    Grid[2].texCoords = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y + 640.0f);
    Grid[3].texCoords = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y + 640.0f);

    Grid[0].position = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y);
    Grid[1].position = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y);
    Grid[2].position = sf::Vector2f(EditorInterpolatedPos.x, EditorInterpolatedPos.y + 640.0f);
    Grid[3].position = sf::Vector2f(EditorInterpolatedPos.x + 640.0f, EditorInterpolatedPos.y + 640.0f);
}
void TilePosUpdate(const float dt) {
    if (!EDITOR_ShowProperty && !EDITOR_OpenDialog && !EDITOR_SaveDialog) {
        TileX = std::floor((WindowFrame::getMousePosition().x + EditorPos.x + getEditorPosOffset().x) / 32.0f) * 32.0f; // 640 default width
        TileY = std::floor((WindowFrame::getMousePosition().y + EditorPos.y + getEditorPosOffset().y) / 32.0f) * 32.0f; // 480 default height
    }
    AlphaUpdate(SelectBoxAlpha, SelectBoxAlphaState, SELECTBOXALPHA_MIN, SELECTBOXALPHA_MAX, SELECTBOXALPHA_CHANGE, dt);
    AlphaUpdate(GridAlpha, GridAlphaState, GRIDALPHA_MIN, GRIDALPHA_MAX, GRIDALPHA_CHANGE, dt);

    if (PrevSelectTile != CurrSelectTile || PrevPage != CurrPage) {
        SelectedBlock.setTexture(ImageManager::getReturnTexture(TilePage[CurrPage][CurrSelectTile].name), true);
        SelectedBlock.setTextureRect(sf::IntRect(TilePage[CurrPage][CurrSelectTile].texPos, {std::min(static_cast<int>(SelectedBlock.getGlobalBounds().size.x - TilePage[CurrPage][CurrSelectTile].texPos.x), 32), std::min(static_cast<int>(SelectedBlock.getGlobalBounds().size.y - TilePage[CurrPage][CurrSelectTile].texPos.y), 32)}));
        PrevSelectTile = CurrSelectTile;
        PrevPage = CurrPage;
    }
    SelectBox.setColor(sf::Color(255, 255, 255, static_cast<int>(SelectBoxAlpha)));
    Grid[0].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    Grid[1].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    Grid[2].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    Grid[3].color = sf::Color(255, 255, 255, static_cast<int>(GridAlpha));
    SelectBox.setPosition(sf::Vector2f(TileX, TileY));
}
void EditorEvent(const std::optional<sf::Event>& event) {
    if (EDITOR_OpenDialog || EDITOR_SaveDialog) return;
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Space:
                if (EDITOR_Setting) return;
                if (!EDITOR_ShowProperty && EDITOR_BuildMode) {
                    SoundManager::PlaySound(SoundID::EDITOR_MENU);
                    EDITOR_SELECTTILE = !EDITOR_SELECTTILE;
                    if (EDITOR_SELECTTILE) PreviewPage = CurrPage;
                    SelectTileDisplayUpdate();
                }
                break;
            case sf::Keyboard::Key::S:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    if (!EDITOR_OpenDialog)
                        EDITOR_SaveDialog = true;
                }
                break;
            case sf::Keyboard::Key::D:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    if (!EDITOR_SaveDialog)
                        EDITOR_OpenDialog = true;
                }
                break;
            case sf::Keyboard::Key::B:
                if (!EDITOR_ShowProperty)
                    EDITOR_BuildMode = !EDITOR_BuildMode;
                break;
            default: ;
        }
    }
    else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        switch (mousePressed->button) {
            case sf::Mouse::Button::Left:
                if (EDITOR_Setting) return;
                if (EDITOR_SELECTTILE) {
                    if (SettingButton.isMouseHovered(EditorInterpolatedPos, WindowFrame::getMousePosition()))
                        EDITOR_Setting = true;
                    bool isClickedTab = false;
                    for (int i = 0; i < TabList.size(); ++i) {
                        if (TabList[i].isMouseHovered(EditorInterpolatedPos, WindowFrame::getMousePosition())) {
                            PreviewPage = i;
                            isClickedTab = true;
                            break;
                        }
                    }
                    if (isClickedTab) {
                        SelectTileDisplayUpdate();
                        SoundManager::PlaySound(SoundID::EDITOR_TAB_SELECT);
                    }
                }
                if (const int exist = CheckExistPos(); EDITOR_SELECTTILE && exist != -1) {
                    SoundManager::PlaySound(SoundID::EDITOR_CLOSE);
                    CurrSelectTile = exist;

                    CurrPage = PreviewPage;
                    EDITOR_SELECTILE_CLOCK.restart();
                    EDITOR_SELECTTILE = false;
                }
                break;
            default: ;
        }
    }
}
void PlaceTile() {
    //MFCPP::Log::InfoPrint(fmt::format("CurrPage: {}", CurrPage));
    if (EDITOR_SELECTTILE) return;
    if (EDITOR_SELECTILE_CLOCK.getElapsedTime().asSeconds() < 0.15f) return;

    if (EDITOR_ShowProperty || EDITOR_OpenDialog || EDITOR_SaveDialog) return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && WindowFrame::getWindow().hasFocus()) {
        if ((lastDeleteX != TileX || lastDeleteY != TileY) || (lastPlaceX == TileX && lastPlaceY == TileY)) {
            if (EDITOR_BuildMode) {
                if (CurrPage != LevelTab || (CurrPage == LevelTab && CurrSelectTile > 2)) {
                    if (Tile.contains(RenderTile(sf::Vector2f(TileX, TileY), CurrPage))) {
                        SoundManager::PlaySound(SoundID::EDITOR_DELETE);
                        Tile.erase(RenderTile(sf::Vector2f(TileX, TileY), CurrPage));
                        if (!EDITOR_CanPlace) EDITOR_CanPlace = true;
                        lastDeleteX = TileX;
                        lastDeleteY = TileY;
                        lastPlaceX = -1.0f;
                        lastPlaceY = -1.0f;
                    }
                }
            }
            else {
                if (!EDITOR_isRightHolding) {
                    //if (SelectTile)
                    if (const auto it = Tile.find(RenderTile(sf::Vector2f(TileX, TileY), CurrPage)); it != Tile.end()) {
                        if (it->getProperty().getPropertyCount() > 0) {
                            EDITOR_ShowProperty = true;
                            SaveTile = *it;
                        }
                        //it_save = it->getProperty();
                        //MFCPP::Log::WarningPrint(fmt::format("PageID: {} {}", it->getPage(), it->getID()));
                        //MFCPP::Log::WarningPrint(fmt::format("EndPos: {} {}", it->getEndPos().x, it->getEndPos().y));
                    }
                    EDITOR_isRightHolding = true;
                }
            }
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && WindowFrame::getWindow().hasFocus()) {
        if (!EDITOR_BuildMode) return;
        if ((lastPlaceX != TileX || lastPlaceY != TileY) || (lastDeleteX == TileX && lastDeleteY == TileY)) {
            if (CurrPage != LevelTab || (CurrPage == LevelTab && CurrSelectTile > 2)) {
                if (TileX == EDITOR_Mario.getPosition().x && TileY == EDITOR_Mario.getPosition().y) return;
                if (TileX == EDITOR_ExitGateIndicator.getPosition().x && TileY == EDITOR_ExitGateIndicator.getPosition().y) return;

                if (EDITOR_isLeftHolding) return;
                if (!EDITOR_CanPlace) {
                    if (CurrPage == PlatformTab) {
                        if (CurrSelectTile >= 0 && CurrSelectTile <= 1) {
                            Tile.erase(RenderTile(EDITOR_SavePos, EDITOR_SavePosPage));

                            RenderTile tile(TilePage[CurrPage][CurrSelectTile].prop, *ImageManager::getReturnTexture(TilePage[CurrPage][CurrSelectTile].name), EDITOR_SavePos, CurrPage, CurrSelectTile, sf::Vector2f(TileX, TileY));
                            tile.setOrigin(sf::Vector2f(0.0f, ImageManager::getReturnTexture(TilePage[CurrPage][CurrSelectTile].name)->getSize().y - 32.0f));
                            Tile.insert(tile);
                            EDITOR_SavePos = sf::Vector2f(0.f, 0.f);
                            EDITOR_SavePosPage = CurrPage;

                            EDITOR_CanPlace = true;
                        }
                    }
                    EDITOR_isLeftHolding = true;
                    return;
                }

                if (!Tile.contains(RenderTile(sf::Vector2f(TileX, TileY), CurrPage))) {
                    SoundManager::PlaySound(SoundID::EDITOR_PLACE);
                    RenderTile tile(TilePage[CurrPage][CurrSelectTile].prop, *ImageManager::getReturnTexture(TilePage[CurrPage][CurrSelectTile].name), sf::Vector2f(TileX, TileY), CurrPage, CurrSelectTile);
                    tile.setOrigin(sf::Vector2f(0.0f, ImageManager::getReturnTexture(TilePage[CurrPage][CurrSelectTile].name)->getSize().y - 32.0f));
                    Tile.insert(tile);
                    if (CurrPage == PlatformTab) {
                        switch (CurrSelectTile) {
                            case 0:
                                EDITOR_SavePos = sf::Vector2f(TileX, TileY);
                                EDITOR_SavePosPage = CurrPage;
                                EDITOR_CanPlace = false;
                                break;
                            case 1:
                                EDITOR_SavePos = sf::Vector2f(TileX, TileY);
                                EDITOR_SavePosPage = CurrPage;
                                EDITOR_CanPlace = false;
                                break;
                            default: ;
                        }
                    }
                }
            }
            else {
                switch (CurrSelectTile) {
                    case 0:
                        EDITOR_Mario.setPosition(sf::Vector2f(TileX, TileY));
                        break;
                    case 1:
                        EDITOR_ExitGateIndicator.setPosition(sf::Vector2f(TileX, TileY));
                        break;
                    case 2:
                        EDITOR_ExitGate.setPosition(sf::Vector2f(TileX, TileY));
                    default: ;
                }
            }
            //else std::cout << "Already Placed\n";
            lastPlaceX = TileX;
            lastPlaceY = TileY;
            lastDeleteX = -1.0f;
            lastDeleteY = -1.0f;
        }
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (EDITOR_isLeftHolding) EDITOR_isLeftHolding = false;
    }
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        if (EDITOR_isRightHolding) EDITOR_isRightHolding = false;
    }
}
void EditorScreenMove(const float dt) {
    if (EDITOR_ShowProperty || EDITOR_OpenDialog || EDITOR_SaveDialog) return;

    if (!EDITOR_SELECTTILE) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) EditorPos.x += 8.0f * dt * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? 2.0f : 1.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) EditorPos.x -= 8.0f * dt * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? 2.0f : 1.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) EditorPos.y -= 8.0f * dt * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? 2.0f : 1.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) EditorPos.y += 8.0f * dt * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? 2.0f : 1.0f);
        if (EditorPos.x <= -5 * 32.f) EditorPos.x = -5 * 32.f;
        else if (EditorPos.x >= LevelSize.getProperty<FloatProps>("Level Width")->val - WindowFrame::getGameSize().x + 5 * 32.f) EditorPos.x = LevelSize.getProperty<FloatProps>("Level Width")->val - WindowFrame::getGameSize().x + 5 * 32.f;
        if (EditorPos.y <= -5 * 32.f) EditorPos.y = -5 * 32.f;
        else if (EditorPos.y >= LevelSize.getProperty<FloatProps>("Level Height")->val - WindowFrame::getGameSize().y + 5 * 32.f) EditorPos.y = LevelSize.getProperty<FloatProps>("Level Height")->val - WindowFrame::getGameSize().y + 5 * 32.f;
    }
}

void DrawTile() {
    if (EDITOR_SELECTTILE) return;
    MFCPP::EditorImgui::EditPropertyDialog();
    MFCPP::EditorImgui::OpenFileDialog();
    MFCPP::EditorImgui::SaveFileDialog();
    for (int j = 0; j < TilePage.size(); ++j) {
        for (const auto &i : Tile) {
            if (i.getPage() != j) continue;

            const bool isTileTouch = TileX == i.getPosition().x && TileY == i.getPosition().y;
            if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(i.getPosition(), i.getOrigin(), sf::FloatRect({0.f, 0.f}, {32.f, 32.f})), 32)) {
                const unsigned int PageDiff = (CurrPage != i.getPage() ? 160 : 255);
                if (i.getEndPos() != sf::Vector2f(-1.f, -1.f)) {
                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    line[0].position = (i.getPosition() + i.getPosition() + static_cast<sf::Vector2f>(i.getTexture()->getSize())) * 0.5f;
                    line[1].position = (i.getEndPos() + i.getEndPos() + static_cast<sf::Vector2f>(i.getTexture()->getSize())) * 0.5f;

                    line[0].color = sf::Color::Black;
                    line[1].color = sf::Color::Black;
                    WindowFrame::getWindow().draw(line);

                    MFCPP::SimpleSprite R(i.getTexture());
                    R.setPosition(i.getEndPos());
                    R.setOrigin(i.getOrigin());
                    R.setColor(sf::Color(255, 255, 255, 160 - 255 + PageDiff));
                    WindowFrame::getWindow().draw(R);
                }
                MFCPP::SimpleSprite T(i.getTexture());
                T.setPosition(i.getPosition());
                T.setOrigin(i.getOrigin());
                if (!EDITOR_BuildMode && !EDITOR_ShowProperty && isTileTouch) {
                    T.setColor(sf::Color(0, 255, 0, PageDiff));
                    T.setRenderTexture(false);
                }
                else T.setColor(sf::Color(255, 255, 255, PageDiff));
                WindowFrame::getWindow().draw(T);
            }
        }
    }
    WindowFrame::getWindow().draw(EDITOR_ExitGate);
    WindowFrame::getWindow().draw(EDITOR_Mario);
    WindowFrame::getWindow().draw(EDITOR_ExitGateIndicator);
    if (EDITOR_BuildMode) WindowFrame::getWindow().draw(SelectBox);
    WindowFrame::getWindow().draw(Grid, ImageManager::getReturnTexture("EDITOR_GRID"));
    WindowFrame::getWindow().draw(SelectedBlock);
}