/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_UXCONTROLLER_H
#define OCHER_UX_FB_UXCONTROLLER_H

#include "ocher/ux/Controller.h"
#include "ocher/ux/fb/BootActivityFb.h"
#include "ocher/ux/fb/HomeActivityFb.h"
#include "ocher/ux/fb/LibraryActivityFb.h"
#include "ocher/ux/fb/NavBar.h"
#include "ocher/ux/fb/ReadActivityFb.h"
#include "ocher/ux/fb/SettingsActivityFb.h"
#include "ocher/ux/fb/SleepActivityFb.h"
#include "ocher/ux/fb/SyncActivityFb.h"
#include "ocher/ux/fb/SystemBar.h"


/**
 */
class UxControllerFb : public UxController {
public:
    UxControllerFb();
    ~UxControllerFb();

    const char *getName() const
    {
        return m_name.c_str();
    }

    bool init();

    FrameBuffer *getFrameBuffer()
    {
        return m_frameBuffer;
    }
    FontEngine *getFontEngine()
    {
        return m_fontEngine;
    }
    Renderer *getRenderer()
    {
        return m_renderer;
    }

    SystemBar *m_systemBar;
    NavBar *m_navBar;

    void setNextActivity(enum ActivityType a);

protected:
    ActivityFb *m_activity;

    BootActivityFb *m_bootActivity;
    HomeActivityFb *m_homeActivity;
    LibraryActivityFb *m_libraryActivity;
    ReadActivityFb *m_readActivity;
    SettingsActivityFb *m_settingsActivity;
    SleepActivityFb *m_sleepActivity;
    SyncActivityFb *m_syncActivity;

    Renderer *m_renderer;
    FontEngine *m_fontEngine;
    FrameBuffer *m_frameBuffer;
    FbScreen m_screen;

    std::string m_name;
};

#endif
