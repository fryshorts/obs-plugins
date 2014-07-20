#include <QCoreApplication>
//#include <QMetaObject>

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
