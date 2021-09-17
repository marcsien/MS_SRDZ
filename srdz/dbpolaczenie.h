#ifndef DBPOLACZENIE_H
#define DBPOLACZENIE_H

#include <QSqlDatabase>

class DBPolaczenie
{
public:
    DBPolaczenie(const QString &serwer,
                 const QString &driver,
                 const QString &uzytkownik,
                 const QString &haslo,
                 const QString &bazaNazwa,
                 bool trustedConnection = true);
    bool otworzBaze(QString *error = nullptr);
    bool otwartaBaza();
private:
    QSqlDatabase mBaza;
    QString mSerwer;
    QString mDriver;
    QString mUzytkownik;
    QString mHaslo;
    QString mBazaNazwa;
    bool mTrustedConnection;
};

#endif // DBPOLACZENIE_H
