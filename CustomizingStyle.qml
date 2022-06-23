import QtQuick
import QtQuick.Controls


//T.Button {
//    // ...

//    background: Rectangle {
//        // ...
//    }

//    contentItem: Text {
//        // ...
//    }

//    // ...
//}

Item{
}
//Customizing ApplicationWindow
//    ApplicationWindow {
//        visible: true

//        background: Rectangle {
//            gradient: Gradient {
//                GradientStop { position: 0; color: "#ffffff" }
//                GradientStop { position: 1; color: "#c1bbf9" }
//            }
//        }

//        overlay.modal: Rectangle {
//            color: "#8f28282a"
//        }

//        overlay.modeless: Rectangle {
//            color: "#2f28282a"
//        }
//    }




//Customizing BusyIndicator
//    BusyIndicator {
//        id: controlBusyIndicator

//        contentItem: Item {
//            implicitWidth: 64
//            implicitHeight: 64

//            Item {
//                id: item
//                x: parent.width / 2 - 32
//                y: parent.height / 2 - 32
//                width: 64
//                height: 64
//                opacity: control.running ? 1 : 0

//                Behavior on opacity {
//                    OpacityAnimator {
//                        duration: 250
//                    }
//                }

//                RotationAnimator {
//                    target: item
//                    running: control.visible && control.running
//                    from: 0
//                    to: 360
//                    loops: Animation.Infinite
//                    duration: 1250
//                }

//                Repeater {
//                    id: repeater
//                    model: 6

//                    Rectangle {
//                        id: delegate
//                        x: item.width / 2 - width / 2
//                        y: item.height / 2 - height / 2
//                        implicitWidth: 10
//                        implicitHeight: 10
//                        radius: 5
//                        color: "#21be2b"

//                        required property int index

//                        transform: [
//                            Translate {
//                                y: -Math.min(item.width, item.height) * 0.5 + 5
//                            },
//                            Rotation {
//                                angle: delegate.index / repeater.count * 360
//                                origin.x: 5
//                                origin.y: 5
//                            }
//                        ]
//                    }
//                }
//            }
//        }
//    }

//    //Customizing Button
//    Button {
//        id: controlButton
//        text: qsTr("Button")

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            elide: Text.ElideRight
//        }

//        background: Rectangle {
//            implicitWidth: 100
//            implicitHeight: 40
//            opacity: enabled ? 1 : 0.3
//            border.color: control.down ? "#17a81a" : "#21be2b"
//            border.width: 1
//            radius: 2
//        }
//    }

//    //Customizing CheckBox
//    CheckBox {
//        id: controlCheckBox
//        text: qsTr("CheckBox")
//        checked: true

//        indicator: Rectangle {
//            implicitWidth: 26
//            implicitHeight: 26
//            x: control.leftPadding
//            y: parent.height / 2 - height / 2
//            radius: 3
//            border.color: control.down ? "#17a81a" : "#21be2b"

//            Rectangle {
//                width: 14
//                height: 14
//                x: 6
//                y: 6
//                radius: 2
//                color: control.down ? "#17a81a" : "#21be2b"
//                visible: control.checked
//            }
//        }

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            verticalAlignment: Text.AlignVCenter
//            leftPadding: control.indicator.width + control.spacing
//        }
//    }

//    //Customizing CheckDelegate
//    CheckDelegate {
//        id: controlCheckDelegate
//        text: qsTr("CheckDelegate")
//        checked: true

//        contentItem: Text {
//            rightPadding: control.indicator.width + control.spacing
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            elide: Text.ElideRight
//            verticalAlignment: Text.AlignVCenter
//        }

//        indicator: Rectangle {
//            implicitWidth: 26
//            implicitHeight: 26
//            x: control.width - width - control.rightPadding
//            y: control.topPadding + control.availableHeight / 2 - height / 2
//            radius: 3
//            color: "transparent"
//            border.color: control.down ? "#17a81a" : "#21be2b"

//            Rectangle {
//                width: 14
//                height: 14
//                x: 6
//                y: 6
//                radius: 2
//                color: control.down ? "#17a81a" : "#21be2b"
//                visible: control.checked
//            }
//        }

