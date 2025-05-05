/*
 * Copyright 2024-2025 Toyota Connected North America
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "generated_plugin_registrant.h"

#include "config/plugins.h"

static constexpr char kKeyId[] = "id";
static constexpr char kKeyViewType[] = "viewType";
static constexpr char kKeyDirection[] = "direction";
static constexpr char kKeyWidth[] = "width";
static constexpr char kKeyHeight[] = "height";
static constexpr char kKeyParams[] = "params";
static constexpr char kKeyTop[] = "top";
static constexpr char kKeyLeft[] = "left";

static constexpr bool kPlatformViewDebug = false;

void PluginsApiRegisterPlugins(FlutterDesktopEngineRef engine) {
  (void)engine;
#if ENABLE_PLUGIN_CARVERX5_CNC
  CarverX5CncPluginCApiRegisterWithRegistrar(
      FlutterDesktopGetPluginRegistrar(engine, ""));
#endif
}

void PluginsAoiPlatformViewCreate(
    FlutterDesktopEngineRef engine,
    const std::string& flutter_asset_directory,
    const flutter::EncodableValue* const arguments,
    const PlatformViewAddListener addListener,
    const PlatformViewRemoveListener removeListener,
    void* platform_view_context,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  int32_t id = 0;
  std::string viewType;
  int32_t direction = 0;
  double top = 0;
  double left = 0;
  double width = 0;
  double height = 0;
  std::vector<uint8_t> params{};
  if (kPlatformViewDebug) {
    plugin_common::Encodable::PrintFlutterEncodableValue(
        "PluginsAoiPlatformViewCreate", *arguments);
  }

  const auto args = std::get_if<flutter::EncodableMap>(arguments);

  for (const auto& [fst, snd] : *args) {
    if (const auto key = std::get<std::string>(fst);
        key == kKeyDirection && std::holds_alternative<int32_t>(snd)) {
      direction = std::get<int32_t>(snd);
    } else if (key == kKeyHeight && std::holds_alternative<double>(snd)) {
      height = std::get<double>(snd);
    } else if (key == kKeyId && std::holds_alternative<int32_t>(snd)) {
      id = std::get<int32_t>(snd);
    } else if (key == kKeyParams &&
               std::holds_alternative<std::vector<uint8_t>>(snd)) {
      params = std::get<std::vector<uint8_t>>(snd);
    } else if (key == kKeyViewType &&
               std::holds_alternative<std::string>(snd)) {
      viewType.assign(std::get<std::string>(snd));
    } else if (key == kKeyWidth && std::holds_alternative<double>(snd)) {
      width = std::get<double>(snd);
    } else if (key == kKeyTop && std::holds_alternative<double>(snd)) {
      top = std::get<double>(snd);
    } else if (key == kKeyLeft && std::holds_alternative<double>(snd)) {
      left = std::get<double>(snd);
    } else {
      plugin_common::Encodable::PrintFlutterEncodableValue(
          "PluginsAoiPlatformViewCreate unknown", *arguments);
    }
  }

  if (width == 0 || height == 0) {
    spdlog::critical(
        "[platform_views_handler] UiKitView is not supported.  Change to "
        "AndroidView or PlatformView");
    exit(EXIT_FAILURE);
  }

  auto registrar = FlutterDesktopGetPluginRegistrar(engine, viewType.c_str());

  // Register platform view listener here

  {
    (void)flutter_asset_directory;
    (void)addListener;
    (void)removeListener;
    (void)platform_view_context;
    (void)registrar;
    (void)id;
    (void)direction;
    (void)top;
    (void)left;
    (void)width;
    (void)height;
    (void)params;
    spdlog::error("Platform View type not registered: {}", viewType);
    result->NotImplemented();
  }
}