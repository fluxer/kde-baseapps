/***************************************************************************
 *   Copyright (C) 2006 by Peter Penz (peter.penz@gmx.at) and              *
 *   and Patrice Tremblay                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/
#ifndef STATUSBARSPACEINFO_H
#define STATUSBARSPACEINFO_H

#include <KUrl>

#include <QScopedPointer>
#include <QColor>
#include <QtGui/qevent.h>
#include <QString>

#include <kcapacitybar.h>

#include <QHideEvent>
#include <QShowEvent>

class SpaceInfoObserver;

/**
 * @short Shows the available space for the volume represented
 *        by the given URL as part of the status bar.
 */
class StatusBarSpaceInfo : public KCapacityBar
{
    Q_OBJECT

public:
    explicit StatusBarSpaceInfo(QWidget* parent = 0);
    virtual ~StatusBarSpaceInfo();

    void setUrl(const KUrl& url);
    KUrl url() const;

protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private slots:
    void slotValuesChanged();

private:
    QScopedPointer<SpaceInfoObserver> m_observer;
    KUrl m_url;
};

#endif
