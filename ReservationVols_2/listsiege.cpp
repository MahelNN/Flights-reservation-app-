#include "listsiege.h"
#include "ui_listsiege.h"

#include "siege.h"
#include <communication.h>
#include <QJsonArray>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QString>


ListSiege::ListSiege(int r, int c, QJsonObject jO, QWidget *parent) :
    QDialog(parent), ui(new Ui::ListSiege), nbLigne{r}, nbColonne{c},
    mData{jO}
{
    ui->setupUi(this);
    mcom3 = new Communication("http://127.0.0.1:8585");
    QJsonArray jS = mData["Sieges"].toArray();

    siegeOccupes.resize(jS.count());
    for(int i=0; i< jS.count(); i++)
    {
        siegeOccupes[i] = jS.at(i).toObject().value("idSiege").toString();
    }
//    qDebug() << QJsonDocument(jO);
    constSetMap();

    connect(ui->pbValiderSige, SIGNAL(clicked(bool)), this, SLOT(validerSiege(bool)));
}

ListSiege::~ListSiege()
{
    delete ui;
}


void ListSiege::setID(int i)
{
    // recuperation de ID du Vols selectionnée sur tablewidget
    mID = i;
    mcom3->getListeSiege(mID);
//    connect(mcom3,SIGNAL(listSiege(QJsonObject)),this,SLOT(setSiegeOccupe(QJsonObject)));
    ui->leIDduVol->setText(QString::number(mID));
    QJsonArray jA = mData["Avion"].toArray();
    QJsonArray jV = mData["Vol"].toArray();
    QJsonArray jS = mData["Sieges"].toArray();

    ui->leAvion->setText(jA.at(0).toObject().value("NomAvion").toString());
    nbLigne = jA.at(0).toObject().value("nbLigne").toInt();
    nbColonne = jA.at(0).toObject().value("nbColonne").toInt();
    int capacite = nbLigne*nbColonne;
    ui->leCapaciteAvion->setText(QString::number(capacite));

}


const QString &ListSiege::selection() const
{
    return mSelection;
}


void ListSiege::setSiegeOccupe(QJsonObject jO)
{
//    QJsonArray jA = jO["Avion"].toArray();
//    QJsonArray jV = jO["Vol"].toArray();
//    QJsonArray jS = jO["Sieges"].toArray();

//    ui->leAvion->setText(jA.at(0).toObject().value("NomAvion").toString());
//    nbLigne = jA.at(0).toObject().value("nbLigne").toInt();
//    nbColonne = jA.at(0).toObject().value("nbColonne").toInt();
//    int capacite = nbLigne*nbColonne;
//    ui->leCapaciteAvion->setText(QString::number(capacite));
}


void ListSiege::constSetMap()
{

    //    qDebug() << nbLigne;
    //    qDebug() << nbColonne;
    int r = nbLigne;
    int c = nbColonne;

    mScene = new QGraphicsScene(0,0,500,r*50,this);
    ui->graphicsView->setScene(mScene);

    QMap<int, QString> col;
    col[0]= "A";
    col[1]= "B";
    col[2]= "C";
    col[3]= "D";
    col[4]= "E";
    col[5]= "F";
    col[6]= "G";
    col[7]= "H";


    for(int i=0; i< r; i++){
        for(int j=0; j< c/2; j++){

            QString nomSiege = QString::number(i+1)+ col[j];
            Siege* s = new Siege(nomSiege);
            for(QString idSiege : siegeOccupes)
            {
                if(idSiege==nomSiege)
                {
                    s->setOccupe(true);
                }
            }
            QPoint p1;
            p1.setX(j*50 + 50);
            p1.setY(i*50 +50);
            s->setPos(p1);

            mScene->addItem(s);
            connect(s,SIGNAL(siegeClicked(QString,bool)),this,SLOT(selectSiege(QString,bool)));
            //s->setBleu();
        }

        for(int j=c/2; j< c; j++){

            QString nomSiege = QString::number(i+1)+ col[j];
            Siege* s = new Siege(nomSiege);
            for(QString idSiege : siegeOccupes)
            {
                if(idSiege==nomSiege)
                {
                    s->setOccupe(true);
//                    qDebug() << "idsiege2";
//                    qDebug() << idSiege;
                }
            }
            QPoint p1;
            p1.setX(j*50 + 100);
            p1.setY(i*50 +50);
            s->setPos(p1);
            mScene->addItem(s);
            connect(s,SIGNAL(siegeClicked(QString,bool)),this,SLOT(selectSiege(QString,bool)));
            //s->setBleu();
            //connect(s,SIGNAL(clicked(bool)),this,SLOT(setBleu()));
        }

    }
    //    ui->tw->setHorizontalHeaderLabels(QString("Nom;Prenom;ID Siège;Code UUID").split(";"));

}

void ListSiege::setInfoSiege(QString nomSiege)
{
    QJsonArray jS = mData["Sieges"].toArray();
    for(int i=0; i<jS.count();i++)
    {
        if(nomSiege == jS.at(i).toObject().value("idSiege").toString())
        {
            QString Nom = jS.at(i).toObject().value("Nom").toString();
            ui->tw->setCellWidget(2,0,new QLabel("Nom:"));
            ui->tw->setCellWidget(2,1,new QLabel(Nom));

            QString Prenom = jS.at(i).toObject().value("Prenom").toString();
            ui->tw->setCellWidget(3,0,new QLabel("Prenom:"));
            ui->tw->setCellWidget(3,1,new QLabel(Prenom));

            QString UUID = jS.at(i).toObject().value("UUID").toString();
            ui->tw->setCellWidget(4,0,new QLabel("UUID:"));
            ui->tw->setCellWidget(4,1,new QLabel(UUID));
        }

    }

}



void ListSiege::selectSiege(QString s, bool b)
{
    ui->tw->setCellWidget(0,0,new QLabel("Siège:"));
        ui->tw->setCellWidget(0,1,new QLabel(s));
    ui->tw->setCellWidget(1,0,new QLabel("reservé:"));
        ui->tw->setCellWidget(1,1,new QLabel("Non"));

    for(int i=2; i<5; i++ )
    {
        ui->tw->setCellWidget(i,0,new QLabel(""));
        ui->tw->setCellWidget(i,1,new QLabel(""));
    }

    if(b)
    {
        setInfoSiege(s);
        ui->tw->setCellWidget(1,1,new QLabel("Oui") );
    }

    if ( b)
        mSelection = "";
    else
        mSelection = s;

}


void ListSiege::validerSiege(bool)
{
    if(mSelection != "")
    {
        emit siegeValide(mSelection);

        this->done(0);

    }


}

/*
void Widget::setBleu()
{
    mState = Siege::Bleu;
    update();
}
*/


void ListSiege::on_pbAnnuler_clicked()
{
    //Quitter la Qdialog lorsque le boutton "Annuler" est cliqué
    this->done(0);
}

