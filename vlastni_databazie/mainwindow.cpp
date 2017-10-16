#include "mainwindow.h"
#include "ui_mainwindow.h"
//TODO:
//Done:Info o vyrobci, nepreskocim jinam, pokud nemam vybraneho vyrobce
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../vlastni_databazie/db.sqlite");
    if(!db.open()){
        qDebug()<<db.lastError();
    }
    QSqlQuery dotaz;
    dotaz.prepare("SELECT * FROM Vyrobce");
    dotaz.exec();
    ui->comboBox->addItem("");
    while(dotaz.next())
    {
        ui->comboBox->addItem(dotaz.value("name").toString());
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::jeprazdny() //opraveno, nehrabat se
{

    if(ui->comboBox->currentIndex() == 0){
    QMessageBox::warning(this,"wut", "Nevybral jsi vyrobce, ty kokos!");
    ui->tabWidget->setCurrentIndex(0);
    }
}



void MainWindow::on_pushButton_2_clicked()//videt telefony, done
{
    //vidiet telefony
    ui->tabWidget->setCurrentIndex(1);
    jeprazdny();
    if(ui->comboBox->currentText()=="Samsung"){
        ui->comboBox_2->clear();
        QSqlQuery dotaz;
        dotaz.prepare("Select * from telefoni where ID_Tvurce=1");
        dotaz.exec();
        ui->comboBox_2->addItem("");
        while(dotaz.next()){
            ui->comboBox_2->addItem(dotaz.value("jmeno").toString());
        }
        ui->label_3->setText("Vybral jsi si vyrobce: " +ui->comboBox->currentText() );
    }else if(ui->comboBox->currentText()=="Zouny"){
        ui->comboBox_2->clear();
        QSqlQuery dotaz;
        dotaz.prepare("Select * from telefoni where ID_Tvurce=2");
        dotaz.exec();
        ui->comboBox_2->addItem("");
        while(dotaz.next()){
            ui->comboBox_2->addItem(dotaz.value("jmeno").toString());
        }
        ui->label_3->setText("Vybral jsi si vyrobce: " +ui->comboBox->currentText() );
    }


}

void MainWindow::on_pushButton_4_clicked()//v druhem tabu kde prodavaj primo jeden telefon.
{
    int id=0;
    ui->tabWidget->setCurrentIndex(2);
    ui->textBrowser_3->clear();
    QSqlQuery dotaz;//nejdriv musim ziskat id telefonu v comboboxu
    dotaz.prepare("select * from telefoni where jmeno= :jmeno");
    qDebug()<<dotaz.lastError();
    dotaz.bindValue(":jmeno", ui->comboBox_2->currentText());
    qDebug()<<dotaz.lastError();
    dotaz.exec();
    qDebug()<<dotaz.lastError();
    while(dotaz.next()){
        id=dotaz.value("id").toInt();
    }
    qDebug()<<id;

    QSqlQuery dotaz2;
    dotaz2.prepare("Select nazev_prodejny from Prodejny where id_telefonu=:idcko");
    dotaz2.bindValue(":idcko", id );
    dotaz2.exec();
    while(dotaz2.next()){
        ui->textBrowser_3->append(dotaz2.value("nazev_prodejny").toString());
    }
}

void MainWindow::on_pushButton_3_clicked()//kde prodavaji jeho produkty - done
{

    ui->tabWidget->setCurrentIndex(2);
    jeprazdny();
    if(ui->comboBox->currentText()=="Samsung"){
        ui->textBrowser_3->clear();
        QSqlQuery dotaz;
        dotaz.prepare("select * from Prodejny, Telefoni, Vyrobce  where Prodejny.id_telefonu=Telefoni.ID AND  Telefoni.ID_Tvurce=Vyrobce.ID and  Vyrobce.Name like :jmeno");
        dotaz.bindValue(":jmeno", ui->comboBox->currentText());
        qDebug()<<ui->comboBox->currentText();
        dotaz.exec();
        while(dotaz.next()){
            ui->textBrowser_3->append(dotaz.value("nazev_prodejny").toString());
            qDebug()<<dotaz.value("nazev_prodejny").toString();
        }
    }else if(ui->comboBox->currentText()=="Zouny"){
        ui->textBrowser_3->clear();
        QSqlQuery dotaz;
        dotaz.prepare("select * from Prodejny, Telefoni, Vyrobce  where Prodejny.id_telefonu=Telefoni.ID AND  Telefoni.ID_Tvurce=Vyrobce.ID and  Vyrobce.Name like :jmeno");
        dotaz.bindValue(":jmeno", ui->comboBox->currentText());
        dotaz.exec();
        while(dotaz.next()){
            ui->textBrowser_3->append(dotaz.value("nazev_prodejny").toString());
            qDebug()<<dotaz.value("nazev_prodejny").toString();
        }
    }
}


void MainWindow::on_pushButton_clicked()//done-about o zvolenem vyrobci - sem se uz vubec nehrabat
{
    if(ui->comboBox->currentIndex()==0){
        QMessageBox::warning(this, "Ehm...", "Ty trdlín, nevybral jsi si vyrobce");
    }
    else{
        QSqlQuery dotaz;
        dotaz.prepare("Select * from Vyrobce where name=:jmeno");
        dotaz.bindValue(":jmeno", ui->comboBox->currentText());
        qDebug()<< ui->comboBox->currentText();
        dotaz.exec();
        while(dotaz.next())
        {

           ui->textBrowser->setText(dotaz.value("about").toString());
        };
    }
}




void MainWindow::on_tabWidget_currentChanged(int index)//uzspravneupravene
{

    if(ui->comboBox->currentIndex()==0 && ui->tabWidget->currentIndex()>0){
        QMessageBox::warning(this, "Wut", "Prečo si sem išiel, keď nič nieje vybrané?. Btw, taky si musíš vybrat akcičku. ;) ");
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_pushButton_5_clicked()//popis telefonu-done
{

    QSqlQuery dotaz;
    dotaz.prepare("Select O_Telefonu from Telefoni where jmeno=:jmeno");
    dotaz.bindValue(":jmeno", ui->comboBox_2->currentText());
    dotaz.exec();
    while(dotaz.next()){
        ui->textBrowser_2->setText(dotaz.value("O_Telefonu").toString());
    }
}
