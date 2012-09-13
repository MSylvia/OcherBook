#include <exception>
#include "SDL/SDL.h"

#include "clc/support/Logger.h"

#include "ocher/ux/fb/sdl/FbSdl.h"


FbSdl::FbSdl() :
    m_sdl(0),
    m_screen(0)
{
}

FbSdl::~FbSdl()
{
    if (m_sdl)
        SDL_Quit();
}

bool FbSdl::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        const char *err = SDL_GetError();
        clc::Log::error("ocher.sdl", "SDL_Init failed: %s", err);
        return false;
    }

    // TODO:  store window size in user settings
    // TODO:  move this to SDL event thread
    m_screen = SDL_SetVideoMode(600, 800, 8, SDL_SWSURFACE);
    if (! m_screen) {
        const char *err = SDL_GetError();
        clc::Log::error("ocher.sdl", "SDL_SetVideoMode failed: %s", err);
        SDL_Quit();
        return false;
    }
    m_mustLock = SDL_MUSTLOCK(m_screen);
    clear();
    m_sdl = 1;
    return true;
}

inline unsigned int FbSdl::height()
{
    return m_screen->h;
}

inline unsigned int FbSdl::width()
{
    return m_screen->w;
}

unsigned int FbSdl::dpi()
{
    return 120;  // TODO
}

void FbSdl::clear()
{
    clc::Log::debug("ocher.sdl", "clear");
    SDL_FillRect(m_screen, NULL, 0x0);
    SDL_UpdateRect(m_screen, 0, 0, 0, 0);
}

void FbSdl::blit(unsigned char *p, int x, int y, int w, int h)
{
    clc::Log::trace("ocher.sdl", "blit");
    if (m_mustLock) {
        if (SDL_LockSurface(m_screen) < 0) {
            return;
        }
    }

    const int fbw = width();
    if (x + w >= fbw) {
        if (x >= fbw)
            return;
        w = fbw - x;
    }
    if (x < 0) {
        // TODO
    }
    const int fbh = height();
    if (y + h >= fbh) {
        if (y >= fbh)
            return;
        h = fbh - y;
    }
    if (y < 0) {
        // TODO
    }

    // TODO: beware SDL's bpp
    for (int i = 0; i < h; ++i) {
        memcpy(((unsigned char*)m_screen->pixels) + y*m_screen->pitch + x, p, w);
        y++;
        p = p + w;
    }
    if (m_mustLock) {
        SDL_UnlockSurface(m_screen);
    }
}

int FbSdl::update(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool /*full*/)
{
    clc::Log::debug("ocher.sdl", "update");
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_UpdateRects(m_screen, 1, &dest);
    return 0;
}
