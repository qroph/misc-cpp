import QtQuick 2.0

Rectangle {
    property int toothCount: 8
    property string gearColor: "black"
    property real velocity: 0
    property alias angle: rotation.angle
    readonly property real gearRadius: toothCount * 3

    // Private properties
    QtObject {
        id: m
        readonly property real space: 1
        readonly property real toothHeight: 10
        readonly property real toothWidth:
            Math.PI * gearRadius / toothCount - space
    }

    width: 2 * gearRadius
    height: 2 * gearRadius
    color: "#00000000" // Do not show the rectangle

    transform: [
        Translate {
            x: -gearRadius
            y: -gearRadius
        },
        Rotation {
            id: rotation
            angle: 0
        }
    ]

    // Draw a circle
    Rectangle {
        width: parent.width - m.toothHeight
        height: parent.height - m.toothHeight
        x: m.toothHeight / 2
        y: m.toothHeight / 2
        radius: height / 2
        color: gearColor
    }

    // Draw teeth
    Repeater {
        model: toothCount
        Rectangle {
            y: -m.toothHeight / 2 + m.space
            x: gearRadius - m.toothWidth / 2
            height: m.toothHeight + 1
            width: m.toothWidth
            color: gearColor
            transform: Rotation {
                origin {
                  x: width / 2
                  y: m.toothHeight / 2 - m.space + gearRadius
                }
                angle: index * 360 / toothCount
              }
        }
    }

    // Animate the gear
    RotationAnimation on rotation {
        loops: Animation.Infinite
        from: velocity > 0 ? 0 : 360
        to: velocity > 0 ? 360 : 0
        duration: Math.abs(velocity) * 1000
    }
}
