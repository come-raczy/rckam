import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import myextension 1.0

ApplicationWindow {
    objectName: "rckamApplicationWindow"
    id: window
    //width: 360
    //height: 520
    //visible: true
    //title: qsTr("Side Panel")

    visible: true
    width: 1024
    height: 940
    title: qsTr("rckam: a remote control for digital cameras and peripherals")
    readonly property bool inPortrait: window.width < window.height
    menuBar: MenuBar {
        Menu {
            title: qsTr("Devices")
            MenuItem {
                text: qsTr("&Camera")
                onTriggered: console.log("Camera selection trigered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("Camera")
            MenuItem {
                text: qsTr("Click here")
                onTriggered: camera.click();
            }
        }
        Menu {
            title: qsTr("Help")
            MenuItem {
                text: qsTr("Contents...")
                onTriggered: console.log("Help contents requested");
            }
            MenuItem {
                text: qsTr("About")
                onTriggered: console.log("About info requested");
            }
        }
    }

    ToolBar {
        id: toolbar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        ColumnLayout {
            id: column
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            ButtonGroup {
                id: radioGroup
                exclusive: true
                onClicked: console.log("Clicked", button.text);
            }
            ToolButton {
                id: globalSettingsButton
                Layout.fillWidth : true
                icon.source: "icons/settings.svg"
                // attached tooltip cand be positionned - use custom tooltip instead
                //ToolTip.visible: hovered
                //ToolTip.text: "Global\nSettings"
                ToolTip {
                  visible: parent.hovered
                  text: "global settings"
                  x : parent.width
                  y : 0
                }
                checkable: true
                checked: false
                ButtonGroup.group: radioGroup
                onClicked: {console.log("clicked", text); cameraSettingsButton.checkable = cameraSettingsButton.checkable ? false : true;}
                onCheckedChanged: {console.log("Checked changed", text); globalSettings.visible = checked; cameraSettingsButton.checkable = true;}
            }
            ToolButton {
                id: cameraSettingsButton
                Layout.fillWidth : true
                icon.source: checkable ? "icons/camera.svg" : "icons/camera-off.svg"
                ToolTip {
                  visible: parent.hovered
                  text: "camera settings"
                  x : parent.width
                  y : 0
                }
                checkable: false
                checked: false
                ButtonGroup.group: radioGroup
                // onCheckableChanged: {icon.source = checkable ? "qrc:/icons/camera.svg" : "qrc:/icons/camera-off.svg";}
                onCheckedChanged: {console.log("Checked changed", text); cameraSettings.visible = checked;}
            }
            ToolButton {
                id: liveViewButton
                Layout.fillWidth : true
                ToolTip {
                  visible: parent.hovered
                  text: "live view"
                  x : parent.width
                  y : 0
                }
                icon.source: checked ? "icons/eye.svg" : "icons/eye-off.svg"
                //icon.color: "transparent"
                checkable: true
                checked: true
                ButtonGroup.group: radioGroup
                onCheckableChanged: {console.log("liveViewButton: Check blechanged", text); liveView.visible = checked;}
                onCheckedChanged: {console.log("liveViewButton: Checked changed", text); liveView.visible = checked;}
            }
            Item { Layout.fillHeight : true }
        }
    }
    GlobalSettings {
        id: globalSettings
        visible: true
        color: "green"
    }
    CameraSettings {
        id: cameraSettings
        visible: false
        color: "red"
    }
    LiveView {
        id: liveView
        visible: true
        //color: "blue"
        //Image {
        //  id: liveViewImage
        //  source: "file:///mnt/c/Users/comer/Downloads/tmp/preview-00000-19\:32\:41.850.jpg"
        //  visible: true
        //  onStatusChanged: if (status == Image.Ready) console.log('Loaded')
        //}
        ImagePreview {
          objectName: "imagePreview"
          id: imagePreview
          height: 640
          width: 960
          //height: parent.height
          //width: parent.width
        }
    }
}
