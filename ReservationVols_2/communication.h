#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBitArray>



class Communication : public QObject
{
    Q_OBJECT


public:
    explicit Communication(QString sUrl, QObject *parent = nullptr);
    void getVilles();
    void getVols(QString depart, QString arrivee, QString date);
    void getListeSiege(int id);
    void getPassagers(QString recherche);
    void addPassager(QString nom, QString prenom, QString telephone, QString email);
    void updatePassager(qint32 id, QString nom, QString prenom, QString telephone, QString email);
    void addReservation(qint32 idPassager, QString idSiege, qint32 idVol, QString uuid);
    void getReservation(QString uuid);
    void updateReservation(QString UUID,  QString idSiege, qint32 idVol, qint32 id );
    void deleteReservation(QString uuid);

signals:
    void listVille(QJsonObject);
    void listVols(QJsonObject);
    void InfoPassager(QJsonObject);
    void listSiege(QJsonObject);
    void RecherchePassager(QJsonObject);
    void MajPassager(QJsonObject);
    void InfoReservation(QJsonObject);
    void addinfoReservation(QJsonObject);
    void InfoDeleteReservation(QJsonObject);
    void InfoUpdateReservation(QJsonObject);


public slots:
    void villesReceived();
    void volsReceived();
    void listeSiegeReceived();
    void passagersReceived();
    void passagerAdded();
    void passagerUpdated();
    void reservationAdded();
    void reservationReceived();
    void reservationUpdated();
    void reservationDeleted();

public:
    QNetworkAccessManager *manager;
    QString  mUrlServer;

};

#endif // COMMUNICATION_H
