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

#include <KIcon>
#include <KLocale>
#include <KDebug>
#include <KAction>
#include <KStandardShortcut>
#include <KStandardAction>
#include <KActionCollection>
#include <KCMultiDialog>
#include <KInputDialog>
#include <KFileDialog>
#include <KToolBar>
#include <KStatusBar>
#include <QMessageBox>
#include <QApplication>
#include <QMenuBar>

#include "kmediawindow.h"

KMediaWindow::KMediaWindow(QWidget *parent, Qt::WindowFlags flags)
    : KXmlGuiWindow(parent, flags)
{
    m_player = new KMediaWidget(this, KMediaWidget::AllOptions);
    setCentralWidget(m_player);

    KAction *a = actionCollection()->addAction("file_open", this, SLOT(openPath()));
    a->setText(i18n("Open"));
    a->setIcon(KIcon("document-open"));
    a->setShortcut(KStandardShortcut::open());
    a->setWhatsThis(i18n("Open a path."));

    KAction *b = actionCollection()->addAction("file_open_url", this, SLOT(openURL()));
    b->setText(i18n("Open URL"));
    b->setIcon(KIcon("document-open-remote"));
    b->setWhatsThis(i18n("Open a URL."));

    KAction *c = actionCollection()->addAction("file_quit", this, SLOT(quit()));
    c->setText(i18n("Quit"));
    c->setIcon(KIcon("application-exit"));
    c->setShortcut(KStandardShortcut::quit());
    c->setWhatsThis(i18n("Close the application."));

    KAction *d = actionCollection()->addAction("player_fullscreen", this, SLOT(fullscreen()));
    d->setText(i18n("Fullscreen"));
    d->setIcon(KIcon("view-fullscreen"));
    d->setShortcut(KStandardShortcut::fullScreen());
    d->setWhatsThis(i18n("Set the player view to fullscreen/non-fullscreen"));

/*
    // TODO: can this be less hacky?
    KAction *e = actionCollection()->addAction("player_audio_1", this, SLOT(configure()));
    e->setText(i18n("test audio track"));
    e->setIcon(KIcon("audio-input-line"));
*/

    KAction *f = actionCollection()->addAction("settings_player", this, SLOT(configure()));
    f->setText(i18n("Configure KMediaPlayer"));
    f->setIcon(KIcon("preferences-desktop-sound"));
    f->setWhatsThis(i18n("Configure KMediaPlayer and applications that use it."));

    m_settings = new QSettings("KMediaPlayer", "kmediaplayer");
    m_recentfiles = new KRecentFilesAction(KIcon("document-open-recent"), "Open recent", this);
    m_recentfiles->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::OpenRecent));
    m_recentfiles->setWhatsThis(i18n("Open recently opened files."));
    connect(m_recentfiles, SIGNAL(urlSelected(KUrl)), this, SLOT(openURL(KUrl)));
    actionCollection()->addAction("file_open_recent", m_recentfiles);
    QStringList recenturls = m_settings->value("KMultiMedia/recenturls", QStringList()).toStringList();
    foreach (QString url, recenturls) {
        KUrl kurl(url);
        m_recentfiles->addUrl(kurl);
    }

    setupGUI();
    setAutoSaveSettings();

    bool firstrun = m_settings->value("KMultiMedia/firstrun", true).toBool();
    if (firstrun) {
        // no toolbar unless explicitly enabled
        toolBar()->setVisible(false);
        // also set a decent window size
        resize(640, 480);
    }

    connect(m_player, SIGNAL(controlsHidden(bool)), this, SLOT(hideMenuBar(bool)));
    m_menu = new QMenu();
    m_menu->addAction(KIcon("show-menu"), i18n("Show/hide menubar"), this, SLOT(menubar()));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menu(QPoint)));
}

KMediaWindow::~KMediaWindow()
{
    disconnect(m_player, SIGNAL(controlsHidden(bool)), this, SLOT(hideMenuBar(bool)));
    menuBar()->setVisible(m_menuvisible);
    toolBar()->setVisible(m_toolvisible);
    statusBar()->setVisible(m_statusvisible);
    saveAutoSaveSettings();

    KUrl::List recenturls = m_recentfiles->urls();
    QStringList recentlist;
    foreach (KUrl url, recenturls) {
        recentlist.append(url.prettyUrl());
    }
    m_settings->setValue("KMultiMedia/recenturls", recentlist);
    m_settings->setValue("KMultiMedia/firstrun", false);
    m_settings->sync();
}

void KMediaWindow::showEvent(QShowEvent *event)
{
    m_menuvisible = menuBar()->isVisible();
    m_toolvisible = toolBar()->isVisible();
    m_statusvisible = statusBar()->isVisible();
    Q_UNUSED(event);
}

void KMediaWindow::hideMenuBar(bool visible)
{
    if (!visible) {
        m_menuvisible = menuBar()->isVisible();
        m_toolvisible = toolBar()->isVisible();
        m_statusvisible = statusBar()->isVisible();
        menuBar()->setVisible(false);
        toolBar()->setVisible(false);
        statusBar()->setVisible(false);
    } else {
        menuBar()->setVisible(m_menuvisible);
        toolBar()->setVisible(m_toolvisible);
        statusBar()->setVisible(m_statusvisible);
    }
}

void KMediaWindow::openPath()
{
    QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select paths"));
    if (!path.isEmpty()) {
        if (!m_player->player()->isPathSupported(path)) {
            QMessageBox::warning(this, i18n("Invalid path"),
                i18n("The path is invalid:\n%1", path));
        } else {
            m_player->open(path);
            m_recentfiles->addUrl(KUrl(path));
        }
    }
}

void KMediaWindow::openURL()
{
    bool dummy;
    QString protocols = m_player->player()->protocols().join(", ");
    KUrl url = KInputDialog::getText(i18n("Input URL"),
        i18n("Supported protocols are:\n\n%1", protocols),
        QString(), &dummy, this);
    if (!url.isEmpty()) {
        QString urlstring = url.prettyUrl();
        if (!m_player->player()->isPathSupported(urlstring)) {
            QMessageBox::warning(this, i18n("Invalid URL"),
                i18n("Invalid URL:\n%1", urlstring));
        } else {
            m_player->open(urlstring);
            m_recentfiles->addUrl(url);
        }
    }
}

void KMediaWindow::quit()
{
    qApp->quit();
}

void KMediaWindow::fullscreen()
{
    m_player->setFullscreen();
}

void KMediaWindow::configure()
{
    KCMultiDialog kcmdialg(this);
    kcmdialg.addModule("kcmplayer");
    kcmdialg.exec();
}

void KMediaWindow::openURL(KUrl url)
{
    m_player->open(url.prettyUrl());
}

void KMediaWindow::menubar() {
    menuBar()->setVisible(!menuBar()->isVisible());
}

void KMediaWindow::menu(QPoint position)
{
    // it is bogus, just ignore it
    Q_UNUSED(position);
    m_menu->exec(QCursor::pos());
}
