#include <SDL3/SDL.h>
#include <print>

constexpr int kScreenWidth{640};
constexpr int kScreenHeight{480};

SDL_Window *gWindow{nullptr};
SDL_Surface *gScreenSurface{nullptr};

int main(int, char **)
{
    std::println("SDL compiled version: {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    std::println("SDL runtime  version: {}.{}.{}", SDL_VERSIONNUM_MAJOR(SDL_GetVersion()), SDL_VERSIONNUM_MINOR(SDL_GetVersion()), SDL_VERSIONNUM_MICRO(SDL_GetVersion()));

    std::println("NumVideoDrivers {}", SDL_GetNumVideoDrivers());
    for (auto i = 0; i < SDL_GetNumVideoDrivers(); i++) {
        std::println(" - {}", SDL_GetVideoDriver(i));
    }

    std::println("NumAudioDrivers {}", SDL_GetNumAudioDrivers());
    for (auto i = 0; i < SDL_GetNumAudioDrivers(); i++) {
        std::println(" - {}", SDL_GetAudioDriver(i));
    }

    std::println("NumRenderDrivers {}", SDL_GetNumRenderDrivers());
    for (auto i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        std::println(" - {}", SDL_GetRenderDriver(i));
    }

    std::println("NumGPUDrivers {}", SDL_GetNumGPUDrivers());
    for (auto i = 0; i < SDL_GetNumGPUDrivers(); i++) {
        std::println(" - {}", SDL_GetGPUDriver(i));
    }


    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return -1;
    }

    if (gWindow = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0); gWindow == nullptr)
    {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return -1;
    }
    else
    {
        gScreenSurface = SDL_GetWindowSurface(gWindow);
    }

    bool quit = false;
    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            // If event is quit type
            if (e.type == SDL_EVENT_QUIT)
            {
                // End the main loop
                quit = true;
            }

            SDL_FillSurfaceRect( gScreenSurface, nullptr, SDL_MapSurfaceRGB( gScreenSurface, 0xFF, 0xAA, 0xFF ) );
            SDL_UpdateWindowSurface( gWindow );
        }
    }

    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}
