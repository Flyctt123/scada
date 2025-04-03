#include "customview.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsItem>
#include <QMimeData>
#include <QDateTime>
#include <QLibrary>
#include "componentfactory.h"

CustomView::CustomView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setAcceptDrops(true);    // 启用放下功能
}

void CustomView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        // 接受所有组件类型的拖拽
        event->acceptProposedAction();
    }
}

void CustomView::dragMoveEvent(QDragMoveEvent *event)
{
    // 检查是否包含文本数据（组件类型）
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction();    // 接受拖动操作
    }
}

void CustomView::dropEvent(QDropEvent *event)
{
    // 获取鼠标在场景中的位置
    QPointF pos = mapToScene(event->pos());

    QString componentType = event->mimeData()->text();

    // 创建组件
    QGraphicsItem *item = ComponentFactory::createComponent(componentType, pos);
    
    if (item) {
        // 设置组件位置（不需要再次设置，因为 createComponent 已经设置了位置）
        scene()->addItem(item);
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setFlag(QGraphicsItem::ItemIsSelectable);

        // 生成唯一的组件ID
        QString componentId = QString("Component_%1").arg(
            QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
        item->setData(Qt::UserRole, componentId);

        event->acceptProposedAction();
    }
}
