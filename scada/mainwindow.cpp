#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customview.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include "xmlconfig.h"
#include "variablebindingdialog.h"
#include <QDebug>
#include "componentfactory.h"
#include "componentdesigner.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , xmlConfig(new XmlConfig(this))  // 初始化XML配置对象
{
    ui->setupUi(this);

    // 初始化图形视图
    setupGraphicsView();

    // 初始化组件列表
    createComponentList();

    // 创建动作和菜单
    createActions();
}

void MainWindow::setupGraphicsView()
{
    // 创建场景并设置大小
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    // 创建自定义视图并设置基本属性
    view = new CustomView(scene);
    view->setRenderHint(QPainter::Antialiasing);    // 启用抗锯齿
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);    // 设置视图更新模式
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    // 隐藏水平滚动条
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);      // 隐藏垂直滚动条
    view->setDragMode(QGraphicsView::RubberBandDrag);    // 启用橡皮筋选择模式

    setCentralWidget(view);    // 设置为主窗口的中央部件
}

void MainWindow::createComponentList()
{
    // 如果已经存在组件列表，直接返回
    if (componentList) {
        return;
    }

    // 创建组件库面板
    componentDock = new QDockWidget(tr("组件库"), this);
    componentList = new QListWidget(componentDock);

    // 配置列表控件属性
    componentList->setViewMode(QListWidget::IconMode);
    componentList->setIconSize(QSize(100, 100));
    componentList->setSpacing(10);
    componentList->setMovement(QListWidget::Static);
    componentList->setResizeMode(QListWidget::Adjust);
    componentList->setDragEnabled(true);
    componentList->setAcceptDrops(false);
    componentList->setMinimumWidth(120);
    componentList->setUniformItemSizes(true);
    componentList->setWordWrap(true);

    // 先添加默认组件
    QStringList defaultTypes = {"Button", "Gauge", "Valve", "ValueDisplay"};
    for (const QString &type : defaultTypes) {
        QString displayName = ComponentFactory::getComponentDisplayName(type);
        QListWidgetItem *item = new QListWidgetItem(componentList);
        item->setIcon(ComponentFactory::createComponentIcon(type));
        item->setText(displayName);
        item->setData(Qt::UserRole, type);
        item->setSizeHint(QSize(120, 140));
        item->setTextAlignment(Qt::AlignCenter);
    }

    // 获取并添加自定义组件
    QStringList types = ComponentFactory::getAvailableComponents();
    for (const QString &type : types) {
        // 跳过默认组件
        if (defaultTypes.contains(type)) {
            continue;
        }
        QString displayName = ComponentFactory::getComponentDisplayName(type);
        QListWidgetItem *item = new QListWidgetItem(componentList);
        item->setIcon(ComponentFactory::createComponentIcon(type));
        item->setText(displayName);
        item->setData(Qt::UserRole, type);
        item->setSizeHint(QSize(120, 140));
        item->setTextAlignment(Qt::AlignCenter);
    }

    connect(componentList, &QListWidget::itemPressed, this, &MainWindow::handleDragItem);

    componentDock->setWidget(componentList);
    addDockWidget(Qt::LeftDockWidgetArea, componentDock);
}

void MainWindow::handleDragItem(QListWidgetItem *item)
{
    // 创建拖动对象
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    // 设置要传输的数据
    mimeData->setText(item->data(Qt::UserRole).toString());
    drag->setMimeData(mimeData);

    // 设置拖动时的预览图像
    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::lightGray);
    drag->setPixmap(pixmap);

    // 执行拖动操作
    drag->exec(Qt::CopyAction);
}

