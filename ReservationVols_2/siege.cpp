#include "siege.h"
#include <QDir>
#include <QImage>

Siege::Siege(QString nomSiege, QGraphicsObject *parent): QGraphicsObject(parent)
{
    mNom = nomSiege;
    nbLigne = 0;
    nbColonne = 0;

}
QRectF Siege::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

void Siege::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF r = boundingRect();
    QRectF r1 = QRectF(5, 5, 35, 45);
    QDir path = QDir::currentPath();
    //    qDebug() << path.dirName();
    path.cd("../ReservationVols");
    QImage image(path.absoluteFilePath("siege.png"));

    if (mOccupe == false) {
        //painter->setPen(QPen(Qt::green));
        QPointF points[4] = { QPointF(5, 5), QPointF(38, 5),
                              QPointF(38, 48), QPointF(5, 48)
                            };
        painter->setBrush(QColor(Qt::GlobalColor(Qt::green)));
        painter->drawPolygon(points, 4);
        painter->drawImage(r1, image);
        mState = Vert;
    }
    else {
        QPointF points[4] = { QPointF(5, 5), QPointF(38, 5),
                              QPointF(38, 48), QPointF(5, 48)
                            };
        painter->setBrush(QColor(Qt::GlobalColor(Qt::red)));
        painter->drawPolygon(points, 4);
        painter->drawImage(r1, image);
        mState = Rouge;
    }
    /*
    if (mState == Bleu){
        QPointF points[4] = { QPointF(5, 5),QPointF(38,5),
                               QPointF(38,48),QPointF(5, 48) };
        painter->setBrush(QColor(Qt::GlobalColor(Qt::blue)));
        painter->drawPolygon(points, 4);
        painter->drawImage(r1,image);
    }
    */
    //painter->drawRect(r);
}

//void Siege::setSiegeOccupe(QJsonObject jO)
//{
//    //    ui->tw->setItem(i,2,new QTableWidgetItem(jsonArray1.at(i).toObject().value("idSiege").toString()));
//    QJsonArray jA = jO["sieges"].toArray();
//    siegeOccupes.resize(jA.count());
//    for(int i=0; i< jA.count(); i++)
//    {
//        siegeOccupes[i] = jA.at(i).toObject().value("idSiege").toString();
//        qDebug() << siegeOccupes[i];
//    }
//}

void Siege::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //    qDebug() << event->scenePos();
    //    qDebug() << mNom;

    //event->scenePos() ;
    emit siegeClicked(mNom, mOccupe);
}
