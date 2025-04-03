#ifndef MQTTCOMM_H
#define MQTTCOMM_H

#include <QObject>
#include <QtMqtt/qmqttclient.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class MqttComm : public QObject
{
    Q_OBJECT
public:
    explicit MqttComm(QObject *parent = nullptr);
    ~MqttComm();

    // 连接MQTT服务器
    void connectToBroker(const QString &host, quint16 port);
    
    // 订阅主题
    void subscribe(const QString &topic);
    
    // 发布消息
    void publish(const QString &topic, const QJsonObject &data);
    
    // 设置变量地址到主题的映射
    void setAddressTopicMap(const QMap<QString, QString> &addressTopicMap);
    
    // 获取变量值
    double getValue(const QString &address);

signals:
    // 当收到新数据时发出信号
    void valueChanged(const QString &address, double value);

private slots:
    // 处理MQTT消息
    void handleMessage(const QByteArray &message, const QMqttTopicName &topic);
    
    // 处理连接状态变化
    void handleStateChanged(QMqttClient::ClientState state);
    
    // 处理错误
    void handleError(QMqttClient::ClientError error);

private:
    QMqttClient *m_client;                          // MQTT客户端
    QMap<QString, QString> m_addressTopicMap;       // 地址到主题的映射
    QMap<QString, double> m_addressValueMap;        // 地址到值的映射
    QMap<QString, QMqttSubscription*> m_subscriptions;  // 主题订阅对象
};

#endif // MQTTCOMM_H 