//        background: Rectangle {
//            implicitWidth: 100
//            implicitHeight: 40
//            visible: control.down || control.highlighted
//            color: control.down ? "#bdbebf" : "#eeeeee"
//        }
//    }

//    //Customizing ComboBox
//    ComboBox {
//        id: controlComboBox
//    //    model: ["First", "Second", "Third"]

//        delegate: ItemDelegate {
//            width: control.width
//            contentItem: Text {
//                text: modelData
//                color: "#21be2b"
//                font: control.font
//                elide: Text.ElideRight
//                verticalAlignment: Text.AlignVCenter
//            }
//            highlighted: control.highlightedIndex === index

//            required property int index
//            required property var modelData
//        }

//        indicator: Canvas {
//            id: canvasComboBox
//            x: control.width - width - control.rightPadding
//            y: control.topPadding + (control.availableHeight - height) / 2
//            width: 12
//            height: 8
//            contextType: "2d"

//            Connections {
//                target: control
//                function onPressedChanged() { canvasComboBox.requestPaint(); }
//            }

//            onPaint: {
//                context.reset();
//                context.moveTo(0, 0);
//                context.lineTo(width, 0);
//                context.lineTo(width / 2, height);
//                context.closePath();
//                context.fillStyle = control.pressed ? "#17a81a" : "#21be2b";
//                context.fill();
//            }
//        }

//        contentItem: Text {
//            leftPadding: 0
//            rightPadding: control.indicator.width + control.spacing

//            text: control.displayText
//            font: control.font
//            color: control.pressed ? "#17a81a" : "#21be2b"
//            verticalAlignment: Text.AlignVCenter
//            elide: Text.ElideRight
//        }

//        background: Rectangle {
//            implicitWidth: 120
//            implicitHeight: 40
//            border.color: control.pressed ? "#17a81a" : "#21be2b"
//            border.width: control.visualFocus ? 2 : 1
//            radius: 2
//        }

//        popup: Popup {
//            y: control.height - 1
//            width: control.width
//            implicitHeight: contentItem.implicitHeight
//            padding: 1

//            contentItem: ListView {
//                clip: true
//                implicitHeight: contentHeight
//                model: control.popup.visible ? control.delegateModel : null
//                currentIndex: control.highlightedIndex

//                ScrollIndicator.vertical: ScrollIndicator { }
//            }

//            background: Rectangle {
//                border.color: "#21be2b"
//                radius: 2
//            }
//        }
//    }

//    //Customizing DelayButton
//    DelayButton {
//        id: controlDelayButton
//        checked: true
//        text: qsTr("Delay\nButton")

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: "white"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            elide: Text.ElideRight
//        }

//        background: Rectangle {
//            implicitWidth: 100
//            implicitHeight: 100
//            opacity: enabled ? 1 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            radius: size / 2

//            readonly property real size: Math.min(control.width, control.height)
//            width: size
//            height: size
//            anchors.centerIn: parent

//            Canvas {
//                id: canvasDelayButton
//                anchors.fill: parent

//                Connections {
//                    target: control
//                    function onProgressChanged() { canvasDelayButton.requestPaint(); }
//                }

//                onPaint: {
//                    var ctx = getContext("2d")
//                    ctx.clearRect(0, 0, width, height)
//                    ctx.strokeStyle = "white"
//                    ctx.lineWidth = parent.size / 20
//                    ctx.beginPath()
//                    var startAngle = Math.PI / 5 * 3
//                    var endAngle = startAngle + control.progress * Math.PI / 5 * 9
//                    ctx.arc(width / 2, height / 2, width / 2 - ctx.lineWidth / 2 - 2, startAngle, endAngle)
//                    ctx.stroke()
//                }
//            }
//        }
//    }

//    //Customizing Dial
//    Dial {
//        id: controlDial
//        background: Rectangle {
//            x: control.width / 2 - width / 2
//            y: control.height / 2 - height / 2
//            width: Math.max(64, Math.min(control.width, control.height))
//            height: width
//            color: "transparent"
//            radius: width / 2
//            border.color: control.pressed ? "#17a81a" : "#21be2b"
//            opacity: control.enabled ? 1 : 0.3
//        }

