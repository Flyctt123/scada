#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include "runtimeviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString sceneFile;
    if (argc > 1) {
        // 如果命令行提供了场景文件路径
        sceneFile = argv[1];
    } else {
        // 否则弹出文件选择对话框
        sceneFile = QFileDialog::getOpenFileName(nullptr,
            QObject::tr("打开场景文件"),
            "",
            QObject::tr("JSON文件 (*.json);;所有文件 (*)"));

        if (sceneFile.isEmpty()) {
            return -1;
        }
    }

    // 创建运行时视图并显示
    RuntimeViewer viewer(sceneFile);
    viewer.show();

    return a.exec();
} 