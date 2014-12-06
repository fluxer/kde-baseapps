/* This file is part of the KDE Project
   Copyright (C) 2001 Kurt Granroth <granroth@kde.org>
     Original code: plugin code, connecting to Babelfish and support for selected text
   Copyright (C) 2003 Rand2342 <rand2342@yahoo.com>
     Submenus, KConfig file and support for other translation engines
   Copyright (C) 2008 Montel Laurent <montel@kde.org>
     Add webkitkde support
   Copyright (C) 2010 David Faure <faure@kde.org>
     Ported to KParts::TextExtension

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
#include "plugin_babelfish.h"

#include <kparts/part.h>
#include <kparts/browserextension.h>
#include <kparts/textextension.h>

#include <kactionmenu.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kmenu.h>
#include <kcomponentdata.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kconfig.h>
#include <kpluginfactory.h>
#include <kaboutdata.h>
#include <kicon.h>
#include <KConfigGroup>
#include <KDebug>

static const KAboutData aboutdata("babelfish", 0, ki18n("Translate Web Page") , "1.0" );
K_PLUGIN_FACTORY(BabelFishFactory, registerPlugin<PluginBabelFish>();)
K_EXPORT_PLUGIN(BabelFishFactory( aboutdata ) )

PluginBabelFish::PluginBabelFish( QObject* parent,
	                          const QVariantList & )
  : Plugin( parent ),
    m_actionGroup(this)
{
  setComponentData(BabelFishFactory::componentData());

  m_menu = new KActionMenu(KIcon("babelfish"), i18n("Transla&te"),
                          actionCollection() );
  actionCollection()->addAction( "translatewebpage", m_menu );
  m_menu->setDelayed( false );

  KParts::ReadOnlyPart* part = qobject_cast<KParts::ReadOnlyPart *>(parent);
  if (part) {
    connect(part, SIGNAL(started(KIO::Job*)), this, SLOT(slotEnableMenu()));
    connect(part, SIGNAL(completed()), this, SLOT(slotEnableMenu()));
    connect(part, SIGNAL(completed(bool)), this, SLOT(slotEnableMenu()));
	connect(m_menu, SIGNAL(triggered()), this, SLOT(translateURL()));
  }
}

PluginBabelFish::~PluginBabelFish()
{
  delete m_menu;
}

// Decide whether or not to enable the menu
void PluginBabelFish::slotEnableMenu()
{
    KParts::ReadOnlyPart* part = qobject_cast<KParts::ReadOnlyPart *>(parent());
    KParts::TextExtension* textExt = KParts::TextExtension::childObject(parent());

    //if (part)
    //    kDebug() << part->url();

  // Babelfish wants http/https URLs only.
  if ( part && textExt &&
       (part->url().protocol().compare (QLatin1String("http"), Qt::CaseInsensitive) == 0 ||
       part->url().protocol().compare (QLatin1String("https"), Qt::CaseInsensitive) == 0) &&
       part->inherits("KWebKitPart") )
  {
      m_menu->setEnabled( true );
  }
  else
  {
      m_menu->setEnabled( false );
  }
}

void PluginBabelFish::translateURL()
{
  // KWebKitPart provides a TextExtension, at least.
  // So if we got a part without a TextExtension, give an error.
  KParts::TextExtension* textExt = KParts::TextExtension::childObject(parent());
  Q_ASSERT(textExt); // already checked in slotAboutToShow

  KParts::BrowserExtension * ext = KParts::BrowserExtension::childObject(parent());
  if (!ext)
    return;
  KParts::ReadOnlyPart* part = qobject_cast<KParts::ReadOnlyPart *>(parent());

  // we check if we have text selected.  if so, we translate that. If
  // not, we translate the url
  QString textForQuery;
  KUrl url = part->url();
  const bool hasSelection = textExt->hasSelection();

  if (hasSelection) {
      QString selection = textExt->selectedText(KParts::TextExtension::PlainText);
      textForQuery = QString::fromLatin1(QUrl::toPercentEncoding(selection));
  } else {
    // Check syntax
    if ( !url.isValid() )
    {
      QString title = i18nc( "@title:window", "Malformed URL" );
      QString text = i18n( "The URL you entered is not valid, please "
                           "correct it and try again." );
      KMessageBox::sorry( 0L, text, title );
      return;
    }
  }
  const QString urlForQuery = QLatin1String(QUrl::toPercentEncoding( url.url() ));

  // Create URL
  QString query;
  if (hasSelection) { // http://translate.google.com/#en|de|text_to_translate
    query += "http://google.com/translate_t?langpair=auto|auto&text=" + textForQuery;
  }
  else { // http://translate.google.com/translate?hl=en&sl=en&tl=de&u=http%3A%2F%2Fkde.org%2F%2F
    query += "http://google.com/translate_c?langpair=auto|auto&ie=UTF-8&u=" + urlForQuery;
  }

  // Connect to the fish
  emit ext->openUrlRequest( KUrl( query ) );
}

#include <plugin_babelfish.moc>
