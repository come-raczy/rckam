import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12

Rectangle {
    anchors.topMargin: 0
    anchors.left: toolbar.right
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    Layout.fillWidth : true


    Rectangle { // no other reason than providing a background color
        width: 200; height: 300
        color: "lightgrey"
        // Delegate component. Instantiated for each visible item in the list.
        Component {
            id: myDelegate
            Item {
                id: wrapper
                width: 200; height: 55
                Column {
                    Text { text: 'Vendor: ' + vendor }
                    Text { text: 'Product: ' + product }
                }
                // indent the item if it is the current item
                states: State {
                    name: "Current"
                    when: wrapper.ListView.isCurrentItem
                    PropertyChanges { target: wrapper; x: 20 }
                }
                transitions: Transition {
                    NumberAnimation { properties: "x"; duration: 200 }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: wrapper.ListView.view.currentIndex = index
/*
onClicked: {
    // note: lm here is the id of your ListModel
    ism.select(lm.index(index, 0), ItemSelectionModel.Select | ItemSelectionModel.Current)
    console.log(ism.selectedIndexes)
    console.log(ism.hasSelection)
}
*/
                }
            }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            // specify width, height and y to fill on the whole width
            //width: listView.currentItem.width
            //height: listView.currentItem.height
            //y: listView.currentItem.y;
            // or use anchors to fill with the indent and an overhang
            anchors.fill: listView.currentItem
            color: "#FFFF88"
            Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
        }
    }
/*
    ItemSelectionModel {
        id: ism
        model: lm
    }
*/
    ScrollView {
        width: 180
        height: 200

        ListView
        {
            id: listView
            width: 600; height: 250
            keyNavigationEnabled: true
            focus: true
            //---/ C++ model set using context property in main.cpp /---//
            model: usbCameraList
            //---// items are drawn by a delegate. /---//
            //delegate: Text { text: "name: " + name + ", " + ", id:" + id}
            delegate: myDelegate
            // Set the highlight delegate. Note we must also set highlightFollowsCurrentItem
            // to false so the highlight delegate can control how the highlight is moved.
            highlight: highlightBar
            highlightFollowsCurrentItem: false

        } // ListView



/*
        ListView {
            width: 180
            //height: 200
        
            model: lm
            delegate: Rectangle { // needed for background color
                color: mouseArea.containsMouse ? "grey" : "white";
                width: parent.width
                height: childrenRect.height
                Text {
                    text: name + ": " + number
                    //style: Text.Outline;
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true // enables containsMouse
                    onClicked: {
                        // lm.index(index, row) is the method from QAbstractItemModel
                        ism.select(lm.index(index, 0), ItemSelectionModel.Select | ItemSelectionModel.Current)
                        console.log(ism.selectedIndexes)
                        console.log(ism.hasSelection)
                    }
                }
            }
        } // ListView
*/
    } // ScrollView
}

