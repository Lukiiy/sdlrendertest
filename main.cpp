#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string backend = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--v") {
            backend = "vulkan";
        } else if (arg == "--o") {
            backend = "opengl";
        }
    }

    if (!backend.empty()) SDL_SetHint(SDL_HINT_RENDER_DRIVER, backend.c_str());

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "error when initializing sdl: " << SDL_GetError() << std::endl;

        return 0;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("a.. polygon, probably", 400, 400, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        std::cerr << "error when init: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return 1;
    }

    const char* driver = SDL_GetRendererName(renderer);
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        int w = 0;
        int h = 0;

        SDL_GetWindowSize(window, &w, &h);

        float centerX = static_cast<float>(w) / 2;
        float centerY = static_cast<float>(h) / 2;

        SDL_Vertex vertices[3] = { // the classic colorful triangle
            { // red
                { centerX, centerY - 120.0f },
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f }
            }, { // green (bottom right)
                { centerX + 120.0f, centerY + 80.0f },
                { 0.0f, 1.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f }
            }, { // blue (bottom left)
                { centerX - 120.0f, centerY + 80.0f },
                { 0.0f, 0.0f, 1.0f, 1.0f },
                { 0.0f, 0.0f }
            }
        };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // bg
        SDL_RenderClear(renderer);
        SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);

        const char* label = driver ? driver : "what";
        float textX = (centerX / 2.0f) - ((SDL_strlen(label) * 8.0f) / 2.0f);
        float textY = (centerY / 2.0f) + 60.0f;

        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, textX, textY, label);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f); // reset scale
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}