
add_library(Qt5::VirtualCanBusPlugin MODULE IMPORTED)

set(_Qt5VirtualCanBusPlugin_MODULE_DEPENDENCIES "Network;SerialBus;Core")

foreach(_module_dep ${_Qt5VirtualCanBusPlugin_MODULE_DEPENDENCIES})
    if(NOT Qt5${_module_dep}_FOUND)
        find_package(Qt5${_module_dep}
            1.0.0 ${_Qt5SerialBus_FIND_VERSION_EXACT}
            ${_Qt5SerialBus_DEPENDENCIES_FIND_QUIET}
            ${_Qt5SerialBus_FIND_DEPENDENCIES_REQUIRED}
            PATHS "${CMAKE_CURRENT_LIST_DIR}/.." NO_DEFAULT_PATH
        )
    endif()
endforeach()

_qt5_SerialBus_process_prl_file(
    "${_qt5SerialBus_install_prefix}/plugins/canbus/libqtvirtualcanbus.prl" RELEASE
    _Qt5VirtualCanBusPlugin_STATIC_RELEASE_LIB_DEPENDENCIES
    _Qt5VirtualCanBusPlugin_STATIC_RELEASE_LINK_FLAGS
)


set_property(TARGET Qt5::VirtualCanBusPlugin PROPERTY INTERFACE_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Qt5SerialBus_VirtualCanBusPlugin_Import.cpp"
)

_populate_SerialBus_plugin_properties(VirtualCanBusPlugin RELEASE "canbus/libqtvirtualcanbus.a" FALSE)

list(APPEND Qt5SerialBus_PLUGINS Qt5::VirtualCanBusPlugin)
set_property(TARGET Qt5::SerialBus APPEND PROPERTY QT_ALL_PLUGINS_canbus Qt5::VirtualCanBusPlugin)
# $<GENEX_EVAL:...> wasn't added until CMake 3.12, so put a version guard around it
if(CMAKE_VERSION VERSION_LESS "3.12")
    set(_manual_plugins_genex "$<TARGET_PROPERTY:QT_PLUGINS>")
    set(_plugin_type_genex "$<TARGET_PROPERTY:QT_PLUGINS_canbus>")
    set(_no_plugins_genex "$<TARGET_PROPERTY:QT_NO_PLUGINS>")
else()
    set(_manual_plugins_genex "$<GENEX_EVAL:$<TARGET_PROPERTY:QT_PLUGINS>>")
    set(_plugin_type_genex "$<GENEX_EVAL:$<TARGET_PROPERTY:QT_PLUGINS_canbus>>")
    set(_no_plugins_genex "$<GENEX_EVAL:$<TARGET_PROPERTY:QT_NO_PLUGINS>>")
endif()
set(_user_specified_genex
    "$<IN_LIST:Qt5::VirtualCanBusPlugin,${_manual_plugins_genex};${_plugin_type_genex}>"
)
set(_user_specified_genex_versionless
    "$<IN_LIST:Qt::VirtualCanBusPlugin,${_manual_plugins_genex};${_plugin_type_genex}>"
)
string(CONCAT _plugin_genex
    "$<$<OR:"
        # Add this plugin if it's in the list of manually specified plugins or in the list of
        # explicitly included plugin types.
        "${_user_specified_genex},"
        "${_user_specified_genex_versionless},"
        # Add this plugin if all of the following are true:
        # 1) the list of explicitly included plugin types is empty
        # 2) the QT_PLUGIN_EXTENDS property for the plugin is empty or equal to one of the modules
        #    listed in the plugin's PLUGIN_EXTEND qmake variable
        # 3) the user hasn't explicitly excluded the plugin.
        # TODO: Note that the current implementation of (2) is not entirely correct QTBUG-93501
        "$<AND:"
            "$<STREQUAL:${_plugin_type_genex},>,"
            "$<OR:"
                
                "$<STREQUAL:$<TARGET_PROPERTY:Qt5::VirtualCanBusPlugin,QT_PLUGIN_EXTENDS>,>"
            ">,"
            "$<NOT:$<IN_LIST:Qt5::VirtualCanBusPlugin,${_no_plugins_genex}>>,"
            "$<NOT:$<IN_LIST:Qt::VirtualCanBusPlugin,${_no_plugins_genex}>>"
        ">"
    ">:Qt5::VirtualCanBusPlugin>"
)
set_property(TARGET Qt5::SerialBus APPEND PROPERTY INTERFACE_LINK_LIBRARIES
    ${_plugin_genex}
)
set_property(TARGET Qt5::VirtualCanBusPlugin APPEND PROPERTY INTERFACE_LINK_LIBRARIES
    "Qt5::Network;Qt5::SerialBus;Qt5::Core"
)
set_property(TARGET Qt5::VirtualCanBusPlugin PROPERTY QT_PLUGIN_TYPE "canbus")
set_property(TARGET Qt5::VirtualCanBusPlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::VirtualCanBusPlugin PROPERTY QT_PLUGIN_CLASS_NAME "VirtualCanBusPlugin")
