#include "AndroidEnvironment.h"

#include <SDL.h>

std::string AndroidEnvironment::queryRegistry(const std::string &path) const {
    return {};
}

std::string AndroidEnvironment::path(EnvironmentPath path) const {
    const char *result = nullptr;
    if (path == PATH_ANDROID_STORAGE_INTERNAL) {
        result = SDL_AndroidGetInternalStoragePath();
    } else if (path == PATH_ANDROID_STORAGE_EXTERNAL) {
        result = SDL_AndroidGetExternalStoragePath();
    }

    // TODO(captainurist): No PATH_HOME on Android? Verify & write a comment here.

    if (result)
        return result;
    return {};
}

std::string AndroidEnvironment::getenv(const std::string &key) const {
    const char *result = SDL_getenv(key.c_str());
    if (result)
        return result;
    return {};
}

std::unique_ptr<Environment> Environment::createStandardEnvironment() {
    return std::make_unique<AndroidEnvironment>();
}
