/*
 * Copyright 2020-2024 Toyota Connected North America
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

#include "carverx5_cnc_plugin.h"

#include <filesystem>
#include <string>

#include <sys/utsname.h>

#include "plugins/common/common.h"

namespace carverx5_cnc_plugin {

// static
void CarverX5CncPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrar *registrar) {
  auto plugin = std::make_unique<CarverX5CncPlugin>();

  SetUp(registrar->messenger(), plugin.get());

  registrar->AddPlugin(std::move(plugin));
}

CarverX5CncPlugin::CarverX5CncPlugin() { spdlog::debug("[CarverX5CncPlugin]"); }

CarverX5CncPlugin::~CarverX5CncPlugin() = default;

// Get Platform version.
ErrorOr<std::string> CarverX5CncPlugin::GetPlatformVersion() {
  utsname uname_data = {};
  uname(&uname_data);
  return "Linux " + std::string(uname_data.version);
}

// Get TTY
ErrorOr<std::string> CarverX5CncPlugin::GetTty() {
  namespace fs = std::filesystem;

  for (const auto &entry : fs::directory_iterator("/dev")) {
    if (entry.is_character_file() &&
        entry.path().filename().string().find("ttyRPM") != std::string::npos) {
      return entry.path().string();
    }
  }

  return FlutterError("Fail", "No ttyRPM device found");
}

} // namespace carverx5_cnc_plugin