// This file was generated by qtwaylandscanner
// source file is /home/steve/BuildQt/qt-everywhere-src-5.15.16/qtwayland/src/extensions/touch-extension.xml

#ifndef QT_WAYLAND_SERVER_TOUCH_EXTENSION
#define QT_WAYLAND_SERVER_TOUCH_EXTENSION

#include "wayland-server-core.h"
#include <QtWaylandCompositor/private/wayland-touch-extension-server-protocol.h>
#include <QByteArray>
#include <QMultiMap>
#include <QString>

#ifndef WAYLAND_VERSION_CHECK
#define WAYLAND_VERSION_CHECK(major, minor, micro) \
    ((WAYLAND_VERSION_MAJOR > (major)) || \
    (WAYLAND_VERSION_MAJOR == (major) && WAYLAND_VERSION_MINOR > (minor)) || \
    (WAYLAND_VERSION_MAJOR == (major) && WAYLAND_VERSION_MINOR == (minor) && WAYLAND_VERSION_MICRO >= (micro)))
#endif

QT_BEGIN_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wmissing-field-initializers")
QT_WARNING_DISABLE_CLANG("-Wmissing-field-initializers")

#if !defined(Q_WAYLAND_SERVER_TOUCH_EXTENSION_EXPORT)
#  if defined(QT_SHARED)
#    define Q_WAYLAND_SERVER_TOUCH_EXTENSION_EXPORT Q_DECL_EXPORT
#  else
#    define Q_WAYLAND_SERVER_TOUCH_EXTENSION_EXPORT
#  endif
#endif

namespace QtWaylandServer {
    class Q_WAYLAND_SERVER_TOUCH_EXTENSION_EXPORT qt_touch_extension
    {
    public:
        qt_touch_extension(struct ::wl_client *client, int id, int version);
        qt_touch_extension(struct ::wl_display *display, int version);
        qt_touch_extension(struct ::wl_resource *resource);
        qt_touch_extension();

        virtual ~qt_touch_extension();

        class Resource
        {
        public:
            Resource() : touch_extension_object(nullptr), handle(nullptr) {}
            virtual ~Resource() {}

            qt_touch_extension *touch_extension_object;
            qt_touch_extension *object() { return touch_extension_object; } 
            struct ::wl_resource *handle;

            struct ::wl_client *client() const { return wl_resource_get_client(handle); }
            int version() const { return wl_resource_get_version(handle); }

            static Resource *fromResource(struct ::wl_resource *resource);
        };

        void init(struct ::wl_client *client, int id, int version);
        void init(struct ::wl_display *display, int version);
        void init(struct ::wl_resource *resource);

        Resource *add(struct ::wl_client *client, int version);
        Resource *add(struct ::wl_client *client, int id, int version);
        Resource *add(struct wl_list *resource_list, struct ::wl_client *client, int id, int version);

        Resource *resource() { return m_resource; }
        const Resource *resource() const { return m_resource; }

        QMultiMap<struct ::wl_client*, Resource*> resourceMap() { return m_resource_map; }
        const QMultiMap<struct ::wl_client*, Resource*> resourceMap() const { return m_resource_map; }

        bool isGlobal() const { return m_global != nullptr; }
        bool isResource() const { return m_resource != nullptr; }

        static const struct ::wl_interface *interface();
        static QByteArray interfaceName() { return interface()->name; }
        static int interfaceVersion() { return interface()->version; }


        enum flags {
            flags_mouse_from_touch = 0x1,
        };

        void send_touch(uint32_t time, uint32_t id, uint32_t state, int32_t x, int32_t y, int32_t normalized_x, int32_t normalized_y, int32_t width, int32_t height, uint32_t pressure, int32_t velocity_x, int32_t velocity_y, uint32_t flags, const QByteArray &rawdata);
        void send_touch(struct ::wl_resource *resource, uint32_t time, uint32_t id, uint32_t state, int32_t x, int32_t y, int32_t normalized_x, int32_t normalized_y, int32_t width, int32_t height, uint32_t pressure, int32_t velocity_x, int32_t velocity_y, uint32_t flags, const QByteArray &rawdata);
        void send_configure(uint32_t flags);
        void send_configure(struct ::wl_resource *resource, uint32_t flags);

    protected:
        virtual Resource *touch_extension_allocate();

        virtual void touch_extension_bind_resource(Resource *resource);
        virtual void touch_extension_destroy_resource(Resource *resource);

        virtual void touch_extension_dummy(Resource *resource);

    private:
        static void bind_func(struct ::wl_client *client, void *data, uint32_t version, uint32_t id);
        static void destroy_func(struct ::wl_resource *client_resource);
        static void display_destroy_func(struct ::wl_listener *listener, void *data);

        Resource *bind(struct ::wl_client *client, uint32_t id, int version);
        Resource *bind(struct ::wl_resource *handle);

        static const struct ::qt_touch_extension_interface m_qt_touch_extension_interface;

        static void handle_dummy(
            ::wl_client *client,
            struct wl_resource *resource);

        QMultiMap<struct ::wl_client*, Resource*> m_resource_map;
        Resource *m_resource;
        struct ::wl_global *m_global;
        uint32_t m_globalVersion;
        struct DisplayDestroyedListener : ::wl_listener {
            qt_touch_extension *parent;
        };
        DisplayDestroyedListener m_displayDestroyedListener;
    };
}

QT_WARNING_POP
QT_END_NAMESPACE

#endif
