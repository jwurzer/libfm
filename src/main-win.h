/*
 *      main-win.h
 *      
 *      Copyright 2009 PCMan <pcman@thinkpad>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */


#ifndef __MAIN_WIN_H__
#define __MAIN_WIN_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define FM_MAIN_WIN_TYPE				(fm_main_win_get_type())
#define FM_MAIN_WIN(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj),\
			FM_MAIN_WIN_TYPE, FmMainWin))
#define FM_MAIN_WIN_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass),\
			FM_MAIN_WIN_TYPE, FmMainWinClass))
#define IS_FM_MAIN_WIN(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
			FM_MAIN_WIN_TYPE))
#define IS_FM_MAIN_WIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
			FM_MAIN_WIN_TYPE))

typedef struct _FmMainWin			FmMainWin;
typedef struct _FmMainWinClass		FmMainWinClass;

struct _FmMainWin
{
	GtkWindow parent;
	/* add your public declarations here */
	GtkWidget* toolbar;
	GtkWidget* location;
	GtkWidget* folder_view;
	GtkWidget* statusbar;
};

struct _FmMainWinClass
{
	GtkWindowClass parent_class;
};

GType		fm_main_win_get_type		(void);
GtkWidget*	fm_main_win_new			(void);
void fm_main_win_chdir(FmMainWin* win, const char* path);

G_END_DECLS

#endif /* __MAIN-WIN_H__ */
