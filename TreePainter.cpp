#include "TreePainter.h"
#include <iostream>

TreePainter::TreePainter(QQuickItem *parent) : QQuickPaintedItem(parent) {    
    connect(&timer, &QTimer::timeout,this, &TreePainter::processAnimation);
}

void TreePainter::paint(QPainter *painter){
    painter->translate(offsetX, offsetY);
    painter->scale(zoom, zoom);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::white);

    unsigned max_h = findMaxH();
    x_step = std::pow(2,static_cast<int>(max_h)-1)*(l_step+d);


    if(!tree.head)
    {
        return;
    }
    addCircle(tree.head,0,0,0,painter);
}

void TreePainter::addNode(int val)
{
    //qDebug()<<"addNode "<<val;
    tree.insert(val);
    update();
}
void TreePainter::deleteNode(int val)
{
    bool res = tree.erase(val);
    if(res)
    {
        update();
    }
    else
    {
        qDebug()<<"Can't delete "<<val;
    }
}
void TreePainter::rotateRight()
{
    if(tree.head)
    {
        tree.head = RotateRight(tree.head);
        update();
    }
}
void TreePainter::rotateLeft()
{
    if(tree.head)
    {
        tree.head = RotateLeft(tree.head);
        update();
    }
}
void TreePainter::nextStep()
{
    processAnimation();
}

static void processNodeHeight(Node<int>* node, unsigned currentDepth, unsigned& depth)
{
    depth = currentDepth>depth ? currentDepth : depth;
    if(node->left)
    {
        processNodeHeight(node->left,currentDepth+1,depth);
    }
    if(node->right)
    {
        processNodeHeight(node->right,currentDepth+1,depth);
    }
}


unsigned TreePainter::findMaxH()
{
    if(!tree.head)
    {
        return 0;
    }
    unsigned depth = 0;

    processNodeHeight(tree.head,0,depth);

    return depth;
}

void TreePainter::addCircle(Node<int>* node,float x,float y,unsigned depth,QPainter *painter)
{
    QRectF rect(x, y, d, d);
    QRectF rect2(x+d, y, d, d);

    painter->drawEllipse(rect);

    painter->drawText(rect, Qt::AlignCenter,
                    QString::number(node->data));

    
    // painter->drawText(rect, Qt::AlignBottom,
    //                 QString::number(node->height));

    QString parent = node->parent ? QString::number(node->parent->data) : "null";
    QString left = node->left ? QString::number(node->left->data) : "null";
    QString right = node->right ? QString::number(node->right->data) : "null";

    QString Line = QString("P:")+parent+QString("\nL:")+left+QString("\nR:")+right;

    painter->drawText(rect2, Qt::AlignCenter,Line);

    if(node->left)
    {
        float new_x = x-x_step/std::pow(2,depth+1);
        float new_y = y-y_step;
        QLineF line = shrinkLine(QPointF(x+d/2,y+d/2),QPointF(new_x+d/2,new_y+d/2),d/2);

        painter->drawLine(line);

        addCircle(node->left,new_x,new_y,depth+1,painter);
    }
    if(node->right)
    {
        float new_x = x+x_step/std::pow(2,depth+1);
        float new_y = y-y_step;
        QLineF line = shrinkLine(QPointF(x+d/2,y+d/2),QPointF(new_x+d/2,new_y+d/2),d/2);

        painter->drawLine(line);

        addCircle(node->right,new_x,new_y,depth+1,painter);
    }
}


QLineF TreePainter::shrinkLine(const QPointF& p1, const QPointF& p2, float distance) {
    // Вектор от p1 к p2
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();
    
    // Длина отрезка
    float length = std::sqrt(dx * dx + dy * dy);
    
    // Защита от вырожденного случая (точки совпадают)
    if (length < 1e-12) {
        return {p1, p2}; // Или можно вернуть p1, p2 без изменений
    }
    
    // Нормализованный вектор направления
    float ux = dx / length;
    float uy = dy / length;
    
    // Если расстояние больше половины длины, сдвигаем точки к центру
    // но не даём им пересечься (можно оставить небольшой зазор)
    float halfLength = length / 2.0;
    float shift = std::min(distance, halfLength - 1e-6f); // Не даём точкам пересечься
    
    // Смещаем первую точку вперёд на shift
    QPointF newP1(p1.x() + ux * shift, p1.y() + uy * shift);
    
    // Смещаем вторую точку назад на shift
    QPointF newP2(p2.x() - ux * shift, p2.y() - uy * shift);
    
    return QLineF(newP1, newP2);
}

void TreePainter::resetPosition()
{
    offsetX = standart_offsetX;
    offsetY = standart_offsetY;
    zoom = standart_zoom;
    update();
}

void TreePainter::processAnimation()
{
    bool insert = true;
    size_t index = 0;
    int number = dist100(gen);

    if(!extraVector.empty())
    {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(extraVector.size()-1));
        index = dist(gen);
        insert = dist01(gen);
    }

    if(insert && !tree.find(number))
    {
        addNode(number);
        extraVector.push_back(number);
    }
    else
    {
        Node<int>* node = tree.find(extraVector[index]);
        if(node)
        {
            deleteNode(node->data);
            extraVector[index] = extraVector.back();
            extraVector.pop_back();
        }
        else
        {
            timer.stop();
            std::cout<<"Error\nextraVector: ";
            for(const auto i : extraVector)
            {
                std::cout<<i<<" ";
            }
            std::cout<<"\ncan't find index "<<index<<"with number "<<extraVector[index]<<std::endl;
        }
    }
}

void TreePainter::startAnimation()
{
    timer.start(1000);
}

void TreePainter::stopAnimation()
{
    timer.stop();
}