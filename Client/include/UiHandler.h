#pragma once
#include "../src/imgui/imgui.h"
#include "../src/imgui/imgui_impl_glfw.h"
#include "../src/imgui/imgui_impl_opengl3.h"
class UiHandler
{
public:
	static void drawGui();
	static void cleanup();
	static void initialize();
private:
	static void drawTitle();
	static void drawPlayerInfo();
	static void drawEnd();
	static void drawReadyUp();
	static void drawRound();
};