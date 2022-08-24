#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "communication.h"
#include "listsiege.h"
#include <QObject>
#include <QCoreApplication>
#include <QComboBox>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QTableWidgetItem>
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mcom = new Communication("http://127.0.0.1:8585");
    connect(mcom, SIGNAL(listVille(QJsonObject)), this, SLOT(afficherVilles(QJsonObject)));
    connect(mcom, SIGNAL(InfoReservation(QJsonObject)), this, SLOT(AffichageReservation(QJsonObject)));
    connect(mcom, SIGNAL(listVols(QJsonObject)), this, SLOT(afficherVols(QJsonObject)));
    connect(mcom, SIGNAL(listSiege(QJsonObject)), this, SLOT(infoAvion(QJsonObject)));

    connect(mcom, SIGNAL(RecherchePassager(QJsonObject)), this, SLOT(AfficheRecherchePassager(QJsonObject)));
    connect(mcom, SIGNAL(MajPassager(QJsonObject)), this, SLOT(MiseaJourPassager(QJsonObject)));
    connect(mcom, SIGNAL(InfoDeleteReservation(QJsonObject)), this, SLOT(DeleteReservation(QJsonObject)));
    connect(mcom, SIGNAL(InfoUpdateReservation(QJsonObject)), this, SLOT(AffichageUpdateReserv(QJsonObject)));

    connect(mcom,SIGNAL(InfoPassager(QJsonObject)),this,SLOT(reloadPassager()));
    connect(mcom,SIGNAL(addinfoReservation(QJsonObject)),this,SLOT(reloadReservation()));
    connect(ui->pbRechercheListVols, SIGNAL(clicked(bool)), this, SLOT(rechercheListVolsClicked(bool)));
    connect(ui->pbValiderUnVol, SIGNAL(clicked(bool)), this, SLOT(validerUnVolClicked(bool)));

    //Les villes sont chargées des le départ dans un menu déroulant
    mcom->getVilles();

    //Charger les entetes des differents champs
    setVolsHeader();
    setPassagerHeader();
    setReservationHeader();

}


MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::Controlephone(QString tel) {

    if ((tel[0] != '+') || tel.size() > 15) { return false;}
    else {
        for (int i = 1; i < tel.size(); i++) {
            if (tel[i].isDigit() == false) {return false;}
        }
    }
    return true;
}

//Entete Champ liste des vols
void MainWindow::setVolsHeader()
{
    ui->twListeDesVols->setHorizontalHeaderLabels(QString(" Id vol ; Id avion ; Nom avion ; Départ ; Arrivée ; Date départ ; Date arrivée ; Heure départ ; Heure arrivée ").split(";"));
    //    ui->twListeDesVols->resizeColumnsToContents();//Ajuster la tailles des cellules de table en fonction du texte contenu

}

//Entete Champ informations passager
void MainWindow::setPassagerHeader()
{
    ui->twInformationPassager->setHorizontalHeaderLabels(QString(" Id client ; Nom;Prenom ; N° téléphone ;Email ").split(";"));
    //    ui->twInformationPassager->resizeColumnsToContents();//Ajuster la tailles des cellules de table en fonction du texte contenu

}

//Entete champ reservation
void MainWindow::setReservationHeader()
{
    ui->twInformation_Reservation->setHorizontalHeaderLabels(QString(" Nom ; Prenom ; Id siège ; ID Vol ; Ville départ ; Ville arrivée ; Date départ ; Heure depart ; Date arrivée ; Heure arrivée ; UUID ").split(";"));
    //    ui->twInformation_Reservation->resizeColumnsToContents();//Ajuster la tailles des cellules de table en fonction du texte contenu
}


void MainWindow::afficherVilles(QJsonObject o)
{
    QJsonArray jsArr = o["Villes"].toArray();
    QStringList listeVilles;
    //Chaque nom de ville est ajoutée dans une Qliste
    for(auto js : jsArr) listeVilles.append(js.toObject()["nomVille"].toString());
    //Puis chargée dans un menu déroulant
    ui->leVilleDepart->addItems(listeVilles);
    ui->leVilleArrivee->addItems(listeVilles);
}

//Recherche de vols correspond au départ, arrivée et date de départ
void MainWindow::rechercheListVolsClicked(bool)
{
    QString villeDep_Select = ui->leVilleDepart->currentText();
    QString villeArrivee_Select = ui->leVilleArrivee->currentText();
    QString dateVoyage_Select = ui->dateEdit->date().toString("dd-MM-yyyy");

    mcom->getVols(villeDep_Select, villeArrivee_Select, dateVoyage_Select);
}

