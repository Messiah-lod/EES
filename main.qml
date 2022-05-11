import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts 6.2


//import QtQuick 2.6
//import QtQuick.Window 2.2
//import QtQuick.Controls 1.4
//import QtGraphicalEffects 1.0
//import QtQuick.Window 2.2
//import QtQuick.Layouts 1.3
//import QtQuick.Controls.Styles 1.4


import ModelFillingDB 1.0

Window {
    width: 1000
    height: 800
    visible: true
    color: "#efefef"
    minimumWidth: 700
    minimumHeight: 500
    title: qsTr("EESv2")

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        rows: 3
        columns: 4

        Rectangle {
            id: rectangle
            color: "#ffffff"
            border.color: "#00b3ff"
            border.width: 3
            Layout.maximumHeight: 40
            Layout.maximumWidth: 65535
            Layout.preferredHeight: -1
            Layout.preferredWidth: -1

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 3
            Layout.rowSpan: 1
            Layout.row: 0
            Layout.column: 0

            TextInput {
                id: connectString
                text: qsTr("Path to BD")
                anchors.fill: parent
                font.letterSpacing: 1
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                selectionColor: "#00b3ff"
                leftPadding: 10
                selectByMouse: true
                overwriteMode: true
                cursorVisible: false
                mouseSelectionMode: TextInput.SelectWords
                font.wordSpacing: 0
                font.weight: Font.Normal
                scale: 1
                font.family: "Times New Roman"
            }
        }

        Button {
            id: btnConnect
            width: 150
            height: 40
            text: qsTr("Connect BD")
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.maximumHeight: 40
            font.pointSize: 14
            font.family: "Times New Roman"
            display: AbstractButton.TextOnly

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.row: 0
            Layout.column: 3
        }

        Button {
            id: btnLoadFile
            width: 150
            height: 40
            text: qsTr("Load file")
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.maximumHeight: 40
            font.pointSize: 14
            font.family: "Times New Roman"
            display: AbstractButton.TextOnly

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.row: 1
            Layout.column: 0
        }

        Button {
            id: btnStart
            width: 150
            height: 40
            text: qsTr("Start")
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.maximumHeight: 40
            font.pointSize: 14
            font.family: "Times New Roman"
            display: AbstractButton.TextOnly

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.row: 1
            Layout.column: 1
        }

        Rectangle {
            id: fieldSplit
            color: "#ffffff"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.row: 2
            Layout.column: 0

            SplitView {
                id: splitVertical
                anchors.fill: parent
                orientation: Qt.Vertical
                height: parent.height

                handle: Rectangle {
                    implicitWidth: 4
                    implicitHeight: 4
                    color: SplitHandle.pressed ? "#efefef"
                                               : (SplitHandle.hovered ? Qt.lighter("#efefef", 1.1) : "#efefef")
                }

                GridLayout {
                    height: parent.height*2/3
                    SplitView.preferredHeight: 500
                    SplitView.minimumHeight: bar.height
                    Rectangle {
                        id: fieldView
                        color: "#e5e5e5"
                        anchors.fill: parent

                        TabBar {
                            id: bar
                            width: parent.width
                            // anchors.bottom: parent.bottom
                            TabButton {
                                text: qsTr("Filling")
                                font.family: "Times New Roman"
                                font.pointSize: 14
                            }
                            TabButton {
                                text: qsTr("Link")
                                font.family: "Times New Roman"
                                font.pointSize: 14
                            }
                        }

                        StackLayout {
                            currentIndex: bar.currentIndex
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom

                            Item {
                                id: fillingTab
                                clip: true
                                GridView{
                                    anchors.fill: parent
                                    anchors.centerIn: parent
                                    focus: true
                                    anchors.topMargin: bar.height
                                    Rectangle {
                                        id: fieldFillingTab
                                        color: "#e5e5e5"
                                        border.color: "#00b3ff"
                                        border.width: 3
                                        anchors.fill: parent
                                        TableView{
                                            id: tableView
                                            model: model
                                            HorizontalHeaderView {
                                                id: horizontalHeader
                                                syncView: tableView
                                                anchors.left: tableView.left
                                            }
                                        }
                                    }

                                }
                            }

                        }
                        Item {
                            id: linkTab
                            clip: true
                            GridView{
                                anchors.fill: parent
                                anchors.centerIn: parent
                                focus: true
                                anchors.topMargin: bar.height
                                Rectangle {
                                    id: fieldLinkTab
                                    color: "#e5e5e5"
                                    border.color: "#00b3ff"
                                    border.width: 3
                                    anchors.fill: parent
                                }
                            }
                        }
                    }
                }
            }
            GridLayout {
                //           SplitView.minimumHeight: 50
                height: parent.height/3
                Rectangle {
                    id: fieldLog
                    color: "#e5e5e5"
                    border.color: "#00b3ff"
                    border.width: 3
                    anchors.fill: parent
                }
            }
        }
    }
}

