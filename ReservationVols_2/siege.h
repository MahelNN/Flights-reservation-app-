#ifndef SIEGE_H
#define SIEGE_H

#include <QGraphicsObject>
#include <QObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonObject>
class Siege : public QGraphicsObject
{
    Q_OBJECT
public:
    enum State {Rouge = Qt::red, Vert = Qt::green, Blanc = Qt::white, Bleu = Qt::blue};
    Siege(QString nomSiege, QGraphicsObject *parent = 0);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setOccupe(bool o) {mOccupe = o;}
    void setBleu();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mOccupe = false;
    QString mNom;
    int nbLigne;
    int nbColonne;
    State mState = State::Blanc;


signals:
    void siegeClicked(QString, bool);
};

#endif // SIEGE_H
