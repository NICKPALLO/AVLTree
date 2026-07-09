cmake_minimum_required(VERSION 3.5)

project(AVLTree)

#set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/llvm-mingw_64")

set(CMAKE_AUTOMOC ON) # Автоматическая генерация moc-файлов
set(CMAKE_AUTORCC ON) # Автоматическая генерация ресурсов

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick QuickControls2)

qt_standard_project_setup()

add_executable(${PROJECT_NAME} main.cpp)

qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
        qml/Main.qml
    SOURCES
        TreePainter.h
        TreePainter.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2)

qt_finalize_executable(${PROJECT_NAME})