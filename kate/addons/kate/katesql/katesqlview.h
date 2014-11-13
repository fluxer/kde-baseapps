/*
   Copyright (C) 2010  Marco Mentasti  <marcomentasti@gmail.com>

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

#ifndef KATESQLVIEW_H
#define KATESQLVIEW_H

class KateSQLOutputWidget;
class SchemaBrowserWidget;
class SQLManager;

class KConfigBase;
class KComboBox;

class QSqlQuery;
class QActionGroup;

#include <kate/plugin.h>
#include <kate/mainwindow.h>

class KateSQLView : public Kate::PluginView, public Kate::XMLGUIClient
{
  Q_OBJECT

  public:
    explicit KateSQLView(Kate::MainWindow *mw);
    ~KateSQLView();

    void readSessionConfig (KConfigBase* config, const QString& groupPrefix);
    void writeSessionConfig (KConfigBase* config, const QString& groupPrefix);

   
    SchemaBrowserWidget *schemaBrowserWidget() const { return m_schemaBrowserWidget; }

  public slots:
    void slotConnectionCreate();
    void slotConnectionEdit();
    void slotConnectionRemove();
    void slotConnectionReconnect();
    void slotConnectionChanged(const QString &connection);
    void slotRunQuery();
    void slotError(const QString &message);
    void slotSuccess(const QString &message);
    void slotQueryActivated(QSqlQuery &query, const QString &connection);
    void slotConnectionCreated(const QString &name);
    void slotGlobalSettingsChanged();
    void slotSQLMenuAboutToShow();
    void slotConnectionSelectedFromMenu(QAction *action);
    void slotConnectionAboutToBeClosed(const QString &name);

  protected:
    void setupActions();

  private:
    QWidget *m_outputToolView;
    QWidget *m_schemaBrowserToolView;
    QActionGroup *m_connectionsGroup;

    KateSQLOutputWidget *m_outputWidget;
    
    SchemaBrowserWidget *m_schemaBrowserWidget;

    KComboBox *m_connectionsComboBox;

    SQLManager *m_manager;

    QString m_currentResultsetConnection;
};

#endif // KATESQLVIEW_H

