import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.3

Item {
    id: superItem
    property bool onPolygonCreate: false
    property MapPolygon newZone

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

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: {
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
                        newZone.addCoordinate(map.toCoordinate(Qt.point(mouse.x, mouse.y)))
                    }
                }
            }

            onDoubleClicked: {
                if (onPolygonCreate) {
                    // end create polygon
                    // add to polygongroup our new zone
                    // check for drawing our group
                    map.addMapItem(newZone)
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
                superItem.newZone = Qt.createQmlObject('import QtLocation 5.9; MapPolygon { }', superItem)
            }

            Image {
                id: image
                x: 5
                y: 5
                width: 30
                height: 30
                sourceSize.width: 0
                source: "icons/createZone1.png"
            }
        }
    }



}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:7;anchors_height:30;anchors_width:30;anchors_x:0;anchors_y:0}
}
 ##^##*/
