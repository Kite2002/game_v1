#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

#include <gameMain.h>

int main() {
	#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE); // no log output to the console by raylib
	#endif
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "game");
	SetTargetFPS(240);
	SetExitKey(KEY_NULL); // Disable Esc from closing window

	#pragma region imgui
	rlImGuiSetup(true);
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2; // Make GUI bigger
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	#pragma endregion


	if (!initGame()) {
		return 0;
	}

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		#pragma region imgui
		rlImGuiBegin();
		rlImGuiEnd();
		#pragma endregion

		if (!updateGame()) {
			CloseWindow();
		}

		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
	closeGame();
	return 0;
}