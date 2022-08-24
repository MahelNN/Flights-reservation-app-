#include <QCoreApplication>
#include <QHttpServer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlRecord>

/* requete 0*/
QJsonObject getVilles()
{
    QJsonArray tab;

    QSqlDatabase db = QSqlDatabase::database("REP");
    QSqlQuery query("SELECT * from Villes" , db);

    while (query.next())
    {
        QJsonObject o;
        o["id"] = query.value(0).toInt();
        o["nomAbrige"] = query.value(2).toString();
        o["nomVille"] = query.value(1).toString();
        tab.append(o);
    }
    return QJsonObject{
        {
            "Villes", tab
        }
    };
}


/* requete 1*/
QJsonObject getVols(QString depart, QString arrivee, QString dateDepart)
{
    QJsonArray tab;
    QSqlDatabase db = QSqlDatabase::database("REP");
    //QString requete1 = QString("INSERT INTO Produits (nom,prix) VALUES (\"%1\",%2)").arg(nom).arg(prix);

    QString requete = QString("SELECT v.id, v.idAvion, Avions.NomAvion, d.nomVille AS Depart,a.nomVille AS Arrivee, v.dateDepart, v.dateArrivee,v.heureDepart, v.heureArrivee FROM Vols AS v INNER JOIN Villes AS d ON v.idDepart = d.id  INNER JOIN Villes AS a ON v.idArrivee = a.id  INNER JOIN Avions ON v.idAvion = Avions.id WHERE d.nomVille = \"%1\" AND a.nomVille = \"%2\" And v.dateDepart = \"%3\" ").arg(depart).arg(arrivee).arg(dateDepart);

    QSqlQuery query(requete, db);

    while (query.next())
    {
        QJsonObject o;
        o["id"] = query.value(0).toString();
        o["idAvion"] = query.value(1).toString();
        o["Nom"] = query.value(2).toString();
        o["Depart"] = query.value(3).toString();
        o["Arrivee"] = query.value(4).toString();
        o["dateDepart"] = query.value(5).toString();
        o["dateArrivee"] = query.value(6).toString();
        o["heureDepart"] = query.value(7).toString();
        o["heureArrivee"] = query.value(8).toString();

        tab.append(o);
    }
    return QJsonObject{
        {
            "Vols", tab
        }
    };
}


/* requete2 */

//QJsonObject getListeSiege(qint32 idVol)
//{

//    QJsonArray tab0;
//    QJsonArray tab1;
//    QJsonArray tab2;

//    QSqlDatabase db = QSqlDatabase::database("REP");

//    QSqlQuery query("SELECT * FROM liste_seige " , db);
//    qDebug() << "lisste::::::::!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:::";
//    qDebug() << query.value(0).toInt();
//    qDebug() << "lisste::::::::!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:::";
//    while (query.next())
//    {
//        if (idVol == query.value(0).toInt() ){
//            QJsonObject o;

//            o["Nom"] = query.value(10).toString();
//            o["nbColonne"] = query.value(12).toInt();
//            o["nbLigne"] = query.value(11).toInt();
//            tab0.append(o);

//            QJsonObject o1;
//            o1["Arrivee"] = query.value(14).toString();
//            o1["Depart"] = query.value(13).toString();
//            o1["Nom"] = query.value(10).toString();
//            o1["dateArrive"] = query.value(6).toString();
//            o1["dateDepart"] = query.value(7).toString();
//            o1["heureArrive"] = query.value(8).toString();
//            o1["heureDepart"] = query.value(9).toString();
//            tab1.append(o1);

//            QJsonObject o2;
//            o2["Nom"] = query.value(4).toString();
//            o2["Prenom"] = query.value(5).toString();
//            o2["UUID"] = query.value(2).toString();
//            o2["idSiege"] = query.value(1).toString();
//            tab2.append(o2);
//        }
//    }
//    return QJsonObject{
//        {
//            "Avion", tab0
//        },
//        {
//            "Liste", tab2
//        },
//        {
//            "Vols", tab1
//        }
//    };
//}

