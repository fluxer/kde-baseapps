/* This file is part of the KDE project
   Copyright (C) 2000 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KSHELLCMDPLUGIN_H
#define KSHELLCMDPLUGIN_H

#include <kparts/plugin.h>

class KShellCmdPlugin : public KParts::Plugin
{
    Q_OBJECT
public:
    KShellCmdPlugin( QObject* parent, const QVariantList & );
    ~KShellCmdPlugin() {}

public Q_SLOTS:
    void slotExecuteShellCommand();
};

#endif
