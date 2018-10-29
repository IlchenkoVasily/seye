import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.3

Item {
    id: superItem
    width: 640
    height: 480

    property bool onPolygonCreate: false
    property variant newZone

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

        // this is an Polygons view container
        MapItemView {
            model: polygonModel
            delegate: MapPolygon {
                color: Qt.rgba(64, 255, 64, 0.5)
                border.color: "red"
                border.width: 2
                path: model.path
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
                    color: Qt.rgba(255, 0, 0, 1)
                }
                zoomLevel: 16
                opacity: 1.0
                anchorPoint: Qt.point(sourceItem.width / 2, sourceItem.height / 2)
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: {
//                console.log((map.toCoordinate(Qt.point(mouse.x,mouse.y)).latitude)
//                            + ' ' + (map.toCoordinate(Qt.point(mouse.x,mouse.y)).longitude));
                //
                if (onPolygonCreate) {
                    if (mouse.button & Qt.RightButton) {
                        // cancel create polygon
                        onPolygonCreate = false;
                        newZone.destroy();
                    }

                    // add new point to new polygon
                    if (mouse.button & Qt.LeftButton) {
                        // add new point to polygon
                        var coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                        newZone.addCoordinate(coord)
                        console.log(coord + ' added')
                    }
                }
            }

            onDoubleClicked: {
                if (onPolygonCreate) {/*
                    // Изменяем то, как выглядить зона
                    // цвет
                    newZone.color = Qt.rgba(64, 255, 64, 0.5)
                    // цвет границ
                    newZone.border.color = "green"
                    // толщина грацниц
                    newZone.border.width = 2*/

                    // добавляем к карте эту зону
//                    map.addMapItem(newZone)

//                    var poly = QtPositioning.polygon(newZone)
                    console.log(newZone)

                    polygonModel.addPolygon(newZone)
                    // зону больше не создаём
                    onPolygonCreate = false
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
//                superItem.newZone = Qt.createQmlObject('import QtLocation 5.9; MapPolygon { }', superItem)
                newZone = QtPositioning.polygon()
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