//        handle: Rectangle {
//            id: handleItem
//            x: control.background.x + control.background.width / 2 - width / 2
//            y: control.background.y + control.background.height / 2 - height / 2
//            width: 16
//            height: 16
//            color: control.pressed ? "#17a81a" : "#21be2b"
//            radius: 8
//            antialiasing: true
//            opacity: control.enabled ? 1 : 0.3
//            transform: [
//                Translate {
//                    y: -Math.min(control.background.width, control.background.height) * 0.4 + handleItem.height / 2
//                },
//                Rotation {
//                    angle: control.angle
//                    origin.x: handleItem.width / 2
//                    origin.y: handleItem.height / 2
//                }
//            ]
//        }
//    }

//    //Customizing Drawer
//    background: Rectangle {
//        Rectangle {
//            x: parent.width - 1
//            width: 1
//            height: parent.height
//            color: "#21be2b"
//        }
//    }


//    //Customizing Frame
//    Frame {
//        background: Rectangle {
//            color: "transparent"
//            border.color: "#21be2b"
//            radius: 2
//        }

//        Label {
//            text: qsTr("Content goes here!")
//        }
//    }

//    //Customizing GroupBox
//    GroupBox {
//        id: controlGroupBox
//        title: qsTr("GroupBox")

//        background: Rectangle {
//            y: control.topPadding - control.bottomPadding
//            width: parent.width
//            height: parent.height - control.topPadding + control.bottomPadding
//            color: "transparent"
//            border.color: "#21be2b"
//            radius: 2
//        }

//        label: Label {
//            x: control.leftPadding
//            width: control.availableWidth
//            text: control.title
//            color: "#21be2b"
//            elide: Text.ElideRight
//        }

//        Label {
//            text: qsTr("Content goes here!")
//        }
//    }

//    //Customizing ItemDelegate
//    ItemDelegate {
//        id: controlItemDelegate
//        text: qsTr("ItemDelegate")

//        contentItem: Text {
//            rightPadding: control.spacing
//            text: control.text
//            font: control.font
//            color: control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
//            elide: Text.ElideRight
//            verticalAlignment: Text.AlignVCenter
//        }

//        background: Rectangle {
//            implicitWidth: 100
//            implicitHeight: 40
//            opacity: enabled ? 1 : 0.3
//            color: control.down ? "#dddedf" : "#eeeeee"

//            Rectangle {
//                width: parent.width
//                height: 1
//                color: control.down ? "#17a81a" : "#21be2b"
//                anchors.bottom: parent.bottom
//            }
//        }
//    }

//    //Customizing Label
//    Label {
//        text: qsTr("Label")
//        color: "#21be2b"
//    }

//    //Customizing Menu
//    Menu {
//        id: menu

//        Action { text: qsTr("Tool Bar"); checkable: true }
//        Action { text: qsTr("Side Bar"); checkable: true; checked: true }
//        Action { text: qsTr("Status Bar"); checkable: true; checked: true }

//        MenuSeparator {
//            contentItem: Rectangle {
//                implicitWidth: 200
//                implicitHeight: 1
//                color: "#21be2b"
//            }
//        }

//        Menu {
//            title: qsTr("Advanced")
//            // ...
//        }

//        topPadding: 2
//        bottomPadding: 2

//        delegate: MenuItem {
//            id: menuItem
//            implicitWidth: 200
//            implicitHeight: 40

//            arrow: Canvas {
//                x: parent.width - width
//                implicitWidth: 40
//                implicitHeight: 40
//                visible: menuItem.subMenu
//                onPaint: {
//                    var ctx = getContext("2d")
//                    ctx.fillStyle = menuItem.highlighted ? "#ffffff" : "#21be2b"
//                    ctx.moveTo(15, 15)
//                    ctx.lineTo(width - 15, height / 2)
//                    ctx.lineTo(15, height - 15)
//                    ctx.closePath()
//                    ctx.fill()
//                }
//            }

