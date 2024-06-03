#define WLR_USE_UNSTABLE

#include <unistd.h>
#include <vector>
#include <iostream>
#include <map>

#include <hyprland/src/includes.hpp>
#include <any>

#define private public
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/EventManager.hpp>
#include <hyprland/src/helpers/Monitor.hpp>
#include <hyprland/src/render/Renderer.hpp>
#undef private

#include <hyprland/src/plugins/PluginAPI.hpp>

inline HANDLE PHANDLE = nullptr;


APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

struct MonitorTransition {
    int previous_workspace = -1;
    int active_workspace = -1;
    Vector2D unit_offset;
};

std::map<uint64_t, MonitorTransition> g_lastTransition;

bool transitionEqual(MonitorTransition* a, MonitorTransition* b) {
    return a->previous_workspace == b->previous_workspace 
        && a->active_workspace == b->active_workspace
        && a->unit_offset.x == b->unit_offset.x
        && b->unit_offset.y == b->unit_offset.y;
}

bool getCurrentTransition(std::shared_ptr<CMonitor> monitor, MonitorTransition* transition) {
    PHLWORKSPACE active = monitor->activeWorkspace;
    if (active == nullptr) return false;
    
    Vector2D offset = active->m_vRenderOffset.value();

    transition->previous_workspace = active->m_sPrevWorkspace.iID;
    transition->active_workspace = active->m_iID;
    transition->unit_offset.x = offset.x / monitor->vecPixelSize.x;
    transition->unit_offset.y = offset.y / monitor->vecPixelSize.y;

    return true;
}

void onNewTransitionValue(std::shared_ptr<CMonitor> monitor, MonitorTransition* transition) {
    SHyprIPCEvent event;
    event.event = "workspaceoffset";
    event.data = std::format("{},{},{},{},{:.6f},{:.6f}",
        monitor->szName,
        monitor->ID,
        transition->active_workspace,
        transition->previous_workspace,
        transition->unit_offset.x,
        transition->unit_offset.y
    );
    
    g_pEventManager->postEvent(event);
}

void onRenderStage(eRenderStage stage) {
    if (stage != RENDER_PRE_WINDOWS)
        return;

    
    for (auto& monitor : g_pCompositor->m_vMonitors) {
        MonitorTransition transition;
        if (!getCurrentTransition(monitor, &transition)) continue;

        if(!g_lastTransition.contains(monitor->ID) 
            || !transitionEqual(&g_lastTransition[monitor->ID], &transition)) {
            g_lastTransition[monitor->ID] = transition;
            onNewTransitionValue(monitor, &transition);
        }
        
    }
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    HyprlandAPI::registerCallbackDynamic(PHANDLE, "render", [&](void* self, SCallbackInfo& info, std::any data) { onRenderStage(std::any_cast<eRenderStage>(data)); });

    return {
        "HyprScrollReporter", 
        "A simple plugin that reports the current workspace transition offset.", 
        "Connor Macdonald <kiwiconnor@proton.me>", 
        "0.1"
    };
}

APICALL EXPORT void PLUGIN_EXIT() {
    // Nothing to do
}
