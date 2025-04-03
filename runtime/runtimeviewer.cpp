#include "runtimeviewer.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QDateTime>
#include <QMap>
#include "mqttcomm.h"

RuntimeViewer::RuntimeViewer(const QString &sceneFile, QWidget *parent)
    : QMainWindow(parent)
{
    // 创建场景和视图
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(m_view);

    // 创建定时器
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &RuntimeViewer::updateValues);
    m_updateTimer->start(1000);  // 每秒更新一次

    // 加载场景
    loadScene(sceneFile);

    // 设置MQTT连接
    setupMqtt();

    // 设置窗口属性
    setWindowTitle(tr("运行时查看器"));
    resize(800, 600);
}

RuntimeViewer::~RuntimeViewer()
{
    if (m_updateTimer) {
        m_updateTimer->stop();
    }
}

void RuntimeViewer::loadScene(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("错误"),
                            tr("无法打开场景文件：%1\n%2")
                            .arg(fileName)
                            .arg(file.errorString()));
        return;
    }

    // 解析JSON文档
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if (doc.isNull()) {
        QMessageBox::critical(this, tr("错误"),
                            tr("解析场景文件失败：%1\n%2")
                            .arg(fileName)
                            .arg(jsonError.errorString()));
        return;
    }

    // 清除现有场景
    m_scene->clear();
    m_valueAddresses.clear();

    // 重建场景
    QJsonObject sceneObject = doc.object();
    QJsonArray itemsArray = sceneObject["items"].toArray();

    for (const QJsonValue &value : itemsArray) {
        QJsonObject itemObject = value.toObject();
        QString itemType = itemObject["itemType"].toString();
        qreal x = itemObject["x"].toDouble();
        qreal y = itemObject["y"].toDouble();
        qreal width = itemObject["width"].toDouble();
        qreal height = itemObject["height"].toDouble();

        QGraphicsItem *item = nullptr;

        if (itemType == "ValueDisplay") {
            // 创建数值显示组件
            QGraphicsRectItem *rectItem = new QGraphicsRectItem(0, 0, width, height);
            rectItem->setPos(x, y);
            
            // 添加文本显示
            QGraphicsTextItem *textItem = new QGraphicsTextItem(rectItem);
            textItem->setPlainText("0.0");
            textItem->setDefaultTextColor(Qt::black);
            textItem->setPos(10, 10);
            
            m_scene->addItem(rectItem);
            item = rectItem;

            // 保存地址映射
            if (itemObject.contains("binding")) {
                QJsonObject bindingObject = itemObject["binding"].toObject();
                QString address = bindingObject["address"].toString();
                if (!address.isEmpty()) {
                    m_valueAddresses[rectItem] = address;
                }
            }
        }
        else if (itemType == "Rectangle") {
            QGraphicsRectItem *rectItem = new QGraphicsRectItem(0, 0, width, height);
            rectItem->setPos(x, y);
            m_scene->addItem(rectItem);
            item = rectItem;
        }
        else if (itemType == "Ellipse") {
            QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, width, height);
            ellipseItem->setPos(x, y);
            m_scene->addItem(ellipseItem);
            item = ellipseItem;
        }

        if (item) {
            // 运行时组件不可移动和选择
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        }
    }

    // 调整视图以显示整个场景
    m_view->setSceneRect(m_scene->itemsBoundingRect());
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void RuntimeViewer::setupMqtt()
{
    m_mqtt = new MqttComm(this);
    
    // 连接到MQTT服务器
    m_mqtt->connectToBroker("mqtt.eclipseprojects.io", 1883);

    // 创建地址到主题的映射
    QMap<QString, QString> addressTopicMap;
    for (auto it = m_valueAddresses.begin(); it != m_valueAddresses.end(); ++it) {
        QString address = it.value();
        // 所有变量使用同一个主题
        QString topic = "scada/values";
        addressTopicMap[address] = topic;
        qDebug() << "Mapping address:" << address << "to topic:" << topic;
    }

    // 设置映射并订阅主题
    m_mqtt->setAddressTopicMap(addressTopicMap);

    // 连接值变化信号
    connect(m_mqtt, &MqttComm::valueChanged,
            this, &RuntimeViewer::handleValueChanged);
}

void RuntimeViewer::handleValueChanged(const QString &address, double value)
{
    // 查找使用此地址的所有组件
    for (auto it = m_valueAddresses.begin(); it != m_valueAddresses.end(); ++it) {
        if (it.value() == address) {
            QGraphicsItem *item = it.key();
            // 更新显示值
            foreach (QGraphicsItem *child, item->childItems()) {
                if (QGraphicsTextItem *textItem = qgraphicsitem_cast<QGraphicsTextItem*>(child)) {
                    textItem->setPlainText(QString::number(value, 'f', 1));
                    break;
                }
            }
        }
    }
}

void RuntimeViewer::updateValues()
{
    // 遍历所有数值显示组件
    for (auto it = m_valueAddresses.begin(); it != m_valueAddresses.end(); ++it) {
        QGraphicsItem *item = it.key();
        QString address = it.value();
        
        // 从MQTT获取值
        double value = m_mqtt->getValue(address);
        // 更新显示值
        foreach (QGraphicsItem *child, item->childItems()) {
            if (QGraphicsTextItem *textItem = qgraphicsitem_cast<QGraphicsTextItem*>(child)) {
                textItem->setPlainText(QString::number(value, 'f', 1));
                break;
            }
        }
    }
}
