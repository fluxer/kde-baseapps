/*  This file is part of the Kate project.
 *
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

#include "kateprojectinfoviewterminal.h"
#include "kateprojectpluginview.h"

#include <klocale.h>
#include <kde_terminal_interface.h>
#include <kservice.h>

KateProjectInfoViewTerminal::KateProjectInfoViewTerminal (KateProjectPluginView *pluginView, KateProject *project)
  : QWidget ()
  , m_pluginView (pluginView)
  , m_project (project)
  , m_konsolePart (0)
{
  /**
   * layout widget
   */
  m_layout = new QVBoxLayout;
  m_layout->setSpacing (0);
  setLayout (m_layout);
  
  /**
   * initial terminal creation
   */
  loadTerminal ();
}

KateProjectInfoViewTerminal::~KateProjectInfoViewTerminal ()
{
  /**
   * avoid endless loop
   */
  if (m_konsolePart)
    disconnect (m_konsolePart, SIGNAL(destroyed()), this, SLOT(loadTerminal()));
}

void KateProjectInfoViewTerminal::loadTerminal ()
{
  /**
   * null in any case, if loadTerminal fails below and we are in the destroyed event
   */
  m_konsolePart = 0;
  
  /**
   * get konsole part service
   */
  KService::Ptr service = KService::serviceByDesktopName("konsolepart");
  if (!service)
    return;

  /**
   * create part
   */
  m_konsolePart = service->createInstance<KParts::ReadOnlyPart>(this, this, QVariantList());
  if (!m_konsolePart)
    return;

  /**
   * init locale translation stuff
   */
  KGlobal::locale()->insertCatalog("konsole");
  
  /**
   * switch to right directory
   */
  qobject_cast<TerminalInterface*>(m_konsolePart)->showShellInDir (QFileInfo (m_project->fileName()).absolutePath());
  
  /**
   * add to widget
   */
  m_layout->addWidget (m_konsolePart->widget());
  setFocusProxy(m_konsolePart->widget());
  
  /**
   * guard destruction, create new terminal!
   */
  connect (m_konsolePart, SIGNAL(destroyed()), this, SLOT(loadTerminal()));
  connect (m_konsolePart, SIGNAL(overrideShortcut(QKeyEvent*,bool&)),
                    this, SLOT(overrideShortcut(QKeyEvent*,bool&)));
}

void KateProjectInfoViewTerminal::overrideShortcut (QKeyEvent *, bool &override)
{
  /**
   * let konsole handle all shortcuts
   */
  override = true;
}

// kate: space-indent on; indent-width 2; replace-tabs on;