//            indicator: Item {
//                implicitWidth: 40
//                implicitHeight: 40
//                Rectangle {
//                    width: 26
//                    height: 26
//                    anchors.centerIn: parent
//                    visible: menuItem.checkable
//                    border.color: "#21be2b"
//                    radius: 3
//                    Rectangle {
//                        width: 14
//                        height: 14
//                        anchors.centerIn: parent
//                        visible: menuItem.checked
//                        color: "#21be2b"
//                        radius: 2
//                    }
//                }
//            }

//            contentItem: Text {
//                leftPadding: menuItem.indicator.width
//                rightPadding: menuItem.arrow.width
//                text: menuItem.text
//                font: menuItem.font
//                opacity: enabled ? 1.0 : 0.3
//                color: menuItem.highlighted ? "#ffffff" : "#21be2b"
//                horizontalAlignment: Text.AlignLeft
//                verticalAlignment: Text.AlignVCenter
//                elide: Text.ElideRight
//            }

//            background: Rectangle {
//                implicitWidth: 200
//                implicitHeight: 40
//                opacity: enabled ? 1 : 0.3
//                color: menuItem.highlighted ? "#21be2b" : "transparent"
//            }
//        }

//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 40
//            color: "#ffffff"
//            border.color: "#21be2b"
//            radius: 2
//        }
//    }

//    //Customizing MenuBar
//    MenuBar {
//        id: menuBar

//        Menu { title: qsTr("File") }
//        Menu { title: qsTr("Edit") }
//        Menu { title: qsTr("View") }
//        Menu { title: qsTr("Help") }

//        delegate: MenuBarItem {
//            id: menuBarItem

//            contentItem: Text {
//                text: menuBarItem.text
//                font: menuBarItem.font
//                opacity: enabled ? 1.0 : 0.3
//                color: menuBarItem.highlighted ? "#ffffff" : "#21be2b"
//                horizontalAlignment: Text.AlignLeft
//                verticalAlignment: Text.AlignVCenter
//                elide: Text.ElideRight
//            }

//            background: Rectangle {
//                implicitWidth: 40
//                implicitHeight: 40
//                opacity: enabled ? 1 : 0.3
//                color: menuBarItem.highlighted ? "#21be2b" : "transparent"
//            }
//        }

//        background: Rectangle {
//            implicitWidth: 40
//            implicitHeight: 40
//            color: "#ffffff"

//            Rectangle {
//                color: "#21be2b"
//                width: parent.width
//                height: 1
//                anchors.bottom: parent.bottom
//            }
//        }
//    }

//    //Customizing PageIndicator
//    PageIndicator {
//        id: controlPageIndicator
//        count: 5
//        currentIndex: 2

//        delegate: Rectangle {
//            implicitWidth: 8
//            implicitHeight: 8

//            radius: width / 2
//            color: "#21be2b"

//            opacity: index === control.currentIndex ? 0.95 : pressed ? 0.7 : 0.45

//            required property int index

//            Behavior on opacity {
//                OpacityAnimator {
//                    duration: 100
//                }
//            }
//        }
//    }

//    //Customizing Pane
//    Pane {
//        background: Rectangle {
//            color: "#eeeeee"
//        }

//        Label {
//            text: qsTr("Content goes here!")
//        }
//    }

//    //Customizing Popup
//    Popup {
//        id: controlPopup
//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 200
//            border.color: "#444"
//        }
//        contentItem: Column {}
//    }

//    //Customizing ProgressBar
//    ProgressBar {
//        id: controlProgressBar
//        value: 0.5
//        padding: 2

//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 6
//            color: "#e6e6e6"
//            radius: 3
//        }

//        contentItem: Item {
//            implicitWidth: 200
//            implicitHeight: 4

//            Rectangle {
//                width: control.visualPosition * parent.width
//                height: parent.height
//                radius: 2
//                color: "#17a81a"
//            }
//        }
//    }

//    //Customizing RadioButton
//    RadioButton {
//        id: controlRadioButton
//        text: qsTr("RadioButton")
//        checked: true

//        indicator: Rectangle {
//            implicitWidth: 26
//            implicitHeight: 26
//            x: control.leftPadding
//            y: parent.height / 2 - height / 2
//            radius: 13
//            border.color: control.down ? "#17a81a" : "#21be2b"

