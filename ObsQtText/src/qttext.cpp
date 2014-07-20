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
#include <QPaintDevice>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>
#include <QSize>
#include <QFont>

#include <obs-module.h>

#include "qttext.h"

QtText::QtText(obs_data_t settings, obs_source_t source)
	:QObject(0),
	updated(true),
	tex(NULL)
{
	UNUSED_PARAMETER(source);

	moveToThread(qApp->thread());

	/* initialize settings and texture */
	update(settings);
	tick(0);
}

QtText::~QtText()
{
	gs_entercontext(obs_graphics());
	texture_destroy(tex);
	gs_leavecontext();
}

void QtText::update(obs_data_t settings)
{
	text = QString::fromUtf8(obs_data_getstring(settings, "text"));
	updated = true;
}

obs_properties_t QtText::properties()
{
	obs_properties_t props = obs_properties_create();

	obs_properties_add_text(props, "text", "Text", OBS_TEXT_DEFAULT);

	return props;
}


void QtText::tick(float seconds)
{
	if (!updated || !text.length())
		return;

	QFont font("Arial", 150);
	QFontMetrics metrics(font);
	QSize size = metrics.size(Qt::TextSingleLine, text);

	QImage img(size.width(), size.height(), QImage::Format_RGBA8888);
	img.fill(Qt::transparent);

	QPainter painter(&img);
	painter.setPen(Qt::red);
	painter.setFont(font);
	painter.setCompositionMode(QPainter::CompositionMode_Overlay);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.drawText(0, img.height() - metrics.descent(), text);
	painter.end();

	gs_entercontext(obs_graphics());
	/* update texture if needed */
	if (!tex || texture_getwidth(tex) != img.width()
			|| texture_getheight(tex) != img.height()) {
		if (tex)
			texture_destroy(tex);
		tex = gs_create_texture(img.width(), img.height(), GS_RGBA, 1,
			NULL, GS_DYNAMIC);
	}

	texture_setimage(tex, (uint8_t *) img.constBits(), img.width() * 4,
		false);
	gs_leavecontext();

	updated = true;
}

void QtText::render(effect_t effect)
{
	if (!text.length())
		return;

	eparam_t image = effect_getparambyname(effect, "image");
	effect_settexture(image, tex);

	gs_enable_blending(true);
	gs_draw_sprite(tex, 0, 0, 0);
	gs_reset_blend_state();
}

quint32 QtText::getWidth()
{
	return texture_getwidth(tex);
}

quint32 QtText::getHeight()
{
	return texture_getheight(tex);
}
