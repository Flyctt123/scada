#include "variablebindingdialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDebug>

VariableBindingDialog::VariableBindingDialog(QWidget *parent)
    : QDialog(parent)
{
    createUI();
    setupConnections();
    setWindowTitle(tr("变量绑定设置"));
}

void VariableBindingDialog::createUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout;

    // 组件ID
    componentIdEdit = new QLineEdit(this);
    formLayout->addRow(tr("组件ID:"), componentIdEdit);

    // 变量选择
    variableCombo = new QComboBox(this);
    formLayout->addRow(tr("变量:"), variableCombo);

    // 数据类型
    dataTypeCombo = new QComboBox(this);
    formLayout->addRow(tr("数据类型:"), dataTypeCombo);

    // 地址
    addressCombo = new QComboBox(this);
    formLayout->addRow(tr("地址:"), addressCombo);

    // 更新频率
    updateRateCombo = new QComboBox(this);
    formLayout->addRow(tr("更新频率:"), updateRateCombo);

    // 访问模式
    accessModeCombo = new QComboBox(this);
    formLayout->addRow(tr("访问模式:"), accessModeCombo);

    mainLayout->addLayout(formLayout);

    // 按钮
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void VariableBindingDialog::setupConnections()
{
    connect(variableCombo, &QComboBox::currentTextChanged,
            this, &VariableBindingDialog::onVariableSelected);
}

void VariableBindingDialog::setBinding(const QString &componentId, const VariableBinding &binding)
{
    qDebug() << "Setting binding for component:" << componentId;
    qDebug() << "Binding info:" << binding.variableName 
             << binding.dataType << binding.address 
             << binding.updateRate << binding.accessMode;

    m_componentId = componentId;
    componentIdEdit->setText(componentId);
    
    // 如果有现有绑定，先选择对应的变量
    if (!binding.variableName.isEmpty()) {
        int index = variableCombo->findText(binding.variableName);
        qDebug() << "Found variable index:" << index << "for" << binding.variableName;
        if (index >= 0) {
            // 暂时断开信号连接，避免触发 onVariableSelected
            disconnect(variableCombo, &QComboBox::currentTextChanged,
                      this, &VariableBindingDialog::onVariableSelected);
            
            variableCombo->setCurrentIndex(index);
            dataTypeCombo->setCurrentText(binding.dataType);
            addressCombo->setCurrentText(binding.address);
            updateRateCombo->setCurrentText(binding.updateRate);
            accessModeCombo->setCurrentText(binding.accessMode);
            
            // 重新连接信号
            connect(variableCombo, &QComboBox::currentTextChanged,
                   this, &VariableBindingDialog::onVariableSelected);
        }
    } else {
        // 如果没有绑定信息，设置为默认选项
        variableCombo->setCurrentIndex(0);  // "请选择变量"
        if (dataTypeCombo->count() > 0) dataTypeCombo->setCurrentIndex(0);
        if (addressCombo->count() > 0) addressCombo->setCurrentIndex(0);
        if (updateRateCombo->count() > 0) updateRateCombo->setCurrentIndex(0);
        if (accessModeCombo->count() > 0) accessModeCombo->setCurrentIndex(0);
    }
}

void VariableBindingDialog::setAvailableVariables(const QList<VariableInfo> &variables)
{
    qDebug() << "Setting" << variables.size() << "variables";
    m_variables = variables;

    // 保存当前选择的值
    QString currentVariable = variableCombo->currentText();
    QString currentDataType = dataTypeCombo->currentText();
    QString currentAddress = addressCombo->currentText();
    QString currentUpdateRate = updateRateCombo->currentText();
    QString currentAccessMode = accessModeCombo->currentText();

    // 填充变量下拉框
    variableCombo->clear();
    variableCombo->addItem(tr("请选择变量"));
    for (const VariableInfo &var : variables) {
        qDebug() << "Adding variable:" << var.name;
        variableCombo->addItem(var.name);
    }

    // 收集所有唯一值
    QStringList dataTypes, addresses, updateRates, accessModes;
    for (const VariableInfo &var : variables) {
        if (!dataTypes.contains(var.dataType))
            dataTypes << var.dataType;
        if (!addresses.contains(var.address))
            addresses << var.address;
        if (!updateRates.contains(var.updateRate))
            updateRates << var.updateRate;
        if (!accessModes.contains(var.accessMode))
            accessModes << var.accessMode;
    }

    // 填充其他下拉框
    dataTypeCombo->clear();
    dataTypeCombo->addItems(dataTypes);

    addressCombo->clear();
    addressCombo->addItems(addresses);

    updateRateCombo->clear();
    updateRateCombo->addItems(updateRates);

    accessModeCombo->clear();
    accessModeCombo->addItems(accessModes);

    // 恢复之前的选择（如果存在）
    int varIndex = variableCombo->findText(currentVariable);
    if (varIndex >= 0) variableCombo->setCurrentIndex(varIndex);

    if (!currentDataType.isEmpty()) dataTypeCombo->setCurrentText(currentDataType);
    if (!currentAddress.isEmpty()) addressCombo->setCurrentText(currentAddress);
    if (!currentUpdateRate.isEmpty()) updateRateCombo->setCurrentText(currentUpdateRate);
    if (!currentAccessMode.isEmpty()) accessModeCombo->setCurrentText(currentAccessMode);

    qDebug() << "Combo boxes filled:";
    qDebug() << "Variables:" << variableCombo->count() - 1;  // 减去"请选择变量"
    qDebug() << "Data types:" << dataTypes;
    qDebug() << "Addresses:" << addresses;
    qDebug() << "Update rates:" << updateRates;
    qDebug() << "Access modes:" << accessModes;
}

void VariableBindingDialog::onVariableSelected(const QString &variableName)
{
    qDebug() << "Variable selected:" << variableName;
    
    // 如果选择了"请选择变量"，清空其他字段
    if (variableName.isEmpty() || variableName == tr("请选择变量")) {
        dataTypeCombo->setCurrentIndex(0);
        addressCombo->setCurrentIndex(0);
        updateRateCombo->setCurrentIndex(0);
        accessModeCombo->setCurrentIndex(0);
        return;
    }

    // 当选择变量时，自动填充其他字段
    for (const VariableInfo &var : m_variables) {
        if (var.name == variableName) {
            qDebug() << "Found matching variable:" << var.name
                     << "type:" << var.dataType
                     << "address:" << var.address;
            
            dataTypeCombo->setCurrentText(var.dataType);
            addressCombo->setCurrentText(var.address);
            updateRateCombo->setCurrentText(var.updateRate);
            accessModeCombo->setCurrentText(var.accessMode);
            break;
        }
    }
}

void VariableBindingDialog::updateUIFromBinding(const VariableBinding &binding)
{
    if (!binding.variableName.isEmpty()) {
        int index = variableCombo->findText(binding.variableName);
        if (index >= 0) {
            variableCombo->setCurrentIndex(index);
        }
    }
    dataTypeCombo->setCurrentText(binding.dataType);
    addressCombo->setCurrentText(binding.address);
    updateRateCombo->setCurrentText(binding.updateRate);
    accessModeCombo->setCurrentText(binding.accessMode);
}

QString VariableBindingDialog::getComponentId() const
{
    return componentIdEdit->text();
}

VariableBinding VariableBindingDialog::getBinding() const
{
    VariableBinding binding;
    binding.variableName = variableCombo->currentText();
    binding.dataType = dataTypeCombo->currentText();
    binding.address = addressCombo->currentText();
    binding.updateRate = updateRateCombo->currentText();
    binding.accessMode = accessModeCombo->currentText();
    return binding;
}
