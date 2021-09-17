#include "dbpolaczenie.h"
#include <QSqlQuery>
#include <QSqlError>

DBPolaczenie::DBPolaczenie(const QString &serwer,
                           const QString &driver,
                           const QString &uzytkownik,
                           const QString &haslo,
                           const QString &bazaNazwa,
                           bool trustedConnection)
{
    mBaza = QSqlDatabase::addDatabase("QODBC");
    mBazaNazwa = bazaNazwa;
    mSerwer = serwer;
    mDriver = driver;
    mUzytkownik = uzytkownik;
    mHaslo = haslo;
    mTrustedConnection = trustedConnection;
}

bool DBPolaczenie::otworzBaze(QString *error)
{
    mBaza.setDatabaseName(QString("DRIVER={%1};SERVER=%2;"
                                  "DATABASE=%3;UID=%4;PWD=%5;"
                                  "Trusted_Connection=%6;")
                          .arg(mDriver)
                          .arg(mSerwer)
                          .arg(mBazaNazwa)
                          .arg(mUzytkownik)
                          .arg(mHaslo)
                          .arg(mTrustedConnection ? "Yes" : "No"));

    if (! mBaza.open())
    {
        if (error != nullptr)
        {
            *error = mBaza.lastError().text();
        }
        return false;
    }
    return true;
}

bool DBPolaczenie::otwartaBaza()
{
    return mBaza.isOpen();
}
