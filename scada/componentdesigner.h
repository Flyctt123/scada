#ifndef COMPONENTDESIGNER_H
#define COMPONENTDESIGNER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>
#include <QDockWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMap>
#include <QVariant>

class ComponentDesigner : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComponentDesigner(QWidget *parent = nullptr);

private slots:
    void addText();
    void saveComponent();
    void loadComponent();  // 无参数版本，用于菜单动作
    void loadComponent(const QString &filename);  // 带参数版本，用于直接加载指定文件
    void deleteSelected();
    void onCategoryChanged(int index);

private:
    void setupUI();
    void createActions();
    void createToolBars();
    void createPropertyDock();
    void saveComponentToLibrary(const QString &name, const QString &description);
    QPixmap createComponentPreview();

    QGraphicsScene *scene;
    QGraphicsView *view;
    QDockWidget *propertyDock;
    QListWidget *propertyList;
    QLineEdit *componentNameEdit;    // 添加组件名称输入框
    QLineEdit *componentDescEdit;    // 添加组件描述输入框
    QComboBox *categoryCombo;  // 添加类别下拉框成员

    // 动作
    QAction *addRectAction;
    QAction *addEllipseAction;
    QAction *addLineAction;
    QAction *addTextAction;
    QAction *saveAction;
    QAction *loadAction;
    QAction *deleteAction;

    // 添加组件属性映射
    QMap<QString, QVariant> componentProperties;

signals:
    void componentLibraryChanged();  // 添加信号
    void propertiesChanged();
};

#endif // COMPONENTDESIGNER_H
