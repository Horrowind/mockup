#ifndef QTILED_GRAPHICS_SCENE
#define QTILED_GRAPHICS_SCENE

#include <QGraphicsScene>

class QTiledGraphicsScene : public QGraphicsScene {
    Q_OBJECT
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void objects_changed();
    
private:
    bool drag = false;
    bool left_mouse_buttom_down = false;

    QPointF offset;
    QPointF round16_offset;
    QPointF round16_offset_old;
};


#endif //QTILED_GRAPHICS_SCENE
