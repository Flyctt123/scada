#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include <QObject>
#include <QDomDocument>
#include <QMap>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QStringList>

/**
 * @brief 变量信息结构体
 * 存储单个变量的完整定义信息
 */
struct VariableInfo {
    QString name;           // 变量名称
    QString dataType;       // 数据类型（bool/int/float等）
    QString address;        // 变量地址（如DB1.DBD0）
    QString updateRate;     // 更新频率（毫秒）
    QString accessMode;     // 访问模式（read/write/readwrite）
};

/**
 * @brief 变量绑定信息结构体
 * 存储图形组件与变量的绑定关系
 */
struct VariableBinding {
    QString variableName;    // 绑定的变量名称
    QString dataType;        // 数据类型
    QString address;         // 变量地址
    QString updateRate;      // 更新频率
    QString accessMode;      // 访问模式
};

/**
 * @brief XML配置管理类
 * 负责管理变量定义和绑定信息的加载、保存
 */
class XmlConfig : public QObject
{
    Q_OBJECT
public:
    explicit XmlConfig(QObject *parent = nullptr);
    ~XmlConfig();

    /**
     * @brief 加载配置文件
     * @param filename 配置文件路径
     * @return 是否加载成功
     */
    bool loadConfig(const QString &filename);
    
    /**
     * @brief 获取所有可用变量
     * @return 变量信息列表
     */
    QList<VariableInfo> getAvailableVariables() const;
    
    /**
     * @brief 获取指定组件的变量绑定信息
     * @param componentId 组件ID
     * @return 绑定信息
     */
    VariableBinding getVariableBinding(const QString &componentId) const;
    
    /**
     * @brief 添加或更新变量绑定
     * @param componentId 组件ID
     * @param binding 绑定信息
     */
    void addVariableBinding(const QString &componentId, const VariableBinding &binding);
    
    /**
     * @brief 保存配置到文件
     * @param filename 目标文件路径
     * @return 是否保存成功
     */
    bool saveConfig(const QString &filename) const;

private:
    QDomDocument m_doc;                          // XML文档对象
    QMap<QString, VariableBinding> m_bindings;   // 组件ID到变量绑定的映射
    QList<VariableInfo> m_variables;             // 可用变量列表
};

#endif // XMLCONFIG_H 
