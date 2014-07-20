/*
Copyright (C) 2014 by Leonhard Oelke <leonhard@in-verted.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QCoreApplication>
#include <obs-module.h>

#include "qttext.h"

OBS_DECLARE_MODULE()

static const char* qttext_getname(void)
{
	return "Qt Text Input";
}

static void *qttext_create(obs_data_t settings, obs_source_t source)
{
	return new QtText(settings, source);
}

static void qttext_destroy(void *data)
{
	delete reinterpret_cast<QtText *>(data);
}

static void qttext_update(void *data, obs_data_t settings)
{
	reinterpret_cast<QtText *>(data)->update(settings);
}

static obs_properties_t qttext_properties()
{
	return QtText::properties();
}

static void qttext_tick(void *data, float seconds)
{
	reinterpret_cast<QtText *>(data)->tick(seconds);
}

static void qttext_render(void *data, effect_t effect)
{
	reinterpret_cast<QtText *>(data)->render(effect);
}

static uint32_t qttext_getwidth(void *data)
{
	return reinterpret_cast<QtText *>(data)->getWidth();
}

static uint32_t qttext_getheight(void *data)
{
	return reinterpret_cast<QtText *>(data)->getHeight();
}

bool obs_module_load(uint32_t libobs_ver)
{
	UNUSED_PARAMETER(libobs_ver);

	if(qApp == 0) {
		blog(LOG_ERROR, "No QApplication found, can't render widgets");
		return false;
	}

	struct obs_source_info qttext;
	memset(&qttext, 0, sizeof(qttext));
	qttext.id           = "qttext_input";
	qttext.type         = OBS_SOURCE_TYPE_INPUT;
	qttext.output_flags = OBS_SOURCE_VIDEO;
	qttext.getname      = qttext_getname;
	qttext.create       = qttext_create;
	qttext.destroy      = qttext_destroy;
	qttext.update       = qttext_update;
	qttext.properties   = qttext_properties;
	qttext.video_tick   = qttext_tick;
	qttext.video_render = qttext_render;
	qttext.getwidth     = qttext_getwidth;
	qttext.getheight    = qttext_getheight;

	obs_register_source(&qttext);

	return true;
}

void obs_module_unload()
{
}
