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
#include <KStandardDirs>

static const KAboutData aboutdata("babelfish", 0, ki18n("Translate Web Page") , "1.0" );
K_PLUGIN_FACTORY(BabelFishFactory, registerPlugin<PluginBabelFish>();)
K_EXPORT_PLUGIN(BabelFishFactory( aboutdata ) )

PluginBabelFish::PluginBabelFish( QObject* parent,
	                          const QVariantList & )
  : Plugin( parent ),
    m_actionGroup(this)
{
  setComponentData(BabelFishFactory::componentData());

  m_menu = new KActionMenu(KIcon("babelfish"), i18n("Transla&te to"),
                          actionCollection() );
  actionCollection()->addAction( "translatewebpage", m_menu );
  m_menu->setDelayed( false );

  // https://cloud.google.com/translate/v2/using_rest#language-params
#warning "shortcuts - any policy for that? also, some API codes do not represent the actual locale code"
  static const char * const translations[] = {
      I18N_NOOP("&Auto"), "auto",
      I18N_NOOP("&Afrikaans"), "af",
      I18N_NOOP("&Albanian"), "sq",
      I18N_NOOP("&Arabic"), "ar",
      I18N_NOOP("&Azerbaijani"), "az",
      I18N_NOOP("&Basque"), "eu",
      I18N_NOOP("&Bengali"), "bn",
      I18N_NOOP("&Belarusian"), "be",
      I18N_NOOP("&Bulgarian"), "bg",
      I18N_NOOP("&Catalan"), "ca",
      I18N_NOOP("&Chinese (Simplified)"), "zh",
      I18N_NOOP("&Chinese (Traditional)"), "zh-TW",
      I18N_NOOP("&Croatian"), "hr",
      I18N_NOOP("&Czech"), "cs",
      I18N_NOOP("&Danish"), "da",
      I18N_NOOP("&Dutch"), "nl",
      I18N_NOOP("&English"), "en",
      I18N_NOOP("&Esperanto"), "eo",
      I18N_NOOP("&Estonian"), "et",
      I18N_NOOP("&Filipino"), "tl",
      I18N_NOOP("&Finnish"), "fi",
      I18N_NOOP("&French"), "fr",
      I18N_NOOP("&Galician"), "gl",
      I18N_NOOP("&Georgian"), "ka",
      I18N_NOOP("&German"), "de",
      I18N_NOOP("&Greek"), "el", // API ok, flag not
      I18N_NOOP("&Gujarati"), "gu",
      I18N_NOOP("&Haitian Creole"), "ht",
      I18N_NOOP("&Hebrew"), "iw",
      I18N_NOOP("&Hindi"), "hi",
      I18N_NOOP("&Hungarian"), "hu",
      I18N_NOOP("&Icelandic"), "is",
      I18N_NOOP("&Indonesian"), "id",
      I18N_NOOP("&Irish"), "ga",
      I18N_NOOP("&Italian"), "it",
      I18N_NOOP("&Japanese"), "ja",
      I18N_NOOP("&Kannada"), "kn",
      I18N_NOOP("&Korean"), "ko",
      I18N_NOOP("&Latin"), "la",
      I18N_NOOP("&Latvian"), "lv",
      I18N_NOOP("&Lithuanian"), "mk",
      I18N_NOOP("&Macedonian"), "lt",
      I18N_NOOP("&Malay"), "ms",
      I18N_NOOP("&Maltese"), "mt",
      I18N_NOOP("&Norwegian"), "no",
      I18N_NOOP("&Persian"), "fa",
      I18N_NOOP("&Polish"), "pl",
      I18N_NOOP("&Portuguese"), "pt",
      I18N_NOOP("&Romanian"), "ro",
      I18N_NOOP("&Russian"), "ru",
      I18N_NOOP("&Serbian"), "sr",
      I18N_NOOP("&Slovak"), "lt",
      I18N_NOOP("&Slovenian"), "sk",
      I18N_NOOP("&Spanish"), "es",
      I18N_NOOP("&Swahili"), "sw",
      I18N_NOOP("&Swedish"), "sv",
      I18N_NOOP("&Tamil"), "ta",
      I18N_NOOP("&Telugu"), "ta",
      I18N_NOOP("&Thai"), "th",
      I18N_NOOP("&Turkish"), "tr",
      I18N_NOOP("&Ukrainian"), "uk",
      I18N_NOOP("&Urdu"), "ur",
      I18N_NOOP("&Vietnamese"), "vi",
      I18N_NOOP("&Welsh"), "cy",
      I18N_NOOP("&Yiddish"), "yi",
      0, 0
    };

  for (int i = 0; translations[i]; i += 2) {
    const QString translation = QString::fromLatin1(translations[i + 1]);
    QAction* a = actionCollection()->addAction(translation);
    a->setText(i18n(translations[i]));
    QString flag = KStandardDirs::locate("locale", QString("l10n/%1/flag.png").arg(translation));
    if (flag.isEmpty()) {
        flag = "babelfish";
    }
    a->setIcon(KIcon(flag));
    m_menu->addAction(a);
    m_actionGroup.addAction(a);
    connect(&m_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(translateURL(QAction*)));
  }

  KParts::ReadOnlyPart* part = qobject_cast<KParts::ReadOnlyPart *>(parent);
  if (part) {
    connect(part, SIGNAL(started(KIO::Job*)), this, SLOT(slotEnableMenu()));
    connect(part, SIGNAL(completed()), this, SLOT(slotEnableMenu()));
    connect(part, SIGNAL(completed(bool)), this, SLOT(slotEnableMenu()));
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

void PluginBabelFish::translateURL(QAction* action)
{
  // KWebKitPart provides a TextExtension, at least.
  // So if we got a part without a TextExtension, give an error.
  KParts::TextExtension* textExt = KParts::TextExtension::childObject(parent());
  Q_ASSERT(textExt);

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
      KMessageBox::sorry( 0L, i18nc( "@title:window", "Malformed URL"),
                            i18n("The URL you entered is not valid, please "
                                 "correct it and try again."));
      return;
    }
  }
  const QString urlForQuery = QLatin1String(QUrl::toPercentEncoding( url.url() ));

  // Create URL
  const QString language = action->objectName();
  QString query;
  if (hasSelection && !textForQuery.isEmpty()) {
    query += "http://translate.google.com/translate_t?langpair=auto|" + language + "&text=" + textForQuery;
  }
  else {
    query += "http://translate.google.com/translate_c?langpair=auto|" + language + "&ie=UTF-8&u=" + urlForQuery;
  }

  // Connect to the fish
  emit ext->openUrlRequest( KUrl( query ) );
}

#include <plugin_babelfish.moc>
