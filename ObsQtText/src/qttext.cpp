#include <QCoreApplication>
#include <QPaintDevice>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>
#include <QSize>
#include <QFont>

#include <util/platform.h>
#include <obs-module.h>

#include "qttext.h"

QtText::QtText(obs_data_t settings, obs_source_t source)
	:QObject(0)
{
	UNUSED_PARAMETER(source);

	moveToThread(qApp->thread());

	QString text("Hello World!");

	QFont font("Arial", 150);
	QFontMetrics metrics(font);
	QSize size = metrics.size(Qt::TextSingleLine, text);

	img = new QImage(size.width(), size.height(), QImage::Format_RGBA8888);
	img->fill(Qt::transparent);

	QPainter painter(img);
	painter.setPen(Qt::red);
	painter.setFont(font);
	painter.setCompositionMode(QPainter::CompositionMode_Overlay);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.drawText(0, img->height() - metrics.descent(), text);
	painter.end();

	gs_entercontext(obs_graphics());
	tex = gs_create_texture(img->width(), img->height(), GS_RGBA, 1, NULL,
		GS_DYNAMIC);
	texture_setimage(tex, (uint8_t *) img->constBits(), img->width() * 4,
		false);
	gs_leavecontext();

}

QtText::~QtText()
{
	gs_entercontext(obs_graphics());
	texture_destroy(tex);
	gs_leavecontext();
}

void QtText::update(obs_data_t settings)
{

}

void QtText::tick(float seconds)
{

}

void QtText::render(effect_t effect)
{
	eparam_t image = effect_getparambyname(effect, "image");
	effect_settexture(image, tex);

	gs_enable_blending(true);
	gs_draw_sprite(tex, 0, 0, 0);
	gs_reset_blend_state();
}

quint32 QtText::getWidth()
{
	return img->width();
}

quint32 QtText::getHeight()
{
	return img->height();
}
