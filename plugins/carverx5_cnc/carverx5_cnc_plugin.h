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

#ifndef FLUTTER_PLUGIN_CARVERX5_CNC_PLUGIN_H
#define FLUTTER_PLUGIN_CARVERX5_CNC_PLUGIN_H

#include <flutter/plugin_registrar.h>

#include "messages.h"

namespace carverx5_cnc_plugin {
class CarverX5CncPlugin final : public flutter::Plugin, public CarverX5CncApi {
public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar *registrar);

  CarverX5CncPlugin();

  ~CarverX5CncPlugin() override;

  // Get Platform version.
  ErrorOr<std::string> GetPlatformVersion() override;

  // Get TTY
  ErrorOr<std::string> GetTty() override;

  // Disallow copy and assign.
  CarverX5CncPlugin(const CarverX5CncPlugin &) = delete;

  CarverX5CncPlugin &operator=(const CarverX5CncPlugin &) = delete;

private:
};
} // namespace carverx5_cnc_plugin

#endif // FLUTTER_PLUGIN_CARVERX5_CNC_PLUGIN_H
