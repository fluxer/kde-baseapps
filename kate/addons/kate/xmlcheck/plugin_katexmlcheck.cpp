/***************************************************************************
                           plugin_katexmlcheck.cpp - checks XML files using xmllint
                           -------------------
	begin                : 2002-07-06
	copyright            : (C) 2002 by Daniel Naber
	email                : daniel.naber@t-online.de
 ***************************************************************************/

/***************************************************************************
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 ***************************************************************************/

/*
-fixme: show dock if "Validate XML" is selected (doesn't currently work when Kate
 was just started and the dockwidget isn't yet visible)
-fixme(?): doesn't correctly disappear when deactivated in config
*/

#include "plugin_katexmlcheck.h"
#include <QHBoxLayout>
#include "moc_plugin_katexmlcheck.cpp"

#include <qfile.h>
#include <qinputdialog.h>
#include <qregexp.h>
#include <qtextstream.h>
#include <QApplication>
#include <QTreeWidget>
#include <QHeaderView>
#include <QProcess>

#include <kaction.h>
#include <kactioncollection.h>
#include <kcursor.h>
#include <kdebug.h>
#include <kcomponentdata.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktemporaryfile.h>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY(PluginKateXMLCheckFactory, registerPlugin<PluginKateXMLCheck>();)
K_EXPORT_PLUGIN(PluginKateXMLCheckFactory("katexmlcheck"))

PluginKateXMLCheck::PluginKateXMLCheck( QObject* parent, const QVariantList& )
	: Kate::Plugin ( (Kate::Application *)parent )
{
}


PluginKateXMLCheck::~PluginKateXMLCheck()
{
}


Kate::PluginView *PluginKateXMLCheck::createView(Kate::MainWindow *mainWindow)
{
    return new PluginKateXMLCheckView(mainWindow);
}


//---------------------------------
PluginKateXMLCheckView::PluginKateXMLCheckView(Kate::MainWindow *mainwin)
    : Kate::PluginView (mainwin), Kate::XMLGUIClient(PluginKateXMLCheckFactory::componentData()),win(mainwin)
{
    dock = win->createToolView("kate_plugin_xmlcheck_ouputview", Kate::MainWindow::Bottom, SmallIcon("misc"), i18n("XML Checker Output"));
    listview = new QTreeWidget( dock );
    m_tmp_file=0;
    m_proc=0;
    QAction *a = actionCollection()->addAction("xml_check");
    a->setText(i18n("Validate XML"));
    connect(a, SIGNAL(triggered()), this, SLOT(slotValidate()));
    // TODO?:
    //(void)  new KAction ( i18n("Indent XML"), KShortcut(), this,
    //	SLOT(slotIndent()), actionCollection(), "xml_indent" );

    listview->setFocusPolicy(Qt::NoFocus);
    QStringList headers;
    headers << i18n("#");
    headers << i18n("Line");
    headers << i18n("Column");
    headers << i18n("Message");
    listview->setHeaderLabels(headers);
    listview->setRootIsDecorated(false);
    connect(listview, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(slotClicked(QTreeWidgetItem*,int)));

    QHeaderView *header = listview->header();
    header->setResizeMode(0, QHeaderView::ResizeToContents);
    header->setResizeMode(1, QHeaderView::ResizeToContents);
    header->setResizeMode(2, QHeaderView::ResizeToContents);

/* TODO?: invalidate the listview when document has changed
   Kate::View *kv = application()->activeMainWindow()->activeView();
   if( ! kv ) {
   kDebug() << "Warning: no Kate::View";
   return;
   }
   connect(kv, SIGNAL(modifiedChanged()), this, SLOT(slotUpdate()));
*/

    m_proc = new QProcess();
    connect(m_proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotProcExited(int,QProcess::ExitStatus)));
    // we currently only want errors:
    m_proc->setReadChannel(QProcess::StandardError);

    mainWindow()->guiFactory()->addClient(this);
}