void MainWindow::createActions()
{
    // 创建打开动作
    QAction *openAction = new QAction(tr("打开场景"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFromFile);

    // 创建保存动作
    saveAction = new QAction(tr("保存场景"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveToFile);

    // 创建配置动作
    QAction *configAction = new QAction(tr("读取变量配置"), this);
    connect(configAction, &QAction::triggered, this, &MainWindow::loadXmlConfig);

    // 创建变量绑定动作
    QAction *bindingAction = new QAction(tr("变量绑定"), this);
    connect(bindingAction, &QAction::triggered, this, &MainWindow::editVariableBinding);

    // 创建新增组件动作
    addComponentAction = new QAction(tr("组件编辑器"), this);
    connect(addComponentAction, &QAction::triggered, this, &MainWindow::addNewComponent);

    // 恢复导入组件库动作
    importLibraryAction = new QAction(tr("导入组件库"), this);
    connect(importLibraryAction, &QAction::triggered, this, &MainWindow::importComponentLibrary);

    // 创建工具栏
    QToolBar *toolBar = addToolBar(tr("工具栏"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(configAction);
    toolBar->addAction(bindingAction);
    toolBar->addAction(addComponentAction);
    toolBar->addAction(importLibraryAction);  // 恢复导入组件库按钮
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("保存场景"), "",
        tr("JSON文件 (*.json);;所有文件 (*)"));

    if (fileName.isEmpty())
        return;

    // 确保文件扩展名为.json
    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("保存失败"),
                           tr("无法写入文件 %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
        return;
    }

    // 创建JSON文档
    QJsonObject sceneObject;
    QJsonArray itemsArray;

    // 保存所有图形项
    foreach (QGraphicsItem *item, scene->items()) {
        QJsonObject itemObject;
        QPointF pos = item->pos();  // 获取位置
        QRectF rect;  // 用于存储矩形或椭圆的大小

        if (item->type() == QGraphicsRectItem::Type) {
            QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            // 检查是否是数值显示组件
            bool isValueDisplay = false;
            foreach (QGraphicsItem *child, rectItem->childItems()) {
                if (qgraphicsitem_cast<QGraphicsTextItem*>(child)) {
                    isValueDisplay = true;
                    break;
                }
            }

            if (isValueDisplay) {
                itemObject["itemType"] = "ValueDisplay";
            } else {
                itemObject["itemType"] = "Rectangle";
            }
            rect = rectItem->rect();
        }
        else if (item->type() == QGraphicsEllipseItem::Type) {
            QGraphicsEllipseItem *ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            itemObject["itemType"] = "Ellipse";
            rect = ellipseItem->rect();
        }

        // 保存位置和大小信息
        itemObject["x"] = pos.x();
        itemObject["y"] = pos.y();
        itemObject["width"] = rect.width();
        itemObject["height"] = rect.height();

        // 保存组件ID和变量绑定信息
        QString componentId = item->data(Qt::UserRole).toString();
        if (!componentId.isEmpty()) {
            itemObject["componentId"] = componentId;

            // 获取并保存变量绑定信息
            VariableBinding binding = xmlConfig->getVariableBinding(componentId);
            if (!binding.variableName.isEmpty()) {
                QJsonObject bindingObject;
                bindingObject["variableName"] = binding.variableName;
                bindingObject["dataType"] = binding.dataType;
                bindingObject["address"] = binding.address;
                bindingObject["updateRate"] = binding.updateRate;
                bindingObject["accessMode"] = binding.accessMode;
                itemObject["binding"] = bindingObject;
            }
        }

        itemsArray.append(itemObject);
    }

    sceneObject["items"] = itemsArray;
    QJsonDocument document(sceneObject);
    file.write(document.toJson(QJsonDocument::Indented));  // 使用缩进格式保存JSON

    QMessageBox::information(this, tr("保存成功"),
                           tr("场景已保存到文件：%1").arg(fileName));
}

void MainWindow::openFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("打开场景"), "",
        tr("JSON文件 (*.json);;所有文件 (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开失败"),
                           tr("无法读取文件 %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
        return;
    }

    // 解析JSON文档
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if (document.isNull()) {
        QMessageBox::warning(this, tr("解析失败"),
                           tr("解析JSON文件失败：%1")
                           .arg(jsonError.errorString()));
        return;
    }

    // 清除现有场景
    scene->clear();

    // 重建图形项
    QJsonObject sceneObject = document.object();
    QJsonArray itemsArray = sceneObject["items"].toArray();

    for (const QJsonValue &value : itemsArray) {
        QJsonObject itemObject = value.toObject();
        QString itemType = itemObject["itemType"].toString();
        qreal x = itemObject["x"].toDouble();
        qreal y = itemObject["y"].toDouble();
        qreal width = itemObject["width"].toDouble();
        qreal height = itemObject["height"].toDouble();

        QGraphicsItem *item = nullptr;
        if (itemType == "Rectangle") {
            QGraphicsRectItem *rectItem = new QGraphicsRectItem(0, 0, width, height);
            rectItem->setPos(x, y);
            scene->addItem(rectItem);
            item = rectItem;
        }
        else if (itemType == "Ellipse") {
            QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, width, height);
            ellipseItem->setPos(x, y);
            scene->addItem(ellipseItem);
            item = ellipseItem;
        }

        if (item) {
            item->setFlag(QGraphicsItem::ItemIsMovable);
            item->setFlag(QGraphicsItem::ItemIsSelectable);

            // 恢复组件ID和变量绑定信息
            if (itemObject.contains("componentId")) {
                QString componentId = itemObject["componentId"].toString();
                item->setData(Qt::UserRole, componentId);

                // 恢复变量绑定信息
                if (itemObject.contains("binding")) {
                    QJsonObject bindingObject = itemObject["binding"].toObject();
                    VariableBinding binding;
                    binding.variableName = bindingObject["variableName"].toString();
                    binding.dataType = bindingObject["dataType"].toString();
                    binding.address = bindingObject["address"].toString();
                    binding.updateRate = bindingObject["updateRate"].toString();
                    binding.accessMode = bindingObject["accessMode"].toString();
                    xmlConfig->addVariableBinding(componentId, binding);
                }
            }
        }
    }
}

void MainWindow::loadXmlConfig()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("打开变量配置文件"), "",
        tr("XML配置文件 (*.xml);;所有文件 (*)"));

    if (fileName.isEmpty())
        return;

    if (!xmlConfig->loadConfig(fileName)) {
        QMessageBox::warning(this, tr("配置加载失败"),
                           tr("无法加载配置文件：%1").arg(fileName));
        return;
    }

    QList<VariableInfo> variables = xmlConfig->getAvailableVariables();
    qDebug() << "Loaded" << variables.size() << "variables";

    if (variables.isEmpty()) {
        QMessageBox::warning(this, tr("配置为空"),
                           tr("未在配置文件中找到任何变量定义"));
        return;
    }

    QMessageBox::information(this, tr("配置加载成功"),
                           tr("成功加载 %1 个变量").arg(variables.size()));
}

