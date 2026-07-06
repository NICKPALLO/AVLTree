#include "TreePainter.h"


void TreePainter::paint(QPainter *painter){
    painter->translate(offsetX, offsetY);
    painter->scale(zoom, zoom);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::white);
    // painter->drawLine(10, 10, 100, 100);

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
    //qDebug()<<"rotateRight";
    if(tree.head)
    {
        tree.head = RotateRight(tree.head);
        update();
    }
}
void TreePainter::rotateLeft()
{
    // qDebug()<<"rotateLeft";
    if(tree.head)
    {
        tree.head = RotateLeft(tree.head);
        update();
    }
}
void TreePainter::nextStep()
{
    qDebug()<<"nextStep";
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

    painter->drawEllipse(rect);

    painter->drawText(rect, Qt::AlignCenter,
                    QString::number(node->data));
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