#include "componentfactory.h"
#include <QGraphicsTextItem>
#include <QtMath>
#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

// 静态成员变量，用于存储组件库
static QDomDocument componentLibrary;
static bool libraryLoaded = false;

// 加载组件库
bool ComponentFactory::loadComponentLibrary(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open component library file:" << filename;
        return false;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    
    // 读取文件内容进行调试
    QByteArray content = file.readAll();
    qDebug() << "XML Content:" << content;
    file.seek(0);  // 重置文件指针

    if (!componentLibrary.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "Parse XML failed:" << errorMsg 
                 << "at line" << errorLine 
                 << "column" << errorColumn;
        file.close();
        return false;
    }

    file.close();

    // 验证文档结构
    QDomElement root = componentLibrary.documentElement();
    if (root.isNull()) {
        qDebug() << "No root element found";
        return false;
    }

    if (root.tagName() != "components") {
        qDebug() << "Root element is not 'components', found:" << root.tagName();
        return false;
    }

    // 检查是否有组件
    QDomNodeList components = root.elementsByTagName("component");
    qDebug() << "Found" << components.count() << "components";

    libraryLoaded = true;
    return true;
}

QIcon ComponentFactory::createComponentIcon(const QString &type)
{
    if (!libraryLoaded) {
        // 尝试加载默认组件库
        if (!loadComponentLibrary("components.xml")) {
            return createDefaultIcon(type);
        }
    }

    // 从组件库中查找组件
    QDomElement root = componentLibrary.documentElement();
    QDomNodeList components = root.elementsByTagName("component");
    
    for (int i = 0; i < components.count(); i++) {
        QDomElement component = components.at(i).toElement();
        if (component.attribute("name") == type) {
            // 找到组件，加载预览图
            QDomElement preview = component.firstChildElement("preview");
            if (!preview.isNull()) {
                // 从元素的文本内容中读取Base64数据
                QString previewData = preview.text();
                if (!previewData.isEmpty()) {
                    QByteArray imageData = QByteArray::fromBase64(previewData.toLatin1());
                    QPixmap pixmap;
                    if (pixmap.loadFromData(imageData, "PNG")) {
                        qDebug() << "Successfully loaded preview for component:" << type
                                << "size:" << pixmap.size();
                        return QIcon(pixmap);
                    }
                    qDebug() << "Failed to load preview image data for component:" << type
                            << "data length:" << imageData.length();
                } else {
                    qDebug() << "Preview data is empty for component:" << type;
                }
            } else {
                qDebug() << "No preview element found for component:" << type;
            }
            break;
        }
    }

    qDebug() << "Using default icon for component:" << type;
    return createDefaultIcon(type);
}

QGraphicsItem* ComponentFactory::createComponent(const QString &type, const QPointF &pos)
{
    if (!libraryLoaded) {
        // 尝试加载默认组件库
        if (!loadComponentLibrary("components.xml")) {
            return createDefaultComponent(type, pos);
        }
    }

    // 从组件库中查找组件
    QDomElement root = componentLibrary.documentElement();
    QDomNodeList components = root.elementsByTagName("component");
    
    for (int i = 0; i < components.count(); i++) {
        QDomElement component = components.at(i).toElement();
        if (component.attribute("name") == type) {
            // 找到组件，创建图形项组
            QGraphicsItemGroup *group = new QGraphicsItemGroup();
            
            // 计算组件的边界矩形
            QRectF bounds;
            bool firstItem = true;

            // 读取所有图形项
            QDomElement items = component.firstChildElement("items");
            QDomNodeList itemList = items.elementsByTagName("item");
            
            for (int j = 0; j < itemList.count(); j++) {
                QDomElement item = itemList.at(j).toElement();
                QString itemType = item.attribute("type");
                
                QGraphicsItem *graphicsItem = nullptr;
                QRectF itemRect;
                
                if (itemType == "rect") {
                    qreal x = item.attribute("x").toDouble();
                    qreal y = item.attribute("y").toDouble();
                    qreal w = item.attribute("width").toDouble();
                    qreal h = item.attribute("height").toDouble();
                    QGraphicsRectItem *rectItem = new QGraphicsRectItem(x, y, w, h);
                    graphicsItem = rectItem;
                    itemRect = QRectF(x, y, w, h);
                }
                else if (itemType == "ellipse") {
                    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(
                        item.attribute("x").toDouble(),
                        item.attribute("y").toDouble(),
                        item.attribute("width").toDouble(),
                        item.attribute("height").toDouble()
                    );
                    graphicsItem = ellipseItem;
                    itemRect = ellipseItem->rect();
                }
                else if (itemType == "line") {
                    QGraphicsLineItem *lineItem = new QGraphicsLineItem(
                        item.attribute("x1").toDouble(),
                        item.attribute("y1").toDouble(),
                        item.attribute("x2").toDouble(),
                        item.attribute("y2").toDouble()
                    );
                    graphicsItem = lineItem;
                    QPointF p1 = lineItem->line().p1();
                    QPointF p2 = lineItem->line().p2();
                    itemRect = QRectF(p1, p2).normalized();
                }
                else if (itemType == "text") {
                    QGraphicsTextItem *textItem = new QGraphicsTextItem(
                        item.attribute("text")
                    );
                    textItem->setPos(
                        item.attribute("x").toDouble(),
                        item.attribute("y").toDouble()
                    );
                    graphicsItem = textItem;
                    itemRect = textItem->boundingRect();
                }

                if (graphicsItem) {
                    // 设置变换
                    QPointF itemPos(item.attribute("posX").toDouble(),
                                  item.attribute("posY").toDouble());
                    graphicsItem->setPos(itemPos);
                    graphicsItem->setRotation(item.attribute("rotation").toDouble());
                    graphicsItem->setScale(item.attribute("scale").toDouble());
                    
                    // 更新边界矩形
                    if (firstItem) {
                        bounds = itemRect.translated(itemPos);
                        firstItem = false;
                    } else {
                        bounds = bounds.united(itemRect.translated(itemPos));
                    }
                    
                    group->addToGroup(graphicsItem);
                }
            }

            // 计算偏移量，使组件中心对齐到目标位置
            QPointF offset = bounds.center();
            group->setPos(pos - offset);
            
            return group;
        }
    }

    // 如果没有找到，创建默认组件
    return createDefaultComponent(type, pos);
}

