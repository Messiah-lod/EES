//import QtQuick 2.0
//import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts 6.2
import QtQuick 2.4
import QtQuick.Window 2.2

import FileJob 1.0

Rectangle  {
    id: rootRect
    anchors.fill: parent
    visible: true
    color: "#ffffff"
    border.color: "#00a2da"
    border.width: 4

    FileJob {
        id: myFile
        source: ":/text/README.md"
        //      onError: console.log(msg)
    }

    Component.onCompleted: {
        // console.log( "WRITE "+ myFile.write("TEST"));
        textAbout.text =  myFile.read();
    }


    GridLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        rows: 3
        columns: 3

        Rectangle {
            id: text1
            width: 271
            height: 206

            border.color: "#00a2da"
            border.width: 2
            radius: 3

            Layout.row: 0
            Layout.column: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.rowSpan: 2

            ScrollView {
                id: scroll_view
                anchors.fill: parent
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.bottomMargin: 10
                anchors.topMargin: 10
                TextEdit{
                    id: textAbout
                    readOnly: true

                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    font.family: "Times New Roman"
                    color: "#030303"
                }
            }
        }

        Button {
            id: button

            contentItem: Text {
                text: qsTr("OK")
                font.family: "Times New Roman"
                font.pointSize: 16
                opacity: enabled ? 1.0 : 0.3
                color: "#000000"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }


            width: 150
            height: 40

            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.maximumHeight: 40
            Layout.maximumWidth: 300

            Layout.row: 2
            Layout.column: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 3
            Layout.rowSpan: 1

            onClicked: {
           //     rootRect.visible = false
           //     console.log("Click on button")
                Qt.quit()
            }
        }

        Image {
            id: image
            source: "qrc:/pic/icon.ico"
            Layout.margins: 5
            fillMode: Image.PreserveAspectFit

            Layout.minimumHeight: 50
            Layout.minimumWidth: 50
            Layout.maximumHeight: 100
            Layout.maximumWidth: 100
            Layout.row: 0
            Layout.column: 0
            Layout.fillHeight: true
            Layout.fillWidth: true

        }

    }
}
