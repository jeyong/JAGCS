import QtQuick 2.6
import QtQuick.Layouts 1.3
import JAGCS 1.0

import "qrc:/Controls" as Controls

Controls.Popup {
    id: info

    closePolicy: Controls.Popup.CloseOnEscape | Controls.Popup.CloseOnPressOutsideParent

    ColumnLayout {
        spacing: sizings.spacing

        Controls.Label {
            text: qsTr("RSSI:") + " " + rssi.toFixed(1) + " " + qsTr("dBm")
            font.pixelSize: sizings.fontSize * 0.75
            font.bold: true
        }

        Controls.Label {
            text: qsTr("Rem. RSSI:") + " " + remoteRssi.toFixed(1) + " " + qsTr("dBm")
            font.pixelSize: sizings.fontSize * 0.75
            font.bold: true
        }
    }
}
