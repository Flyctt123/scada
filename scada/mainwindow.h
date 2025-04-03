#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QListWidget>
#include <QDockWidget>
#include <QAction>
#include <QtWidgets>  // 包含所有 QtWidgets 模块的头文件
#include <xmlconfig.h>
#include <QMap>
#include "../common/componentinfo.h"  // 添加这行
#include <QLibrary>
#include <QFileInfo>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //处理组件库中项目的拖拽事件
    void handleDragItem(QTreeWidgetItem *item);

    //保存场景到JSON文件
    void saveToFile();

    //从JSON文件加载场景
    void openFromFile();

    //加载XML格式的变量配置文件,读取变量定义，包括：变量名、数据类型、地址等信息
    void loadXmlConfig();

    //编辑选中图形项的变量绑定
    void editVariableBinding();

    void addNewComponent();  // 组件编辑器
    void importComponentLibrary();  // 恢复导入组件库功能

    void drawBackground(QPainter *painter, const QRectF &rect);  // 添加绘制背景的槽函数

private:
    //创建左侧组件库面板,初始化组件列表，添加可拖拽的组件项
    void createComponentList();

    //设置中央图形编辑视图,初始化场景和视图，设置基本属性
    void setupGraphicsView();

    //创建菜单和工具栏
    void createActions();

    // 添加新的辅助函数
    QString getComponentCategory(const QString &type);

    Ui::MainWindow *ui;
    QGraphicsScene *scene;    // 场景对象，用于管理所有图形项
    QGraphicsView *view;      // 视图对象，用于显示场景
    QDockWidget *componentDock;    // 左侧可停靠的组件面板
    QTreeWidget *componentTree;    // 替换 QListWidget
    QMap<QString, QTreeWidgetItem*> categoryNodes;  // 类别节点映射

    // 动作
    QAction *saveAction;      // 保存动作
    QAction *editAction;      // 添加编辑动作
    QAction *addComponentAction;  // 组件编辑器动作
    QAction *importLibraryAction;  // 恢复导入组件库动作

    XmlConfig *xmlConfig;  // 添加XML配置对象

    // 添加网格相关成员
    QAction *toggleGridAction;  // 切换网格显示的动作
    bool showGrid;             // 网格显示状态
};

#endif
