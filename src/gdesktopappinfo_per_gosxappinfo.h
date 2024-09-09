#ifndef GDESKTOPAPPINFO_PER_GOSXAPPINFO_H
#define GDESKTOPAPPINFO_PER_GOSXAPPINFO_H

#include <gio/gosxappinfo.h>

#define GDesktopAppInfo GOsxAppInfo

//#define G_DESKTOP_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_DESKTOP_APP_INFO, GDesktopAppInfo))
//#define G_OSX_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_OSX_APP_INFO, GOsxAppInfo))
#define G_DESKTOP_APP_INFO(o) G_OSX_APP_INFO(o)


//#define G_IS_DESKTOP_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_DESKTOP_APP_INFO))
//#define G_IS_OSX_APP_INFO(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_OSX_APP_INFO))
#define G_IS_DESKTOP_APP_INFO(o) G_IS_OSX_APP_INFO(o)




//GType g_desktop_app_info_get_type (void) G_GNUC_CONST;
//GType g_osx_app_info_get_type (void) G_GNUC_CONST;
//#define g_desktop_app_info_get_type() g_osx_app_info_get_type()


//const char * g_desktop_app_info_get_filename (GDesktopAppInfo *info);
//char * g_osx_app_info_get_filename (GOsxAppInfo *info);
#define g_desktop_app_info_get_filename(info) g_osx_app_info_get_filename(info)


//GDesktopAppInfo *g_desktop_app_info_new_from_keyfile (GKeyFile *key_file);
//GOsxAppInfo *g_osx_app_info_new_from_keyfile  (GKeyFile *key_file); // doesn't exist
#define g_desktop_app_info_new_from_keyfile(key_file) dummy_osx_app_info_new_from_keyfile(key_file)

static GOsxAppInfo *dummy_osx_app_info_new_from_keyfile(GKeyFile *key_file)
{
	g_warning("Using dummy_osx_app_info_new_from_keyfile(GKeyFile *key_file) for %p", key_file);
	return NULL;
}


// called at: base/fm-file-launcher.c:76: app = (GAppInfo*)g_desktop_app_info_new_from_filename(file_or_id);
// --> return type can be GAppInfo* instead of GOsxAppInfo*
//GDesktopAppInfo *g_desktop_app_info_new_from_filename (const char *filename);
//GOsxAppInfo *g_osx_app_info_new_from_filename (const char *filename); // doesn't exist
#define g_desktop_app_info_new_from_filename(filename) dummy_osx_app_info_new_from_filename(filename)

static GOsxAppInfo *dummy_osx_app_info_new_from_filename(const char *filename)
{
	g_warning("Using dummy_osx_app_info_new_from_filename(const char *filename) for %s", filename);
	return NULL;
}

//GDesktopAppInfo *g_desktop_app_info_new (const char *desktop_id);
//GOsxAppInfo *g_osx_app_info_new (const char *desktop_id); // doesn't exist
#define g_desktop_app_info_new(desktop_id) dummy_osx_app_info_new(desktop_id)

static GOsxAppInfo *dummy_osx_app_info_new(const char *desktop_id)
{
	g_warning("Using dummy_osx_app_info_new(const char *desktop_id) for %s", desktop_id);
	return NULL;
}

#endif

