#include "communication.h"
#include <QString>
#include <QUuid>


Communication::Communication(QString sUrl, QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    mUrlServer = sUrl;
}


void Communication::getVilles()
{
    // Requete de la liste de villes au serveur
    QNetworkReply *r = manager->get(QNetworkRequest(QUrl(mUrlServer + "/villes")));
    connect(r, SIGNAL(readyRead()), this, SLOT(villesReceived()));
}

void Communication::villesReceived()
{
    //Reception de la liste des villes
    QNetworkReply *r = (QNetworkReply *)sender();
    if (r->error() == QNetworkReply::NoError)
    {
        QByteArray DataVille = r->readAll();
        // transformation du format QByteArray en QJsonObject
        QJsonDocument JsonDoc1 = QJsonDocument::fromJson(DataVille);
        QJsonObject  JsonObj1 = JsonDoc1.object();
        //Signaler que la liste des ville a ete bien recu
        emit listVille(JsonObj1);
    }

    else {
        qDebug() << "Erreur:" << r->errorString();
    }
}


void Communication::getVols(QString depart, QString arrivee, QString date)
{
    // Requete de la liste de vols au serveur
    QNetworkReply *r1 = manager->get(QNetworkRequest(QUrl(mUrlServer + "/vols/" + depart + "/" + arrivee + "/" + date)));
    connect(r1, SIGNAL(readyRead()), this, SLOT(volsReceived()));
}


void Communication::volsReceived()
{
    //Reception de liste de vols
    QNetworkReply *r1 = (QNetworkReply *)sender();
    if (r1->error() == QNetworkReply::NoError)
    {
        QByteArray DataVols = r1->readAll();
        // transformation du format QByteArray en QJsonObject
        QJsonDocument JsonDoc2 = QJsonDocument::fromJson(DataVols);
        QJsonObject  JsonObj2 = JsonDoc2.object();
        //Signaler que la liste des vols a ete bien recu
        emit listVols(JsonObj2);
    }
    else
    {
        qDebug() << "Erreur:" << r1->errorString();
    }
    r1->deleteLater();

}




/*  requete 2 */



void Communication::getListeSiege(int id)
{
    QNetworkReply *r = manager->get(QNetworkRequest(QUrl(mUrlServer + "/listeSiege/" + QString::number(id))));
    connect(r, SIGNAL(readyRead()), this, SLOT(listeSiegeReceived()));

}


void Communication::listeSiegeReceived()
{
    QNetworkReply *r = (QNetworkReply *)sender();
    if (r->error() == QNetworkReply::NoError)
    {
        QByteArray DataSiege = r->readAll();
        QJsonDocument JsonDoc4 = QJsonDocument::fromJson(DataSiege);
        QJsonObject  JsonObj4 = JsonDoc4.object();
        //        qDebug() << "liste siege receive";
        //        qDebug() << JsonDoc4;
        emit listSiege(JsonObj4);
    }
    else
    {
        qDebug() << "Erreur:" << r->errorString();
    }
    //    r->deleteLater();
}



/*   requete 3 */
void Communication::getPassagers(QString recherche)
{
    QNetworkReply *r = manager->get(QNetworkRequest(QUrl(mUrlServer + "/passagers/" + recherche)));
    connect(r, SIGNAL(readyRead()), this, SLOT(passagersReceived()));
}

void Communication::passagersReceived()
{
    QNetworkReply *r = (QNetworkReply *)sender();
    if (r->error() == QNetworkReply::NoError)
    {
        QByteArray DataPassager = r->readAll();
        QJsonDocument JsonDoc5 = QJsonDocument::fromJson(DataPassager);
        QJsonObject  JsonObj5 = JsonDoc5.object();

        emit RecherchePassager(JsonObj5);
    }
    else
    {
        qDebug() << "Erreur:" << r->errorString();
    }
    r->deleteLater();

}



/* requete 4  */
void Communication::addPassager(QString nom, QString prenom, QString telephone, QString email)
{
    QJsonObject o;
    o["Nom"] = nom;
    o["Prenom"] = prenom;
    o["Telephone"] = telephone;
    o["Email"] = email;
    QJsonDocument d(o);
    QByteArray b = d.toJson();
    QNetworkRequest requete(QUrl(mUrlServer + "/passagers"));
    requete.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *r = manager->post(requete, b);
    connect(r, SIGNAL(readyRead()), this, SLOT(passagerAdded()));
}


void Communication::passagerAdded()
{
    QNetworkReply *r = (QNetworkReply *)sender();

    if (r->error() == QNetworkReply::NoError)
    {
        QByteArray InfoClient = r->readAll();

        // transformation en QJsonObject
        QJsonDocument JsonDoc3 = QJsonDocument::fromJson(InfoClient);
        QJsonObject  JsonObj3 = JsonDoc3.object();
        //        qDebug() << r->readAll() ;
        emit InfoPassager(JsonObj3);
    }

    else {
        qDebug() << "Erreur:" << r->errorString();
    }
    r->deleteLater();
}




/* requete 4-1 */



