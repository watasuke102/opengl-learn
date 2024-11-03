#include "graphic.hpp"
#include <IUnityGraphics.h>
#include <IUnityInterface.h>

#define UNITY_EXPORT extern "C" UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API

namespace {
IUnityInterfaces* unity;
IUnityGraphics*   graphics;

void handle_device_event(UnityGfxDeviceEventType event_type) {
  if (event_type == kUnityGfxDeviceEventInitialize) {
    init();
  }
}
} // namespace

// Unity-defined functions
UNITY_EXPORT void UnityPluginLoad(IUnityInterfaces* i) {
  unity    = i;
  graphics = unity->Get<IUnityGraphics>();
  graphics->RegisterDeviceEventCallback(handle_device_event);
  handle_device_event(kUnityGfxDeviceEventInitialize);
}
UNITY_EXPORT void UnityPluginUnload() {
  graphics->UnregisterDeviceEventCallback(handle_device_event);
}
