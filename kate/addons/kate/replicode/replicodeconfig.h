/* This file is part of the KDE project
   Copyright (C) 2014 Martin Sandsmark <martin.sandsmark@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef REPLICODECONFIG_H
#define REPLICODECONFIG_H

#include <QTabWidget>

QT_BEGIN_NAMESPACE
class Ui_tabWidget;
QT_END_NAMESPACE

class ReplicodeSettings;

class ReplicodeConfig : public QTabWidget
{
    Q_OBJECT
public:
    explicit ReplicodeConfig(QWidget *parent = 0);
    virtual ~ReplicodeConfig();

public slots:
    void reset();
    void save();
    void load();

    const ReplicodeSettings *settingsObject() { save(); return m_settings; }

private:
    Ui_tabWidget *m_ui;
    ReplicodeSettings *m_settings;
};

#endif//REPLICODECONFIG_H