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
    // TODO: enable all options once mouse tracking on all relevant widgets can be done
    m_player = new KMediaWidget(this, KMediaWidget::DragDrop | KMediaWidget::FullscreenVideo);
    setCentralWidget(m_player);

    KAction *a = actionCollection()->addAction("file_open_path", this, SLOT(openPath()));
    a->setText(i18n("Open"));
    a->setIcon(KIcon("document-open"));
    a->setShortcut(KStandardShortcut::open());
    a->setWhatsThis(i18n("Open a path."));

    KAction *b = actionCollection()->addAction("file_open_url", this, SLOT(openURL()));
    b->setText(i18n("Open URL"));
    b->setIcon(KIcon("document-open-remote"));
    b->setWhatsThis(i18n("Open a URL."));

    KAction *c = actionCollection()->addAction("file_close", this, SLOT(closePath()));
    c->setText(i18n("Close"));
    c->setIcon(KIcon("document-close"));
    c->setShortcut(KStandardShortcut::close());
    c->setWhatsThis(i18n("Close the the current path/URL."));

    KAction *d = actionCollection()->addAction("file_quit", this, SLOT(quit()));
    d->setText(i18n("Quit"));
    d->setIcon(KIcon("application-exit"));
    d->setShortcut(KStandardShortcut::quit());
    d->setWhatsThis(i18n("Close the application."));

    KAction *e = actionCollection()->addAction("player_fullscreen", this, SLOT(fullscreen()));
    e->setText(i18n("Fullscreen"));
    e->setIcon(KIcon("view-fullscreen"));
    e->setShortcut(KStandardShortcut::fullScreen());
    e->setWhatsThis(i18n("Set the player view to fullscreen/non-fullscreen"));

    KAction *g = actionCollection()->addAction("settings_player", this, SLOT(configure()));
    g->setText(i18n("Configure KMediaPlayer"));
    g->setIcon(KIcon("preferences-desktop-sound"));
    g->setWhatsThis(i18n("Configure KMediaPlayer and applications that use it."));

    m_settings = new KSettings("kmediaplayer");
    m_recentfiles = new KRecentFilesAction(KIcon("document-open-recent"), "Open recent", this);
    m_recentfiles->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::OpenRecent));
    m_recentfiles->setWhatsThis(i18n("Open recently opened files."));
    connect(m_recentfiles, SIGNAL(urlSelected(KUrl)), this, SLOT(openURL(KUrl)));
    actionCollection()->addAction("file_open_recent", m_recentfiles);
    const QVariant recenturls = m_settings->value("KMultiMedia/recenturls", QStringList());
    foreach (const QString url, recenturls.toStringList()) {
        const KUrl kurl(url);
        m_recentfiles->addUrl(kurl);
    }

    setupGUI(KXmlGuiWindow::Keys | KXmlGuiWindow::StatusBar | KXmlGuiWindow::Save | KXmlGuiWindow::Create);
    setAutoSaveSettings();

    const bool firstrun = m_settings->value("KMultiMedia/firstrun", true).toBool();
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
    hideMenuBar(true);
    disconnect(m_player, SIGNAL(controlsHidden(bool)), this, SLOT(hideMenuBar(bool)));
    saveAutoSaveSettings();

    QStringList recentlist;
    foreach (const KUrl url, m_recentfiles->urls()) {
        recentlist.append(url.prettyUrl());
    }
    m_settings->setValue("KMultiMedia/recenturls", recentlist);
    m_settings->setValue("KMultiMedia/firstrun", false);
    m_settings->sync();

    m_player->deleteLater();
    m_recentfiles->deleteLater();
    m_settings->deleteLater();
    m_menu->deleteLater();
}

void KMediaWindow::showEvent(QShowEvent *event)
{
    m_menuvisible = menuBar()->isVisible();
    m_statusvisible = statusBar()->isVisible();
    Q_UNUSED(event);
}

void KMediaWindow::hideMenuBar(bool visible)
{
    if (!visible) {
        m_menuvisible = menuBar()->isVisible();
        m_statusvisible = statusBar()->isVisible();
        menuBar()->setVisible(false);
        statusBar()->setVisible(false);
    } else {
        menuBar()->setVisible(m_menuvisible);
        statusBar()->setVisible(m_statusvisible);
    }
}

void KMediaWindow::openPath()
{
    const QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select paths"));
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

void KMediaWindow::openURL(KUrl url)
{
    m_player->open(url.prettyUrl());
    m_recentfiles->addUrl(url);
}

void KMediaWindow::closePath()
{
    m_player->player()->stop();
    statusBar()->showMessage("");
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

void KMediaWindow::menubar() {
    menuBar()->setVisible(!menuBar()->isVisible());
}

void KMediaWindow::menu(QPoint position)
{
    // it is bogus, just ignore it
    Q_UNUSED(position);
    m_menu->exec(QCursor::pos());
}

void KMediaWindow::quit()
{
    qApp->quit();
}
