/*****************************************************************************
 *   Copyright (C) 2014 by Emmanuel Pescosta <emmanuelpescosta099@gmail.com> *
 *   Copyright (C) 2012 by Sergei Stolyarov <sergei@regolit.com>             *
 *   Copyright (C) 2010 by Thomas Richard <thomas.richard@proan.be>          *
 *   Copyright (C) 2009-2010 by Peter Penz <peter.penz19@gmail.com>          *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA              *
 *****************************************************************************/

#include <KAction>
#include <KFileItem>
#include <KFileItemListProperties>
#include <KLocalizedString>
#include <KActionCollection>
#include <KPluginFactory>
#include <KIcon>

#include <QDir>
#include <QPointer>
#include <QLocalSocket>
#include <QFileSystemWatcher>

#include "fileviewdropboxplugin.h"

K_PLUGIN_FACTORY(FileViewDropboxPluginFactory, registerPlugin<FileViewDropboxPlugin>();)
K_EXPORT_PLUGIN(FileViewDropboxPluginFactory("fileviewdropboxplugin"))

class FileViewDropboxPlugin::Private
{
public:
    Private(FileViewDropboxPlugin* parent) :
        contextFilePaths(),
        controlSocketPath(),
        controlSocket(new QLocalSocket(parent)),
        databaseFileWatcher(new QFileSystemWatcher(parent)),
        contextActions(new KActionCollection(parent))
    {
    }

    QStringList contextFilePaths;
    QString controlSocketPath;
    QPointer<QLocalSocket> controlSocket;
    QPointer<QLocalSocket> itemStateSocket;
    QPointer<QFileSystemWatcher> databaseFileWatcher;
    QPointer<KActionCollection> contextActions;
};

QMap<QString, KVersionControlPlugin2::ItemVersion> FileViewDropboxPlugin::m_itemVersions;

FileViewDropboxPlugin::FileViewDropboxPlugin(QObject* parent, const QVariantList& args):
    KVersionControlPlugin2(parent),
    d(new Private(this))
{
    Q_UNUSED(args);

    if (m_itemVersions.isEmpty()) {
        m_itemVersions.insert("up to date", KVersionControlPlugin2::NormalVersion);
        m_itemVersions.insert("syncing",    KVersionControlPlugin2::UpdateRequiredVersion);
        m_itemVersions.insert("unsyncable", KVersionControlPlugin2::ConflictingVersion);
        m_itemVersions.insert("unwatched",  KVersionControlPlugin2::UnversionedVersion);
    }

    const QString dropboxDir = QDir::home().path() % QDir::separator() % fileName() % QDir::separator();
    d->controlSocketPath = QDir::toNativeSeparators(dropboxDir % QLatin1String("command_socket"));
    d->controlSocket->connectToServer(d->controlSocketPath);

    connect(d->databaseFileWatcher, SIGNAL(fileChanged(QString)), SIGNAL(itemVersionsChanged()));
    d->databaseFileWatcher->addPath(QDir::toNativeSeparators(dropboxDir % QLatin1String("aggregation.dbx")));

    connect(d->contextActions, SIGNAL(actionTriggered(QAction*)), SLOT(handleContextAction(QAction*)));
}

FileViewDropboxPlugin::~FileViewDropboxPlugin()
{
    delete d;
}

QString FileViewDropboxPlugin::fileName() const
{
    return QLatin1String(".dropbox");
}

bool FileViewDropboxPlugin::beginRetrieval(const QString& directory)
{
    Q_UNUSED(directory);
    Q_ASSERT(directory.endsWith(QLatin1Char('/')));

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

    d->itemStateSocket = new QLocalSocket;

    return connectWithDropbox(d->itemStateSocket, LongTimeout);
}

KVersionControlPlugin2::ItemVersion FileViewDropboxPlugin::itemVersion(const KFileItem& item) const
{
    const QStringList reply = sendCommand("icon_overlay_file_status\npath\t", QStringList() << QDir(item.localPath()).canonicalPath(),
                                          d->itemStateSocket, WaitForReply, LongTimeout);
    if(reply.count() < 2) {
        // file/dir is not served by dropbox
        return KVersionControlPlugin2::UnversionedVersion;
    }

    return m_itemVersions.value(reply.at(1), KVersionControlPlugin2::UnversionedVersion);
}

void FileViewDropboxPlugin::endRetrieval()
{
    delete d->itemStateSocket;
}

QList<QAction*> FileViewDropboxPlugin::actions(const KFileItemList& items) const
{
    Q_ASSERT(!items.isEmpty());

    d->contextActions->clear();
    d->contextFilePaths.clear();

    const KFileItemListProperties properties(items);
    if (!properties.isLocal()) {
        // not all files/dirs are local files/dirs
        return QList<QAction*>();
    }

    foreach (const KFileItem& item, items) {
        d->contextFilePaths << QDir(item.localPath()).canonicalPath();
    }

    const QStringList reply = sendCommand("icon_overlay_context_options\npaths\t", d->contextFilePaths, d->controlSocket, WaitForReply);
    if (reply.count() < 2) {
        // files/dirs are not served by dropbox
        return QList<QAction*>();
    }

    // analyze item options and dynamically form a menu
    foreach (const QString& replyLine, reply) {
        const QStringList options = replyLine.split("~");

        if (options.count() > 2) {
            KAction* action = d->contextActions->addAction(options.at(2));
            action->setText(options.at(0));
            action->setToolTip(options.at(1));
            action->setIcon(KIcon("dropbox"));
        }
    }

    return d->contextActions->actions();
}

void FileViewDropboxPlugin::handleContextAction(QAction* action)
{
    sendCommand("icon_overlay_context_action\nverb\t" % action->objectName() % "\npaths\t", d->contextFilePaths, d->controlSocket);
}

QStringList FileViewDropboxPlugin::sendCommand(const QString& command,
                                               const QStringList& paths,
                                               const QPointer<QLocalSocket>& socket,
                                               SendCommandMode mode,
                                               SendCommandTimeout timeout) const
{
    if (!connectWithDropbox(socket, timeout)) {
        return QStringList();
    }

    static const QString parameterSeperator('\t');
    static const QString done("\ndone\n");
    static const QString ok("ok\n");

    const QString request = command % paths.join(parameterSeperator) % done;

    socket->readAll();
    socket->write(request.toUtf8());
    socket->flush();

    if (mode == SendCommandOnly) {
        return QStringList();
    }

    QString reply;
    while (socket->waitForReadyRead(timeout == ShortTimeout ? 100 : 500)) {
        reply.append(QString::fromUtf8(socket->readAll()));

        if (reply.endsWith(done)) {
            break;
        }
    }

    reply.remove(done);
    reply.remove(ok);

    return reply.split(parameterSeperator, QString::SkipEmptyParts);
}

bool FileViewDropboxPlugin::connectWithDropbox(const QPointer<QLocalSocket>& socket, SendCommandTimeout timeout) const
{
    if (socket->state() != QLocalSocket::ConnectedState) {
        socket->connectToServer(d->controlSocketPath);

        if (!socket->waitForConnected(timeout == ShortTimeout ? 100 : 500)) {
            socket->abort();
            return false;
        }
    }

    return true;
}

#include "moc_fileviewdropboxplugin.cpp"
