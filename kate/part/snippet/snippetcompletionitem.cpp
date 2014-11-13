/*  This file is part of the Kate project.
 *  Based on the snippet plugin from KDevelop 4.
 *
 *  Copyright (C) 2009 Milian Wolff <mail@milianw.de>
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

#include "snippetcompletionitem.h"

#include <KTextEditor/Document>
#include <KLocalizedString>
#include <QtGui/QTextEdit>
#include <QtCore/QModelIndex>

#include <ktexteditor/templateinterface.h>
#include <ktexteditor/templateinterface2.h>
#include <ktexteditor/view.h>
#include <ktexteditor/codecompletioninterface.h>
#include <ktexteditor/codecompletionmodel.h>

#include "snippet.h"
#include "snippetrepository.h"


SnippetCompletionItem::SnippetCompletionItem( Snippet* snippet, SnippetRepository* repo )
    : m_name(snippet->text()), m_snippet(snippet->snippet()), m_prefix(snippet->prefix()),
      m_arguments(snippet->arguments()), m_postfix(snippet->postfix()), m_repo(repo)
{
    Q_ASSERT(m_repo);
    m_name.prepend( repo->completionNamespace() );
}

SnippetCompletionItem::~SnippetCompletionItem()
{
}

QVariant SnippetCompletionItem::data( const QModelIndex& index, int role, const KTextEditor::CodeCompletionModel2* model ) const
{
    // as long as the snippet completion model is not a kdevelop code completion model,
    // model will usually be 0. hence don't use it.
    Q_UNUSED(model);

    switch ( role ) {
    case Qt::DisplayRole:
        switch ( index.column() ) {
            case KTextEditor::CodeCompletionModel::Name:
                return m_name;
            case KTextEditor::CodeCompletionModel::Prefix:
                return m_prefix;
            case KTextEditor::CodeCompletionModel::Postfix:
                return m_postfix;
            case KTextEditor::CodeCompletionModel::Arguments:
                return m_arguments;
        }
        break;
    case KTextEditor::CodeCompletionModel::IsExpandable:
        return QVariant(true);
    case KTextEditor::CodeCompletionModel::ExpandingWidget:
        {
        QTextEdit *textEdit = new QTextEdit();
        ///TODO: somehow make it possible to scroll like in other expanding widgets
        // don't make it too large, only show a few lines
        textEdit->resize(textEdit->width(), 100);
        textEdit->setPlainText(m_snippet);
        textEdit->setReadOnly(true);
        textEdit->setLineWrapMode(QTextEdit::NoWrap);

        QVariant v;
        v.setValue<QWidget*>(textEdit);
        return v;
        }
    }

    return QVariant();
}

void SnippetCompletionItem::execute( KTextEditor::View* view, const KTextEditor::Range& word )
{
    QMap< QString, QString > values = QMap<QString, QString>();
    KTextEditor::TemplateInterface2* templateIface2 = qobject_cast<KTextEditor::TemplateInterface2*>(view);
    if (templateIface2)
    {
      // Replace matched text...
      view->document()->removeText(word);
      // ... with snippet content
      templateIface2->insertTemplateText(word.start(), m_snippet, values, m_repo->registeredScript());
    }
}
