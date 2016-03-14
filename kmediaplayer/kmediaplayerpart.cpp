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

#include "kmediaplayerpart.h"

#include <KAboutData>
#include <KAction>
#include <KActionCollection>
#include <KLocale>
#include <KMessageBox>
#include <KStandardAction>
#include <KStatusBar>
#include <KPluginFactory>
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtGui/QScrollArea>

K_PLUGIN_FACTORY(KMediaPlayerPartFactory, registerPlugin<KMediaPlayerPart>();)  // produce a factory
K_EXPORT_PLUGIN(KMediaPlayerPartFactory(KAboutData(
               "kmediaplayerpart",
               0,
               ki18n("KMediaPlayerPart"),
               "1.0.0",
               ki18n("Simple media player part for KDE."),
               KAboutData::License_GPL_V2,
               ki18n("(c) 2016 Ivailo Monev"),
               KLocalizedString(),
               "http://github.com/fluxer/katana",
               "xakepa10@gmail.com").
               setProgramIconName(QLatin1String("KMediaPlayerPart")).
               setCatalogName("kmediaplayer")))

BrowserExtension::BrowserExtension(KMediaPlayerPart *parent)
        : KParts::BrowserExtension(parent)
{}

KMediaPlayerPart::KMediaPlayerPart(QWidget *parentWidget, QObject *parent, const QList<QVariant>&)
        : ReadOnlyPart(parent)
        , m_ext(new BrowserExtension(this))
        , m_player(0)
{
    setComponentData(KMediaPlayerPartFactory::componentData());

    m_player = new KMediaWidget(parentWidget, KMediaWidget::AllOptions);
    setWidget(m_player);
}

bool KMediaPlayerPart::openUrl(const KUrl &url)
{
    setUrl(url);
    return openFile();
}

bool KMediaPlayerPart::openFile()
{
    KUrl kurl = url();
    QString kurlstring = kurl.prettyUrl();
    if (!kurl.isValid()) {
        KMessageBox::information(widget(), i18n("The URL is not valid: %1", kurlstring));
    } else if (!m_player->player()->isProtocolSupported(kurl.protocol())) {
        KMessageBox::information(widget(), i18n("The URL protocol is not valid: %1", kurl.scheme()));
    } else {
        m_player->open(kurlstring);
        return true;
    }

    return false;
}

bool KMediaPlayerPart::closeUrl()
{
    return ReadOnlyPart::closeUrl();
}

void KMediaPlayerPart::updateURL(const KUrl &u)
{
    // update the interface
    emit m_ext->openUrlNotify(); //must be done first
    emit m_ext->setLocationBarUrl(u.prettyUrl());

    m_player->open(u.prettyUrl());

    //do this last, or it breaks Konqi location bar
    setUrl(u);
}

#include "moc_kmediaplayerpart.cpp"
