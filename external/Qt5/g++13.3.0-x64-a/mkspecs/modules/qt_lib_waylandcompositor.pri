QT.waylandcompositor.VERSION = 5.15.16
QT.waylandcompositor.name = QtWaylandCompositor
QT.waylandcompositor.module = Qt5WaylandCompositor
QT.waylandcompositor.libs = $$QT_MODULE_LIB_BASE
QT.waylandcompositor.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtWaylandCompositor
QT.waylandcompositor.frameworks =
QT.waylandcompositor.bins = $$QT_MODULE_BIN_BASE
QT.waylandcompositor.plugin_types = wayland-graphics-integration-server wayland-hardware-layer-integration
QT.waylandcompositor.depends = core gui quick qml
QT.waylandcompositor.run_depends = xkbcommon_support_private
QT.waylandcompositor.uses = wayland-server xkbcommon
QT.waylandcompositor.module_config = v2 staticlib
QT.waylandcompositor.DEFINES = QT_WAYLANDCOMPOSITOR_LIB
QT.waylandcompositor.enabled_features = wayland-compositor-quick
QT.waylandcompositor.disabled_features =
QT_CONFIG +=
QT_MODULES += waylandcompositor
