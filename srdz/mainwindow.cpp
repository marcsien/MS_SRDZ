#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QSqlQuery"
#include "logowanie.h"
#include <QTimer>
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>
#include <decyzja.h>
#include <QFileDialog>
#include <pokazdec.h>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mDBPolaczenie ("MS-KOMPUTER\\SQLEXPRESS",
                   "SQL SERVER",
                   "sa",
                   "srdzhaslo1",
                   "SRDZ",
                   true)
{
    ui->setupUi(this);
    mModel = nullptr;
    QString error1;
    if (! mDBPolaczenie.otworzBaze(&error1))
    {
        QMessageBox::critical(this,"Error",error1);
        return;
    }
    ui->comboBoxKryterium->addItems({"Nazwa","Typ Zdarzenia","Użytkownik"});
    QTimer::singleShot(0,this,SLOT(autostart()));
    init();
    initNetwork();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logincheck()
{
    QString error2;
    QString query2 = QString("SELECT Uzytkownik.IDUzytkownika AS [Numer Uzytkownika], Uzytkownik.Login AS [Login], Uzytkownik.Haslo AS [Haslo], RodzajUzytkownika.Nazwa AS [Rodzaj Użytkownika] "
                  "FROM Uzytkownik "
                  "INNER JOIN RodzajUzytkownika ON Uzytkownik.IDUzytkownika=RodzajUzytkownika.IDRodzaju "
                  "WHERE Login = '%1' AND Haslo = '%2'")
            .arg(login)
            .arg(passwd);

    if (! mDBPolaczenie.otworzBaze(&error2))
    {
        QMessageBox::critical(this,"Error",error2);
    }

    QSqlQueryModel mLModel ;
    mLModel.setQuery(query2);

    if(mLModel.rowCount()==1)
    {
        logged = true;
    }
    else
    {
        logged = false;
    }
}

void MainWindow::permitscheck(QString permition)
{
    QString error2;
    QString query2 = QString("SELECT Uzytkownik.IDUzytkownika AS [Numer Uzytkownika], Uzytkownik.Login AS [Login], Uzytkownik.Haslo AS [Haslo], RodzajUzytkownika.Nazwa AS [Rodzaj Uzytkownika], Uprawnienia.IDUprawnienia AS [IDUprawnienia], Uprawnienie.Edycja AS [Edycja], Uprawnienie.Podglad AS [Podglad], Uprawnienie.Dodawanie AS [Dodawanie] "
                             "FROM Uzytkownik "
                             "INNER JOIN RodzajUzytkownika ON Uzytkownik.IDUzytkownika=RodzajUzytkownika.IDRodzaju "
                             "INNER JOIN Uprawnienia ON  Uprawnienia.IDRodzaju=RodzajUzytkownika.IDRodzaju "
                             "INNER JOIN Uprawnienie ON  Uprawnienie.IDUprawnienia=Uprawnienia.IDUprawnienia "
                             "WHERE Login = '%1' AND Haslo = '%2' AND %3 = '1'")
            .arg(login)
            .arg(passwd)
            .arg(permition);


    if (! mDBPolaczenie.otworzBaze(&error2))
    {
        QMessageBox::critical(this,"Error",error2);
    }

    QSqlQueryModel mPModel;
    mPModel.setQuery(query2);

    if (permition == "Edycja")
    {
        if(mPModel.rowCount()>=1)
        {
            edit = true;
        }
        else
        {
            edit = false;
        }
    }
    else if (permition == "Podglad")
    {
        if(mPModel.rowCount()>=1)
        {
            preview = true;
        }
        else
        {
            preview = false;
        }
    }
    else if (permition == "Dodawanie")
    {
        if(mPModel.rowCount()>=1)
        {
            add = true;
        }
        else
        {
            add = false;
        }
    }
    else
    {
        edit = false;
        preview = false;
        add = false;
    }
}

void MainWindow::showselectintable(QString query)
{
    if (mModel == nullptr)
    {
        mModel = new QSqlQueryModel(this);
        mModel -> setQuery(query);
        ui -> tableView->setModel(mModel);
        ui -> tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui -> tableView->verticalHeader()->hide();
        ui -> tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
    else
    {
        mModel -> clear();
        mModel -> setQuery(query);
        ui -> tableView->setModel(mModel);
        ui -> tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui -> tableView->verticalHeader()->hide();
        ui -> tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

void MainWindow::checkforalert()
{
    QString query3="SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                   "FROM Zdarzenia "
                   "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                   "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                   "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                   "LEFT JOIN Obsluga ON Zdarzenia.IDZdarzenia=Obsluga.IDZdarzenia "
                   "WHERE IDObslugi IS NULL";
    QString error3;
    if (! mDBPolaczenie.otworzBaze(&error3))
    {
        QMessageBox::critical(this,"Error",error3);
    }

    QSqlQueryModel mAModel;
    mAModel.setQuery(query3);
    if(mAModel.rowCount())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Masz %1 nowe zdarzenia").arg(mAModel.rowCount()));
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        on_pushButton_2_clicked();
    }
}

int MainWindow::checkloginid()
{
    if (logged)
    {
        QString query4=QString("SELECT Uzytkownik.IDUzytkownika AS [ID], Uzytkownik.Login AS [Login], Uzytkownik.Haslo AS [Haslo] "
                               "FROM Uzytkownik "
                               "WHERE Login = '%1' AND Haslo = '%2'")
                               .arg(login)
                               .arg(passwd);
//        QString error4;
//        if (! mDBPolaczenie.otworzBaze(&error4))
//        {
//            QMessageBox::critical(this,"Error",error4);
//        }

    if(mDBPolaczenie.otwartaBaza())
    {
        QSqlQueryModel mIModel;
        mIModel.setQuery(query4);
        if(mIModel.rowCount()==1)
        {
           int i =  mIModel.record(0).value(0).toInt();
           return i;
        }
    }
    }
}

QString MainWindow::checkrecord(QString query, int row)
{
    if (logged)
    {
        QString error5;
        if (mDBPolaczenie.otwartaBaza())
        {
            QSqlQueryModel *RmIModel = new QSqlQueryModel;
            RmIModel->setQuery(query);
            if(RmIModel->rowCount()==1)
            {
               QString ret =  RmIModel->record(0).value(row).toString();
               delete RmIModel;
               return ret;
            }
            else {
                delete RmIModel;
            }

         }
    }
}

bool MainWindow::isEmptySelect(QString query)
{
    if (logged && mDBPolaczenie.otwartaBaza())
    {
//        QString error6;
//        if (! mDBPolaczenie.otworzBaze(&error6))
//        {
//            QMessageBox::critical(this,"Error",error6);
//        }

        QSqlQueryModel *RmIModel = new QSqlQueryModel;
        RmIModel->setQuery(query);
        if(RmIModel->rowCount()>=1)
        {
           return 1;
        }
        else
        {
            return 0;
        }
        delete RmIModel;
    }
}

void MainWindow::on_actionPo_cz_z_baz_danych_triggered()
{
    if(logged && preview)
    {
        QString query1="SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                       "FROM Zdarzenia "
                       "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                       "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                       "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika";
        showselectintable(query1);

    }
    else if (logged && !preview)
    {
        QMessageBox msgBox;
        msgBox.setText("Brak uprawnień !");
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Należy się najpierw zalogować");
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_actionZako_cz_triggered()
{
    close();
}

void MainWindow::on_actionO_Qt_triggered()
{
    QMessageBox::aboutQt(this,"Qt");
}

void MainWindow::on_actionLogowanie_triggered()
{
   Logowanie * dlg = new Logowanie(this);
   if (dlg->exec() == QDialog::Accepted) {
       passwd = dlg->getPassword();
       login = dlg->getUser();
   }
   delete dlg;

   logincheck();

   if (logged)
   {
       QMessageBox msgBox;
       msgBox.setText("Zalogowano !");
       msgBox.setWindowTitle("Informacja");
       msgBox.setIcon(QMessageBox::Information);
       msgBox.exec();
       permitscheck("Edycja");
       permitscheck("Podglad");
       permitscheck("Dodawanie");

   }
   else
   {
       QMessageBox msgBox;
       msgBox.setText("Niepoprawne dane logowania");
       msgBox.setWindowTitle("Błąd");
       msgBox.setIcon(QMessageBox::Warning);
       msgBox.exec();
       edit = false;
       preview = false;
       add = false;
   }
}

void MainWindow::autostart()
{
  on_actionLogowanie_triggered();
  if (preview)
  {
       checkforalert();
  }
}

void MainWindow::on_Wyszukaj_clicked()
{
    int selectedcombo = ui->comboBoxKryterium->currentIndex();
    if (logged && preview)
    {
        switch (selectedcombo)
        {
        case 0:
        {
            QString query3 = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                                     "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                                     "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE NazwaZdarzenia = '%1'")
                                     .arg(ui->lineEditFraza->text());
            showselectintable(query3);
            break;
        }
        case 1:
        {
            QString query3 = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                                     "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                                     "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE TypZdarzenia = '%1'")
                                     .arg(ui->lineEditFraza->text());
            showselectintable(query3);
            break;
        }
        case 2:
        {
            QString query3 = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                                     "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                                     "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE Login = '%1'")
                                     .arg(ui->lineEditFraza->text());
            showselectintable(query3);
            break;
        }
        }
    }
    else if (logged && !preview)
    {
        QMessageBox msgBox;
        msgBox.setText("Brak uprawnień !");
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Należy się najpierw zalogować");
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_clicked()
{
    on_actionPo_cz_z_baz_danych_triggered();
}

void MainWindow::on_pushButton_5_clicked()
{
    close();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (preview)
    {
        QString query1="SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                       "FROM Zdarzenia "
                       "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                       "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                       "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                       "LEFT JOIN Obsluga ON Zdarzenia.IDZdarzenia=Obsluga.IDZdarzenia "
                       "WHERE IDObslugi IS NULL";
        showselectintable(query1);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Brak uprawnień !");
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->tableView->currentIndex().isValid())
    {
        QModelIndex curIndex = ui -> tableView->currentIndex();
        QModelIndex currIndex = ui->tableView->model()->index(curIndex.row(),0);
        int numerZdarzenia = ui->tableView->model()->data(currIndex).toInt();
        int idUzytkownia = checkloginid();
        QDateTime datetime(QDateTime::currentDateTime());
        QString komentarz;
        QString query9 = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia],Zdarzenia.NazwaZdarzenia AS [Nazwa],Zdarzenia.CzasZdarzenia AS [Czas],Typ.TypZdarzenia AS [Typ],Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                                     "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                                     "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE Zdarzenia.IDZdarzenia = '%1'")
                                     .arg(numerZdarzenia);

        Decyzja * dlg = new Decyzja(this);
        dlg->writeid(numerZdarzenia);
        dlg->writename(checkrecord(query9,1));
        dlg->writetype(checkrecord(query9,3));
        dlg->writetime(checkrecord(query9,2));
        dlg->writeplace(checkrecord(query9,4));
        dlg->writeuser(checkrecord(query9,5));
        if (dlg->exec() == QDialog::Accepted) {
            komentarz = dlg->gettext();
        }
        delete dlg;
        if (!komentarz.isEmpty())
        {
            QString query5 = QString("INSERT INTO Obsluga(IDZdarzenia,CzasObslugi,Komentarz,IDUzytkownika) "
                                     "VALUES ('%1' , '%2' , '%3' , '%4')")
                                     .arg(numerZdarzenia)
                                     .arg(datetime.toString("yyyy-MM-dd HH:mm:ss"))
                                     .arg(komentarz)
                                     .arg(idUzytkownia);

            QSqlQuery addquery;
            addquery.exec(query5);
        }
    }

}

void MainWindow::on_pushButton_7_clicked()
{
    if (preview)
    {
        QString query6="SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                       "FROM Zdarzenia "
                       "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                       "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                       "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                       "LEFT JOIN Obsluga ON Zdarzenia.IDZdarzenia=Obsluga.IDZdarzenia "
                       "WHERE IDObslugi IS NOT NULL";
        showselectintable(query6);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Brak uprawnień !");
        msgBox.setWindowTitle("Informacja");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(ui->tableView->currentIndex().isValid())
    {
        QModelIndex curIndex = ui -> tableView->currentIndex();
        QModelIndex currIndex = ui->tableView->model()->index(curIndex.row(),0);
        int numerZdarzenia = ui->tableView->model()->data(currIndex).toInt();
        QString query9 = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia],Zdarzenia.NazwaZdarzenia AS [Nazwa],Zdarzenia.CzasZdarzenia AS [Czas],Typ.TypZdarzenia AS [Typ],Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający], Obsluga.CzasObslugi AS [czasobsl], Obsluga.Komentarz AS [opis], Uzytkownik.Login AS [dispatcher] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                                     "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                                     "INNER JOIN Obsluga ON Zdarzenia.IDZdarzenia=Obsluga.IDZdarzenia "
                                     "INNER JOIN Uzytkownik ON Obsluga.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE Zdarzenia.IDZdarzenia = '%1'")
                                     .arg(numerZdarzenia);
        QString queryusr = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Uzytkownik.Login AS [Zgłaszający] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Obsluga ON Zdarzenia.IDZdarzenia=Obsluga.IDZdarzenia "
                                     "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE Zdarzenia.IDZdarzenia = '%1'")
                                     .arg(numerZdarzenia);

        Pokazdec * pok = new Pokazdec(this);

        if(isEmptySelect(query9))
        {
            pok->writeid(numerZdarzenia);
            pok->writename(checkrecord(query9,1));
            pok->writetype(checkrecord(query9,3));
            pok->writetime(checkrecord(query9,2));
            pok->writeplace(checkrecord(query9,4));
            pok->writeuser(checkrecord(queryusr,1));

            pok->writedectime(checkrecord(query9,6));
            pok->writedispatcher(checkrecord(query9,8));
            pok->setdescription(checkrecord(query9,7));

            pok->exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText(QString("Zdarzenie o ID: %1 nie posiada dyspozycji !").arg(numerZdarzenia));
            msgBox.setWindowTitle("Informacja");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
        delete pok;

    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ui->tableView->currentIndex().isValid())
    {
        QModelIndex curIndex = ui -> tableView->currentIndex();
        QModelIndex currIndex = ui->tableView->model()->index(curIndex.row(),0);
        int numerZdarzenia = ui->tableView->model()->data(currIndex).toInt();
        QString query9 = QString("SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia],Zdarzenia.NazwaZdarzenia AS [Nazwa],Zdarzenia.CzasZdarzenia AS [Czas],Typ.TypZdarzenia AS [Typ],Miejsce.Wspolrzedne AS [Współrzędne], Uzytkownik.Login AS [Zgłaszający] "
                                     "FROM Zdarzenia "
                                     "INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca "
                                     "INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu "
                                     "INNER JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                     "WHERE Zdarzenia.IDZdarzenia = '%1'")
                                     .arg(numerZdarzenia);
        QString query10 = QString("SELECT  Obsluga.IDObslugi AS [numer],Obsluga.CzasObslugi AS [czasobsl], Uzytkownik.Login AS [Zgłaszający], Obsluga.Komentarz AS [opis] "
                                  "FROM Zdarzenia "
                                  "INNER JOIN Obsluga ON Zdarzenia.IDZdarzenia=Obsluga.IDZdarzenia "
                                  "INNER JOIN Uzytkownik ON Obsluga.IDUzytkownika=Uzytkownik.IDUzytkownika "
                                  "WHERE Zdarzenia.IDZdarzenia = '%1'")
                                  .arg(numerZdarzenia);
        QDateTime datetime(QDateTime::currentDateTime());

        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
        if(!dir.isEmpty())
        {
            int numerZdarzenia2 = numerZdarzenia - 1 ;
            QString filename = QString("%1/RAPORT_ZD%2_%3.pdf").arg(dir).arg(numerZdarzenia2).arg(datetime.toString("yyyy_MM_dd__HH_mm"));
            QPdfWriter pdfwriter(filename);
            pdfwriter.setPageSize(QPageSize(QPageSize::A4));
            QPainter painter(&pdfwriter);
            painter.drawText(8000,500,datetime.toString("yyyy-MM-dd HH:mm:ss"));
            painter.drawText(3750,1250,QString("Raport zdarzenia o numerze: %1").arg(numerZdarzenia2));
            painter.drawText(500,2400,"Dane zgłoszenia");
                painter.drawText(1200,2650,"Nazwa zdarzenia: ");
                painter.drawText(3000,2650,checkrecord(query9,1));
                painter.drawText(1200,2800,"Typ zdarzenia: ");
                painter.drawText(3000,2800,checkrecord(query9,3));
                painter.drawText(1200,2950,"Czas zdarzenia: ");
                painter.drawText(3000,2950,checkrecord(query9,2));
                painter.drawText(1200,3100,"Współrzędne zdarzenia: ");
                painter.drawText(3000,3100,checkrecord(query9,4));
                painter.drawText(1200,3250,"Zgłaszający zdarzenie: ");
                painter.drawText(3000,3250,checkrecord(query9,5));
            painter.drawText(500,5000,"Dane dyspozycji ");
                if (isEmptySelect(query10))
                {
                    painter.drawText(1200,5150,"Numer dyspozycji: ");
                    painter.drawText(3000,5150,checkrecord(query10,0));
                    painter.drawText(1200,5300,"Czas obsługi: ");
                    painter.drawText(3000,5300,checkrecord(query10,1));
                    painter.drawText(1200,5450,"Dyspozytor: ");
                    painter.drawText(3000,5450,checkrecord(query10,2));
                    painter.drawText(1200,5600,"Komentarz dyspozytora: ");
                    painter.drawText(3000,5600,checkrecord(query10,3));
                }
                else
                {
                    painter.drawText(1200,5150,"Numer dyspozycji: ");
                    painter.drawText(3000,5150,"Brak dyspozycji");
                    painter.drawText(1200,5300,"Czas obsługi: ");
                    painter.drawText(3000,5300,"Brak dyspozycji");
                    painter.drawText(1200,5450,"Dyspozytor: ");
                    painter.drawText(3000,5450,"Brak dyspozycji");
                    painter.drawText(1200,5600,"Komentarz dyspozytora: ");
                    painter.drawText(3000,5600,"Brak dyspozycji");
                }
                QMessageBox msgBox;
                msgBox.setText("Raport utworzony !");
                msgBox.setWindowTitle("Informacja");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
        }
    }
}

void MainWindow::readPendingDatagrams()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress sender; //tu jest adres z ktorego przyszla wiadomosc
        quint16 senderPort;
        m_socket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        QString message = QString::fromStdString(datagram.toStdString());

qDebug()<<"<"<<message<<sender.toString();
        if (datagram.toStdString() == "CON")
        {

            if(m_socket){
                QString string = QString("OK");
                int cnt = m_socket->writeDatagram(string.toLatin1(),sender,senderPort);
                qDebug()<<">"<<string<<sender.toString();

            }
        }
        else
        {
            QStringList pieces = message.split("/");

                if (pieces.value(0)=="LOGIN")
                {
                    sendnumber(clientlogin(pieces.value(1),pieces.value(2)),sender);
                }
                else if (pieces.value(0)=="ZDARZ")
                {
                    sendnumber(addevent(pieces.value(1),pieces.value(2).toInt(),pieces.value(3),pieces.value(4).toInt(),pieces.value(5).toInt()),sender);
                }
                else if (pieces.value(0)=="MIEJSCE")
                {
                    sendnumber(addplace(pieces.value(1)),sender);
                }
                else if (pieces.value(0)=="ADDTYP")
                {
                    sendnumber(addtype(pieces.value(1)),sender);
                }
                else if (pieces.value(0)=="READTYP")
                {
                    sendstring(readtype(),sender);
                }
                else if (pieces.value(0)=="T")
                {
                    sendstring("T",sender);
                }



        }

    }
}

void MainWindow::init()
{
    m_socket=nullptr;
    m_spam << "10.1.16.49"
           << "10.1.16.40";
}

void MainWindow::initNetwork()
{
    if (m_socket) {
        disconnect (m_socket,0,0,0);
        delete m_socket;
        m_socket;

    }
    m_socket=new QUdpSocket(this);
    connect(m_socket,SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
    if (m_socket->bind(QHostAddress::Any,
                       port)){
        //qWarning()<<"Błąd bindowania na porcie:"<<port;

    }
    else {
        connect(m_socket,SIGNAL(readyRead()),
                this,SLOT(readPendingDatagrams()));
    }
}

void MainWindow::sendnumber(int i, QHostAddress adresss)
{
    if(m_socket){
        QString string = QString("%1").arg(i);
        int cnt = m_socket->writeDatagram(string.toLatin1(),adresss,port);

                if(cnt>=0)
                {
                qDebug()<<">"<<i<<adresss.toString();
                }
                else
                {
                qDebug()<<"Błąd wysylania >"<<i<<adresss.toString();
                }
    }
}

void MainWindow::sendstring(QString str, QHostAddress adresss)
{
    if(m_socket){
        QString string = QString("%1").arg(str);
        int cnt = m_socket->writeDatagram(str.toLatin1(),adresss,port);

                if(cnt>=0)
                {
                qDebug()<<">"<<str<<adresss.toString();
                }
                else
                {
                qDebug()<<"Błąd wysylania >"<<str<<adresss.toString();
                }
    }
}

int MainWindow::addplace(QString wspol)
{
    QString query5 = QString("INSERT INTO Miejsce(Wspolrzedne) "
                             "VALUES ('%1')")
                             .arg(wspol);
    QSqlQuery addquery;
    addquery.exec(query5);

    QString query4=QString("SELECT Miejsce.IDMiejsca AS [ID] "
                           "FROM Miejsce "
                           "WHERE Miejsce.Wspolrzedne = '%1'")
                           .arg(wspol);

    if(mDBPolaczenie.otwartaBaza())
    {
        QSqlQueryModel mIModel;
        mIModel.setQuery(query4);
        if(mIModel.rowCount()==1)
        {
           int i =  mIModel.record(0).value(0).toInt();
           return i;
        }
    }
}

int MainWindow::clientlogin(QString login, QString password)
{
        QString query4=QString("SELECT Uzytkownik.IDUzytkownika AS [ID], Uzytkownik.Login AS [Login], Uzytkownik.Haslo AS [Haslo] "
                               "FROM Uzytkownik "
                               "WHERE Login = '%1' AND Haslo = '%2'")
                               .arg(login)
                               .arg(password);

    if(mDBPolaczenie.otwartaBaza())
    {
        QSqlQueryModel mIModel;
        mIModel.setQuery(query4);
        if(mIModel.rowCount()==1)
        {
           int i =  mIModel.record(0).value(0).toInt();
           return i;
        }
    }
}

int MainWindow::addevent(QString NazwaZdarzenia, int IDTypu, QString czas, int IDMiejsca, int IDLogin)
{
    QString query5 = QString("INSERT INTO Zdarzenia(NazwaZdarzenia, IDTypu, CzasZdarzenia, IDMiejsca, IDUzytkownika) "
                             "VALUES ('%1' , '%2' , '%3' , '%4' , '%5')")
                             .arg(NazwaZdarzenia)
                             .arg(IDTypu)
                             .arg(czas)
                             .arg(IDMiejsca)
                             .arg(IDLogin);
    QSqlQuery addquery;
    addquery.exec(query5);

    QString query4=QString("SELECT Zdarzenia.IDZdarzenia AS [ID] "
                           "FROM Zdarzenia "
                           "WHERE Zdarzenia.IDTypu = '%1' AND Zdarzenia.IDMiejsca = '%2' AND Zdarzenia.CzasZdarzenia = '%3'")
                           .arg(IDTypu)
                           .arg(IDMiejsca)
                           .arg(czas);
    if(mDBPolaczenie.otwartaBaza())
    {
        QSqlQueryModel mIModel;
        mIModel.setQuery(query4);
        if(mIModel.rowCount()==1)
        {
           int i =  mIModel.record(0).value(0).toInt();
           return i;
        }
    }
}

int MainWindow::addtype(QString nazwa)
{

    QString query5 = QString("INSERT INTO Typ(TypZdarzenia) "
                             "VALUES ('%1')")
                             .arg(nazwa);
    QSqlQuery addquery;
    addquery.exec(query5);

    QString query4=QString("SELECT Typ.IDTypu AS [ID] "
                           "FROM Typ "
                           "WHERE Typ.TypZdarzenia = '%1'")
                           .arg(nazwa);
    if(mDBPolaczenie.otwartaBaza())
    {
        QSqlQueryModel mIModel;
        mIModel.setQuery(query4);
        if(mIModel.rowCount()==1)
        {
           int i =  mIModel.record(0).value(0).toInt();
           return i;
        }
    }
}

QString MainWindow::readtype()
{
    QString query4=QString("SELECT Typ.TypZdarzenia AS [Typy] "
                           "FROM Typ "
                           "ORDER BY Typ.IDTypu ASC");
    if(mDBPolaczenie.otwartaBaza())
    {
        QSqlQueryModel mIModel;
        mIModel.setQuery(query4);
        QString result = "";
        for (int r=0;r<mIModel.rowCount();r++)
        {
            result+=mIModel.record(r).value(0).toString();
            result+="/";
        }
        return result;
    }
}

