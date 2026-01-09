#include "ImGui.h"

#include "math/Angle.h"
#include "math/Mat4.h"
#include "math/Vec3.h"

#include <string>


namespace fuse::Imgui {

void draw(const Mat4& m) {
    const ImGuiTableFlags flags =
      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame;
    if (ImGui::BeginTable("table1", 4, flags)) {
        for (unsigned int row = 0; row < 4; row++) {
            for (unsigned int column = 0; column < 4; column++) {
                ImGui::TableNextColumn();
                TextFmt("{: .4f}", m(row, column));
            }
        }
        ImGui::EndTable();
    }
}

bool dragFloat(const std::string& label, float& value) {
    ImGui::PushID(label.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();
    const bool hasChanged = ImGui::DragFloat("##drag", &value);
    ImGui::Columns(1);
    ImGui::PopID();
    return hasChanged;
}

bool dragAngle(const std::string& label, fuse::Angle& angle) {
    auto degree = angle.asDegrees();
    if (dragFloat(label, degree)) {
        angle = fuse::degrees(degree);
        return true;
    }
    return false;
}

bool dragVec3(const std::string& label, fuse::Vec3& values, float resetValue, float columnWidth) {
    bool           hasChange  = false;
    const ImGuiIO& io         = ImGui::GetIO();
    auto*          boldFont   = io.Fonts->Fonts[0];
    const float    lineHeight = ImGui::GetFontSize() + (ImGui::GetStyle().FramePadding.y * 2.0F);
    const ImVec2   buttonSize = {lineHeight + 3.0F, lineHeight};

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);

    // draw the label on the first column
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());

    // draw each component vector on the next column.
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    //
    // X component
    //
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8F, 0.1F, 0.15F, 1.0F});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9F, 0.2F, 0.2F, 1.0F});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8F, 0.1F, 0.15F, 1.0F});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x  = resetValue;
        hasChange = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    hasChange |= ImGui::DragFloat("##X", &values.x, 0.1F, 0.0F, 0.0F, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    //
    // Y component
    //
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2F, 0.7F, 0.2F, 1.0F});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3F, 0.8F, 0.3F, 1.0F});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2F, 0.7F, 0.2F, 1.0F});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y  = resetValue;
        hasChange = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    hasChange |= ImGui::DragFloat("##Y", &values.y, 0.1F, 0.0F, 0.0F, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    //
    // Z component
    //
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1F, 0.25F, 0.8F, 1.0F});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2F, 0.35F, 0.9F, 1.0F});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1F, 0.25F, 0.8F, 1.0F});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z  = resetValue;
        hasChange = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    hasChange |= ImGui::DragFloat("##Z", &values.z, 0.1F, 0.0F, 0.0F, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    return hasChange;
}


} // namespace fuse::Imgui