void MainWindow::afficherVols(QJsonObject o)
{
    ui->twListeDesVols->clearContents();//Initialisation du champ
    QJsonArray jsArr = o["Vols"].toArray();
    ui->twListeDesVols->setColumnCount(9);//Creation d'une table de taille jsArr.count()*10
    ui->twListeDesVols->setRowCount(jsArr.count());
    setVolsHeader(); //afficher l'entete

//    for(auto js : jsArr) qDebug() << js ;
//    qDebug() << " ";

    for (int i = 0; i < jsArr.count(); i++) {//Remplir la table avec les valeurs de o
        QTableWidgetItem item;
//        auto fillListeVols = [&item, &i, &jsArr, this](QString key, int row){
//            item = QTableWidgetItem(jsArr.at(i).toObject().value(key).toString());
//            item.setTextAlignment(Qt::AlignCenter);
//            ui->twListeDesVols->setItem(i, row, new QTableWidgetItem(item));
//        };
//        int row{0};
//        //        for(auto js : jsArr) fillListeVols(js.to.key(), row++);
//        while(row < 9){
//            qDebug() << jsArr.at(i).toObject().keys().at(row++);
//        }


        item = QTableWidgetItem(jsArr.at(i).toObject().value("id").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 0, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("idAvion").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 1, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("Nom").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 2, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("Depart").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 3, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("Arrivee").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 4, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("dateDepart").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 5, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("dateArrivee").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 6, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("Nom").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 7, new QTableWidgetItem(item));

        item = QTableWidgetItem(jsArr.at(i).toObject().value("heureArrivee").toString());
        item.setTextAlignment(Qt::AlignCenter);
        ui->twListeDesVols->setItem(i, 8, new QTableWidgetItem(item));



    }
    //    ui->twListeDesVols->setAli;
    //    ui->twListeDesVols->resizeColumnsToContents();
    //    ui->twListeDesVols->resizeColumnsToContents();//Ajuster la tailles des cellules de table en fonction du texte contenu
}

//Selection d'un vol
void MainWindow::validerUnVolClicked(bool)
{
    // Ajout colonne pour ID du VOl (Colonne 0)
    // Recupérer la ligne selectionnée dans le tableWidget
    int rowSelect = ui->twListeDesVols->currentRow();
    QTableWidgetItem* idVol = ui->twListeDesVols->item(rowSelect, 0);

    if (rowSelect ==-1)//Rien n'est retourné si aucune ligne selectionnée
        return;
    //Remplir le id vol dans le champ de Reservation
    ui->leReserv_IDVOL->setText(QString::number(idVol->text().toInt()));
    mcom->getListeSiege(idVol->text().toInt());
}

//Selection d'un siege
void MainWindow::infoAvion(QJsonObject o)
{
    QJsonArray jA = o["Avion"].toArray();
    QJsonArray jV = o["Vol"].toArray();
    //Extration du nombre de lignes et de colonnes de l'avion
    int r = jA.at(0).toObject().value("nbLigne").toInt();
    int c = jA.at(0).toObject().value("nbColonne").toInt();

    //Instantiation d'un objet ListeSiege
    ListSiege ls(r, c, o);
    //Les sieges considérés appartiennent au vol d'Id o["Vol"]["idVol"]
    ls.setID(jV.at(0).toObject().value("idVol").toInt());
    ls.setModal(true);

    //Execution la fenetre d'affichage de sieges
    ls.exec();
    //A la fermeture de la fenetre, un choix de siege a été fait.
    //On y a acces avec la fonction selection()
    ui->leIDSiegeReserv->setText(ls.selection());
}

//Rechercher un passager dans la base de donées
void MainWindow::on_RechercherPassager_clicked()
{
    QString NumTelephone = ui->leTelephonePassager->text();
    QString NomContact = ui->leNomPassager->text();
    QString PrenomContact = ui->lePrenomPassager->text();
    QString IDContact = ui->leIDPassager->text();

    if (NumTelephone.isEmpty() && NomContact.isEmpty() && PrenomContact.isEmpty()) {
        mcom->getPassagers(IDContact);
    }
    if (IDContact.isEmpty() && NomContact.isEmpty() && PrenomContact.isEmpty()) {
        mcom->getPassagers(NumTelephone);
    }
    if (IDContact.isEmpty() && NumTelephone.isEmpty() && PrenomContact.isEmpty()) {
        mcom->getPassagers(NomContact);
    }
    if (IDContact.isEmpty() && NumTelephone.isEmpty() && NomContact.isEmpty()) {
        mcom->getPassagers(PrenomContact);
    }
}

