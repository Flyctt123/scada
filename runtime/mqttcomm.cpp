#include "mqttcomm.h"
#include <QDebug>

MqttComm::MqttComm(QObject *parent)
    : QObject(parent)
    , m_client(new QMqttClient(this))
{
    // 连接信号槽
    connect(m_client, &QMqttClient::messageReceived,
            this, &MqttComm::handleMessage);
    connect(m_client, &QMqttClient::stateChanged,
            this, &MqttComm::handleStateChanged);
    connect(m_client, &QMqttClient::errorChanged,
            this, &MqttComm::handleError);
}

MqttComm::~MqttComm()
{
    // 断开所有订阅
    for (auto sub : m_subscriptions.values()) {
        delete sub;
    }
    m_subscriptions.clear();

    // 断开连接
    if (m_client->state() == QMqttClient::Connected) {
        m_client->disconnect();
    }
}

void MqttComm::connectToBroker(const QString &host, quint16 port)
{
    m_client->setHostname(host);
    m_client->setPort(port);
    m_client->connectToHost();
}

void MqttComm::subscribe(const QString &topic)
{
    if (m_client->state() != QMqttClient::Connected) {
        qWarning() << "MQTT client not connected, cannot subscribe to" << topic;
        return;
    }

    // 创建订阅
    auto subscription = m_client->subscribe(topic);
    if (subscription) {
        m_subscriptions[topic] = subscription;
        qDebug() << "Subscribed to topic:" << topic;
    } else {
        qWarning() << "Failed to subscribe to topic:" << topic;
    }
}

void MqttComm::publish(const QString &topic, const QJsonObject &data)
{
    if (m_client->state() != QMqttClient::Connected) {
        qWarning() << "MQTT client not connected, cannot publish to" << topic;
        return;
    }

    // 转换为JSON文档
    QJsonDocument doc(data);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);

    // 发布消息
    auto result = m_client->publish(topic, payload);
    if (result == -1) {
        qWarning() << "Failed to publish message to topic:" << topic;
    } else {
        qDebug() << "Published message to topic:" << topic;
    }
}

void MqttComm::setAddressTopicMap(const QMap<QString, QString> &addressTopicMap)
{
    // 取消旧的订阅
    for (auto sub : m_subscriptions.values()) {
        delete sub;
    }
    m_subscriptions.clear();

    // 设置新的映射
    m_addressTopicMap = addressTopicMap;

    // 订阅新的主题
    for (const QString &topic : addressTopicMap.values()) {
        subscribe(topic);
    }
}

double MqttComm::getValue(const QString &address)
{
    return m_addressValueMap.value(address, 0.0);
}

void MqttComm::handleMessage(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "Received message from topic:" << topic.name();
    
    // 将消息转换为UTF-8编码的QString
    QString messageString = QString::fromUtf8(message);
    qDebug() << "Message:" << messageString;

    // 解析JSON消息
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(messageString.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON message:" << error.errorString();
        return;
    }

    // 处理JSON对象
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        
        // 获取时间戳
        QString timestamp = obj["timestamp"].toString();
        
        // 获取数值数组
        QJsonArray values = obj["body"].toArray();
        if (values.isEmpty()) {
            qWarning() << "Message does not contain body array";
            return;
        }

        // 处理每个数值
        for (const QJsonValue &val : values) {
            QJsonObject valueObj = val.toObject();
            
            // 获取地址和值
            int addr = valueObj["addr"].toInt();
            double value = valueObj["val"].toDouble();
            
            // 转换地址格式为字符串
            QString address = QString::number(addr);
            
            qDebug() << "Processing address:" << address 
                     << "mapped topics:" << m_addressTopicMap.keys();
            
            // 检查是否是我们关注的地址
            if (m_addressTopicMap.contains(address)) {
                qDebug() << "Received value" << value << "for address" << address 
                        << "at time" << timestamp;
                
                // 更新值
                m_addressValueMap[address] = value;
                // 发出信号
                emit valueChanged(address, value);
            } else {
                qDebug() << "Address" << address << "not found in mapping";
            }
        }
    }
}

void MqttComm::handleStateChanged(QMqttClient::ClientState state)
{
    switch (state) {
        case QMqttClient::Connected:
            qDebug() << "MQTT client connected";
            // 重新订阅所有主题
            for (const QString &topic : m_addressTopicMap.values()) {
                subscribe(topic);
            }
            break;
        case QMqttClient::Disconnected:
            qDebug() << "MQTT client disconnected";
            break;
        case QMqttClient::Connecting:
            qDebug() << "MQTT client connecting...";
            break;
    }
}

void MqttComm::handleError(QMqttClient::ClientError error)
{
    qWarning() << "MQTT client error:" << error;
} 