//            Rectangle {
//                width: 14
//                height: 14
//                x: 6
//                y: 6
//                radius: 7
//                color: control.down ? "#17a81a" : "#21be2b"
//                visible: control.checked
//            }
//        }

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            verticalAlignment: Text.AlignVCenter
//            leftPadding: control.indicator.width + control.spacing
//        }
//    }

//    //Customizing RadioDelegate
//    RadioDelegate {
//        id: controlRadioDelegate
//        text: qsTr("RadioDelegate")
//        checked: true

//        contentItem: Text {
//            rightPadding: control.indicator.width + control.spacing
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            elide: Text.ElideRight
//            verticalAlignment: Text.AlignVCenter
//        }

//        indicator: Rectangle {
//            implicitWidth: 26
//            implicitHeight: 26
//            x: control.width - width - control.rightPadding
//            y: parent.height / 2 - height / 2
//            radius: 13
//            color: "transparent"
//            border.color: control.down ? "#17a81a" : "#21be2b"

//            Rectangle {
//                width: 14
//                height: 14
//                x: 6
//                y: 6
//                radius: 7
//                color: control.down ? "#17a81a" : "#21be2b"
//                visible: control.checked
//            }
//        }

//        background: Rectangle {
//            implicitWidth: 100
//            implicitHeight: 40
//            visible: control.down || control.highlighted
//            color: control.down ? "#bdbebf" : "#eeeeee"
//        }
//    }

//    //Customizing RangeSlider
//    RangeSlider {
//        id: controlRangeSlider
//        first.value: 0.25
//        second.value: 0.75

//        background: Rectangle {
//            x: control.leftPadding
//            y: control.topPadding + control.availableHeight / 2 - height / 2
//            implicitWidth: 200
//            implicitHeight: 4
//            width: control.availableWidth
//            height: implicitHeight
//            radius: 2
//            color: "#bdbebf"

//            Rectangle {
//                x: control.first.visualPosition * parent.width
//                width: control.second.visualPosition * parent.width - x
//                height: parent.height
//                color: "#21be2b"
//                radius: 2
//            }
//        }

//        first.handle: Rectangle {
//            x: control.leftPadding + control.first.visualPosition * (control.availableWidth - width)
//            y: control.topPadding + control.availableHeight / 2 - height / 2
//            implicitWidth: 26
//            implicitHeight: 26
//            radius: 13
//            color: control.first.pressed ? "#f0f0f0" : "#f6f6f6"
//            border.color: "#bdbebf"
//        }

//        second.handle: Rectangle {
//            x: control.leftPadding + control.second.visualPosition * (control.availableWidth - width)
//            y: control.topPadding + control.availableHeight / 2 - height / 2
//            implicitWidth: 26
//            implicitHeight: 26
//            radius: 13
//            color: control.second.pressed ? "#f0f0f0" : "#f6f6f6"
//            border.color: "#bdbebf"
//        }
//    }

//    //Customizing RoundButton
//    ScrollBar {
//        id: controlScrollBar
//        size: 0.3
//        position: 0.2
//        active: true
//        orientation: Qt.Vertical

//        contentItem: Rectangle {
//            implicitWidth: 6
//            implicitHeight: 100
//            radius: width / 2
//            color: control.pressed ? "#81e889" : "#c2f4c6"
//            // Hide the ScrollBar when it's not needed.
//            opacity: control.policy === ScrollBar.AlwaysOn || (control.active && control.size < 1.0) ? 0.75 : 0

//            // Animate the changes in opacity (default duration is 250 ms).
//            Behavior on opacity {
//                NumberAnimation {}
//            }
//        }
//    }

//    //Customizing ScrollIndicator
//    ScrollIndicator {
//        id: controlScrollIndicator
//        size: 0.3
//        position: 0.2
//        active: true
//        orientation: Qt.Vertical

//        contentItem: Rectangle {
//            implicitWidth: 2
//            implicitHeight: 100
//            color: "#c2f4c6"
//        }
//    }

//    //Customizing ScrollView
//    ScrollView {
//        id: controlScrollView

//        width: 200
//        height: 200
//        focus: true

