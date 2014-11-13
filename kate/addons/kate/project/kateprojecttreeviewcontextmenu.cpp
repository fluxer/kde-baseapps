/*  This file is part of the Kate project.
 *
 *  Copyright (C) 2013 Dominik Haumann <dhaumann.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "kateprojecttreeviewcontextmenu.h"

#include <KMimeType>
#include <KMimeTypeTrader>
#include <KStandardDirs>
#include <QMenu>
#include <QFileInfo>
#include <QDir>
#include <KRun>
#include <KIcon>
#include <QProcess>
#include <QApplication>
#include <QClipboard>

KateProjectTreeViewContextMenu::KateProjectTreeViewContextMenu ()
{
}

KateProjectTreeViewContextMenu::~KateProjectTreeViewContextMenu ()
{
}

static bool isGit(const QString& filename)
{
  QFileInfo fi(filename);
  QDir dir (fi.absoluteDir());
  QProcess git;
  git.setWorkingDirectory (dir.absolutePath());
  QStringList args;
  args << "ls-files" << fi.fileName();
  git.start("git", args);
  bool isGit = false;
  if (git.waitForStarted() && git.waitForFinished()) {
    QStringList files = QString::fromLocal8Bit (git.readAllStandardOutput ()).split (QRegExp("[\n\r]"), QString::SkipEmptyParts);
    isGit = files.contains(fi.fileName());
  }
  return isGit;
}

static bool appExists(const QString& appname)
{
  return !KStandardDirs::findExe(appname).isEmpty();
}

static void launchApp(const QString &app, const QString& file)
{
  QFileInfo fi(file);
  QDir dir (fi.absoluteDir());

  QStringList args;
  args << file;

  QProcess::startDetached(app, QStringList(), dir.absolutePath());
}

void KateProjectTreeViewContextMenu::exec(const QString& filename, const QPoint& pos, QWidget* parent)
{
  /**
   * create context menu
   */
  QMenu menu;

  QAction *copyAction=menu.addAction(KIcon("edit-copy"),i18n("Copy Filename"));
    
  /**
   * handle "open with"
   * find correct mimetype to query for possible applications
   */
  QMenu *openWithMenu = menu.addMenu(i18n("Open With"));
  KMimeType::Ptr mimeType = KMimeType::findByPath(filename);
  KService::List offers = KMimeTypeTrader::self()->query(mimeType->name(), "Application");

  /**
   * for each one, insert a menu item...
   */
  for(KService::List::Iterator it = offers.begin(); it != offers.end(); ++it)
  {
    KService::Ptr service = *it;
    if (service->name() == "Kate") continue; // omit Kate
    QAction *action = openWithMenu->addAction(KIcon(service->icon()), service->name());
    action->setData(service->entryPath());
  }

  /**
   * perhaps disable menu, if no entries!
   */
  openWithMenu->setEnabled (!openWithMenu->isEmpty());

  QList<QAction*> appActions;
  if (isGit(filename)) {
    QMenu* git = menu.addMenu(i18n("Git Tools"));
    if (appExists("gitk")) {
      QAction* action = git->addAction(i18n("Launch gitk"));
      action->setData("gitk");
      appActions.append(action);
    }
    if (appExists("qgit")) {
      QAction* action = git->addAction(i18n("Launch qgit"));
      action->setData("qgit");
      appActions.append(action);
    }
    if (appExists("git-cola")) {
      QAction* action = git->addAction(i18n("Launch git-cola"));
      action->setData("git-cola");
      appActions.append(action);
    }

    if (appActions.size() == 0) {
      delete git;
    }
  }

  /**
   * run menu and handle the triggered action
   */
  if (QAction *action = menu.exec (pos)) {

    // handle apps
    if (copyAction == action) {
      QApplication::clipboard()->setText(filename);
    } else if (appActions.contains(action)) {
      launchApp(action->data().toString(), filename);
    } else {
      // handle "open with"
      const QString openWith = action->data().toString();
      if (KService::Ptr app = KService::serviceByDesktopPath(openWith)) {
        QList<QUrl> list;
        list << QUrl::fromLocalFile (filename);
        KRun::run(*app, list, parent);
      }
    }
  }
}

// kate: space-indent on; indent-width 2; replace-tabs on;
