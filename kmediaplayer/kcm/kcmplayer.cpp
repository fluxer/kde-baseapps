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

#include <kaboutdata.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kdesktopfile.h>
#include <kicon.h>

#include "kcmplayer.h"
#include "ui_kcmplayer.h"

K_PLUGIN_FACTORY( PlayerFactory, registerPlugin<KCMPlayer>(); )
K_EXPORT_PLUGIN( PlayerFactory("kcmnotify") )

KCMPlayer::KCMPlayer(QWidget *parent, const QVariantList &arguments )
    : KCModule(PlayerFactory::componentData(), parent)
{
    m_settings = new QSettings("KMediaPlayer", "kmediaplayer");
    m_ui = new Ui_KCMPlayer();
    m_ui->setupUi(this);
    m_player = new KAudioPlayer(m_settings);

    setButtons( KCModule::Default | KCModule::Apply );

    KAboutData* ab = new KAboutData(
        "kcmplayer", 0, ki18n("KMediaPlayer"), "1.0",
        ki18n("System Media Player Configuration"),
        KAboutData::License_GPL, ki18n("(c) 2016 Ivailo Monev"));

    ab->addAuthor( ki18n("Ivailo Monev"), KLocalizedString(), "xakepa10@gmail.com" );
    setAboutData( ab );

    Q_UNUSED(arguments);

    QString globalaudio = m_settings->value("global/audiooutput", "auto").toString();
    int globalvolume = m_settings->value("global/volume", 90).toInt();
    bool globalmute = m_settings->value("global/mute", false).toBool();

    m_ui->w_audiooutput->clear();
    m_ui->w_audiooutput->addItems(m_player->audiooutputs());
    m_ui->w_appaudiooutput->clear();
    m_ui->w_appaudiooutput->addItems(m_player->audiooutputs());
    int audioindex = m_ui->w_audiooutput->findText(globalaudio);
    m_ui->w_audiooutput->setCurrentIndex(audioindex);
    m_ui->w_volume->setValue(globalvolume);
    m_ui->w_mute->setChecked(globalmute);

    connect(m_ui->w_audiooutput, SIGNAL(currentIndexChanged(QString)),
        this, SLOT(setGlobalOutput(QString)));
    connect(m_ui->w_volume, SIGNAL(valueChanged(int)),
        this, SLOT(setGlobalVolume(int)));
    connect(m_ui->w_mute, SIGNAL(stateChanged(int)),
        this, SLOT(setGlobalMute(int)));

    QStringList desktopfiles = KGlobal::dirs()->findAllResources("xdgdata-apps",
        "*/*.desktop", KStandardDirs::Recursive | KStandardDirs::NoDuplicates);

    foreach (const QString desktopfile, desktopfiles )
    {
        KDesktopFile *desktop = new KDesktopFile(desktopfile);
        QString appname = desktop->readName();
        if (appname.isEmpty()) {
            kDebug() << "ignoring application without name" << appname;
            continue;
        }
        QString appicon = desktop->readIcon();
        KIcon appkicon(appicon);
        m_ui->w_application->addItem(appkicon, appname);
    }
    connect(m_ui->w_application, SIGNAL(currentIndexChanged(QString)),
        this, SLOT(setApplicationSettings(QString)));
    connect(m_ui->w_appaudiooutput, SIGNAL(currentIndexChanged(QString)),
        this, SLOT(setApplicationOutput(QString)));
    connect(m_ui->w_appvolume, SIGNAL(valueChanged(int)),
        this, SLOT(setApplicationVolume(int)));
    connect(m_ui->w_appmute, SIGNAL(stateChanged(int)),
        this, SLOT(setApplicationMute(int)));
}

KCMPlayer::~KCMPlayer()
{
}

void KCMPlayer::defaults()
{
    // TODO:
}
void KCMPlayer::load()
{
    // just to load the appplication values
    m_ui->w_application->setCurrentIndex(1);
    m_ui->w_application->setCurrentIndex(0);

    emit changed(false);
}

void KCMPlayer::save()
{
    if (m_settings && m_settings->isWritable()) {
        m_settings->setValue("global/audiooutput", m_ui->w_audiooutput->currentText());
        m_settings->setValue("global/volume", m_ui->w_volume->value());
        m_settings->setValue("global/mute", m_ui->w_mute->isChecked());
        m_settings->sync();
    } else {
        qWarning() << i18n("Could not save state");
    }

    if (!m_application.isEmpty()) {
        if (m_settings && m_settings->isWritable()) {
            m_settings->setValue(m_application + "/audiooutput", m_ui->w_appaudiooutput->currentText());
            m_settings->setValue(m_application + "/volume", m_ui->w_appvolume->value());
            m_settings->setValue(m_application + "/mute", m_ui->w_appmute->isChecked());
            m_settings->sync();
        } else {
            qWarning() << i18n("Could not save state");
        }
    }

    emit changed(false);
}

void KCMPlayer::setGlobalOutput(QString output)
{
    kDebug() << output;
    if (m_settings->value("global/audiooutput").toString() != output) {
        emit changed(true);
    } else {
        emit changed(false);
    }
}

void KCMPlayer::setGlobalVolume(int volume)
{
    kDebug() << volume;
    if (m_settings->value("global/volume").toInt() != volume) {
        emit changed(true);
    } else {
        emit changed(false);
    }
}

void KCMPlayer::setGlobalMute(int mute)
{
    kDebug() << mute;
    if (m_settings->value("global/mute").toBool() != bool(mute)) {
        emit changed(true);
    } else {
        emit changed(false);
    }
}

void KCMPlayer::setApplicationSettings(QString application)
{
    m_application = application;

    QString appaudio = m_settings->value(m_application + "/audiooutput", "auto").toString();
    int appvolume = m_settings->value(m_application + "/volume", 90).toInt();
    bool appmute = m_settings->value(m_application +"/mute", false).toBool();

    m_ui->w_appaudiooutput->setEditText(appaudio);
    m_ui->w_appvolume->setValue(appvolume);
    m_ui->w_appmute->setChecked(appmute);
}

void KCMPlayer::setApplicationOutput(QString output)
{
    kDebug() << output;
    if (m_settings->value(m_application + "/audiooutput").toString() != output) {
        emit changed(true);
    } else {
        emit changed(false);
    }
}

void KCMPlayer::setApplicationVolume(int volume)
{
    kDebug() << volume;
    if (m_settings->value(m_application + "/volume").toInt() != volume) {
        emit changed(true);
    } else {
        emit changed(false);
    }
}

void KCMPlayer::setApplicationMute(int mute)
{
    kDebug() << mute;
    if (m_settings->value(m_application + "/mute").toBool() != bool(mute)) {
        emit changed(true);
    } else {
        emit changed(false);
    }
}

#include "moc_kcmplayer.cpp"
