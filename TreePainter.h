#ifndef TREEPAINTER_H
#define TREEPAINTER_H

#include<QObject>
#include<QQuickPaintedItem>
#include "AVLTree.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <random>


class TreePainter : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    Q_PROPERTY(float offsetX READ getOffsetX WRITE setOffsetX NOTIFY offsetXChanged)
    Q_PROPERTY(float offsetY READ getOffsetY WRITE setOffsetY NOTIFY offsetYChanged)
    Q_PROPERTY(float zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)


    float getOffsetX() {return offsetX;}
    void setOffsetX(float val) {offsetX = val; emit offsetXChanged();}
    float getOffsetY() {return offsetY;}
    void setOffsetY(float val) {offsetY = val; emit offsetYChanged();}
    float getZoom() {return zoom;}
    void setZoom(float val) {zoom = val; emit zoomChanged();}

    explicit TreePainter(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    Q_INVOKABLE void addNode(int val);
    Q_INVOKABLE void deleteNode(int val);
    Q_INVOKABLE void rotateRight();
    Q_INVOKABLE void rotateLeft();
    Q_INVOKABLE void nextStep();
    Q_INVOKABLE void resetPosition();

public slots:
    void processAnimation();

signals:
 void offsetXChanged();
 void offsetYChanged();
 void zoomChanged();

private:
    AVL<int> tree;

    const float standart_offsetX = 800;
    const float standart_offsetY = 200;
    const float standart_zoom = 1;


    float offsetX = standart_offsetX;
    float offsetY = standart_offsetY;
    float zoom = standart_zoom;

    float d = 50;
    float y_step = -60;
    float l_step = 30;
    float x_step = 1;

    unsigned findMaxH();

    void addCircle(Node<int>* node,float x,float y,unsigned depth,QPainter *painter);
    QLineF shrinkLine(const QPointF& p1, const QPointF& p2, float distance);

    QTimer timer;


    std::vector<int> extraVector;
    std::random_device rd;
    std::mt19937 gen{rd()};
    
    std::uniform_int_distribution<int> dist100{1, 100};
    std::uniform_int_distribution<int> dist01{0, 1};

};

#endif //TREEPAINTER_H