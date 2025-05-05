/*
 * Copyright 2024 Toyota Connected North America
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

#include "time_tools.h"

#include <chrono>
#include <ctime>

namespace plugin_common::TimeTools {

int64_t GetEpochTimeInSeconds() {
  const auto now = std::chrono::system_clock::now();
  const auto epoch = now.time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
  return seconds.count();
}

std::string GetCurrentTimeString() {
  const std::chrono::system_clock::time_point now =
      std::chrono::system_clock::now();
  const auto duration = now.time_since_epoch();
  const auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() %
      1000;
  const time_t tt = std::chrono::system_clock::to_time_t(now);
  const tm local_tm = *localtime(&tt);

  std::string time_start;
  time_start.resize(80);
  const size_t len =
      strftime(&time_start[0], time_start.size(), "%Y_%m%d_%H%M%S_", &local_tm);
  if (len > 0) {
    time_start.resize(len);
  }

  // Add milliseconds to make sure the filename is unique
  return time_start + std::to_string(millis);
}

}  // namespace plugin_common::TimeTools
