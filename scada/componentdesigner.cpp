#include "componentdesigner.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPainter>
#include <QDomDocument>
#include <QBuffer>
#include <QTextStream>
#include <QDomProcessingInstruction>
#include <QDebug>
#include "componentfactory.h"

ComponentDesigner::ComponentDesigner(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createActions();
    createToolBars();
    createPropertyDock();
}

void ComponentDesigner::setupUI()
{
    // 创建场景和视图
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 400, 400);
    
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    
    setCentralWidget(view);
    setWindowTitle(tr("组件设计器"));
}

void ComponentDesigner::createActions()
{
    // 创建形状动作
    addRectAction = new QAction(tr("添加矩形"), this);
    connect(addRectAction, &QAction::triggered, this, [this]() {
        QGraphicsRectItem *rect = scene->addRect(0, 0, 100, 60);
        rect->setFlag(QGraphicsItem::ItemIsMovable);
        rect->setFlag(QGraphicsItem::ItemIsSelectable);
    });

    addEllipseAction = new QAction(tr("添加椭圆"), this);
    connect(addEllipseAction, &QAction::triggered, this, [this]() {
        QGraphicsEllipseItem *ellipse = scene->addEllipse(0, 0, 100, 60);
        ellipse->setFlag(QGraphicsItem::ItemIsMovable);
        ellipse->setFlag(QGraphicsItem::ItemIsSelectable);
    });

    addLineAction = new QAction(tr("添加直线"), this);
    connect(addLineAction, &QAction::triggered, this, [this]() {
        QGraphicsLineItem *line = scene->addLine(0, 0, 100, 0);
        line->setFlag(QGraphicsItem::ItemIsMovable);
        line->setFlag(QGraphicsItem::ItemIsSelectable);
    });

    addTextAction = new QAction(tr("添加文本"), this);
    connect(addTextAction, &QAction::triggered, this, &ComponentDesigner::addText);

    // 文件操作
    saveAction = new QAction(tr("保存组件"), this);
    connect(saveAction, &QAction::triggered, this, &ComponentDesigner::saveComponent);

    loadAction = new QAction(tr("加载组件"), this);
    connect(loadAction, &QAction::triggered, this, &ComponentDesigner::loadComponent);

    // 删除操作
    deleteAction = new QAction(tr("删除"), this);
    connect(deleteAction, &QAction::triggered, this, &ComponentDesigner::deleteSelected);
}

void ComponentDesigner::createToolBars()
{
    QToolBar *shapeToolBar = addToolBar(tr("形状"));
    shapeToolBar->addAction(addRectAction);
    shapeToolBar->addAction(addEllipseAction);
    shapeToolBar->addAction(addLineAction);
    shapeToolBar->addAction(addTextAction);

    QToolBar *fileToolBar = addToolBar(tr("文件"));
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(loadAction);
    fileToolBar->addAction(deleteAction);
}

void ComponentDesigner::createPropertyDock()
{
    propertyDock = new QDockWidget(tr("属性"), this);
    QWidget *widget = new QWidget(propertyDock);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // 添加组件名称输入
    QLabel *nameLabel = new QLabel(tr("组件名称:"), widget);
    componentNameEdit = new QLineEdit(widget);
    layout->addWidget(nameLabel);
    layout->addWidget(componentNameEdit);

    // 添加组件描述输入
    QLabel *descLabel = new QLabel(tr("组件描述:"), widget);
    componentDescEdit = new QLineEdit(widget);
    layout->addWidget(descLabel);
    layout->addWidget(componentDescEdit);

    // 添加属性列表
    propertyList = new QListWidget(widget);
    layout->addWidget(propertyList);

    propertyDock->setWidget(widget);
    addDockWidget(Qt::RightDockWidgetArea, propertyDock);
}

