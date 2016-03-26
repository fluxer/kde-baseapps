/*  This file is part of the KDE libraries
    Copyright (C) 2016 Ivailo Monev <xakepa10@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2, as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KMEDIAPART_H
#define KMEDIAPART_H

#include "kmediawidget.h"

#include <KParts/BrowserExtension>
#include <KParts/Part>
#include <KUrl>

class KMediaPlayerPart;
class BrowserExtension : public KParts::BrowserExtension
{
public:
    explicit BrowserExtension(KMediaPlayerPart*);
};

class KMediaPlayerPart : public KParts::ReadOnlyPart
{
    Q_OBJECT
public:
    KMediaPlayerPart(QWidget *, QObject *, const QList<QVariant>&);

    virtual bool openUrl(const KUrl &url);
    virtual bool openFile();

public slots:
    virtual bool closeUrl();

private slots:
    void updateURL(const KUrl &);

private:
    BrowserExtension   *m_ext;
    KMediaWidget *m_player;
};

#endif // KMEDIAPART_H
