import QtQuick 2.0
import QtQml.Models 2.2
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

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

    // Данный компонент обеспечивает связь для модели
    // объектов и карты, используемая для центрирования
    // карты на выбранном объекте.
    Connections {
        target: objectModel

        onObjectCentering: {
            map.center = coordinate
            map.zoomLevel = 18
        }
    }

    Connections {
        target: polygonModel

        onPolygonCentering: {
            map.center = coordinate
            map.zoomLevel = 16
        }
    }

    //
    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
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
                color: model.mapColor
                border.color: model.mapBorderColor
                border.width: 2

                MouseArea {
                    anchors.fill: parent
                    enabled: model.id == 0 ? false : true
                    onClicked: {
                        // При клике без доп. клавиш для единичного выделения
                        if ((mouse.button == Qt.LeftButton) &&
                            (mouse.modifiers == Qt.NoModifier))
                        {
                            polygonSelection.select(polygonModel.index(index, 0),
                                  ItemSelectionModel.ClearAndSelect |
                                  ItemSelectionModel.Rows)
                        }

                        // При клике с ктрл`ом для множественно выделения
                        if ((mouse.button == Qt.LeftButton) &&
                            (mouse.modifiers & Qt.ControlModifier))
                        {
                            polygonSelection.select(polygonModel.index(index, 0),
                                  ItemSelectionModel.Select |
                                  ItemSelectionModel.Rows |
                                  ItemSelectionModel.Toggle)
                        }
                    }
                }
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
                        if (map.zoomLevel > 17)
                            return Qt.rgba(0, 0, 0, 0)

                        switch(model.state) {
                        case States.Allowed:
                            return "green"
                        case States.Intruder:
                            return "red"
                        case States.OutOfAttention:
                            return "grey"
                        case States.Destroyed:
                            return "blue"
                        default:
                            return "white"
                        }
                    }

                    // Здесь задаётся наша иконка.
                    Image {
                        id: objectIcon
                        source: {
                            switch (model.role) {
                            case Roles.Worker:
                                return "qrc:/icons/worker.svg"
                            case Roles.Pilot:
                                return "qrc:/icons/pilot.svg"
                            case Roles.Car:
                                return "qrc:/icons/car.svg"
                            case Roles.Security:
                                return "qrc:/icons/security.svg"
                            case Roles.FuelCar:
                                return "qrc:/icons/fuel.svg"
                            }
                        }
                        anchors.fill: parent
                        antialiasing: true
                        visible: false
                    } // end Image

                    // Здесь задаётся цвет нашей иконки.
                    ColorOverlay {
                        source: objectIcon
                        anchors.fill: objectIcon
                        color: {
                            switch(model.state) {
                            case States.Allowed:
                                return "green"
                            case States.Intruder:
                                return "red"
                            case States.OutOfAttention:
                                return "grey"
                            case States.Destroyed:
                                return "blue"
                            default:
                                return "white"
                            }
                        }
                        antialiasing: true
                    } // end ColorOverlay
                } // end sourceItem: Rectangle
                zoomLevel: 16
                opacity: 1.0
                anchorPoint: Qt.point(sourceItem.width / 2, sourceItem.height / 2)
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            cursorShape: polygonModel.onCreate ? Qt.CrossCursor : Qt.ArrowCursor

            onClicked: {
                // Чистим какие-либо выделения
                polygonSelection.clearSelection()

                // Если создаётеся полигнон
                if (polygonModel.onCreate) {
                    // Правая кнопка - отменение создания полигона
                    if (mouse.button & Qt.RightButton) {
//                        polygonModel.onCreate = false
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
                if (polygonModel.onCreate) {
//                    onPolygonCreate = false
                    firstAdded = false
                    polygonModel.endCreatePolygon()
                    // чистим line
                    for (var i = line.pathLength() - 1; i >= 0; i--) {
                        line.removeCoordinate(i)
                    }
                }
            }

            onPositionChanged: {
                if (polygonModel.onCreate) {
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
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:7;anchors_height:30;anchors_width:30;anchors_x:0;anchors_y:0}
}
 ##^##*/
