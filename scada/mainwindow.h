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

namespace Ui {
class MainWindow;
}

/**
 * @brief 主窗口类
 * 
 * 负责管理整个应用程序的主界面，包括：
 * - 图形编辑区域
 * - 组件库面板
 * - 菜单栏和工具栏
 * - 变量绑定功能
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief 处理组件库中项目的拖拽事件
     * @param item 被拖拽的列表项
     */
    void handleDragItem(QListWidgetItem *item);

    /**
     * @brief 保存场景到JSON文件
     * 保存内容包括：图形项位置、大小、组件ID和变量绑定信息
     */
    void saveToFile();

    /**
     * @brief 从JSON文件加载场景
     * 加载内容包括：图形项位置、大小、组件ID和变量绑定信息
     */
    void openFromFile();

    /**
     * @brief 加载XML格式的变量配置文件
     * 读取变量定义，包括：变量名、数据类型、地址等信息
     */
    void loadXmlConfig();

    /**
     * @brief 编辑选中图形项的变量绑定
     * 打开变量绑定对话框，设置或修改绑定信息
     */
    void editVariableBinding();

    void addNewComponent();  // 组件编辑器
    void importComponentLibrary();  // 恢复导入组件库功能

private:
    /**
     * @brief 创建左侧组件库面板
     * 初始化组件列表，添加可拖拽的组件项
     */
    void createComponentList();

    /**
     * @brief 设置中央图形编辑视图
     * 初始化场景和视图，设置基本属性
     */
    void setupGraphicsView();

    /**
     * @brief 创建菜单和工具栏
     * 初始化各种动作和菜单项
     */
    void createActions();

    /**
     * @brief 加载自定义控件
     * @param path 控件库文件路径
     * @return 是否加载成功
     */
    bool loadCustomComponent(const QString &path);

    Ui::MainWindow *ui;
    QGraphicsScene *scene;    // 场景对象，用于管理所有图形项
    QGraphicsView *view;      // 视图对象，用于显示场景
    QDockWidget *componentDock;    // 左侧可停靠的组件面板
    QListWidget *componentList;    // 组件列表控件

    // 动作
    QAction *saveAction;      // 保存动作
    QAction *editAction;      // 添加编辑动作
    QAction *addComponentAction;  // 组件编辑器动作
    QAction *importLibraryAction;  // 恢复导入组件库动作

    XmlConfig *xmlConfig;  // 添加XML配置对象
};

#endif
