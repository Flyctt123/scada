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

class ComponentDesigner : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComponentDesigner(QWidget *parent = nullptr);

private slots:
    void addText();
    void saveComponent();
    void loadComponent();
    void deleteSelected();

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

    // 动作
    QAction *addRectAction;
    QAction *addEllipseAction;
    QAction *addLineAction;
    QAction *addTextAction;
    QAction *saveAction;
    QAction *loadAction;
    QAction *deleteAction;

signals:
    void componentLibraryChanged();  // 添加信号
};

#endif // COMPONENTDESIGNER_H 