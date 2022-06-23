import QtQuick.Controls
import QtQuick.Layouts
import QtCharts 6.2
import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import SettingsApp 1.0

Rectangle  {
    id: rootRect
    width: 250
    height: 100
    anchors.fill: parent
    visible: true
    color: "#ffffff"
    border.color: "#00a2da"
    border.width: 4


    signal qmlSettingsChangeLanguage()
    signal qmlSettingsChangeTheme()


    CustomizingStyle{
        id: customizingStyle
    }

    SettingsApp{
        id: settings
    }

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        rows: 2
        columns: 2

        Text {
            id: labelDarkTheme

            Layout.row: 0
            Layout.column: 0
            Layout.fillHeight: true
            Layout.fillWidth: true

            text: qsTr("Dark theme")
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            font.family: "Times New Roman"
        }


        Switch {
            id: switchDark

            checked: settings.getDarkTheme()
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

            Layout.row: 0
            Layout.column: 1
            Layout.fillHeight: true
            Layout.fillWidth: false

            onClicked: {
            //    console.log("Click on switchDark " + switchDark.checked)
                settings.darkTheme = switchDark.checked
                rootRect.qmlSettingsChangeTheme()

                if(switchDark.checked) {
             //       Universal.theme: Universal.Dark
                }
                else {}
            }

            indicator: Rectangle {
                implicitWidth: 48
                implicitHeight: 24
                x: switchDark.leftPadding
                y: parent.height / 2 - height / 2
                radius: 13
                color: switchDark.checked ? "#00a2da" : "#ffffff"
                border.color: switchDark.checked ? "#00a2da" : "#00a2da"

                Rectangle {
                    x: switchDark.checked ? parent.width - width : 0
                    width: 24
                    height: 24
                    radius: 13
                    color: switchDark.down ? "#cccccc" : "#ffffff"
                    border.color: switchDark.checked ? (switchDark.down ? "#17a81a" : "#21be2b") : "#999999"
                }
            }
        }

        Text {
            id: labelLanguage

            Layout.row: 1
            Layout.column: 0
            Layout.fillHeight: true
            Layout.fillWidth: true

            text: qsTr("Language")
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            font.family: "times New Roman"
        }

        ComboBox {
            id: comboBoxLanguage
            objectName: "comboBoxLanguage"

            font.family: "Times New Roman"
       //     font.color: "#000000"
            currentIndex: settings.getLanguage()

            Layout.row: 1
            Layout.column: 1
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: [ "English", "Русский"]

            onActivated: {
                console.log("Click on ComboBox: " + comboBoxLanguage.currentIndex)
                settings.language = comboBoxLanguage.currentIndex
                rootRect.qmlSettingsChangeLanguage()
            }
        }


    }
}


