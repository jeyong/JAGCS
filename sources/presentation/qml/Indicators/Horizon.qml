import QtQuick 2.6

import "qrc:/JS/helper.js" as Helper

Item {
    id: root

    property real pitch: 0.0
    property real roll: 0.0
    property real minPitch: -25.0
    property real maxPitch: 25.0
    property real effectiveHeight: height

    Rectangle {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: effectiveHeight / 2 - Helper.mapToRange(
                                          -pitch, minPitch, maxPitch, effectiveHeight)
        width: 10000
        height: 10000
        rotation: -roll
        color: palette.textColor

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2 - 0.5
            color: enabled ? palette.skyColor : "#c6c9d1" // TODO: palette
            Behavior on color { ColorAnimation { duration: 200 } }
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2 - 0.5
            color: enabled ? palette.groundColor : "#798f99" // TODO: palette
            Behavior on color { ColorAnimation { duration: 200 } }
        }
    }
}