void Communication::updatePassager(qint32 id, QString nom, QString prenom, QString telephone, QString email)
{
    QJsonObject o;
    o["id"] = id;
    o["Nom"] = nom;
    o["Prenom"] = prenom;
    o["Telephone"] = telephone;
    o["Email"] = email;
    QJsonDocument d(o);
    QByteArray b = d.toJson();
    QNetworkReply *r = manager->put(QNetworkRequest(QUrl(mUrlServer + "/passagers")), b);
    connect(r, SIGNAL(readyRead()), this, SLOT(passagerUpdated()));
}


void Communication::passagerUpdated()
{
    QNetworkReply *r = (QNetworkReply *)sender();
    QByteArray MajClient = r->readAll();
    // transformation en QJsonObject
    QJsonDocument JsonDoc4 = QJsonDocument::fromJson(MajClient);
    QJsonObject  JsonObj4 = JsonDoc4.object();
    emit MajPassager(JsonObj4);

}




/* requete 5 */


void Communication::addReservation(qint32 idPassager, QString idSiege, qint32 idVol, QString uuid)
{
    QJsonObject o;
    o["idPassager"] = idPassager;
    o["idSiege"] = idSiege;
    o["idVol"] = idVol;
    o["UUID"] = uuid;
    //    QUuid q= QUuid();
    //    QUuid q=QUuid::createUuid();
    //    QString str=q.toString();
    //    qDebug() << q;
    QJsonDocument d(o);
    QByteArray b = d.toJson();
    //    qDebug() << "addreservation";
    //    qDebug() << b;

    QNetworkRequest requete(QUrl(mUrlServer + "/reservations"));
    requete.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *r = manager->post(requete, b);
    connect(r, SIGNAL(readyRead()), this, SLOT(reservationAdded()));

}

void Communication::reservationAdded()
{
    QNetworkReply *r = (QNetworkReply *)sender();

    QByteArray addReserv = r->readAll();

    // transformation en QJsonObject
    QJsonDocument JsonDoc6 = QJsonDocument::fromJson(addReserv);
    QJsonObject  JsonObj6 = JsonDoc6.object();
    emit addinfoReservation(JsonObj6);

}



/* requete 7  */
void Communication::updateReservation(QString UUID,  QString idSiege, qint32 idVol,  qint32 id )
{
    QJsonObject o;
    o["UUID"] = UUID;
    //o["Nom"] = nom;
    //o["Prenom"] = prenom;
    o["idSiege"] = idSiege;
    o["idVol"] = idVol;
    //    o["numBillet"] = numBillet;
    o["idPassager"] = id;
    QJsonDocument d(o);
    QByteArray b = d.toJson();
    QNetworkReply *r = manager->put(QNetworkRequest(QUrl(mUrlServer + "/reservations")), b);
    connect(r, SIGNAL(readyRead()), this, SLOT(reservationUpdated()));
}


void Communication::reservationUpdated()
{
    QNetworkReply *r = (QNetworkReply *)sender();
    QByteArray DataUpdate = r->readAll();
    // transformation en QJsonObject
    QJsonDocument JsonDoc7 = QJsonDocument::fromJson(DataUpdate);
    QJsonObject  JsonObj7 = JsonDoc7.object();
    emit InfoUpdateReservation(JsonObj7);

    //  qDebug() << r->readAll();
    //  << "error:" << r->errorString();
    //    delete r;
}



/*  requete 6 */
void Communication::getReservation(QString UUID)
{
    QNetworkReply *r = manager->get(QNetworkRequest(QUrl(mUrlServer +
                                                         "/reservations/" + UUID )));

    connect(r, SIGNAL(readyRead()), this, SLOT(reservationReceived()));

}

void Communication::reservationReceived()
{
    QNetworkReply *r = (QNetworkReply *)sender();
    if (r->error() == QNetworkReply::NoError)
    {
        QNetworkReply *r = (QNetworkReply *)sender();
        QByteArray DataReserv = r->readAll();
        // transformation en QJsonObject
        QJsonDocument JsonDoc5 = QJsonDocument::fromJson(DataReserv);
        QJsonObject  JsonObj5 = JsonDoc5.object();
        //        qDebug() << "reservation received";
        //        qDebug() << JsonObj5;
        emit InfoReservation(JsonObj5);
    }
    else
    {
        qDebug() << "Erreur:" << r->errorString();
    }
    //delete r;
}



/* requte 8  */
void Communication::deleteReservation(QString uuid)
{
    QNetworkReply *r = manager->deleteResource(QNetworkRequest(QUrl(mUrlServer + "/reservations/" + uuid) ));
    connect(r, SIGNAL(readyRead()), this, SLOT(reservationDeleted()));
}

void Communication::reservationDeleted()
{
    QNetworkReply *r = (QNetworkReply *)sender();
    QByteArray DeleteReserv = r->readAll();
    // transformation en QJsonObject
    QJsonDocument JsonDoc6 = QJsonDocument::fromJson(DeleteReserv);
    QJsonObject  JsonObj6 = JsonDoc6.object();
    emit InfoDeleteReservation(JsonObj6);

}

