import QtQuick 2.6
import QtQuick.Layouts 1.3

import "qrc:/Controls" as Controls
import "../Map"

Controls.Pane {
    id: root

    property alias vehicles: vehiclesBox.model
    property alias selectedVehicle: vehiclesBox.currentIndex

    signal selectVehicle(int index)

    ColumnLayout {
        id: column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Layout.margins: palette.margins
        spacing: palette.spacing
        width: height * 0.43

        RowLayout {

            Controls.Label {
                text: qsTr("Vehicle")
                font.pixelSize: palette.fontPixelSize * 0.6
                font.bold: true
            }

            Controls.ComboBox {
                id: vehiclesBox
                onCurrentIndexChanged: selectVehicle(currentIndex)
                Layout.fillWidth: true
            }
        }

        DashboardView {
            objectName: "dashboard"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    MapView {
        id: map
        objectName: "map"
        anchors.left: column.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: palette.margins
    }
}