QJsonObject getListeSiege(qint32 id)
{
    QJsonArray tab0;
    QJsonArray tab1;
    QJsonArray tab2;

    QSqlDatabase db = QSqlDatabase::database("REP");
    QString requete = QString("SELECT Reservation.idVol, Avions.NomAvion, Avions.nbLigne, Avions.nbColonne,"
                              "depart.nomVille AS Depart, arrivee.nomVille AS Arrivee, Vols.dateDepart, Vols.dateArrivee, Vols.heureDepart,   Vols.heureArrivee, "
                              "Passagers.Nom, Passagers.Prenom, Reservation.idSiege, Reservation.UUID "
                              "FROM Reservation "
                              "INNER JOIN Passagers ON Passagers.id = Reservation.idPassager "
                              "INNER JOIN Vols ON Vols.id = Reservation.idVol "
                              "INNER JOIN Avions ON Avions.id = Vols.idAvion "
                              "INNER JOIN Villes as arrivee ON arrivee.id = Vols.idArrivee "
                              "INNER JOIN Villes as depart ON depart.id = Vols.idDepart "
                              "WHERE Vols.id = %1").arg(id);
    QSqlQuery query(requete, db);

    int it = 0;
    while (query.next())
    {
        it++;
        //        if (id == query.value(0).toInt() )
        //        {
        if (it == 1)
        {

            QJsonObject o;
            o["NomAvion"] = query.value(1).toString();
            o["nbLigne"] = query.value(2).toInt();
            o["nbColonne"] = query.value(3).toInt();
            tab0.append(o);
            //            qDebug() << "get siege main cpp";
            //            qDebug() << tab0;
            QJsonObject o1;

            o1["idVol"] = query.value(0).toInt();
            o1["Depart"] = query.value(4).toString();
            o1["Arrivee"] = query.value(5).toString();
            o1["NomAvion"] = query.value(1).toString();
            o1["dateDepart"] = query.value(6).toString();
            o1["dateArrivee"] = query.value(7).toString();
            o1["heureDepart"] = query.value(8).toString();
            o1["heureArrivee"] = query.value(9).toString();
            tab1.append(o1);
            //            qDebug() << tab1;


        }
        QJsonObject o2;
        o2["Nom"] = query.value(10).toString();
        o2["Prenom"] = query.value(11).toString();
        o2["idSiege"] = query.value(12).toString();
        o2["UUID"] = query.value(13).toString();
        tab2.append(o2);
        //        qDebug() << tab2;

        //        }
    }

    return QJsonObject{{ "Avion", tab0 }, { "Vol", tab1 }, { "Sieges", tab2 }};
}


//__________________________________________
//Requete 3

QJsonObject getPassager(QString recherche)
{
    QJsonArray tab;

    QSqlDatabase db = QSqlDatabase::database("REP");
    QSqlQuery query("SELECT * from Passagers" , db);
    qint32 i = 0;

    while (query.next())
    {
        //        qDebug() <<  query.record().count();

        if (recherche == query.value(3).toString() || recherche == query.value(4).toString() || recherche == query.value(2).toString() || recherche == query.value(1).toString() || recherche == query.value(0).toString() ) {
            QJsonObject o;
            o["id"] = query.value(0).toInt();
            o["Nom"] = query.value(1).toString();
            o["Prenom"] = query.value(2).toString();
            o["Email"] = query.value(4).toString();
            o["Telephone"] = query.value(3).toString();
            tab.append(o);
        }
        else i++;
    }
    return QJsonObject{
        {
            "data", tab
        }
    };
}


//__________________________________________
//Requete 4
QString addPassager(QJsonObject o)
{
    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("INSERT INTO Passagers (Nom,Prenom,Telephone,Email) VALUES (\"%1\",\"%2\",\"%3\",\"%4\" )").arg(o["Nom"].toString()).arg(o["Prenom"].toString()).arg(o["Telephone"].toString()).arg(o["Email"].toString());
    QSqlQuery query(db);
    query.exec(requete);
    //    qDebug() << requete;
    return query.lastError().text();
}




//__________________________________________
//Requete 4-1
void updatePassager(QJsonObject o)
{
    QSqlDatabase db = QSqlDatabase::database("REP");
    QSqlQuery query0("SELECT * from Passagers" , db);
    QSqlQuery query;
    int i = 0;
    while (query0.next())
    {

        if (o["id"].toInt() == query0.value(0).toInt() ) {

            if (o["Nom"].toString() != query0.value(1).toString()) {
                QString requete = QString("UPDATE Passagers SET Nom= \"%1\" WHERE id = %2  ").arg(o["Nom"].toString()).arg(o["id"].toInt());
                QSqlQuery query(db);
                query.exec(requete);
            }

            if (o["Prenom"].toString() != query0.value(2).toString()) {
                QString requete = QString("UPDATE Passagers SET Prenom= \"%1\" WHERE id = %2  ").arg(o["Prenom"].toString()).arg(o["id"].toInt());
                QSqlQuery query(db);
                query.exec(requete);
            }

            if (o["Telephone"].toString() != query0.value(3).toString()) {
                QString requete = QString("UPDATE Passagers SET Telephone= \"%1\" WHERE id = %2  ").arg(o["Telephone"].toString()).arg(o["id"].toInt());
                QSqlQuery query(db);
                query.exec(requete);
            }

            if (o["Email"].toString() != query0.value(4).toString()) {
                QString requete = QString("UPDATE Passagers SET Email= \"%1\" WHERE id = %2  ").arg(o["Email"].toString()).arg(o["id"].toInt());
                QSqlQuery query(db);
                query.exec(requete);
            }

        }
        else i++;
    }
}



