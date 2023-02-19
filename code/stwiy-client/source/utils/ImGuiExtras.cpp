#include "../common.hpp"
#include "ImGuiExtras.hpp"

void ImGui::TextCentered(const char* fmt, ...)
{
	va_list args;
    va_start(args, fmt);
	size_t reqTxtSize = vsnprintf(nullptr, 0, fmt, args);
	char* fmtText = (char*)malloc(reqTxtSize + 1);
	memset(fmtText, 0, reqTxtSize + 1);

	vsnprintf(fmtText, reqTxtSize + 1, fmt, args);

	auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth   = ImGui::CalcTextSize(fmtText).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);

    Text(fmtText);
    va_end(args);
	free(fmtText);
}