//        Label {
//            text: "ABC"
//            font.pixelSize: 224
//        }

//        ScrollBar.vertical: ScrollBar {
//            parent: control
//            x: control.mirrored ? 0 : control.width - width
//            y: control.topPadding
//            height: control.availableHeight
//            active: control.ScrollBar.horizontal.active
//        }

//        ScrollBar.horizontal: ScrollBar {
//            parent: control
//            x: control.leftPadding
//            y: control.height - height
//            width: control.availableWidth
//            active: control.ScrollBar.vertical.active
//        }

//        background: Rectangle {
//            border.color: control.activeFocus ? "#21be2b" : "#bdbebf"
//        }
//    }

//    //Customizing Slider
//    Slider {
//        id: controlSlider
//        value: 0.5

//        background: Rectangle {
//            x: control.leftPadding
//            y: control.topPadding + control.availableHeight / 2 - height / 2
//            implicitWidth: 200
//            implicitHeight: 4
//            width: control.availableWidth
//            height: implicitHeight
//            radius: 2
//            color: "#bdbebf"

//            Rectangle {
//                width: control.visualPosition * parent.width
//                height: parent.height
//                color: "#21be2b"
//                radius: 2
//            }
//        }

//        handle: Rectangle {
//            x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
//            y: control.topPadding + control.availableHeight / 2 - height / 2
//            implicitWidth: 26
//            implicitHeight: 26
//            radius: 13
//            color: control.pressed ? "#f0f0f0" : "#f6f6f6"
//            border.color: "#bdbebf"
//        }
//    }

//    //Customizing SpinBox
//    SpinBox {
//        id: controlSpinBox
//        value: 50
//        editable: true

//        contentItem: TextInput {
//            z: 2
//            text: control.textFromValue(control.value, control.locale)

//            font: control.font
//            color: "#21be2b"
//            selectionColor: "#21be2b"
//            selectedTextColor: "#ffffff"
//            horizontalAlignment: Qt.AlignHCenter
//            verticalAlignment: Qt.AlignVCenter

//            readOnly: !control.editable
//            validator: control.validator
//            inputMethodHints: Qt.ImhFormattedNumbersOnly
//        }

//        up.indicator: Rectangle {
//            x: control.mirrored ? 0 : parent.width - width
//            height: parent.height
//            implicitWidth: 40
//            implicitHeight: 40
//            color: control.up.pressed ? "#e4e4e4" : "#f6f6f6"
//            border.color: enabled ? "#21be2b" : "#bdbebf"

//            Text {
//                text: "+"
//                font.pixelSize: control.font.pixelSize * 2
//                color: "#21be2b"
//                anchors.fill: parent
//                fontSizeMode: Text.Fit
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//            }
//        }

//        down.indicator: Rectangle {
//            x: control.mirrored ? parent.width - width : 0
//            height: parent.height
//            implicitWidth: 40
//            implicitHeight: 40
//            color: control.down.pressed ? "#e4e4e4" : "#f6f6f6"
//            border.color: enabled ? "#21be2b" : "#bdbebf"

//            Text {
//                text: "-"
//                font.pixelSize: control.font.pixelSize * 2
//                color: "#21be2b"
//                anchors.fill: parent
//                fontSizeMode: Text.Fit
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//            }
//        }

//        background: Rectangle {
//            implicitWidth: 140
//            border.color: "#bdbebf"
//        }
//    }

//    //Customizing SplitView
//    SplitView {
//        id: splitView
//        anchors.fill: parent

//        handle: Rectangle {
//            implicitWidth: 4
//            implicitHeight: 4
//            color: SplitHandle.pressed ? "#81e889"
//                : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")
//        }

//        Rectangle {
//            implicitWidth: 150
//            color: "#444"
//        }
//        Rectangle {
//            implicitWidth: 50
//            color: "#666"
//        }
//    }

//    //Customizing StackView
//    StackView {
//        id: controlStackView

//        popEnter: Transition {
//            XAnimator {
//                from: (control.mirrored ? -1 : 1) * -control.width
//                to: 0
//                duration: 400
//                easing.type: Easing.OutCubic
//            }
//        }

