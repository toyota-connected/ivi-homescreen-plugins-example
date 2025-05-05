
#
# Copyright 2020-2025 Toyota Connected North America
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${PROJECT_BINARY_DIR}/config
)

find_package(PkgConfig REQUIRED)

macro(ENABLE_PLUGIN plugin)

    string(TOUPPER ${plugin} ucase_plugin)

    set(ENABLE_PLUGIN_${ucase_plugin} ON)

    if (${plugin} MATCHES ".*_view")
        message(STATUS "View: ${plugin}")
    else ()
        message(STATUS "Plugin: ${plugin}")
    endif ()

    add_subdirectory(${plugin})

    target_compile_definitions(plugin_${plugin} PUBLIC ENABLE_PLUGIN_${ucase_plugin})

    add_sanitizers(plugin_${plugin})

    if (IPO_SUPPORT_RESULT)
        set_property(TARGET plugin_${plugin} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()


    target_link_libraries(plugins PUBLIC plugin_${plugin})

    target_compile_definitions(plugins PUBLIC ENABLE_PLUGIN_${ucase_plugin})

    #print_target_properties(plugin_${plugin})

endmacro(ENABLE_PLUGIN)

macro(PLUGIN_OPTION plugin description default)

    string(TOUPPER ${plugin} ucase_plugin)

    if (ENABLE_PLUGINS)
        option(BUILD_PLUGIN_${ucase_plugin} ${description} ${default})
    else ()
        set(BUILD_PLUGIN_${ucase_plugin} OFF)
    endif ()

    set(ENABLE_PLUGIN_${ucase_plugin} OFF)
    if (BUILD_PLUGIN_${ucase_plugin})
        ENABLE_PLUGIN(${plugin})
    endif ()

endmacro(PLUGIN_OPTION)

# target used at top level
add_library(plugins generated_plugin_registrant.cc)
target_include_directories(plugins PUBLIC . ${CMAKE_BINARY_DIR} ${PLUGINS_DIR})
target_link_libraries(plugins PUBLIC rapidjson spdlog flutter)

# Plugins
PLUGIN_OPTION(common "Includes Common Plugin" OFF)

PLUGIN_OPTION(carverx5_cnc "Include Carver X5 CNC plugin" OFF)


#
# Configuration File
#
configure_file(config_plugins.h.in ${PROJECT_BINARY_DIR}/config/plugins.h)
