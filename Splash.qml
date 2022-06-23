import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

Window {
    id: mainWindow
    width: 600
    height: 400
    visible: false
    x: Screen.width / 2 - width / 2;
    y: Screen.height / 2 - height / 2;

    Window {
        id: splashWindow
        width: 300
        height: 200
        visible: true
        x: Screen.width / 2 - width / 2;
        y: Screen.height / 2 - height / 2;
        flags: Qt.SplashScreen;
        Rectangle {
            anchors.fill: parent
            color: "green";
        }

        ProgressBar {
            id: progressBar
            anchors.centerIn: parent
            anchors.margins: 20
            value: 0.01
        }

        Timer {
            id: timer
            interval: 50
            repeat: true
            running: true
            onTriggered: {
                progressBar.value += 0.01
                if(progressBar.value >= 1.0) {
                    timer.stop();
                    mainWindow.visible = true;
                    splashWindow.destroy();
                }
            }
        }
    }
}