// 创建默认图标
QIcon ComponentFactory::createDefaultIcon(const QString &type)
{
    if (type == "Button") {
        QPixmap pixmap(100, 60);  // 增大尺寸
        pixmap.fill(Qt::lightGray);
        {
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::black);
            painter.setBrush(QColor(200, 200, 200));
            painter.drawRoundedRect(10, 10, 80, 40, 10, 10);  // 调整大小
            painter.setFont(QFont("Arial", 12));  // 增大字体
            painter.drawText(pixmap.rect(), Qt::AlignCenter, QObject::tr("按钮"));
        }
        return QIcon(pixmap);
    }
    else if (type == "Gauge") {
        QPixmap pixmap(50, 50);
        pixmap.fill(Qt::transparent);
        {
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::black);
            painter.setBrush(Qt::white);
            painter.drawEllipse(5, 5, 40, 40);
            painter.save();
            painter.translate(25, 25);
            for (int i = 0; i < 8; i++) {
                painter.drawLine(15, 0, 20, 0);
                painter.rotate(45);
            }
            painter.setPen(QPen(Qt::red, 2));
            painter.drawLine(0, 0, 12, 0);
            painter.restore();
        }
        return QIcon(pixmap);
    }
    else if (type == "Valve") {
        QPixmap pixmap(50, 50);
        pixmap.fill(Qt::transparent);
        {
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::black);
            painter.setBrush(QColor(200, 200, 255));
            painter.drawRect(15, 5, 20, 40);
            painter.drawRect(5, 20, 40, 10);
        }
        return QIcon(pixmap);
    }
    else if (type == "ValueDisplay") {
        QPixmap pixmap(50, 30);
        pixmap.fill(Qt::white);
        {
            QPainter painter(&pixmap);
            painter.setPen(Qt::black);
            painter.drawRect(0, 0, 49, 29);
            painter.setFont(QFont("Arial", 8));
            painter.drawText(pixmap.rect(), Qt::AlignCenter, "123.4");
        }
        return QIcon(pixmap);
    }
    return QIcon();
}

// 创建默认组件
QGraphicsItem* ComponentFactory::createDefaultComponent(const QString &type, const QPointF &pos)
{
    if (type == "Button") return createButton(pos);
    if (type == "Gauge") return createGauge(pos);
    if (type == "Valve") return createValve(pos);
    if (type == "ValueDisplay") return createValueDisplay(pos);
    return nullptr;
}

QGraphicsItem* ComponentFactory::createButton(const QPointF &pos)
{
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(0, 0, 80, 30);
    rectItem->setPos(pos);
    rectItem->setBrush(QColor(200, 200, 200));
    rectItem->setPen(QPen(Qt::black));
    
    QGraphicsTextItem *textItem = new QGraphicsTextItem(rectItem);
    textItem->setPlainText(QObject::tr("按钮"));
    textItem->setDefaultTextColor(Qt::black);
    QRectF rect = rectItem->rect();
    QRectF textRect = textItem->boundingRect();
    textItem->setPos(rect.center().x() - textRect.width()/2,
                    rect.center().y() - textRect.height()/2);
    
    return rectItem;
}

