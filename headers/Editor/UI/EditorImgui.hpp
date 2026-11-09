#ifndef MFCPP_EDITORIMGUI_HPP
#define MFCPP_EDITORIMGUI_HPP

#include "Editor/SelectTile.hpp"

namespace MFCPP::EditorImgui {
    void EditPropertyDialog();
    void EditPropertyHelper(TileProperty& prop);
    void OpenFileDialog();
    void SaveFileDialog();
    void SettingDialog();
    void DialogPopup(std::string_view name);
    void AddBGPopup();
}

#endif //MFCPP_EDITORIMGUI_HPP
