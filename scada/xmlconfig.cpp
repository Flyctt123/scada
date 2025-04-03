#include "xmlconfig.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

XmlConfig::XmlConfig(QObject *parent) : QObject(parent)
{
}

XmlConfig::~XmlConfig()
{
}

bool XmlConfig::loadConfig(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file:" << filename;
        return false;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    if (!m_doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "Parse XML failed:" << errorMsg << "at line" << errorLine;
        file.close();
        return false;
    }
    file.close();

    // 清除现有数据
    m_variables.clear();
    m_bindings.clear();

    // 读取变量配置
    QDomElement root = m_doc.documentElement();
    qDebug() << "Root element tag name:" << root.tagName();

    // 读取变量定义
    QDomElement varsElement = root.firstChildElement("variables");
    if (varsElement.isNull()) {
        qDebug() << "No variables element found";
        return false;
    }

    QDomNodeList variableNodes = varsElement.elementsByTagName("variable");
    qDebug() << "Found" << variableNodes.count() << "variables";
    
    for (int i = 0; i < variableNodes.count(); i++) {
        QDomElement varElem = variableNodes.at(i).toElement();
        if (varElem.isNull()) {
            qDebug() << "Variable element" << i << "is null";
            continue;
        }

        VariableInfo info;
        info.name = varElem.attribute("name");
        info.dataType = varElem.attribute("dataType");
        info.address = varElem.attribute("address");
        info.updateRate = varElem.attribute("updateRate");
        info.accessMode = varElem.attribute("accessMode");

        qDebug() << "Loading variable:" << info.name
                 << "type:" << info.dataType
                 << "address:" << info.address;
        m_variables.append(info);
    }

    // 读取绑定信息
    QDomElement bindingsElement = root.firstChildElement("bindings");
    if (!bindingsElement.isNull()) {
        QDomNodeList bindingNodes = bindingsElement.elementsByTagName("binding");
        qDebug() << "Found" << bindingNodes.count() << "bindings";
        
        for (int i = 0; i < bindingNodes.count(); i++) {
            QDomElement bindElem = bindingNodes.at(i).toElement();
            if (bindElem.isNull()) {
                qDebug() << "Binding element" << i << "is null";
                continue;
            }

            QString componentId = bindElem.attribute("componentId");
            if (componentId.isEmpty()) {
                qDebug() << "Empty component ID for binding" << i;
                continue;
            }

            VariableBinding binding;
            binding.variableName = bindElem.attribute("variableName");
            binding.dataType = bindElem.attribute("dataType");
            binding.address = bindElem.attribute("address");
            binding.updateRate = bindElem.attribute("updateRate");
            binding.accessMode = bindElem.attribute("accessMode");

            qDebug() << "Loading binding for component:" << componentId
                     << "variable:" << binding.variableName;
            m_bindings[componentId] = binding;
        }
    }

    qDebug() << "Successfully loaded" << m_variables.size() << "variables and"
             << m_bindings.size() << "bindings";
    return !m_variables.isEmpty();  // 只有成功读取到变量才返回true
}

QList<VariableInfo> XmlConfig::getAvailableVariables() const
{
    return m_variables;
}

VariableBinding XmlConfig::getVariableBinding(const QString &componentId) const
{
    return m_bindings.value(componentId);
}

void XmlConfig::addVariableBinding(const QString &componentId, const VariableBinding &binding)
{
    m_bindings[componentId] = binding;
}

bool XmlConfig::saveConfig(const QString &filename) const
{
    QDomDocument doc;
    QDomElement root = doc.createElement("config");
    doc.appendChild(root);

    // 保存变量定义
    QDomElement varsElement = doc.createElement("variables");
    root.appendChild(varsElement);

    for (const VariableInfo &var : m_variables) {
        QDomElement varElem = doc.createElement("variable");
        varElem.setAttribute("name", var.name);
        varElem.setAttribute("dataType", var.dataType);
        varElem.setAttribute("address", var.address);
        varElem.setAttribute("updateRate", var.updateRate);
        varElem.setAttribute("accessMode", var.accessMode);
        varsElement.appendChild(varElem);
    }

    // 保存绑定信息
    QDomElement bindingsElement = doc.createElement("bindings");
    root.appendChild(bindingsElement);

    for (auto it = m_bindings.constBegin(); it != m_bindings.constEnd(); ++it) {
        QDomElement bindingElem = doc.createElement("binding");
        bindingElem.setAttribute("componentId", it.key());
        bindingElem.setAttribute("variableName", it.value().variableName);
        bindingElem.setAttribute("dataType", it.value().dataType);
        bindingElem.setAttribute("address", it.value().address);
        bindingElem.setAttribute("updateRate", it.value().updateRate);
        bindingElem.setAttribute("accessMode", it.value().accessMode);
        bindingsElement.appendChild(bindingElem);
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << filename;
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString(4);  // 使用4个空格缩进
    file.close();

    qDebug() << "Saved" << m_variables.size() << "variables and" 
             << m_bindings.size() << "bindings to" << filename;
    return true;
}