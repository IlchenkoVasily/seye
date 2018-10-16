import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.3

Item {
    //id: item
    property bool onPolygonCreate: false

    //
    Plugin {
        id: mapPlugin
        name: "osm"
    }


    Map {
//        anchors.topMargin: 40
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
                    // cancel
                    if (mouse.button & Qt.RightButton) {
                        // cancel create polygon
                        onPolygonCreate = false
                    }

                    // add new point to new polygon
                    if (mouse.button & Qt.LeftButton) {
                        // add new point to polygon
                    }
                }
            }

            onDoubleClicked: {
                if (onPolygonCreate) {
                    // end create polygon
                    // add to polygongroup our new zone
                    // check for drawing our group
                }
            }
        }
    }

//    ToolBar {
//        id: toolBar
//        x: 0
//        y: 0
//        width: 640
//        height: 40
//        anchors.top: parent.top
//        anchors.right: parent.right
//        anchors.left: parent.left

//        Button {
//            id: button
//            x: 0
//            y: 0
//            text: qsTr("Добавить полигон")
//            focusPolicy: Qt.TabFocus
//            spacing: 0

//            onClicked: {
//                onPolygonCreate = true
//            }
//        }
//    }



}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
