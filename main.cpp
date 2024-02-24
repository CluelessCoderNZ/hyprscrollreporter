#include <hyprland/src/plugins/PluginAPI.hpp>

inline HANDLE PHANDLE = nullptr;

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    // ...

    return {
        "HyprScrollReporter", 
        "A simple plugin that reports the current workspace transition percentage.", 
        "Connor Macdonald <kiwiconnor@proton.me>", 
        "0.1"
    };
}

APICALL EXPORT void PLUGIN_EXIT() {
    // ...
}
