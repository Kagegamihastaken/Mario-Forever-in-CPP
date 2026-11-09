#include "Editor/UI/EditorImgui.hpp"

#include <ImGuiFileDialog.h>
#include "Core/WindowFrame.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Editor/Editor.hpp"
#include "Editor/EditorUtility.hpp"

void MFCPP::EditorImgui::EditPropertyHelper(TileProperty& prop) {
    std::visit([&]<typename T0>(T0&& arg) {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, BoolProps>)
            ImGui::Checkbox(arg.name.c_str(), &arg.val);
        else if constexpr (std::is_same_v<T, IntProps>) {
            ImGui::PushItemWidth(125);
            ImGui::InputInt(arg.name.c_str(), &arg.val, 1, 100, ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::PopItemWidth();
            if (arg.val > arg.max && arg.max > 0.f) arg.val = arg.max;
            else if (arg.val < arg.min) arg.val = arg.min;
        }
        else if constexpr (std::is_same_v<T, StringProps>)
            ImGui::InputText(arg.name.c_str(), arg.val, IM_ARRAYSIZE(arg.val), ImGuiInputTextFlags_EscapeClearsAll);
        else if constexpr (std::is_same_v<T, FloatProps>) {
            ImGui::PushItemWidth(125);
            ImGui::InputFloat(arg.name.c_str(), &arg.val, 1, 100, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::PopItemWidth();
            if (arg.val > arg.max && arg.max > 0.f) arg.val = arg.max;
            else if (arg.val < arg.min) arg.val = arg.min;
        }
        else if constexpr (std::is_same_v<T, Vector2fProps>) {
            ImGui::PushItemWidth(125);
            ImGui::InputFloat((arg.name+".x").c_str(), &arg.val.x, 0, 0, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::InputFloat((arg.name+".y").c_str(), &arg.val.y, 0, 0, "%.2f", ImGuiInputTextFlags_EscapeClearsAll);
            ImGui::PopItemWidth();
        }
        else if constexpr (std::is_same_v<T, ColorProps>)
            ImGui::ColorEdit4(arg.name.c_str(), reinterpret_cast<float*>(&arg.val), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
        }, prop);
}
void MFCPP::EditorImgui::EditPropertyDialog() {
    if (EDITOR_ShowProperty) {
        ImGui::Begin("Edit Property", &EDITOR_ShowProperty, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
        //ImGui::SetWindowSize("Edit Property", ImVec2(256.f, 256.f));
        ImGui::SetWindowPos(ImVec2(static_cast<float>(WindowFrame::getWindow().getSize().x) / 2.f - ImGui::GetWindowSize().x / 2.f, static_cast<float>(WindowFrame::getWindow().getSize().y) / 2.f - ImGui::GetWindowSize().y / 2.f));
        if (ImGui::BeginTabBar("EditPropertyTab")) {
            if (ImGui::BeginTabItem("Property")) {
                for (int i = 0; i < SaveTile.getProperty().getPropertyCount(); ++i) {
                    EditPropertyHelper(*SaveTile.getProperty().at(i));
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        //ImGui::Checkbox("isSmooth", &smooth);
        //ImGui::Checkbox("isFall", &fall);
        //ImGui::Checkbox("isWait", &wait);
        if (ImGui::Button("Confirm")) {
            Tile.erase(SaveTile);
            Tile.insert(SaveTile);
            EDITOR_ShowProperty = false;
        }
        ImGui::End();
    }
}
void MFCPP::EditorImgui::OpenFileDialog() {
    if (!EDITOR_OpenDialog) return;
    if (!ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey")) {
        if (EDITOR_ShowProperty) EDITOR_ShowProperty = false;
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Open File", ".json", config);
    }
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2(WindowFrame::getGameSize().x / 2.f, WindowFrame::getGameSize().y / 2.f))) {
        if (ImGuiFileDialog::Instance()->IsOk())
            EditorUtility::FileLoad(ImGuiFileDialog::Instance()->GetFilePathName());
        EDITOR_SELECTILE_CLOCK.restart();
        EDITOR_OpenDialog = false;
        ImGuiFileDialog::Instance()->Close();
    }
}
void MFCPP::EditorImgui::SaveFileDialog() {
    if (!EDITOR_SaveDialog) return;
    if (!ImGuiFileDialog::Instance()->IsOpened("SaveFileDlgKey")) {
        if (EDITOR_ShowProperty) EDITOR_ShowProperty = false;
        IGFD::FileDialogConfig config;
        config.path = ".";
        config.fileName = "untitled.json";
        config.flags = ImGuiFileDialogFlags_ConfirmOverwrite;
        ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Save File As...", ".json", config);
    }
    if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2(WindowFrame::getGameSize().x / 2.f, WindowFrame::getGameSize().y / 2.f))) {
        if (ImGuiFileDialog::Instance()->IsOk())
            EditorUtility::FileSave(ImGuiFileDialog::Instance()->GetFilePathName());
        EDITOR_SELECTILE_CLOCK.restart();
        EDITOR_SaveDialog = false;
        ImGuiFileDialog::Instance()->Close();
    }
}
void MFCPP::EditorImgui::SettingDialog() {
    if (EDITOR_Setting) {
        ImGui::Begin("Setting", &EDITOR_Setting, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowPos(ImVec2(static_cast<float>(WindowFrame::getWindow().getSize().x) / 2.f - ImGui::GetWindowSize().x / 2.f, static_cast<float>(WindowFrame::getWindow().getSize().y) / 2.f - ImGui::GetWindowSize().y / 2.f));
        if (ImGui::BeginTabBar("SettingTab")) {
            if (ImGui::BeginTabItem("BGGradient")) {
                for (int i = 0; i < BgColor.getPropertyCount(); ++i) {
                    EditPropertyHelper(*BgColor.at(i));
                }
                static int select = 0;

                if (ImGui::BeginListBox("Background")) {
                    for (int i = 0; i < BackgroundLayers.size(); ++i) {
                        bool is_selected = (select == i);
                        ImGuiSelectableFlags flags = (select == i) ? ImGuiSelectableFlags_Highlight : 0;
                        if (ImGui::Selectable(BackgroundLayers[i].name.c_str(), is_selected, flags))
                            select = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                }
                const bool DisabledBackground = BackgroundLayers.empty();
                if (DisabledBackground)
                    ImGui::BeginDisabled();
                if (!DisabledBackground && select < BackgroundLayers.size()) {
                    ImGui::PushItemWidth(150);
                    // Use InputFloat2 for a compact X/Y editor
                    ImGui::InputFloat2("Parallax Factor", &BackgroundLayers[select].parallaxFactor.x, "%.2f");
                    ImGui::PopItemWidth();
                }
                if (ImGui::Button("Delete")) {
                    BackgroundLayers.erase(BackgroundLayers.begin() + select);
                    select = 0;
                }
                if (DisabledBackground)
                    ImGui::EndDisabled();
                ImGui::SameLine();
                if (ImGui::Button("Add"))
                    ImGui::OpenPopup("AddBackgroundPopup");
                AddBGPopup();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Level Settings")) {
                if (ImGui::CollapsingHeader("Level Data")) {
                    ImGui::SeparatorText("Level Size");
                    for (int i = 0; i < LevelSize.getPropertyCount(); ++i) {
                        EditPropertyHelper(*LevelSize.at(i));
                    }
                    ImGui::SeparatorText("Time");
                    for (int i = 0; i < TimeSetting.getPropertyCount(); ++i) {
                        EditPropertyHelper(*TimeSetting.at(i));
                    }
                    ImGui::SeparatorText("Environment");
                    for (int i = 0; i < LevelEnvironmentEditor.getPropertyCount(); ++i) {
                        EditPropertyHelper(*LevelEnvironmentEditor.at(i));
                    }
                }
                if (ImGui::CollapsingHeader("Music")) {
                    ImGui::SeparatorText("Music");
                    for (int i = 0; i < LevelMusic.getPropertyCount(); ++i) {
                        EditPropertyHelper(*LevelMusic.at(i));
                    }
                }
                if (ImGui::CollapsingHeader("AutoScroll")) {
                    ImGui::SeparatorText("AutoScroll");
                    for (int i = 0; i < AutoScrollSetting.getPropertyCount(); ++i) {
                        EditPropertyHelper(*AutoScrollSetting.at(i));
                    }
                }
                if (ImGui::CollapsingHeader("Weather")) {
                    ImGui::SeparatorText("Weather");
                    for (int i = 0; i < WeatherSetting.getPropertyCount(); ++i) {
                        EditPropertyHelper(*WeatherSetting.at(i));
                    }
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::Separator();
        if (ImGui::Button("Confirm")) {
            BgGradientSetColor(BgColor.getProperty<ColorProps>("First Background Color")->val.ColorNormalize(), BgColor.getProperty<ColorProps>("Second Background Color")->val.ColorNormalize());
            BgGradientInitPos(LevelSize.getProperty<FloatProps>("Level Width")->val, LevelSize.getProperty<FloatProps>("Level Height")->val);
            EDITOR_Setting = false;
        }
        ImGui::SetItemTooltip("Click to Confirm!");
        ImGui::End();
    }
}
void MFCPP::EditorImgui::DialogPopup(std::string_view name) {
    if (ImGui::BeginPopup("AddBGDoesntExist")) {
        ImGui::Text("Cannot Add Texture %s", name.data());
        ImGui::Separator();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
void MFCPP::EditorImgui::AddBGPopup() {
    if (ImGui::BeginPopup("AddBackgroundPopup")) {
        ImGui::Text("Add New Background Layer");
        ImGui::Separator();

        static char BgName[256] = "";
        ImGui::InputText("BG Name", BgName, IM_ARRAYSIZE(BgName));

        if (ImGui::Button("Add")) {
            if (strlen(BgName) > 0) {
                if (ImageManager::isExist(BgName)) {
                    bool isExist = false;
                    for (const auto &i : BackgroundLayers) {
                        if (i.name == BgName) {
                            isExist = true;
                            break;
                        }
                    }
                    if (isExist)
                        ImGui::OpenPopup("AddBGDoesntExist");
                    else {
                        BackgroundLayers.emplace_back(BgName);
                        memset(BgName, 0, sizeof(BgName));
                        ImGui::CloseCurrentPopup();
                    }
                }
                else {
                    ImGui::OpenPopup("AddBGDoesntExist");
                }
            }
        }
        DialogPopup(BgName);
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
