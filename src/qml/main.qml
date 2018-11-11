import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.3

import seye 1.0     // Модуль QML не найден (seye).

Item {
    id: superItem

    property bool onPolygonCreate: false
    property bool firstAdded: false

    // Данное свойство - группа элементов карты.
    // Сюда добавляются "точки" - MapQuickItem, в котором
    // содержится округлый Rectangle.
    property MapItemGroup dots: MapItemGroup { }
    // Линия, отображающая границы полигона.
    property MapPolyline line: MapPolyline {
        line.width: 3
        line.color: "red"
    }

    //
    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.topMargin: 40
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(56.388, 85.210) // Bogachevo
        zoomLevel: 16
        Component.onCompleted: { map.addMapItem(line) }

        // this is an Polygons view container
        MapItemView {
            model: polygonModel
            delegate: polygonDelegate
        }

        Component {
            id: polygonDelegate

            MapPolygon {
                path: model.path
                color: Qt.rgba(64, 255, 64, 0.5)
                border.color: "green"
                border.width: 2
            }
        }

        // this is an Objects view container
        MapItemView {
            model: objectModel
            delegate: objectDelegate
        }

        // this is an Objects delegate
        Component {
            id: objectDelegate

            MapQuickItem {
                coordinate: model.coordinate
                sourceItem: Rectangle {
                    width: 10
                    height: 10
                    radius: 15
                    color: {
                        if (model.state === States.Allowed) {
                            return "green"
                        }
                        if (model.state === States.Intruder) {
                            return "red"
                        }
                        return "blue"
                    }
                }
                zoomLevel: 16
                opacity: 1.0
                anchorPoint: Qt.point(sourceItem.width / 2, sourceItem.height / 2)
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            cursorShape: onPolygonCreate ? Qt.CrossCursor : Qt.ArrowCursor

            onClicked: {
                // Если создаётеся полигнон
                if (onPolygonCreate) {
                    // Правая кнопка - отменение создания полигона
                    if (mouse.button & Qt.RightButton) {
                        onPolygonCreate = false
                        firstAdded = false
                        polygonModel.cancelCreatePolygon()
                        ///------------чистим dots

                        // чистим line
                        for (var i = line.pathLength() - 1; i >= 0; i--) {
                            line.removeCoordinate(i)
                        }
                    }

                    // add new point to new polygon
                    if (mouse.button & Qt.LeftButton) {
                        var coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                        polygonModel.addCoordinate(coord)
                        ///--------------добавляем в dots новый MapQuickItem

                        // добавляем в line новую координату
                        if (!firstAdded) {
                            line.addCoordinate(coord)
                            // Если была добавлена только первая точка,
                            // тогда добавляем временную точку, для
                            // слежки за курсором в момент добавления
                            line.addCoordinate(coord)
                            firstAdded = true
                            return
                        }

                        line.replaceCoordinate(line.pathLength() - 1, coord)
                        line.addCoordinate(coord);
                    }
                }
            }

            onDoubleClicked: {
                if (onPolygonCreate) {
                    onPolygonCreate = false
                    firstAdded = false
                    polygonModel.endCreatePolygon()
                    // чистим line
                    for (var i = line.pathLength() - 1; i >= 0; i--) {
                        line.removeCoordinate(i)
                    }
                }
            }

            onPositionChanged: {
                if (onPolygonCreate) {
                    // Если нету точек, то и не откуда следить
                    if (!firstAdded) {
                        return;
                    }

                    // подмениваем последнюю точку из полигона
                    line.replaceCoordinate(line.pathLength() - 1,
                                           map.toCoordinate(Qt.point(mouse.x, mouse.y)))
                }
            }
        }
    }

    ToolBar {
        id: toolBar
        x: 0
        y: 0
        width: 640
        height: 40
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

        Button {
            id: button
            x: 0
            y: 0
            width: 40
            height: 40
            text: qsTr("Добавить зону")
            rightPadding: 6
            leftPadding: 6
            padding: 5
            font.capitalization: Font.Capitalize
            display: AbstractButton.IconOnly

            ToolTip.visible: down
            ToolTip.delay: Qt.styleHints.useHoverEffects
            ToolTip.text: "Клик - добавить точку \nДвойной клик - закончить зону \nПравый клик - отменить"

            focusPolicy: Qt.TabFocus
            spacing: 0

            onClicked: {
                onPolygonCreate = true
                polygonModel.beginCreatePolygon()
            }

            Image {
                id: image
                x: 5
                y: 5
                width: 30
                height: 30
                sourceSize.width: 0
                source: "../icons/createZone1.png"
            }
        }
    }



}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:7;anchors_height:30;anchors_width:30;anchors_x:0;anchors_y:0}
}
 ##^##*/
