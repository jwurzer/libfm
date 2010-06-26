//      g-udisks-mount.c
//
//      Copyright 2010 Hong Jen Yee (PCMan) <pcman.tw@gmail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "g-udisks-mount.h"
#include "udisks-device.h"

typedef struct
{
    GUDisksMount* mnt;
    GAsyncReadyCallback callback;
    gpointer user_data;
    DBusGProxy* proxy;
    DBusGProxyCall* call;
    gboolean success;
}AsyncData;

static void g_udisks_mount_mount_iface_init(GMountIface *iface);
static void g_udisks_mount_finalize            (GObject *object);

static void g_udisks_mount_unmount_with_operation (GMount* base, GMountUnmountFlags flags, GMountOperation* mount_operation, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);

G_DEFINE_TYPE_EXTENDED (GUDisksMount, g_udisks_mount, G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_MOUNT,
                                               g_udisks_mount_mount_iface_init))


static void g_udisks_mount_class_init(GUDisksMountClass *klass)
{
    GObjectClass *g_object_class;

    g_object_class = G_OBJECT_CLASS(klass);
    g_object_class->finalize = g_udisks_mount_finalize;
}

static void g_udisks_mount_finalize(GObject *object)
{
    GUDisksMount *self;

    g_return_if_fail(object != NULL);
    g_return_if_fail(G_IS_UDISKS_MOUNT(object));

    self = G_UDISKS_MOUNT(object);
    if(self->root)
        g_object_unref(self->root);

    G_OBJECT_CLASS(g_udisks_mount_parent_class)->finalize(object);
}


static void g_udisks_mount_init(GUDisksMount *self)
{
}


GMount *g_udisks_mount_new(GUDisksVolume* vol)
{
    GUDisksMount* mnt = g_object_new(G_TYPE_UDISKS_MOUNT, NULL);
    /* we don't do g_object_ref here to prevent circular reference. */
    mnt->vol = vol;
    return mnt;
}

static gboolean g_udisks_mount_can_eject (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return mnt->vol ? mnt->vol->dev->is_ejectable : FALSE;
}

static gboolean g_udisks_mount_can_unmount (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return mnt->vol ? mnt->vol->dev->is_mounted : FALSE;
}

typedef struct
{
    GUDisksMount* mnt;
    GAsyncReadyCallback callback;
    gpointer user_data;
}EjectData;

static void on_drive_ejected(GObject* drive, GAsyncResult* res, gpointer user_data)
{
    EjectData* data = (EjectData*)user_data;
    if(data->callback)
        data->callback(data->mnt, res, data->user_data);
    g_object_unref(data->mnt);
    g_slice_free(EjectData, data);
}

static void g_udisks_mount_eject_with_operation (GMount* base, GMountUnmountFlags flags, GMountOperation* mount_operation, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    GDrive* drv = g_mount_get_drive(base);
    if(drv)
    {
        EjectData* data = g_slice_new(EjectData);
        data->mnt = g_object_ref(mnt);
        data->callback = callback;
        data->user_data = user_data;
        g_drive_eject(drv, flags, cancellable, on_drive_ejected, data);
        g_object_unref(drv);
    }
}

static gboolean g_udisks_mount_eject_with_operation_finish(GMount* base, GAsyncResult* res, GError** error)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return g_volume_eject_with_operation_finish(mnt->vol, res, error);
}

static void g_udisks_mount_eject (GMount* base, GMountUnmountFlags flags, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    g_udisks_mount_eject_with_operation(base, flags, NULL, cancellable, callback, user_data);
}

static gboolean g_udisks_mount_eject_finish(GMount* base, GAsyncResult* res, GError** error)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return g_udisks_mount_eject_with_operation_finish(base, res, error);
}

static GDrive* g_udisks_mount_get_drive (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return mnt->vol ? g_volume_get_drive(G_VOLUME(mnt->vol)) : NULL;
}

static GIcon* g_udisks_mount_get_icon (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return mnt->vol ? g_volume_get_icon(G_VOLUME(mnt->vol)) : NULL;
}

static char* g_udisks_mount_get_name (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return mnt->vol ? g_volume_get_name(G_VOLUME(mnt->vol)) : NULL;
}

static GFile* g_udisks_mount_get_root (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    if(!mnt->root && mnt->vol->dev->is_mounted && mnt->vol->dev->mount_paths)
    {
        /* TODO */
        mnt->root = g_file_new_for_path(mnt->vol->dev->mount_paths[0]);
    }
    return mnt->root ? (GFile*)g_object_ref(mnt->root) : NULL;
}

static char* g_udisks_mount_get_uuid (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return mnt->vol ? g_volume_get_uuid(G_VOLUME(mnt->vol)) : NULL;
}

static GVolume* g_udisks_mount_get_volume (GMount* base)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return (GVolume*)mnt->vol;
}