QGraphicsItem* ComponentFactory::createGauge(const QPointF &pos)
{
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, 60, 60);
    ellipseItem->setPos(pos);
    ellipseItem->setBrush(Qt::white);
    ellipseItem->setPen(QPen(Qt::black));
    
    // 添加刻度线
    for (int i = 0; i < 8; i++) {
        QGraphicsLineItem *line = new QGraphicsLineItem(ellipseItem);
        line->setPen(QPen(Qt::black));
        // 计算刻度线位置
        qreal angle = i * 45 * M_PI / 180;
        QPointF center = ellipseItem->rect().center();
        line->setLine(center.x() + 15 * cos(angle), center.y() + 15 * sin(angle),
                     center.x() + 20 * cos(angle), center.y() + 20 * sin(angle));
    }
    
    // 添加指针
    QGraphicsLineItem *pointer = new QGraphicsLineItem(ellipseItem);
    pointer->setPen(QPen(Qt::red, 2));
    QPointF center = ellipseItem->rect().center();
    pointer->setLine(center.x(), center.y(), center.x() + 20, center.y());
    
    return ellipseItem;
}

QGraphicsItem* ComponentFactory::createValve(const QPointF &pos)
{
    QGraphicsRectItem *containerItem = new QGraphicsRectItem(0, 0, 50, 50);
    containerItem->setPos(pos);
    containerItem->setPen(Qt::NoPen);  // 容器本身不可见

    // 阀门主体
    QGraphicsRectItem *body = new QGraphicsRectItem(15, 5, 20, 40, containerItem);
    body->setBrush(QColor(200, 200, 255));
    body->setPen(QPen(Qt::black));

    // 阀门手柄
    QGraphicsRectItem *handle = new QGraphicsRectItem(5, 20, 40, 10, containerItem);
    handle->setBrush(QColor(200, 200, 255));
    handle->setPen(QPen(Qt::black));

    return containerItem;
}

QGraphicsItem* ComponentFactory::createValueDisplay(const QPointF &pos)
{
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(0, 0, 100, 40);
    rectItem->setPos(pos);
    rectItem->setBrush(Qt::white);
    rectItem->setPen(QPen(Qt::black));

    // 添加文本显示
    QGraphicsTextItem *textItem = new QGraphicsTextItem(rectItem);
    textItem->setPlainText("123.4");
    textItem->setDefaultTextColor(Qt::black);
    textItem->setFont(QFont("Arial", 12));
    // 居中文本
    QRectF rect = rectItem->rect();
    QRectF textRect = textItem->boundingRect();
    textItem->setPos(rect.center().x() - textRect.width()/2,
                    rect.center().y() - textRect.height()/2);

    return rectItem;
}

QStringList ComponentFactory::getAvailableComponents()
{
    QStringList types;
    
    if (!libraryLoaded) {
        // 如果组件库未加载，返回默认组件列表
        types << "Button" << "Gauge" << "Valve" << "ValueDisplay";
        return types;
    }

    QDomElement root = componentLibrary.documentElement();
    if (root.isNull()) {
        // 如果根元素为空，返回默认组件列表
        types << "Button" << "Gauge" << "Valve" << "ValueDisplay";
        return types;
    }

    QDomNodeList components = root.elementsByTagName("component");
    
    for (int i = 0; i < components.count(); i++) {
        QDomElement component = components.at(i).toElement();
        if (!component.isNull()) {
            types << component.attribute("name");
        }
    }
    
    // 如果没有找到任何组件，返回默认组件列表
    if (types.isEmpty()) {
        types << "Button" << "Gauge" << "Valve" << "ValueDisplay";
    }
    
    return types;
}

QString ComponentFactory::getComponentDisplayName(const QString &type)
{
    if (!libraryLoaded) {
        // 返回默认显示名称
        if (type == "Button") return QObject::tr("按钮");
        if (type == "Gauge") return QObject::tr("仪表");
        if (type == "Valve") return QObject::tr("阀门");
        if (type == "ValueDisplay") return QObject::tr("数值显示");
        return type;
    }

    QDomElement root = componentLibrary.documentElement();
    QDomNodeList components = root.elementsByTagName("component");
    
    for (int i = 0; i < components.count(); i++) {
        QDomElement component = components.at(i).toElement();
        if (component.attribute("name") == type) {
            return component.attribute("displayName", type);
        }
    }
    
    return type;
}

// ... 其他组件的创建函数实现类似 ... 