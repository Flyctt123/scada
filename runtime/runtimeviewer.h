#ifndef RUNTIMEVIEWER_H
#define RUNTIMEVIEWER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMap>
#include "mqttcomm.h"

class RuntimeViewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit RuntimeViewer(const QString &sceneFile, QWidget *parent = nullptr);
    ~RuntimeViewer();

private slots:
    void updateValues();  // 更新数值显示组件的值
    void handleValueChanged(const QString &address, double value);  // 处理MQTT值变化

private:
    void loadScene(const QString &fileName);  // 加载场景文件
    void setupMqtt();  // 设置MQTT连接

    QGraphicsScene *m_scene;  // 场景
    QGraphicsView *m_view;    // 视图
    QTimer *m_updateTimer;    // 定时器
    QMap<QGraphicsItem*, QString> m_valueAddresses;  // 组件和地址的映射
    MqttComm *m_mqtt;  // MQTT通信对象
};

#endif // RUNTIMEVIEWER_H
