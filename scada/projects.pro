TEMPLATE = subdirs

SUBDIRS += \
    scada \
    runtime

scada.file = scada/scada.pro
runtime.file = runtime/runtime.pro

# 确保子项目可以找到它们需要的头文件
scada.depends =
runtime.depends =

# 设置公共的构建目录
CONFIG += ordered 