void ComponentDesigner::addText()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加文本"),
                                       tr("请输入文本:"), QLineEdit::Normal,
                                       tr("文本"), &ok);
    if (ok && !text.isEmpty()) {
        QGraphicsTextItem *textItem = scene->addText(text);
        textItem->setFlag(QGraphicsItem::ItemIsMovable);
        textItem->setFlag(QGraphicsItem::ItemIsSelectable);
    }
}

void ComponentDesigner::saveComponent()
{
    // 检查组件名称
    QString name = componentNameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), 
            tr("请输入组件名称"));
        return;
    }

    // 检查是否有图形项
    if (scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("警告"), 
            tr("请先添加图形项"));
        return;
    }

    // 获取组件描述
    QString description = componentDescEdit->text().trimmed();
    if (description.isEmpty()) {
        description = tr("自定义组件");
    }

    // 保存组件到组件库
    saveComponentToLibrary(name, description);
}

void ComponentDesigner::saveComponentToLibrary(const QString &name, const QString &description)
{
    // 创建组件库目录
    QDir dir("components");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 创建或加载组件库文件
    QString filename = "components.xml";
    QDomDocument doc;
    QFile file(filename);

    // 如果文件存在，先加载现有内容
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QString errorMsg;
        int errorLine, errorColumn;
        if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
            qDebug() << "Failed to parse existing XML:" << errorMsg 
                     << "at line" << errorLine << "column" << errorColumn;
            // 如果无法解析现有文件，创建新的文档
            QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction(
                "xml", "version=\"1.0\" encoding=\"UTF-8\"");
            doc.appendChild(xmlDeclaration);
            QDomElement root = doc.createElement("components");
            doc.appendChild(root);
        }
        file.close();
    } else {
        // 如果文件不存在，创建新的文档
        QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction(
            "xml", "version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(xmlDeclaration);
        QDomElement root = doc.createElement("components");
        doc.appendChild(root);
    }

    // 获取根元素
    QDomElement root = doc.documentElement();
    if (root.isNull() || root.tagName() != "components") {
        root = doc.createElement("components");
        doc.appendChild(root);
    }

    // 检查是否已存在同名组件
    QDomNodeList components = root.elementsByTagName("component");
    for (int i = 0; i < components.count(); i++) {
        QDomElement component = components.at(i).toElement();
        if (component.attribute("name") == name) {
            // 如果存在同名组件，删除它
            root.removeChild(component);
            break;
        }
    }

    // 创建新组件元素
    QDomElement componentElem = doc.createElement("component");
    componentElem.setAttribute("name", name);
    componentElem.setAttribute("displayName", name);
    componentElem.setAttribute("description", description);

    // 保存图形项
    QDomElement itemsElem = doc.createElement("items");
    foreach (QGraphicsItem *item, scene->items()) {
        QDomElement itemElem = doc.createElement("item");

        if (QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
            itemElem.setAttribute("type", "rect");
            QRectF rect = rectItem->rect();
            itemElem.setAttribute("x", rect.x());
            itemElem.setAttribute("y", rect.y());
            itemElem.setAttribute("width", rect.width());
            itemElem.setAttribute("height", rect.height());
        }
        else if (QGraphicsEllipseItem *ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item)) {
            itemElem.setAttribute("type", "ellipse");
            QRectF rect = ellipseItem->rect();
            itemElem.setAttribute("x", rect.x());
            itemElem.setAttribute("y", rect.y());
            itemElem.setAttribute("width", rect.width());
            itemElem.setAttribute("height", rect.height());
        }
        else if (QGraphicsLineItem *lineItem = qgraphicsitem_cast<QGraphicsLineItem*>(item)) {
            itemElem.setAttribute("type", "line");
            QLineF line = lineItem->line();
            itemElem.setAttribute("x1", line.x1());
            itemElem.setAttribute("y1", line.y1());
            itemElem.setAttribute("x2", line.x2());
            itemElem.setAttribute("y2", line.y2());
        }
        else if (QGraphicsTextItem *textItem = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
            itemElem.setAttribute("type", "text");
            itemElem.setAttribute("text", textItem->toPlainText());
            QPointF pos = textItem->pos();
            itemElem.setAttribute("x", pos.x());
            itemElem.setAttribute("y", pos.y());
        }

        // 保存位置和变换
        QPointF pos = item->pos();
        itemElem.setAttribute("posX", pos.x());
        itemElem.setAttribute("posY", pos.y());
        itemElem.setAttribute("rotation", item->rotation());
        itemElem.setAttribute("scale", item->scale());

        itemsElem.appendChild(itemElem);
    }
    componentElem.appendChild(itemsElem);

    // 创建预览图并保存为Base64
    QPixmap preview = createComponentPreview();
    if (!preview.isNull()) {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        preview.save(&buffer, "PNG");
        buffer.close();

        QDomElement previewElem = doc.createElement("preview");
        // 将预览图数据保存为元素的文本内容，而不是属性
        QDomText previewText = doc.createTextNode(byteArray.toBase64());
        previewElem.appendChild(previewText);
        componentElem.appendChild(previewElem);

        // 调试输出
        qDebug() << "Preview image size:" << preview.size();
        qDebug() << "Base64 data length:" << byteArray.toBase64().length();
    } else {
        qDebug() << "Failed to create preview image";
    }

    // 添加到根元素
    root.appendChild(componentElem);

    // 保存到文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("错误"), 
            tr("无法保存组件库文件：%1").arg(filename));
        return;
    }

    // 使用 UTF-8 编码保存
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);  // 添加 BOM 标记
    doc.save(out, 4);  // 使用4个空格缩进
    file.close();

    // 验证保存的文件
    if (file.open(QIODevice::ReadOnly)) {
        QString content = QString::fromUtf8(file.readAll());
        qDebug() << "Saved XML content:" << content;
        file.close();
    }

    QMessageBox::information(this, tr("成功"), 
        tr("组件已保存到组件库：%1").arg(name));
}

