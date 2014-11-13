//
// configview.h
//
// Description: View for configuring the set of targets to be used with the debugger
//
//
// Copyright (c) 2010 Ian Wakeling <ian.wakeling@ntlworld.com>
// Copyright (c) 2012 Kåre Särs <kare.sars@iki.fi>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License version 2 as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with this library; see the file COPYING.LIB.  If not, write to
//  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
//  Boston, MA 02110-1301, USA.

#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H

#include "advanced_settings.h"

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QBoxLayout>
#include <QtGui/QResizeEvent>

#include <QtCore/QList>

#include <kate/mainwindow.h>
#include <kconfiggroup.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <kpushbutton.h>
#include <kactioncollection.h>
#include <kselectaction.h>

struct GDBTargetConf {
    QString     executable;
    QString     workDir;
    QString     arguments;
    QString     gdbCmd;
    QStringList customInit;
};

class ConfigView : public QWidget
{
Q_OBJECT
public:
    enum TargetStringOrder {
        NameIndex = 0,
        ExecIndex,
        WorkDirIndex,
        ArgsIndex,
        GDBIndex,
        CustomStartIndex
    };

    ConfigView(QWidget* parent, Kate::MainWindow* mainWin);
    ~ConfigView();

public:
    void registerActions(KActionCollection* actionCollection);

    void readConfig(KConfigBase* config, QString const& groupPrefix);
    void writeConfig(KConfigBase* config, QString const& groupPrefix);

    const GDBTargetConf currentTarget() const;
    bool  takeFocusAlways() const;
    bool  showIOTab() const;

Q_SIGNALS:
    void showIO(bool show);

private Q_SLOTS:
    void slotTargetEdited(const QString &newText);
    void slotTargetSelected(int index);
    void slotAddTarget();
    void slotCopyTarget();
    void slotDeleteTarget();
    void slotAdvancedClicked();
    void slotBrowseExec();
    void slotBrowseDir();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void saveCurrentToIndex(int index);
    void loadFromIndex(int index);
 
private:
    Kate::MainWindow*   m_mainWindow;
    KComboBox*          m_targetCombo;
    int                 m_currentTarget;
    QToolButton*        m_addTarget;
    QToolButton*        m_copyTarget;
    QToolButton*        m_deleteTarget;
    QFrame*             m_line;

    KLineEdit*          m_executable;
    QToolButton*        m_browseExe;

    KLineEdit*          m_workingDirectory;
    QToolButton*        m_browseDir;

    KLineEdit*          m_arguments;

    QCheckBox*          m_takeFocus;
    QCheckBox*          m_redirectTerminal;
    KPushButton*        m_advancedSettings;
    QBoxLayout*         m_checBoxLayout;

    bool                m_useBottomLayout;
    int                 m_widgetHeights;
    QLabel*             m_execLabel;
    QLabel*             m_workDirLabel;
    QLabel*             m_argumentsLabel;
    KSelectAction*      m_targetSelectAction;

    AdvancedGDBSettings* m_advanced;
};

#endif
