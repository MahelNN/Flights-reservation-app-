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
    mData{jO}{
    ui->setupUi(this);
    mcom3 = new Communication("http://127.0.0.1:8585");
    QJsonArray jS = mData["Sieges"].toArray();

    siegeOccupes.resize(jS.count());
    for(int i=0; i< jS.count(); i++){
        siegeOccupes[i] = jS.at(i).toObject().value("idSiege").toString();
    }
    constSetMap();
    connect(ui->pbValiderSige, SIGNAL(clicked(bool)), this, SLOT(validerSiege(bool)));
}

ListSiege::~ListSiege(){
    delete ui;
}


void ListSiege::setID(int i){
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


const QString &ListSiege::selection() const{
    return mSelection;
}


void ListSiege::constSetMap(){
    int r = nbLigne;
    int c = nbColonne;

    mScene = new QGraphicsScene(0,0,500,r*50,this);
    ui->graphicsView->setScene(mScene);
    //Liste de rangées dans l'ordre alphabetique
    QMap<int, QString> col{{0, "A"},{1, "B"},{2, "C"},{3, "D"},{4, "E"},{5, "F"},{6, "G"},{7, "H"}};

    for(int i=0; i< r; i++){
        for(int j=0; j< c/2; j++){
            QString nomSiege = QString::number(i+1)+ col[j];
            Siege* s = new Siege(nomSiege);
            for(QString idSiege : siegeOccupes){
                if(idSiege==nomSiege){
                    s->setOccupe(true);
                }
            }
            QPoint p1;
            p1.setX(j*50 + 50);
            p1.setY(i*50 +50);
            s->setPos(p1);

            mScene->addItem(s);
            connect(s,SIGNAL(siegeClicked(QString,bool)),this,SLOT(selectSiege(QString,bool)));
            //            s->setBleu();
        }

        for(int j=c/2; j< c; j++){
            QString nomSiege = QString::number(i+1)+ col[j];
            Siege* s = new Siege(nomSiege);
            for(QString idSiege : siegeOccupes){
                if(idSiege==nomSiege){
                    s->setOccupe(true);
                }
            }
            QPoint p1;
            p1.setX(j*50 + 100);
            p1.setY(i*50 +50);
            s->setPos(p1);
            mScene->addItem(s);
            connect(s,SIGNAL(siegeClicked(QString,bool)),this,SLOT(selectSiege(QString,bool)));
            //            s->setBleu();
            //connect(s,SIGNAL(clicked(bool)),this,SLOT(setBleu()));
        }
    }
    //    ui->tw->setHorizontalHeaderLabels(QString("Nom;Prenom;ID Siège;Code UUID").split(";"));
}

void ListSiege::setInfoSiege(QString nomSiege){
    QJsonArray jS = mData["Sieges"].toArray();
    QTableWidgetItem item;
    QList<QString> listeKeys{"Nom", "Prenom", "UUID"};

    for(int i=0; i<jS.count();i++){
        if(nomSiege == jS.at(i).toObject().value("idSiege").toString())
        {
            ui->tw->setRowCount(4);
            auto fillListeVols = [&item, &i, &jS, this](QString key, int row){
                item = QTableWidgetItem(key+":");
                item.setTextAlignment(Qt::AlignCenter);
                ui->tw->setItem( row, 0, new QTableWidgetItem(item));
                item = QTableWidgetItem(jS.at(i).toObject().value(key).toString());
                item.setTextAlignment(Qt::AlignCenter);
                ui->tw->setItem( row, 1, new QTableWidgetItem(item));
            };
            int row{1};//Remplir le champ liste des vols
            for(const auto &key : listeKeys) fillListeVols(key, row++);
        }
    }
}


void ListSiege::selectSiege(QString s, bool b){    
    ui->tw->setHorizontalHeaderLabels(QString("Siège:;"+s).split(";"));
    ui->tw->setRowCount(1);

    QTableWidgetItem item;
    item = QTableWidgetItem("Réservé:");
    item.setTextAlignment(Qt::AlignCenter);
    ui->tw->setItem( 0, 0, new QTableWidgetItem(item));
    item = QTableWidgetItem("Non");
    item.setTextAlignment(Qt::AlignCenter);
    ui->tw->setItem( 0, 1, new QTableWidgetItem(item));
    //    s->setBlue();

    if(b){
        setInfoSiege(s);
        item = QTableWidgetItem("Oui");
        item.setTextAlignment(Qt::AlignCenter);
        ui->tw->setItem( 0, 1, new QTableWidgetItem(item));
    }
    if ( b)
        mSelection = "";
    else
        mSelection = s;
}


void ListSiege::validerSiege(bool){
    if(mSelection != "")    {
        emit siegeValide(mSelection);
        this->done(0);
    }
}


//void Widget::setBleu()
//{
//    mState = Siege::Bleu;
//    update();
//}



void ListSiege::on_pbAnnuler_clicked(){
    //Quitter la Qdialog lorsque le boutton "Annuler" est cliqué
    this->done(0);
}