//__________________________________________
//Requete 5
QString addReservation(QJsonObject o)
{
    //    qDebug() << "addreservation maincpp";
    //    qDebug() << o;

    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("INSERT INTO Reservation (idPassager,idVol, idSiege, UUID) "
                              "VALUES (%1 ,%2,\"%3\",\"%4\") ")
            .arg(o["idPassager"].toInt())
            .arg(o["idVol"].toInt())
            .arg(o["idSiege"].toString())
            .arg(o["UUID"].toString());
    QSqlQuery query(db);
    query.exec(requete);
    //    qDebug() << requete;
    return query.lastError().text();

}




//__________________________________________
//Requete 6
QJsonObject getReservation(QString UUID)
{
    QJsonArray tab;

    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("SELECT Reservation.idVol, Reservation.idSiege, Reservation.UUID, Passagers.Nom, Passagers.Prenom, Vols.dateArrivee, Vols.dateDepart, Vols.heureArrivee, Vols.heureDepart, Avions.NomAvion, Avions.nbLigne, Avions.nbColonne, depart.nomVille AS Depart, arrivee.nomVille AS Arrivee FROM Reservation INNER JOIN Passagers ON Passagers.id = Reservation.idPassager  INNER JOIN Vols ON Vols.id = Reservation.idVol  INNER JOIN Avions ON Avions.id = Vols.idAvion INNER JOIN Villes as arrivee ON arrivee.id = Vols.idArrivee INNER JOIN Villes as depart ON depart.id = Vols.idDepart WHERE UUID = \"%1\" ").arg(UUID);
    //    qDebug() << requete;
    QSqlQuery query(requete, db);
    //qint32 i=0;
    while (query.next())
    {
        QJsonObject o;

        o["idSiege"] = query.value(1).toString();
        o["idVol"] = query.value(0).toInt();
        o["Arrivee"] = query.value(13).toString();
        o["Depart"] = query.value(12).toString();
        o["Nom"] = query.value(9).toString();
        o["dateArrivee"] = query.value(5).toString();
        o["dateDepart"] = query.value(6).toString();
        o["heureArrivee"] = query.value(7).toString();
        o["heureDepart"] = query.value(8).toString();
        o["UUID"] = query.value(2).toString();
        o["Nom"] = query.value(3).toString();
        o["Prenom"] = query.value(4).toString();
        tab.append(o);
    }
    return QJsonObject{
        {
            "data", tab
        }
    };
}


//__________________________________________
//Requete 7

void updateReservation(QJsonObject o)
{
    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("SELECT * FROM Reservation WHERE UUID=\""+o["UUID"].toString()+"\"");
    QSqlQuery query3(requete, db);
    //    qDebug() << requete;
    //QSqlQuery query;
    while (query3.next())
    {
        QString rqte = QString("UPDATE Reservation SET idVol= %1, idSiege= \"%2\", idPassager= %3 "
                               " WHERE UUID=\"%4\" ")
                .arg(o["idVol"].toInt())
                .arg(o["idSiege"].toString())
                .arg(o["idPassager"].toInt())
                .arg(o["UUID"].toString());
        QSqlQuery query(db);
        query.exec(rqte);
        //        qDebug() << "updateReservation";
        //        qDebug() << rqte;

        //        if (o["UUID"].toString() == query3.value(5).toString()) {
        //            if (o["idVol"].toInt() != query3.value(2).toInt()) {
        //                QString requete = QString("UPDATE Reservation SET idVol= %1 WHERE UUID= \"%2\" ").arg(o["idVol"].toInt()).arg(o["UUID"].toString());
        //                QSqlQuery query(db);
        //                query.exec(requete);
        //            }

        //            if (o["idSiege"].toString() != query3.value(3).toString()) {
        //                QString requete = QString("UPDATE Reservation SET idSiege= \"%1\" WHERE UUID= \"%2\"  ").arg(o["idSiege"].toString()).arg(o["UUID"].toString());
        //                QSqlQuery query(db);
        //                query.exec(requete);
        //            }

        //            if (o["idPassager"].toInt() != query3.value(1).toInt()) {
        //                QString requete = QString("UPDATE Reservation SET idPassager= %1  WHERE UUID= \"%2\" ").arg(o["idPassager"].toInt()).arg(o["UUID"].toString());
        //                QSqlQuery query(db);
        //                query.exec(requete);
        //            }

    }
}


//__________________________________________
//Requete 8
void deleteReservation( QString UUID)
{
    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("DELETE FROM Reservation WHERE UUID = \"%1\"   ").arg(UUID);
    QSqlQuery query(db);
    query.exec(requete);
}









//*************************************************************
//               MAIN
//*************************************************************

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //*************************************************************
    QHttpServer server;

    //Ouverture de la base de données
    QDir path = QDir::currentPath();
    path.cd("../WebService_2");
    QString sFile = path.absoluteFilePath("DBReservationVols.db");
    if (QFile::exists(sFile))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "REP");
        db.setDatabaseName(sFile);
        db.open();
    }

    //requete 0:
    //    server.route("/villes", [] (const QHttpServerRequest & request) {
    //        return getVilles();
    //    });
    server.route("/villes", [] () {
        return getVilles();
    });

    //requete 1:
    server.route("/vols/<arg>/<arg>/<arg>", [] (QString depart , QString arrivee, QString date ) {
        return getVols(depart, arrivee, date);
    });

    // requete 2:
    server.route("/listeSiege/<arg>", [] (int id ) {
        return getListeSiege(id);
    });


    //requete 3:
    server.route("/passagers/<arg>", [] (QString recherche ) {
        return getPassager(recherche);
    });

    /*server.route("/reservations/<arg>", [] (QString uuid ,const QHttpServerRequest &request) {
                return getReservation(uuid);
            });*/

    server.route("/passagers", [] (const QHttpServerRequest & request) {
        if (request.method() == QHttpServerRequest::Method::Get)
        {
            return getPassager("aaaa");
        }
        else if (request.method() == QHttpServerRequest::Method::Post)
        {
            QJsonDocument d = QJsonDocument::fromJson(request.body());
            QJsonObject rootObj = d.object();
            QString err = addPassager(rootObj);
            if (err.size() ==0)
            {
                return QJsonObject{
                    {
                        "status", "ok"
                    }
                };
            }
            else
            {
                return QJsonObject{
                    {
                        "status", "ko"
                    },
                    {
                        "erreur", err
                    }
                };
            }
        }
        else if (request.method() == QHttpServerRequest::Method::Put)
        {
            QJsonDocument d = QJsonDocument::fromJson(request.body());
            QJsonObject rootObj = d.object();
            updatePassager(rootObj);
            return QJsonObject{};
        }
        return QJsonObject{
            {
                "status", "ko"
            },
            {
                "erreur", "err"
            }
        };    });

    server.route("/reservations", [] (const QHttpServerRequest & request) {
        if (request.method() == QHttpServerRequest::Method::Get)
        {
            return getReservation("aaaaa");
        }
        else if (request.method() == QHttpServerRequest::Method::Post)
        {
            QJsonDocument d = QJsonDocument::fromJson(request.body());
            QJsonObject rootObj = d.object();
            QString err = addReservation(rootObj);
            if (err.size() ==0)
            {
                return QJsonObject{
                    {
                        "status", "ok"
                    }};
            }
            else
            {
                return QJsonObject{
                    {
                        "status", "ko"
                    },
                    {
                        "erreur", err
                    }
                };
            }
        }
        else if (request.method() == QHttpServerRequest::Method::Put)
        {
            QJsonDocument d = QJsonDocument::fromJson(request.body());
            QJsonObject rootObj = d.object();
            updateReservation(rootObj);
        }
        return QJsonObject{};
    });

    server.route("/reservations/<arg>", [] (QString uuid , const QHttpServerRequest & request) {
        if (request.method() == QHttpServerRequest::Method::Get)
        {
            return getReservation(uuid);
        }
        else if (request.method() == QHttpServerRequest::Method::Delete)
        {

            deleteReservation(uuid);
        }
        return QJsonObject{};
    });

    server.listen(QHostAddress::Any, 8585);

    return a.exec();
}






