import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.3

Item {
    id: item
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
            text: qsTr("Добавить зону")
            focusPolicy: Qt.TabFocus
            spacing: 0

            onClicked: {
                onPolygonCreate = true
                item.newZone = Qt.createQmlObject('import QtLocation 5.9; MapPolygon { }', item)
            }
        }
    }



}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
