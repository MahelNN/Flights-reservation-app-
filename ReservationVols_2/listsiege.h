#ifndef LISTSIEGE_H
#define LISTSIEGE_H

#include <communication.h>


#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class ListSiege;
}

class ListSiege : public QDialog
{
    Q_OBJECT

public:
    explicit ListSiege(int r, int c, QJsonObject jO, QWidget *parent = nullptr);
    ~ListSiege();

    void setNbLigne(int r) {nbLigne = r;}
    void setNbColonne(int c) {nbColonne = c;}
    void constSetMap();

    void setInfoSiege(QString nomSiege);
    const QString &selection() const;


public slots:
    void selectSiege(QString, bool);
    void setID(int);
    void validerSiege(bool);

signals:
    void siegeValide(QString);


private slots:
    void on_pbAnnuler_clicked();

private:
    Ui::ListSiege *ui;
    int mID;
    int nbLigne;
    int nbColonne;

    Communication* mcom3;
    QGraphicsScene* mScene;
    QGraphicsSceneMouseEvent* mEvent;

    QList<QString> siegeOccupes;
    QJsonObject mData;
    QString mSelection = "";

};

#endif // LISTSIEGE_H
