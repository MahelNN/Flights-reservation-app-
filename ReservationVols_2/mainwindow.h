#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <communication.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool Controlephone(QString tel);
    void setVolsHeader();
    void setPassagerHeader();
    void setReservationHeader();


public slots:
    void afficherVilles(QJsonObject);
    void rechercheListVolsClicked(bool);
    void afficherVols(QJsonObject);
    void validerUnVolClicked(bool);
    void infoAvion(QJsonObject);


    ////
    void AddClient(QJsonObject);
    // void ValiderSiege(QJsonObject);
    //    void on_RechercheListVols_clicked();
    void on_AjouterPassager_clicked();
    void AfficheRecherchePassager(QJsonObject);
    void on_RechercherPassager_clicked();
    void MiseaJourPassager(QJsonObject);
    void AffichageReservation(QJsonObject);
    void AffichageAjoutReservation(QJsonObject);
    void DeleteReservation(QJsonObject);
    void AffichageUpdateReserv(QJsonObject);

public:
    Ui::MainWindow *ui;
    Communication* mcom;

public slots:
    //    void on_ValiderUnVol_clicked();
    void on_ModifierPassager_clicked();
    //    void on_Rechercher_Reservation_clicked();
    //    void on_Creer_Reservation_clicked();
    //    void on_Annuler_Reservation_clicked();
    //    void on_Modifier_Reservation_clicked();
private slots:
    void on_pbCreer_Reservation_clicked();
    void on_pbRechercher_Reservation_clicked();
    void on_pbModifier_Reservation_clicked();
    void on_pbAnnuler_Reservation_clicked();

    void reloadPassager();
    void reloadReservation();
};

#endif // MAINWINDOW_H
