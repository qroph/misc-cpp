import QtQuick 2.0

// Shows three gears as an example

Rectangle {
    width: 350
    height: 200

    Gear {
        id: g1;
        gearColor: "red";
        x: 50;
        y: 100;
        toothCount: 8;
        angle: 0;
        velocity: 3;
    }

    Gear {
        id: g2;
        gearColor: "green";
        x: g1.x + g1.gearRadius + gearRadius;
        y: 100;
        toothCount: 16;
        angle: 11.25;
        velocity: -6;
    }

    Gear {
        id: g3;
        gearColor: "blue";
        x: g2.x + g2.gearRadius + gearRadius;
        y: 100;
        toothCount: 24;
        angle: 0;
        velocity: 9;
    }
}
