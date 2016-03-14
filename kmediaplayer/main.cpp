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

#include <KAboutData>
#include <KCmdLineArgs>
#include <KApplication>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KParts/Part>
#include <KLocale>
#include <KMessageBox>
#include <KDebug>
#include <KMainWindow>

int main(int argc, char **argv) {
    KAboutData aboutData("kmediaplayer", 0, ki18n("KMediaPlayer"),
                         "1.0.0", ki18n("Simple media player for KDE."),
                         KAboutData::License_GPL_V2,
                         ki18n("(c) 2016 Ivailo Monev"),
                         KLocalizedString(),
                        "http://github.com/fluxer/katana"
                        );

    aboutData.addAuthor(ki18n("Ivailo Monev"),
                        ki18n("Maintainer"),
                        "xakepa10@gmail.com");
    aboutData.setProgramIconName(QLatin1String("applications-multimedia"));

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions option;
    option.add("+[url]", ki18n("URL to be opened"));
    KCmdLineArgs::addCmdLineOptions(option);

    KApplication *app = new KApplication();

    KMainWindow *window = new KMainWindow();

    KPluginLoader loader(QLatin1String("kmediaplayerpart"));
    KPluginFactory *factory = loader.factory();
    KParts::ReadOnlyPart *part;
    if (factory) {
        part = factory->create<KParts::ReadOnlyPart>(window);
    }
    if (!factory || !part) {
        kWarning() << "Error loading KMediaPlayer KPart:" << loader.errorString();
        KMessageBox::error(window, i18n("Unable to load KMediaPlayer's KPart component, please check your installation."));
        return 1;
    }

    part->setObjectName(QLatin1String("KMediaPlayer"));
    window->setCentralWidget(part->widget());
    window->setAutoSaveSettings();
    window->show();

    app->connect(app, SIGNAL(saveYourself()), part, SLOT(closeUrl()));

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    for (int pos = 0; pos < args->count(); ++pos) {
        part->openUrl(args->url(pos));
    }

    return app->exec();
}
