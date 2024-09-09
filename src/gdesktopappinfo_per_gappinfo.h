#ifndef GDESKTOPAPPINFO_PER_GAPPINFO_H
#define GDESKTOPAPPINFO_PER_GAPPINFO_H

#include <gio/gappinfo.h>

#define GDesktopAppInfo GAppInfo

//#define G_DESKTOP_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_DESKTOP_APP_INFO, GDesktopAppInfo))
//#define G_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_APP_INFO, GAppInfo))
#define G_DESKTOP_APP_INFO(o) G_APP_INFO(o)


//#define G_IS_DESKTOP_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_DESKTOP_APP_INFO))
//#define G_IS_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_APP_INFO))
#define G_IS_DESKTOP_APP_INFO(o) G_IS_APP_INFO(o)




//GType g_desktop_app_info_get_type (void) G_GNUC_CONST;
//GType g_app_info_get_type (void) G_GNUC_CONST;
//#define g_desktop_app_info_get_type() g_app_info_get_type()


//const char * g_desktop_app_info_get_filename (GDesktopAppInfo *info);
//const char * g_app_info_get_filename (GAppInfo *info); // doesn't exist
#define g_desktop_app_info_get_filename(info) dummy_app_info_get_filename(info)
static const char * dummy_app_info_get_filename (GAppInfo *info)
{
	g_warning("Using dummy_app_info_get_filename(GAppInfo *info) for %p", info);
	return NULL;
}


//GDesktopAppInfo *g_desktop_app_info_new_from_keyfile (GKeyFile *key_file);
//GAppInfo *g_app_info_new_from_keyfile  (GKeyFile *key_file); // doesn't exist
#define g_desktop_app_info_new_from_keyfile(key_file) dummy_app_info_new_from_keyfile(key_file)

static GAppInfo *dummy_app_info_new_from_keyfile(GKeyFile *key_file)
{
	g_warning("Using dummy_app_info_new_from_keyfile(GKeyFile *key_file) for %p", key_file);
	return NULL;
}


// called at: base/fm-file-launcher.c:76: app = (GAppInfo*)g_desktop_app_info_new_from_filename(file_or_id);
// --> return type can be GAppInfo* instead of GAppInfo*
//GDesktopAppInfo *g_desktop_app_info_new_from_filename (const char *filename);
//GAppInfo *g_app_info_new_from_filename (const char *filename); // doesn't exist
#define g_desktop_app_info_new_from_filename(filename) dummy_app_info_new_from_filename(filename)

static GAppInfo *dummy_app_info_new_from_filename(const char *filename)
{
	g_warning("Using dummy_app_info_new_from_filename(const char *filename) for %s", filename);
	return NULL;
}

//GDesktopAppInfo *g_desktop_app_info_new (const char *desktop_id);
//GAppInfo *g_app_info_new (const char *desktop_id); // doesn't exist
#define g_desktop_app_info_new(desktop_id) dummy_app_info_new(desktop_id)

static GAppInfo *dummy_app_info_new(const char *desktop_id)
{
	g_warning("Using dummy_app_info_new(const char *desktop_id) for %s", desktop_id);
	return NULL;
}

#endif