//        popExit: Transition {
//            XAnimator {
//                from: 0
//                to: (control.mirrored ? -1 : 1) * control.width
//                duration: 400
//                easing.type: Easing.OutCubic
//            }
//        }
//    }

//    //Customizing SwipeDelegate
//    SwipeDelegate {
//        id: controlSwipeDelegate
//        text: qsTr("SwipeDelegate")

//        Component {
//            id: component

//            Rectangle {
//                color: SwipeDelegate.pressed ? "#333" : "#444"
//                width: parent.width
//                height: parent.height
//                clip: true

//                Label {
//                    text: qsTr("Press me!")
//                    color: "#21be2b"
//                    anchors.centerIn: parent
//                }
//            }
//        }

//        swipe.left: component
//        swipe.right: component

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            color: control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
//            elide: Text.ElideRight
//            verticalAlignment: Text.AlignVCenter

//            Behavior on x {
//                enabled: !control.down
//                NumberAnimation {
//                    easing.type: Easing.InOutCubic
//                    duration: 400
//                }
//            }
//        }
//    }

//    //Customizing SwipeView
//    SwipeView {
//        id: controlSwipeView

//        background: Rectangle {
//            color: "#eeeeee"
//        }
//    }





    //Customizing Switch
//    property alias styleBlackSwitch: styleBlackSwitch
//    property alias styleStandartSwitch: styleStandartSwitch

//    Component
//    {
//        id: styleBlackSwitch

  //      T.Switch {
//            id: controlSwitch

//            indicator: Rectangle {
//                implicitWidth: 48
//                implicitHeight: 26
//                x: controlSwitch.leftPadding
//                y: parent.height / 2 - height / 2
//                radius: 13
//                color: controlSwitch.checked ? "#17a81a" : "#ffffff"
//                border.color: controlSwitch.checked ? "#17a81a" : "#cccccc"

//                Rectangle {
//                    x: controlSwitch.checked ? parent.width - width : 0
//                    width: 26
//                    height: 26
//                    radius: 13
//                    color: controlSwitch.down ? "#cccccc" : "#ffffff"
//                    border.color: controlSwitch.checked ? (controlSwitch.down ? "#17a81a" : "#21be2b") : "#999999"
//                }
//            }

//            contentItem: Text {
//                text: controlSwitch.text
//                font: controlSwitch.font
//                opacity: enabled ? 1.0 : 0.3
//                color: controlSwitch.down ? "#17a81a" : "#21be2b"
//                verticalAlignment: Text.AlignVCenter
//                leftPadding: controlSwitch.indicator.width + controlSwitch.spacing
//            }
//        }
  //  }







//    //Customizing SwitchDelegate
//    SwitchDelegate {
//        id: controlSwitchDelegate
//        text: qsTr("SwitchDelegate")
//        checked: true

//        contentItem: Text {
//            rightPadding: control.indicator.width + control.spacing
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            elide: Text.ElideRight
//            verticalAlignment: Text.AlignVCenter
//        }

//        indicator: Rectangle {
//            implicitWidth: 48
//            implicitHeight: 26
//            x: control.width - width - control.rightPadding
//            y: parent.height / 2 - height / 2
//            radius: 13
//            color: control.checked ? "#17a81a" : "transparent"
//            border.color: control.checked ? "#17a81a" : "#cccccc"

//            Rectangle {
//                x: control.checked ? parent.width - width : 0
//                width: 26
//                height: 26
//                radius: 13
//                color: control.down ? "#cccccc" : "#ffffff"
//                border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
//            }
//        }

//        background: Rectangle {
//            implicitWidth: 100
//            implicitHeight: 40
//            visible: control.down || control.highlighted
//            color: control.down ? "#bdbebf" : "#eeeeee"
//        }
//    }

//    //Customizing TabBar
//    TabBar {
//        id: controlTabBar

//        background: Rectangle {
//            color: "#eeeeee"
//        }

//        TabButton {
//            text: qsTr("Home")
//        }
//        TabButton {
//            text: qsTr("Discover")
//        }
//        TabButton {
//            text: qsTr("Activity")
//        }
//    }

