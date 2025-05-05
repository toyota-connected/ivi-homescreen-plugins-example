
add_library(plugin_carverx5_cnc STATIC
        carverx5_cnc_plugin_c_api.cc
        carverx5_cnc_plugin.cc
        messages.cc
)

target_link_libraries(plugin_carverx5_cnc PUBLIC
        platform_homescreen
        flutter
)