QPixmap ComponentDesigner::createComponentPreview()
{
    // 获取所有项目的边界矩形
    QRectF bounds = scene->itemsBoundingRect();
    if (bounds.isEmpty()) {
        qDebug() << "Empty bounds for preview";
        return QPixmap();
    }
    
    // 获取场景的大小
    QRectF sceneRect = scene->sceneRect();
    
    // 创建预览图（使用更大的尺寸以确保清晰度）
    QPixmap pixmap(200, 200);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 计算缩放比例，保持与场景的比例一致
    qreal sceneAspect = sceneRect.width() / sceneRect.height();
    qreal previewAspect = 200.0 / 200.0;
    
    qreal scale;
    QRectF targetRect;
    
    if (sceneAspect > previewAspect) {
        // 场景更宽，以宽度为准
        scale = 180.0 / sceneRect.width();  // 留出10像素边距
        qreal height = sceneRect.height() * scale;
        targetRect = QRectF(10, (200 - height) / 2, 180, height);
    } else {
        // 场景更高，以高度为准
        scale = 180.0 / sceneRect.height();  // 留出10像素边距
        qreal width = sceneRect.width() * scale;
        targetRect = QRectF((200 - width) / 2, 10, width, 180);
    }
    
    // 渲染整个场景
    scene->render(&painter, targetRect, sceneRect);
    
    // 缩放到列表项大小
    QPixmap scaledPixmap = pixmap.scaled(100, 100, 
        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    // 调试输出
    qDebug() << "Scene rect:" << sceneRect;
    qDebug() << "Bounds rect:" << bounds;
    qDebug() << "Scale factor:" << scale;
    qDebug() << "Target rect:" << targetRect;
    qDebug() << "Final preview size:" << scaledPixmap.size();
    
    return scaledPixmap;
}

void ComponentDesigner::loadComponent()
{
    // 实现加载功能
}

void ComponentDesigner::deleteSelected()
{
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : selectedItems) {
        scene->removeItem(item);
        delete item;
    }
} 