PluginKateXMLCheckView::~PluginKateXMLCheckView()
{
    mainWindow()->guiFactory()->removeClient( this );
    delete m_proc;
    delete m_tmp_file;
    delete dock;
}

void PluginKateXMLCheckView::slotProcExited(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);

    // FIXME: doesn't work correct the first time:
    //if( m_dockwidget->isDockBackPossible() ) {
    //	m_dockwidget->dockBack();
//	}

    if (exitStatus != QProcess::NormalExit) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString("1").rightJustified(4,' '));
        item->setText(3, "Validate process crashed.");
        listview->addTopLevelItem(item);
        return;
    }

    kDebug() << "slotProcExited()";
    QApplication::restoreOverrideCursor();
    delete m_tmp_file;
    QString proc_stderr = QString::fromLocal8Bit(m_proc->readAllStandardError());
    m_tmp_file=0;
    listview->clear();
    uint list_count = 0;
    uint err_count = 0;
    if( ! m_validating ) {
        // no i18n here, so we don't get an ugly English<->Non-english mixup:
        QString msg;
        if( m_dtdname.isEmpty() ) {
            msg = "No DOCTYPE found, will only check well-formedness.";
        } else {
            msg = '\'' + m_dtdname + "' not found, will only check well-formedness.";
        }
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString("1").rightJustified(4,' '));
        item->setText(3, msg);
        listview->addTopLevelItem(item);
        list_count++;
    }
    if( ! proc_stderr.isEmpty() ) {
        QStringList lines = proc_stderr.split("\n", QString::SkipEmptyParts);
        QString linenumber, msg;
        int line_count = 0;
        for(QStringList::Iterator it = lines.begin(); it != lines.end(); ++it) {
            QString line = *it;
            line_count++;
            int semicolon_1 = line.indexOf(':');
            int semicolon_2 = line.indexOf(':', semicolon_1+1);
            int semicolon_3 = line.indexOf(':', semicolon_2+2);
            int caret_pos = line.indexOf('^');
            if( semicolon_1 != -1 && semicolon_2 != -1 && semicolon_3 != -1 ) {
                linenumber = line.mid(semicolon_1+1, semicolon_2-semicolon_1-1).trimmed();
                linenumber = linenumber.rightJustified(6, ' ');	// for sorting numbers
                msg = line.mid(semicolon_3+1, line.length()-semicolon_3-1).trimmed();
            } else if( caret_pos != -1 || line_count == lines.size() ) {
                // TODO: this fails if "^" occurs in the real text?!
                if( line_count == lines.size() && caret_pos == -1 ) {
                    msg = msg+'\n'+line;
                }
                QString col = QString::number(caret_pos);
                if( col == "-1" ) {
                    col = "";
                }
                err_count++;
                list_count++;
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, QString::number(list_count).rightJustified(4,' '));
                item->setText(1, linenumber);
                item->setTextAlignment(1, (item->textAlignment(1) & ~Qt::AlignHorizontal_Mask) | Qt::AlignRight);
                item->setText(2, col);
                item->setTextAlignment(2, (item->textAlignment(2) & ~Qt::AlignHorizontal_Mask) | Qt::AlignRight);
                item->setText(3, msg);
                listview->addTopLevelItem(item);
            } else {
                msg = msg+'\n'+line;
            }
        }
    }
    if( err_count == 0 ) {
        QString msg;
        if( m_validating ) {
            msg = "No errors found, document is valid.";	// no i18n here
        } else {
            msg = "No errors found, document is well-formed.";	// no i18n here
        }
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::number(list_count+1).rightJustified(4,' '));
        item->setText(3, msg);
        listview->addTopLevelItem(item);
    }
}


