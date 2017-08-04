/*    kcookiesmain.h - Cookies configuration
 *
 *    First version of cookies configuration:
 *        Copyright (C) Waldo Bastian <bastian@kde.org>
 *    This dialog box:
 *        Copyright (C) David Faure <faure@kde.org>
 *
 */

#ifndef KCOOKIESMAIN_H
#define KCOOKIESMAIN_H

#include <kcmodule.h>

#include <QTabWidget>
class KCookiesPolicies;
class KCookiesManagement;

class KCookiesMain : public KCModule
{
    Q_OBJECT
public:
    KCookiesMain(QWidget *parent, const QVariantList &args);
    ~KCookiesMain();

    KCookiesPolicies* policyDlg() { return policies; }

    virtual void save();
    virtual void load();
    virtual void defaults();
    virtual QString quickHelp() const;

private:

    QTabWidget* tab;
    KCookiesPolicies* policies;
    KCookiesManagement* management;
};

#endif // KCOOKIESMAIN_H