static void g_udisks_mount_guess_content_type_data_free (gpointer _data)
{

}

static void g_udisks_mount_guess_content_type (GMount* base, gboolean force_rescan, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    // g_content_type_guess_for_tree
}


static char** g_udisks_mount_guess_content_type_sync (GMount* base, gboolean force_rescan, GCancellable* cancellable, int* result_length1, GError** error)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    if(mnt->root)
    {
        char** ret;
        GFile* root = g_udisks_mount_get_root(mnt);
        ret = g_content_type_guess_for_tree(root);
        g_object_unref(root);
        return ret;
    }
    return NULL;
}

static void g_udisks_mount_remount (GMount* base, GMountMountFlags flags, GMountOperation* mount_operation, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);

}

static void unmount_callback(DBusGProxy *proxy, GError *error, gpointer user_data)
{
    AsyncData* data = (AsyncData*)user_data;
    GSimpleAsyncResult* res;
    if(error)
    {
        res = g_simple_async_result_new_from_error(data->mnt,
                                                   data->callback,
                                                   data->user_data,
                                                   error);
    }
    else
    {
        data->success = TRUE;
        res = g_simple_async_result_new(data->mnt,
                                        data->callback,
                                        data->user_data,
                                        NULL);
    }
    g_simple_async_result_complete(res);
    g_object_unref(res);

    g_object_unref(data->mnt);
    g_object_unref(data->proxy);
    g_slice_free(AsyncData, data);
}

static void on_unmount_cancelled(GCancellable* cancellable, gpointer user_data)
{
    AsyncData* data = (AsyncData*)user_data;
    dbus_g_proxy_cancel_call(data->proxy, data->call);
}

static void g_udisks_mount_unmount_with_operation (GMount* base, GMountUnmountFlags flags, GMountOperation* mount_operation, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    if(mnt->vol)
    {
        GUDisksDevice* dev = mnt->vol->dev;
        GUDisksVolumeMonitor* mon = mnt->vol->mon;
        AsyncData* data = g_slice_new(AsyncData);
        DBusGProxy* proxy = g_udisks_device_get_proxy(dev, mon->con);
        data->mnt = g_object_ref(mnt);
        data->callback = callback;
        data->user_data = user_data;
        data->proxy = proxy;

        g_signal_emit_by_name(mon, "mount-pre-unmount", mnt);
        g_signal_emit_by_name(mnt, "pre-unmount");

        data->call = org_freedesktop_UDisks_Device_filesystem_unmount_async(
                        proxy, NULL, unmount_callback, data);
        if(cancellable)
            g_signal_connect(cancellable, "cancelled", G_CALLBACK(on_unmount_cancelled), data);
    }
}

static gboolean g_udisks_mount_unmount_with_operation_finish(GMount* base, GAsyncResult* res, GError** error)
{
    /* TODO */
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    AsyncData* data = (AsyncData*)g_async_result_get_user_data(res);
    g_simple_async_result_propagate_error(res, error);
    return data->success;
}

static void g_udisks_mount_unmount (GMount* base, GMountUnmountFlags flags, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
    g_udisks_mount_unmount_with_operation(base, flags, NULL, cancellable, callback, user_data);
}

static gboolean g_udisks_mount_unmount_finish(GMount* base, GAsyncResult* res, GError** error)
{
    GUDisksMount* mnt = G_UDISKS_MOUNT(base);
    return g_udisks_mount_unmount_with_operation_finish(base, res, error);
}

void g_udisks_mount_mount_iface_init(GMountIface *iface)
{
    iface->get_root = g_udisks_mount_get_root;
    iface->get_name = g_udisks_mount_get_name;
    iface->get_icon = g_udisks_mount_get_icon;
    iface->get_uuid = g_udisks_mount_get_uuid;
    iface->get_drive = g_udisks_mount_get_drive;
    iface->get_volume = g_udisks_mount_get_volume;
    iface->can_unmount = g_udisks_mount_can_unmount;
    iface->can_eject = g_udisks_mount_can_eject;
    iface->unmount = g_udisks_mount_unmount;
    iface->unmount_finish = g_udisks_mount_unmount_finish;
    iface->unmount_with_operation = g_udisks_mount_unmount_with_operation;
    iface->unmount_with_operation_finish = g_udisks_mount_unmount_with_operation_finish;
    iface->eject = g_udisks_mount_eject;
    iface->eject_finish = g_udisks_mount_eject_finish;
    iface->eject_with_operation = g_udisks_mount_eject_with_operation;
    iface->eject_with_operation_finish = g_udisks_mount_eject_with_operation_finish;
    iface->guess_content_type = g_udisks_mount_guess_content_type;
    // iface->guess_content_type_finish = g_udisks_mount_guess_content_type_finish;
    iface->guess_content_type_sync = g_udisks_mount_guess_content_type_sync;
}
