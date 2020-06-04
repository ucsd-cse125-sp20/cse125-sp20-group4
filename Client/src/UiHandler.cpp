#include "UiHandler.h"
#include "../src/imgui/fonts/IconsFontAwesome5.h"
#include "../src/imgui/fonts/IconsMaterialDesign.h"
#include "../src/imgui/fonts/IconsForkAwesome.h"
#include "Window.h"

/*ImFont* UiHandler::baseFont;
ImFont* UiHandler::TitleFont;*/

void UiHandler::initialize() {
    // Imgui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Window::window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 35.0f; // Use if you want to make the icon monospaced
    io.Fonts->AddFontFromFileTTF("fonts/Bangers-Regular.ttf", 30.0f);

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 30.0f, &config, icon_ranges);

    //static const ImWchar icon_ranges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
    //io.Fonts->AddFontFromFileTTF("fonts/MaterialIcons-Regular.ttf", 30.0f, &config, icon_ranges);

    //static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
    //io.Fonts->AddFontFromFileTTF("fonts/forkawesome-webfont.ttf", 30.0f, &config, icon_ranges);
    io.Fonts->Build();
}

void UiHandler::cleanup() {
    //Imgui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void UiHandler::drawGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
    style.FrameBorderSize = 0;
    style.PopupBorderSize = 0;
    
    switch (Window::world->phase.state) {
    case START_STATE:
        drawTitle();
        drawReadyUp();
        break;
    case READY_STATE:
        drawPlayerInfo();
        drawReadyUp();
        break;
    case ROUND_STATE:
        drawPlayerInfo();
        break;
    case END_STATE:
        drawEnd();
        drawReadyUp();
        break;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void UiHandler::drawTitle() {
    bool* p_open = new bool(true);
    const float DISTANCE = 10.0f;
    static int corner = 1;
    int money = Window::money;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("Title", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {

        ImGui::Text("GAME Title");
        ImGui::End();
    }
}
void UiHandler::drawPlayerInfo() {
    bool* p_open = new bool(true);
    const float DISTANCE = 10.0f;
    int corner = 2;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("Player Overlay", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        switch (Window::holding) {
        case 0:
            ImGui::Text("Pick Up an Item");
            break;
        case 1:
            ImGui::Text("You are holding a RED Item");
            break;
        case 2:
            ImGui::Text("You are holding a GREEN Item");
            break;
        case 3:
            ImGui::Text("You are holding a BLUE Item");
            break;
        case 4:
            ImGui::Text("You are holding a BARRICADE");
            break;
        }
        ImGui::Text(ICON_FA_DOLLAR_SIGN " %d", Window::money);
        ImGui::End();
    }

    corner = 1;
    window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("TP Overlay", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text(ICON_FA_TOILET_PAPER " %d", Window::world->phase.health);
        ImGui::End();
    }


    window_pos = ImVec2(io.DisplaySize.x*0.5f, 0.0f);
    window_pos_pivot = ImVec2(0.5f,0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("Round Overlay", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text("Round: %d",Window::world->phase.wave);
        ImGui::End();
    }
}
void UiHandler::drawEnd() {
    bool* p_open = new bool(true);
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("End Game", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text("Game Over");
        ImGui::Text("Score: %d", Window::world->phase.count);
        ImGui::End();
    }
}
void UiHandler::drawReadyUp() {
    bool* p_open = new bool(true);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    window_pos = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0.5f, -1.0f));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("2nd Up Menu", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        // TODO check if readied up
        if (Window::ready) {
            ImGui::Text(ICON_FA_CHECK "%d of 5 Ready", Window::world->phase.count);
        } else {
            ImGui::Text("Press R to ready up");
        }
        ImGui::End();
    }
}
void UiHandler::drawRound() {

}