void PluginKateXMLCheckView::slotClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	kDebug() << "slotClicked";
	if( item ) {
		bool ok = true;
		uint line = item->text(1).toUInt(&ok);
		bool ok2 = true;
		uint column = item->text(2).toUInt(&ok);
		if( ok && ok2 ) {
			KTextEditor::View *kv = win->activeView();
			if( ! kv )
				return;

			kv->setCursorPosition(KTextEditor::Cursor (line-1, column));
		}
	}
}


void PluginKateXMLCheckView::slotUpdate()
{
	kDebug() << "slotUpdate() (not implemented yet)";
}


bool PluginKateXMLCheckView::slotValidate()
{
	kDebug() << "slotValidate()";

	win->showToolView (dock);

	QStringList procargs;
	m_validating = false;
	m_dtdname = "";

	KTextEditor::View *kv = win->activeView();
	if( ! kv )
	  return false;
        delete m_tmp_file;
	m_tmp_file = new KTemporaryFile();
	if( !m_tmp_file->open() ) {
		kDebug() << "Error (slotValidate()): could not create '" << m_tmp_file->fileName() << "': " << m_tmp_file->errorString();
		KMessageBox::error(0, i18n("<b>Error:</b> Could not create "
			"temporary file '%1'.", m_tmp_file->fileName()));
		delete m_tmp_file;
		m_tmp_file=0L;
		return false;
	}
	QTextStream s ( m_tmp_file );
	s << kv->document()->text();
	s.flush();

	const QString exe = KStandardDirs::findExe("xmllint");

	// use catalogs for KDE docbook:
	if( ! getenv("XML_CATALOG_FILES") ) {
		KComponentData ins("katexmlcheckplugin");
		QString catalogs;
		catalogs += ins.dirs()->findResource("data", "ksgmltools2/customization/catalog.xml");
		kDebug() << "catalogs: " << catalogs;
		setenv("XML_CATALOG_FILES", QFile::encodeName( catalogs ).data(), 1);
	}
	//kDebug() << "**catalogs: " << getenv("XML_CATALOG_FILES");

	procargs << "--noout";

	// tell xmllint the working path of the document's file, if possible.
	// otherweise it will not find relative DTDs
	QString path = kv->document()->url().directory();
	kDebug() << path;
	if (!path.isEmpty()) {
		procargs << "--path" << path;
	}

	// heuristic: assume that the doctype is in the first 10,000 bytes:
	QString text_start = kv->document()->text().left(10000);
	// remove comments before looking for doctype (as a doctype might be commented out
	// and needs to be ignored then):
	QRegExp re("<!--.*-->");
	re.setMinimal(true);
	text_start.remove(re);
	QRegExp re_doctype("<!DOCTYPE\\s+(.*)\\s+(?:PUBLIC\\s+[\"'].*[\"']\\s+[\"'](.*)[\"']|SYSTEM\\s+[\"'](.*)[\"'])", Qt::CaseInsensitive);
	re_doctype.setMinimal(true);

	if( re_doctype.indexIn(text_start) != -1 ) {
		QString dtdname;
		if( ! re_doctype.cap(2).isEmpty() ) {
			dtdname = re_doctype.cap(2);
		} else {
			dtdname = re_doctype.cap(3);
		}
		if( !dtdname.startsWith("http:") ) {		// todo: u_dtd.isLocalFile() doesn't work :-(
			// a local DTD is used
			m_validating = true;
			procargs << "--valid";
		} else {
			m_validating = true;
			procargs << "--valid";
		}
	} else if( text_start.indexOf("<!DOCTYPE") != -1 ) {
		// DTD is inside the XML file
		m_validating = true;
		procargs << "--valid";
	}
	procargs << m_tmp_file->fileName();

	m_proc->start(exe, procargs);
	if( ! m_proc->waitForStarted(-1) ) {
		KMessageBox::error(0, i18n("<b>Error:</b> Failed to execute xmllint. Please make "
			"sure that xmllint is installed. It is part of libxml2."));
		return false;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	return true;
}
