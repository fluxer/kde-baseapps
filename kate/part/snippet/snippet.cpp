/*  This file is part of the Kate project.
 *  Based on the snippet plugin from KDevelop 4.
 *
 *  Copyright (C) 2007 Robert Gruber <rgruber@users.sourceforge.net> 
 *  Copyright (C) 2010 Milian Wolff <mail@milianw.de>
 *  Copyright (C) 2012 Christoph Cullmann <cullmann@kde.org>
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

#include "snippet.h"

#include <KLocalizedString>
#include <KIcon>

#include <KColorScheme>

#include "katesnippetglobal.h"
#include <KActionCollection>
#include <KAction>
#include <KParts/MainWindow>

Snippet::Snippet()
    : QStandardItem(i18n("<empty snippet>")), m_action(0)
{
    setIcon(KIcon("text-plain"));
}

Snippet::~Snippet()
{
    delete m_action;
}

QString Snippet::snippet() const
{
    return m_snippet;
}

void Snippet::setSnippet(const QString& snippet)
{
    m_snippet = snippet;
}

QString Snippet::prefix() const
{
    return m_prefix;
}

void Snippet::setPrefix(const QString& prefix)
{
    m_prefix = prefix;
}

QString Snippet::postfix() const
{
    return m_postfix;
}

void Snippet::setPostfix(const QString& postfix)
{
    m_postfix = postfix;
}

QString Snippet::arguments() const
{
    return m_arguments;
}

void Snippet::setArguments(const QString& arguments)
{
    m_arguments = arguments;
}

KAction* Snippet::action()
{
    ///TODO: this is quite ugly, or is it? if someone knows how to do it better, please refactor
    if ( !m_action ) {
        static int actionCount = 0;
        m_action = new KAction(QString("insertSnippet%1").arg(actionCount), KateSnippetGlobal::self());
        m_action->setData(QVariant::fromValue<Snippet*>(this));
        KateSnippetGlobal::self()->connect(m_action, SIGNAL(triggered()),
                                       KateSnippetGlobal::self(), SLOT(insertSnippetFromActionData()));

#if 0 // FIXME
        // action needs to be added to a widget before it can work...
        KDevelop::ICore::self()->uiController()->activeMainWindow()->addAction(m_action);
#endif
    }
    m_action->setText(i18n("insert snippet %1", text()));
    return m_action;
}

QVariant Snippet::data(int role) const
{
    if ( role == Qt::ToolTipRole ) {
        return m_snippet;
    } else if ( (role == Qt::ForegroundRole || role == Qt::BackgroundRole) &&  parent()->checkState() != Qt::Checked ) {
        ///TODO: make the selected items also "disalbed" so the toggle action is seen directly
        KColorScheme scheme(QPalette::Disabled, KColorScheme::View);
        if (role == Qt::ForegroundRole) {
            return scheme.foreground(KColorScheme::ActiveText).color();
        } else {
            return scheme.background(KColorScheme::ActiveBackground).color();
        }
    }
    return QStandardItem::data(role);
}
