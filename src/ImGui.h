#pragma once
#include <imgui.h>
#include <imgui_internal.h>

#include <format>

namespace fuse {
class Angle;
struct Vec3;
class Mat4;
} // namespace fuse

namespace fuse::Imgui {


/// @brief Display formatted text with ImGui.
///
/// This is wrapper for ImGui::Text() which used C++ 20 format library
/// comparing to ImGui::Text() which use C style variadic paramater.
///
/// @param fmt  The formating string.
/// @param args The arguments to display inside the string.
template <typename... Args>
void TextFmt(const std::format_string<Args...>& fmt, const Args&... args) {
    std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
    ImGui::TextUnformatted(str.c_str(), &str[str.size()]);
}

/// @brief Display formatted tooltips text with ImGui.
///
/// This is replacement for ImGui::SetTooltip() which used C++ 20 format library
/// comparing to ImGui::Text() which use C style variadic paramater.
///
/// @param fmt  The formating string.
/// @param args The arguments to display inside the string.
template <typename... Args>
void SetTooltip(const std::format_string<Args...>& fmt, const Args&... args) {
    if (ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, ImGuiWindowFlags_None)) {
        TextFmt(fmt, args...);
        ImGui::EndTooltip();
    }
}

/// @brief Render a Matrix as a table.
/// @param The matrix to render.
void draw(const Mat4&);

/// @brief
/// @param label
/// @param angle
bool dragFloat(const std::string& label, float& value);


/// @brief
/// @param label
/// @param angle
bool dragAngle(const std::string& label, fuse::Angle& angle);

/// @brief Draw a 3d vector drag slider widget with button to reset the value.
/// @param label      The label to display.
/// @param values     The 3d vector to display/edit.
/// @param resetValue The value to use to reset the vector component.
/// @param columnWidth
bool dragVec3(const std::string& label, fuse::Vec3& values, float resetValue = 0.0F,
              float columnWidth = 100.0F);
} // namespace fuse::Imgui