void MainWindow::editVariableBinding()
{
    // 获取选中的图形项
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, tr("警告"),
                           tr("请先选择一个图形项"));
        return;
    }

    QGraphicsItem *item = selectedItems.first();

    // 获取或生成组件ID
    QString componentId = item->data(Qt::UserRole).toString();
    if (componentId.isEmpty()) {
        componentId = QString("Component_%1").arg(
            QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
        item->setData(Qt::UserRole, componentId);
    }

    // 获取现有绑定
    VariableBinding binding = xmlConfig->getVariableBinding(componentId);
    qDebug() << "Current binding for component" << componentId << ":";
    qDebug() << "Variable:" << binding.variableName;
    qDebug() << "Type:" << binding.dataType;
    qDebug() << "Address:" << binding.address;

    // 显示编辑对话框
    VariableBindingDialog dialog(this);

    // 设置可用变量（应该在设置绑定之前）
    QList<VariableInfo> variables = xmlConfig->getAvailableVariables();
    qDebug() << "Available variables:" << variables.size();
    dialog.setAvailableVariables(variables);

    // 设置当前绑定
    dialog.setBinding(componentId, binding);

    if (dialog.exec() == QDialog::Accepted) {
        // 更新绑定
        QString newComponentId = dialog.getComponentId();
        VariableBinding newBinding = dialog.getBinding();

        // 更新组件ID
        item->setData(Qt::UserRole, newComponentId);

        // 保存绑定
        xmlConfig->addVariableBinding(newComponentId, newBinding);

        // 自动保存到文件
//        QString fileName = "config.xml";  // 使用与加载相同的文件名
//        if (!xmlConfig->saveConfig(fileName)) {
//            QMessageBox::warning(this, tr("保存失败"),
//                               tr("无法保存变量绑定到文件：%1").arg(fileName));
//        } else {
//            qDebug() << "Successfully saved binding for component:" << newComponentId;
//        }
    }
}

void MainWindow::addNewComponent()
{
    ComponentDesigner *designer = new ComponentDesigner(this);
    designer->setAttribute(Qt::WA_DeleteOnClose);
    designer->show();
}

void MainWindow::importComponentLibrary()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("导入组件库"), "",
        tr("XML文件 (*.xml);;所有文件 (*)"));
    
    if (!filename.isEmpty()) {
        // 清空现有组件列表
        componentList->clear();

        // 先添加默认组件
        QStringList defaultTypes = {"Button", "Gauge", "Valve", "ValueDisplay"};
        for (const QString &type : defaultTypes) {
            QString displayName = ComponentFactory::getComponentDisplayName(type);
            QListWidgetItem *item = new QListWidgetItem(componentList);
            QIcon icon = ComponentFactory::createComponentIcon(type);
            item->setIcon(icon);
            item->setText(displayName);
            item->setData(Qt::UserRole, type);
            item->setSizeHint(QSize(120, 140));
            item->setTextAlignment(Qt::AlignCenter);
        }

        // 加载自定义组件库
        if (ComponentFactory::loadComponentLibrary(filename)) {
            // 获取组件库中的所有组件
            QStringList types = ComponentFactory::getAvailableComponents();

            // 添加自定义组件到列表
            for (const QString &type : types) {
                // 跳过默认组件
                if (defaultTypes.contains(type)) {
                    continue;
                }
                QString displayName = ComponentFactory::getComponentDisplayName(type);
                QListWidgetItem *item = new QListWidgetItem(componentList);
                QIcon icon = ComponentFactory::createComponentIcon(type);
                qDebug() << "Icon sizes for" << type << ":" << icon.availableSizes();
                item->setIcon(icon);
                item->setText(displayName);
                item->setData(Qt::UserRole, type);
                item->setSizeHint(QSize(120, 140));
                item->setTextAlignment(Qt::AlignCenter);
            }

            QMessageBox::information(this, tr("成功"),
                tr("成功导入组件库：%1").arg(filename));
        } else {
            QMessageBox::warning(this, tr("错误"),
                tr("导入组件库失败：%1").arg(filename));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