//Remplir le champ informations passager
void MainWindow::AfficheRecherchePassager(QJsonObject o)
{
    ui->twInformationPassager->clear();

    QJsonArray jsArr = o["data"].toArray();
    ui->twInformationPassager->setColumnCount(5);
    ui->twInformationPassager->setRowCount(1);

    setPassagerHeader();//Remplir l'entete
    ui->twInformationPassager->setItem(0, 0, new QTableWidgetItem(QString::number(jsArr.at(0).toObject().value("id").toDouble())));
    ui->twInformationPassager->setItem(0, 1, new QTableWidgetItem(jsArr.at(0).toObject().value("Nom").toString()));
    ui->twInformationPassager->setItem(0, 2, new QTableWidgetItem(jsArr.at(0).toObject().value("Prenom").toString()));
    ui->twInformationPassager->setItem(0, 3, new QTableWidgetItem(jsArr.at(0).toObject().value("Telephone").toString()));
    ui->twInformationPassager->setItem(0, 4, new QTableWidgetItem(jsArr.at(0).toObject().value("Email").toString()));
    //    ui->twInformationPassager->resizeColumnsToContents();
    //Remplir l'id du passager dans le champ de reservation a gauche du champ information passager
    ui->leIDPassager->setText(QString::number(jsArr.at(0).toObject().value("id").toDouble()));
    //Remplir l'id du passager dans le champ de reservation
    ui->leReserv_IDPassager->setText(QString::number(jsArr.at(0).toObject().value("id").toDouble()));

}




void MainWindow::on_AjouterPassager_clicked()
{
    QString NomClient, PrenomClient, TelClient, EmailClient;

    // Recupération d'un identifiant unique client
    NomClient = ui->leNomPassager->text();
    PrenomClient = ui->lePrenomPassager->text();
    TelClient = ui->leTelephonePassager->text();
    EmailClient = ui->leEmailPassager->text();

    mcom->addPassager(NomClient, PrenomClient, TelClient, EmailClient);


    ui->twInformationPassager->setColumnCount(5);

    ui->twInformationPassager->setRowCount(1);
    /*
    ui->InformationPassager->setItem(0, 1, new QTableWidgetItem(NomClient));
    ui->InformationPassager->setItem(0, 2, new QTableWidgetItem(PrenomClient));
    ui->InformationPassager->setItem(0, 3, new QTableWidgetItem(EmailClient));
    ui->InformationPassager->setItem(0, 4, new QTableWidgetItem(TelClient));
*/

}


void MainWindow::AddClient(QJsonObject o2)
{
}

void MainWindow::MiseaJourPassager(QJsonObject e)
{
    qDebug() << e;
    QJsonArray jsonArray4 = e["data"].toArray();
}

void MainWindow::on_ModifierPassager_clicked()
{
    QString IdClient, NomClient, PrenomClient, TelClient, EmailClient;

    IdClient = ui->leIDPassager->text();
    NomClient = ui->leNomPassager->text();
    PrenomClient = ui->lePrenomPassager->text();
    TelClient = ui->leTelephonePassager->text();
    EmailClient = ui->leEmailPassager->text();


    mcom->updatePassager(IdClient.toInt(), NomClient, PrenomClient, TelClient, EmailClient);


    mcom->getPassagers(TelClient);


    ui->twInformationPassager->setColumnCount(5);
    ui->twInformationPassager->setRowCount(1);
    ui->twInformationPassager->setItem(0, 0, new QTableWidgetItem(IdClient));
    ui->leReserv_IDPassager->setText(IdClient);

    ui->twInformationPassager->setItem(0, 1, new QTableWidgetItem(NomClient));
    ui->twInformationPassager->setItem(0, 2, new QTableWidgetItem(PrenomClient));
    ui->twInformationPassager->setItem(0, 3, new QTableWidgetItem(EmailClient));
    ui->twInformationPassager->setItem(0, 4, new QTableWidgetItem(TelClient));

}



void MainWindow::reloadPassager()
{
    mcom->getPassagers(ui->leTelephonePassager->text());
}


