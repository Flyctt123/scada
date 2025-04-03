#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QColor>
#include <QFont>
#include <QPointF>

class ComponentFactory
{
public:
    // 加载组件库
    static bool loadComponentLibrary(const QString &filename);
    
    // 创建组件图标
    static QIcon createComponentIcon(const QString &type);
    
    // 创建组件实例
    static QGraphicsItem* createComponent(const QString &type, const QPointF &pos);

    // 获取组件库中所有可用的组件类型
    static QStringList getAvailableComponents();
    
    // 获取组件的显示名称
    static QString getComponentDisplayName(const QString &type);

private:
    // 创建默认图标和组件
    static QIcon createDefaultIcon(const QString &type);
    static QGraphicsItem* createDefaultComponent(const QString &type, const QPointF &pos);

    // 创建具体组件的辅助函数（作为默认实现）
    static QGraphicsItem* createButton(const QPointF &pos);
    static QGraphicsItem* createGauge(const QPointF &pos);
    static QGraphicsItem* createValve(const QPointF &pos);
    static QGraphicsItem* createValueDisplay(const QPointF &pos);
};

#endif // COMPONENTFACTORY_H 