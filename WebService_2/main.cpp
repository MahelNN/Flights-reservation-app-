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
        QList<QString> listeKeys{"id", "idAvion", "Nom", "Depart", "Arrivee", "dateDepart", "dateArrivee", "heureDepart", "heureArrivee"};
        QJsonObject o;
        int i{0};
        for(auto &key  : listeKeys) o[key] = query.value(i++).toString();
   
        tab.append(o);
    }
    return QJsonObject{
        {
            "Vols", tab
        }
    };
}


/* requete2 */
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
        QList<QString> listeKeys{"idVol","NomAvion","nbLigne","Depart", "Depart", "Arrivee", "dateDepart", "dateArrivee", "heureDepart", "heureArrivee", "Nom", "Prenom", "idSiege", "UUID"};
        it++;
        if (it == 1){
            QJsonObject oAvion, oVol;
            for(int i=0; i<10; i++){
                if(i>0 && i<4){             //Remplir oAvion
                    if(i==1) oAvion[listeKeys[i]] = query.value(i).toString();
                    oAvion[listeKeys[i]] = query.value(i).toInt();
                }
                if((i>=0 && i<2) || (i>3)){//Remplir oVol
                    if(i==0) oVol[listeKeys[i]] = query.value(i).toInt();
                    oVol[listeKeys[i]] = query.value(i).toString();
                }
            }
            tab0.append(oAvion);
            tab0.append(oVol);
        }

        QJsonObject oSiege;//Remplir oSiege
        for(int i=10; i<14; i++) oSiege[listeKeys[i]] = query.value(i).toString();
        tab2.append(oSiege);
    }

    return QJsonObject{{ "Avion", tab0 }, { "Vol", tab1 }, { "Sieges", tab2 }};
}


/* Requete 3 */

QJsonObject getPassager(QString recherche)
{
    QJsonArray tab;
    QSqlDatabase db = QSqlDatabase::database("REP");
    QSqlQuery query("SELECT * from Passagers" , db);
    qint32 i = 0;

    while (query.next())
    {
        if (recherche == query.value(3).toString() || recherche == query.value(4).toString() || recherche == query.value(2).toString() || recherche == query.value(1).toString() || recherche == query.value(0).toString() ) {
            QJsonObject o;
            QList<QString> listeKeys{"id","Nom","Prenom","Telephone", "Email"};
            int it{0};
            for(auto &key  : listeKeys) o[key] = query.value(it++).toString();
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


/* Requete 4 */

QString addPassager(QJsonObject o)
{
    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("INSERT INTO Passagers (Nom,Prenom,Telephone,Email) VALUES (\"%1\",\"%2\",\"%3\",\"%4\" )").arg(o["Nom"].toString()).arg(o["Prenom"].toString()).arg(o["Telephone"].toString()).arg(o["Email"].toString());
    QSqlQuery query(db);
    query.exec(requete);
    return query.lastError().text();
}


/* Requete 4-1 */

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



/* Requete 5 */

QString addReservation(QJsonObject o)
{

    QSqlDatabase db = QSqlDatabase::database("REP");
    QString requete = QString("INSERT INTO Reservation (idPassager,idVol, idSiege, UUID) "
                              "VALUES (%1 ,%2,\"%3\",\"%4\") ")
            .arg(o["idPassager"].toInt())
            .arg(o["idVol"].toInt())
            .arg(o["idSiege"].toString())
            .arg(o["UUID"].toString());
    QSqlQuery query(db);
    query.exec(requete);
    return query.lastError().text();
}




/* Requete 6 */

QJsonObject getReservation(QString UUID)
{
    QJsonArray tab;
    QSqlDatabase db = QSqlDatabase::database("REP");

    QString requete = QString("SELECT Reservation.idVol, Reservation.idSiege, Reservation.UUID, Passagers.Nom, Passagers.Prenom, Vols.dateArrivee, Vols.dateDepart, Vols.heureArrivee, Vols.heureDepart, Avions.NomAvion, Avions.nbLigne, Avions.nbColonne, depart.nomVille AS Depart, arrivee.nomVille AS Arrivee FROM Reservation INNER JOIN Passagers ON Passagers.id = Reservation.idPassager  INNER JOIN Vols ON Vols.id = Reservation.idVol  INNER JOIN Avions ON Avions.id = Vols.idAvion INNER JOIN Villes as arrivee ON arrivee.id = Vols.idArrivee INNER JOIN Villes as depart ON depart.id = Vols.idDepart WHERE UUID = \"%1\" ").arg(UUID);
    QSqlQuery query(requete, db);
    while (query.next())
    {
        QJsonObject o;
        QList<QString> listeKeys{"idVol","NoidSiegem","UUID","Nom", "Prenom", "dateArrivee", "dateDepart", "heureArrivee", "heureDepart", "Nom", "Depart", "Arrivee"};
        int it{0};
        for(auto &key  : listeKeys) o[key] = query.value(it++).toString();
        for(int i=0; i<12; i++){
            if(i<10) o[listeKeys[i]] = query.value(i).toString();
            else o[listeKeys[i]] = query.value(i+2).toString();
        }
        tab.append(o);
    }
    return QJsonObject{
        {
            "data", tab
        }
    };
}


/* Requete 7 */

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
    }
}


/* Requete 8 */
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






