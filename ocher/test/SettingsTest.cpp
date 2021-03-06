#include "settings/Settings.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("SecureLevel") {
    Settings s;

    // unrecognized is unchanged
    Settings defaultSettings;
    s.load("{ \"SecureLevel\": \"garbage\" }");
    CHECK(s.secureLevel == defaultSettings.secureLevel);
    s.load("{ \"SecureLevel\": 42 }");
    CHECK(s.secureLevel == defaultSettings.secureLevel);

    s.load("{ \"SecureLevel\": \"open\" }");
    CHECK(s.secureLevel == Settings::SecureLevelOpen);

    s.load("{ \"SecureLevel\": \"personal\" }");
    CHECK(s.secureLevel == Settings::SecureLevelPersonal);

    s.load("{ \"SecureLevel\": \"private\" }");
    CHECK(s.secureLevel == Settings::SecureLevelPrivate);
}

TEST_CASE("TrackReading") {
    Settings s;

    s.load("{ \"TrackReading\": 0 }");
    CHECK(s.trackReading == 0);

    s.load("{ \"TrackReading\": 1 }");
    CHECK(s.trackReading == 1);

    // unrecognized is unchanged
    s.load("{ \"TrackReading\": \"foo\" }");
    CHECK(s.trackReading == 1);
}