void MainWindow::on_pbRechercher_Reservation_clicked()
{
    QString UUID = ui->leReserv_UUID->text();
    //    connect(mcom, SIGNAL(InfoReservation(QJsonObject)), this, SLOT(AffichageReservation(QJsonObject)));
    mcom->getReservation(UUID);
}

void MainWindow::AffichageReservation(QJsonObject resev)
{
    ui->twInformation_Reservation->clear();

    QJsonArray jsonArray4 = resev["data"].toArray();
    //    qDebug() << "afficher reserv";
    //    qDebug() << QJsonDocument(resev);
    ui->twInformation_Reservation->setColumnCount(11);
    ui->twInformation_Reservation->setRowCount(1);
    setReservationHeader();
    ui->twInformation_Reservation->setItem(0, 0, new QTableWidgetItem(jsonArray4.at(0).toObject().value("Nom").toString()));
    ui->twInformation_Reservation->setItem(0, 1, new QTableWidgetItem(jsonArray4.at(0).toObject().value("Prenom").toString()));
    ui->twInformation_Reservation->setItem(0, 2, new QTableWidgetItem(jsonArray4.at(0).toObject().value("idSiege").toString()));
    ui->twInformation_Reservation->setItem(0, 3, new QTableWidgetItem(QString::number(jsonArray4.at(0).toObject().value("idVol").toInt())));
    ui->twInformation_Reservation->setItem(0, 4, new QTableWidgetItem(jsonArray4.at(0).toObject().value("Depart").toString()));
    ui->twInformation_Reservation->setItem(0, 5, new QTableWidgetItem(jsonArray4.at(0).toObject().value("Arrivee").toString()));
    ui->twInformation_Reservation->setItem(0, 6, new QTableWidgetItem(jsonArray4.at(0).toObject().value("dateDepart").toString()));
    ui->twInformation_Reservation->setItem(0, 7, new QTableWidgetItem(jsonArray4.at(0).toObject().value("heureDepart").toString()));
    ui->twInformation_Reservation->setItem(0, 8, new QTableWidgetItem(jsonArray4.at(0).toObject().value("dateArrivee").toString()));
    ui->twInformation_Reservation->setItem(0, 9, new QTableWidgetItem(jsonArray4.at(0).toObject().value("heureArrivee").toString()));
    ui->twInformation_Reservation->setItem(0, 10, new QTableWidgetItem(jsonArray4.at(0).toObject().value("UUID").toString()));

    ui->twInformation_Reservation->resizeColumnsToContents();

}


//void MainWindow::on_Rechercher_Reservation_clicked()
//{


//}




void MainWindow::AffichageAjoutReservation(QJsonObject)
{}



void MainWindow::on_pbCreer_Reservation_clicked()
{
    QString UUID, IDSiege;
    qint32 IDVol, IDPassager;

    //Creation d'un numero UUID
    UUID = QUuid::createUuid().toString(QUuid::WithoutBraces);

    ui->leReserv_UUID->setText(UUID);
    IDSiege = ui->leIDSiegeReserv->text();
    IDVol = ui->leReserv_IDVOL->text().toInt();
    IDPassager = ui->leReserv_IDPassager->text().toInt();

    //    qDebug() << "on_pbcrer_res";
    //    qDebug() << IDPassager<< " " << IDSiege <<" " <<IDVol << " " << UUID;
    mcom->addReservation(IDPassager, IDSiege, IDVol, UUID);

    //mcom->getReservation(UUID); A revoir affichage directe après création reservation

}


void MainWindow::DeleteReservation(QJsonObject e)
{}



void MainWindow::on_pbAnnuler_Reservation_clicked()
{
    QString UUID;
    UUID = ui->leReserv_UUID->text();
    mcom->deleteReservation(UUID);

}


void MainWindow::AffichageUpdateReserv(QJsonObject o)
{}

void MainWindow::on_pbModifier_Reservation_clicked()
{
    QString UUID, IDSiege;
    qint32 IDVol, IDPassager;
    UUID = ui->leReserv_UUID->text();
    IDSiege = ui->leIDSiegeReserv->text();
    IDVol = ui->leReserv_IDVOL->text().toInt();
    IDPassager = ui->leReserv_IDPassager->text().toInt();

    mcom->updateReservation(UUID, IDSiege, IDVol, IDPassager);
    mcom->getReservation(UUID);
}


void MainWindow::reloadReservation()
{
    mcom->getReservation(ui->leReserv_UUID->text());

}

