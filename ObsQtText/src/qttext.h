#pragma once

#include <QObject>

#include <obs-module.h>

class QImage;

class QtText : public QObject
{
	Q_OBJECT

public:
	explicit QtText(obs_data_t settings, obs_source_t source);
	~QtText();

	void update(obs_data_t settings);
	static obs_properties_t properties();
	void tick(float seconds);
	void render(effect_t effect);

	quint32 getWidth();
	quint32 getHeight();

public slots:

private:

private slots:

private:
	bool updated;
	QString text;
	QString fontName;

	texture_t tex;
};
