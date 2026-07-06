import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import AVLTree 1.0

ApplicationWindow {
    color: '#595959'
    visible: true
    width: 1300
    height: 700
    title: "AVL Tree"

    function addNode(val)
    {
        painter.addNode(val)
    }

    function deleteNode(val)
    {
        painter.deleteNode(val)
    }

    component InputRect : Item{
        id: it
        required property string reqText
        required property string reqButtonText
        required property var func


        implicitWidth: row_1.implicitWidth+10
        
        RowLayout{
            id: row_1
            anchors.top: it.top
            anchors.bottom: it.bottom
            spacing: 10

            Rectangle{
                id: rec1
                implicitWidth: row_2.implicitWidth+10
                Layout.fillHeight: true
                color: '#66859a'
                radius: 10
                
                RowLayout{
                    id: row_2
                    anchors.margins: 5
                    anchors.top: rec1.top
                    anchors.bottom: rec1.bottom
                    spacing: 10
                    Text {
                        id: textEditText
                        text: reqText
                        color: "white"
                        font.pixelSize: 14
                        Layout.alignment: Qt.AlignVCenter
                        Layout.leftMargin: 5
                    }
                    TextField {
                        id: textEdit
                        Layout.preferredWidth: 120  // Желаемая ширина
                        Layout.minimumWidth: 100    // Минимальная ширина
                        Layout.maximumWidth: 150   // Максимальная ширина
                        Layout.alignment: Qt.AlignVCenter
                        placeholderText: "число"
                        
                        color: "white"
                        
                        cursorDelegate: Rectangle {
                            width: 2
                            color: '#31000000'  // Цвет курсора
                            visible: parent.activeFocus
                        }
                        background: Rectangle {
                            color: '#003e70'
                            radius: 3
                        }
                        validator: IntValidator {
                            bottom: -100000
                            top: 100000
                        }
                        onAccepted: {
                            if(text !== "")
                            {    
                                func(parseInt(text))
                                text = ""
                            }
                            else
                            {
                                console.log("text empty")
                            }
                        }
                    }
                }
            }
            Button{
                id: button
                Layout.fillHeight: true
                text: reqButtonText
                //implicitWidth: 50
                Layout.minimumWidth: 50
                Layout.preferredWidth: 130
                Layout.maximumWidth: 150
                onClicked: {
                    if(textEdit.text !== "")
                    {    
                        func(parseInt(textEdit.text))
                        textEdit.text = ""
                    }
                    else
                    {
                        console.log("text empty")
                    }
                }
            }
        }
    }

    RowLayout{
        id: rlayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        anchors.margins: 5

        height:50
        spacing: 10

        InputRect{
            id: inputRect_add
            reqText: "Добавить число:"
            reqButtonText: "Добавить"
            func: addNode
            Layout.fillHeight: true
            Layout.preferredWidth: inputRect_add.implicitWidth
        }
        Item{
            Layout.fillWidth: true
        }
        InputRect{
            id: inputRect_delete
            reqText: "Удалить элемент"
            reqButtonText: "Удалить"
            func: deleteNode
            Layout.fillHeight: true
            Layout.preferredWidth: inputRect_delete.implicitWidth
        }
        Item{
            Layout.fillWidth: true
        }
        Button{
            id: button_rotate_left
            Layout.fillHeight: true
            text: "Повернуть влево"
            onClicked: {
                painter.rotateLeft();
            }
        }        
        Item{
            Layout.fillWidth: true
        }
        Button{
            id: button_rotate_right
            Layout.fillHeight: true
            text: "Повернуть вправо"
            onClicked: {
                painter.rotateRight();
            }
        }
        Item{
            Layout.fillWidth: true
        }
        Button{
            id: button_rotate_next
            Layout.fillHeight: true
            text: "Далее"
        }
        Item{
            Layout.fillWidth: true
        }
        Button{
            id: button_reset_painter
            Layout.fillHeight: true
            text: "Сбросить"
            onClicked: {
                painter.resetPosition();
            }
        }
    }
    TreePainter{
        id: painter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: rlayout.bottom
        anchors.topMargin: 5
        WheelHandler {
            onWheel: (event) => {
                const factor = event.angleDelta.y > 0 ? 1.1 : 0.9
                let mx = event.x;
                let my = event.y;
                let worldX = (mx - painter.offsetX) / painter.zoom;
                let worldY = (my - painter.offsetY) / painter.zoom;
                painter.zoom *= factor
                painter.offsetX = mx - worldX * painter.zoom;
                painter.offsetY = my - worldY * painter.zoom;
                painter.update()
            }
        }
        DragHandler {
            target: null

            //Это смещение
            property real lastX: 0
            property real lastY: 0


            //В начале каждого нового перетаскивания обнуляем накопленное смещение.
            //Это собитие начала или окончатия перемещения 
            //Active меняется либо с true на false либо наоборот 
            //и вот когда мы проверяем только на active мы обрабатываем только начало перемещения.
            onActiveChanged: {
                if (active) {
                    lastX = 0
                    lastY = 0
                }
            }

            //Это событие изменения положения мыши в драге
            //Зачем lastX и lastY?  Смещение накопительное
            //пока мы тянем мышь сначала смещение (допустим) равно 30, потом 31
            // и нам не нужно смещать на 31, нам нужно сместить на 30, 
            //поэтому мы запоминаем каждый раз activeTranslation
            onActiveTranslationChanged: {
                painter.offsetX += activeTranslation.x - lastX
                painter.offsetY += activeTranslation.y - lastY

                lastX = activeTranslation.x
                lastY = activeTranslation.y

                painter.update()
            }
        }
    }
}