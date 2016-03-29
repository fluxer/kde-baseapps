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
#include <KMenu>
#include <KStandardShortcut>
#include <KCMultiDialog>
#include <KInputDialog>
#include <KFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QMenuBar>

#include "kmediawindow.h"

KMediaWindow::KMediaWindow(QWidget *parent, Qt::WindowFlags flags)
    : KMainWindow(parent, flags)
{
    setAutoSaveSettings();

    m_player = new KMediaWidget(this, KMediaWidget::AllOptions);
    setCentralWidget(m_player);

    connect(m_player, SIGNAL(controlsHidden(bool)), this, SLOT(hideMenuBar(bool)));

    QMenuBar *menubar = menuBar();

    QMenu *fileMenu = menubar->addMenu(i18n("File"));

    KAction *openAction = new KAction(KIcon("document-open"), i18n("Open"), fileMenu);
    openAction->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::Open));
    openAction->setStatusTip(i18n("Open a path"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openPath()));
    fileMenu->addAction(openAction);

    KAction *openURLAction = new KAction(KIcon("document-open-remote"), i18n("Open URL"), fileMenu);
    openURLAction->setStatusTip(i18n("Open a URL"));
    connect(openURLAction, SIGNAL(triggered()), this, SLOT(openURL()));
    fileMenu->addAction(openURLAction);

    fileMenu->addSeparator();
    KAction *quitAction = new KAction(KIcon("application-exit"), i18n("Quit"), fileMenu);
    quitAction->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::Quit));
    quitAction->setStatusTip(i18n("Exit the program"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    fileMenu->addAction(quitAction);

    QMenu *settingsMenu = menubar->addMenu(i18n("&Settings"));

    KAction *configureAction = new KAction(KIcon("configure"), i18n("Configure KMediaPlayer"), settingsMenu);
    configureAction->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::Preferences));
    configureAction->setStatusTip(i18n("Configure KMediaPlayer"));
    connect(configureAction, SIGNAL(triggered()), this, SLOT(configure()));
    settingsMenu->addAction(configureAction);

    KMenu *helpmenu = helpMenu();
    // FIXME: implement overload for helpMenu() to avoid this bellow
    menubar->addMenu((QMenu*)helpmenu);
}

KMediaWindow::~KMediaWindow()
{
}

void KMediaWindow::hideMenuBar(bool hidden)
{
    menuBar()->setVisible(hidden);
}

void KMediaWindow::openPath()
{
    QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select paths"));
    if (!path.isEmpty()) {
        if (!m_player->player()->isPathSupported(path)) {
            kDebug() << i18n("ignoring unsupported:\n%1", path);
            QMessageBox::warning(this, i18n("Invalid path"),
                i18n("The path is invalid:\n%1", path));
        } else {
            m_player->open(path);
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
            kDebug() << i18n("ignoring unsupported:\n%1", urlstring);
            QMessageBox::warning(this, i18n("Invalid URL"),
                i18n("Invalid URL:\n%1", urlstring));
        } else {
            m_player->open(urlstring);
        }
    }
}

void KMediaWindow::quit()
{
    qApp->quit();
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
