#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "qtiled_graphics_scene.hpp"


QPointF round_16(QPointF p)
{
    int x = p.x();
    int y = p.y();
    return QPointF(x - (x % 16) - (x < 0 ? 16 : 0) ,  y - (y % 16) - (y < 0 ? 16 : 0));
}

void QTiledGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mousePressEvent(event);
    if(!left_mouse_buttom_down) {
        left_mouse_buttom_down = true;
        if(!drag && itemAt(event->scenePos(), QTransform())) {
            if(itemAt(event->scenePos(), QTransform())->flags() & QGraphicsItem::ItemIsSelectable) {
                drag = true;
                int x = event->scenePos().x();
                int y = event->scenePos().y();
                offset = QPointF(16 - (x % 16) - (x < 0 ? 16 : 0) ,  16 - (y % 16) - (y < 0 ? 16 : 0));
                round16_offset_old = QPointF();
            }
        }
    }
}

void QTiledGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    //QGraphicsScene::mouseMoveEvent(event);
    if(drag) {
        offset += event->scenePos() - event->lastScenePos();
        round16_offset = round_16(offset);
        QPointF delta = round16_offset - round16_offset_old;
        if(delta != QPointF()) emit objects_changed();
        foreach(QGraphicsItem* item, selectedItems()) {
            item->moveBy(delta.x(), delta.y());
        }
        round16_offset_old = round16_offset;
    }
    

}
void QTiledGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    drag = false;
    left_mouse_buttom_down = false;
    QGraphicsScene::mouseReleaseEvent(event);
}
