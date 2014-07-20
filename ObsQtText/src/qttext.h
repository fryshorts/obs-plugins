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
