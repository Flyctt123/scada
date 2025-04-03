#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include <QtWidgets>  // 包含所有 QtWidgets 模块的头文件

// 自定义视图类，继承自QGraphicsView，用于处理拖放操作
class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    // 构造函数，可以同时指定场景和父窗口
    explicit CustomView(QGraphicsScene *scene = nullptr, QWidget *parent = nullptr);

    signals:
    void backgroundNeedsPaint(QPainter *painter, const QRectF &rect);

protected:
    // 重写拖放相关的事件处理函数
    void dragEnterEvent(QDragEnterEvent *event) override;    // 拖动进入事件
    void dragMoveEvent(QDragMoveEvent *event) override;      // 拖动移动事件
    void dropEvent(QDropEvent *event) override;              // 放下事件
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif 