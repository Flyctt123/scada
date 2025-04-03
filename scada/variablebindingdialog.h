#ifndef VARIABLEBINDINGDIALOG_H
#define VARIABLEBINDINGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include "xmlconfig.h"

/**
 * @brief 变量绑定编辑对话框
 * 用于编辑图形组件与变量的绑定关系
 */
class VariableBindingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VariableBindingDialog(QWidget *parent = nullptr);

    /**
     * @brief 设置当前绑定信息
     * @param componentId 组件ID
     * @param binding 现有的绑定信息
     */
    void setBinding(const QString &componentId, const VariableBinding &binding);

    /**
     * @brief 设置可用的变量列表
     * @param variables 可用变量列表
     */
    void setAvailableVariables(const QList<VariableInfo> &variables);

    /**
     * @brief 获取组件ID
     * @return 组件ID
     */
    QString getComponentId() const;

    /**
     * @brief 获取编辑后的绑定信息
     * @return 绑定信息
     */
    VariableBinding getBinding() const;

private slots:
    /**
     * @brief 处理变量选择变化
     * 当选择不同变量时，自动填充相关属性
     * @param variableName 选择的变量名
     */
    void onVariableSelected(const QString &variableName);

private:
    /**
     * @brief 创建用户界面
     */
    void createUI();

    /**
     * @brief 设置信号连接
     */
    void setupConnections();

    /**
     * @brief 根据绑定信息更新界面
     * @param binding 绑定信息
     */
    void updateUIFromBinding(const VariableBinding &binding);

    QLineEdit *componentIdEdit;         // 组件ID编辑框
    QComboBox *variableCombo;          // 变量选择下拉框
    QComboBox *dataTypeCombo;          // 数据类型下拉框
    QComboBox *addressCombo;           // 地址下拉框
    QComboBox *updateRateCombo;        // 更新频率下拉框
    QComboBox *accessModeCombo;        // 访问模式下拉框

    QString m_componentId;              // 当前组件ID
    QList<VariableInfo> m_variables;    // 可用变量列表
};

#endif // VARIABLEBINDINGDIALOG_H 