//    //Customizing TextArea
//    TextArea {
//        id: controlTextArea
//        placeholderText: qsTr("Enter description")

//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 40
//            border.color: control.enabled ? "#21be2b" : "transparent"
//        }
//    }

//    //Customizing TextField
//    TextField {
//        id: controlTextField
//        placeholderText: qsTr("Enter description")

//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 40
//            color: control.enabled ? "transparent" : "#353637"
//            border.color: control.enabled ? "#21be2b" : "transparent"
//        }
//    }

//    //Customizing ToolBar
//    ToolBar {
//        id: controlToolBar

//        background: Rectangle {
//            implicitHeight: 40
//            color: "#eeeeee"

//            Rectangle {
//                width: parent.width
//                height: 1
//                anchors.bottom: parent.bottom
//                color: "transparent"
//                border.color: "#21be2b"
//            }
//        }

//        RowLayout {
//            anchors.fill: parent
//            ToolButton {
//                text: qsTr("Undo")
//            }
//            ToolButton {
//                text: qsTr("Redo")
//            }
//        }
//    }

//    //Customizing ToolButton
//    ToolButton {
//        id: controlToolButton
//        text: qsTr("ToolButton")
//        width: 120

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            opacity: enabled ? 1.0 : 0.3
//            color: control.down ? "#17a81a" : "#21be2b"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            elide: Text.ElideRight
//        }

//        background: Rectangle {
//            implicitWidth: 40
//            implicitHeight: 40
//            color: Qt.darker("#33333333", control.enabled && (control.checked || control.highlighted) ? 1.5 : 1.0)
//            opacity: enabled ? 1 : 0.3
//            visible: control.down || (control.enabled && (control.checked || control.highlighted))
//        }
//    }

//    //Customizing ToolSeparator
//    ToolBar {
//        RowLayout {
//            anchors.fill: parent

//            ToolButton {
//                text: qsTr("Action 1")
//            }
//            ToolButton {
//                text: qsTr("Action 2")
//            }

//            ToolSeparator {
//                padding: vertical ? 10 : 2
//                topPadding: vertical ? 2 : 10
//                bottomPadding: vertical ? 2 : 10

//                contentItem: Rectangle {
//                    implicitWidth: parent.vertical ? 1 : 24
//                    implicitHeight: parent.vertical ? 24 : 1
//                    color: "#c3c3c3"
//                }
//            }

//            ToolButton {
//                text: qsTr("Action 3")
//            }
//            ToolButton {
//                text: qsTr("Action 4")
//            }

//            Item {
//                Layout.fillWidth: true
//            }
//        }
//    }

//    //Customizing ToolTip
//    ToolTip {
//        id: controlToolTip
//        text: qsTr("A descriptive tool tip of what the button does")

//        contentItem: Text {
//            text: control.text
//            font: control.font
//            color: "#21be2b"
//        }

//        background: Rectangle {
//            border.color: "#21be2b"
//        }
//    }

//    //Customizing Tumbler
//    Tumbler {
//        id: controlTumbler
//        model: 15

//        background: Item {
//            Rectangle {
//                opacity: control.enabled ? 0.2 : 0.1
//                border.color: "#000000"
//                width: parent.width
//                height: 1
//                anchors.top: parent.top
//            }

//            Rectangle {
//                opacity: control.enabled ? 0.2 : 0.1
//                border.color: "#000000"
//                width: parent.width
//                height: 1
//                anchors.bottom: parent.bottom
//            }
//        }

//        delegate: Text {
//            text: qsTr("Item %1").arg(modelData + 1)
//            font: control.font
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            opacity: 1.0 - Math.abs(Tumbler.displacement) / (control.visibleItemCount / 2)

//            required property var modelData
//            required property int index
//        }

//        Rectangle {
//            anchors.horizontalCenter: control.horizontalCenter
//            y: control.height * 0.4
//            width: 40
//            height: 1
//            color: "#21be2b"
//        }

//        Rectangle {
//            anchors.horizontalCenter: control.horizontalCenter
//            y: control.height * 0.6
//            width: 40
//            height: 1
//            color: "#21be2b"
//        }
//    }


//}
