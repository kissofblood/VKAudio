import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

ApplicationWindow {
    property int widthGroove: 500
    property bool isPlay: true

    visible: true
    width: 800
    height: 1280

    Rectangle {
        color: "#212126"
        anchors.fill: parent
    }

    toolBar: Rectangle {
        id: player
        width: parent.width
        height: 60
        color: "black"

        Rectangle {
            anchors.bottom: player.bottom
            width: player.width
            height: 5
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#237b9c"; }
                GradientStop { position: 0.7; color: "#33b5e5"; }
            }
        }

        RowLayout {
            anchors.fill: player
            anchors.leftMargin: 80
            anchors.rightMargin: 30
            spacing: 6

            Rectangle {
                id: prevTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedPrev.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    anchors.verticalCenter: prevTrack.verticalCenter
                    source: "qrc:/icon/icon/player_rew.png"
                }

                MouseArea {
                    id: clickedPrev
                    anchors.fill: prevTrack
                }
            }

            Rectangle {
                id: playOrPauseTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedPlayOrPause.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    id: imagePlayOrPause
                    anchors.verticalCenter: playOrPauseTrack.verticalCenter
                    source: "qrc:/icon/icon/player_play.png"
                }

                MouseArea {
                    id: clickedPlayOrPause
                    anchors.fill: playOrPauseTrack
                    onReleased: {
                        if(isPlay)
                        {
                            imagePlayOrPause.source = "qrc:/icon/icon/player_pause.png"
                            isPlay = false
                        }
                        else
                        {
                            imagePlayOrPause.source = "qrc:/icon/icon/player_play.png"
                            isPlay = true
                        }
                    }
                }
            }

            Rectangle {
                id: nextTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedNext.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    anchors.verticalCenter: nextTrack.verticalCenter
                    source: "qrc:/icon/icon/player_fwd.png"
                }

                MouseArea {
                    id: clickedNext
                    anchors.fill: nextTrack
                }
            }

            Component {
                id: styleSlider
                SliderStyle {
                    handle: Rectangle {
                        width: 20
                        height: 20
                        radius: height
                        antialiasing: true
                        color: Qt.lighter("#468bb7", 1.2)
                    }

                    groove: Item {
                        id: groovItem
                        implicitHeight: 8
                        implicitWidth: widthGroove

                        Rectangle {
                            height: 8
                            width: groovItem.width
                            color: "#444"
                            opacity: 0.8
                            radius: 2

                            Rectangle {
                                antialiasing: true
                                radius: 2
                                color: "#468bb7"
                                height: groovItem.height
                                width: groovItem.width * control.value / control.maximumValue
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                spacing: 2

                RowLayout {
                    Text {
                        id: nameTrack
                        Layout.fillWidth: true
                        font.pixelSize: 20
                        color: "white"
                        text: "Widget Gallery"
                    }

                    Text {
                        id: durationTrack
                        font.pixelSize: 20
                        color: "white"
                        text: "12:12"
                    }

                }

                Slider {
                    Layout.fillWidth: true
                    style: styleSlider
                    value: 0
                }
            }

            Rectangle {
                id: loopTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedLoop.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    anchors.verticalCenter: loopTrack.verticalCenter
                    source: "qrc:/icon/icon/noatunloopsong.png"
                }

                MouseArea {
                    id: clickedLoop
                    anchors.fill: loopTrack
                }

            }

            Rectangle {
                id: randowTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedRandow.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    anchors.verticalCenter: randowTrack.verticalCenter
                    source: "qrc:/icon/icon/random.png"
                }

                MouseArea {
                    id: clickedRandow
                    anchors.fill: randowTrack
                }
            }

            Rectangle {
                id: volumeTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: "transparent"
                antialiasing: true

                Image {
                    anchors.verticalCenter: volumeTrack.verticalCenter
                    source: "qrc:/icon/icon/player_volume.png"
                }
            }

            Slider {
                Layout.fillWidth: true
                style: {
                    widthGroove = 50
                    styleSlider
                }
                value: 0
            }

            Rectangle {
                id: addTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedAdd.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    anchors.verticalCenter: addTrack.verticalCenter
                    source: "qrc:/icon/icon/add.png"
                }

                MouseArea {
                    id: clickedAdd
                    anchors.fill: addTrack
                }
            }

            Rectangle {
                id: downloadTrack
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.minimumHeight: 60
                Layout.maximumWidth: 50
                Layout.maximumHeight: 60
                color: clickedDonwload.pressed ? "#222" : "transparent"
                antialiasing: true
                radius: 5

                Image {
                    anchors.verticalCenter: downloadTrack.verticalCenter
                    source: "qrc:/icon/icon/download.png"
                }

                MouseArea {
                    id: clickedDonwload
                    anchors.fill: downloadTrack
                }
            }
        }
    }

    TextField {
        id: searchTrack
        width: parent.width
        height: 40
        font.pixelSize: 25
        placeholderText: "search"
        style: TextFieldStyle {
            textColor: "white"
            placeholderTextColor: "lightblue"
            background: BorderImage {
                    source: "qrc:/icon/icon/textinput.png"
                    anchors.leftMargin: 6
                    anchors.rightMargin: 6
                    border.left: 8
                    border.right: 8
                    border.bottom: 8
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
        }
    }

    ScrollView {
        anchors.top: searchTrack.bottom
        width: parent.width
        height: parent.height - searchTrack.height
        flickableItem.interactive: true
        style: ScrollViewStyle {
             transientScrollBars: true
             handle: Item {
                 implicitWidth: 15
                 implicitHeight: 25

                 Rectangle {
                     color: "#424246"
                     anchors.fill: parent
                     anchors.topMargin: 6
                     anchors.leftMargin: 4
                     anchors.rightMargin: 4
                     anchors.bottomMargin: 6
                 }
             }
             scrollBarBackground: Item {
                 implicitWidth: 15
                 implicitHeight: 25
             }
         }

        ListView {
            anchors.leftMargin: 6
            model: model
            delegate: delegate
        }
    }

    Component {
        id: delegate

        Item {
            width: parent.width
            height: 70

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 8
                height: 1
                color: "#424246"
            }

            Rectangle {
                anchors.fill: parent
                anchors.rightMargin: 8
                color: "#11ffffff"
                visible: mouseDelegate.pressed
            }

            MouseArea {
                id: mouseDelegate
                anchors.fill: parent
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                text: artist
                font.pixelSize: 15
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.centerIn: parent
                color: "white"
                text: title
                font.pointSize: 11
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20
                color: "white"
                text: duration
                font.pointSize: 11
            }
        }
    }

    ListModel {
        id: model
        ListElement  {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Dark Princess"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }

        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }



        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }



        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }


        ListElement {
            artist: "Amaranthe"
            title: "Amaranthe"
            duration: 2011
        }

        ListElement {
            artist: "Dark Princess"
            title: "Without you"
            duration: 2005
        }

        ListElement {
            artist: "Wihin Temptation"
            title: "The Unforgiving"
            duration: 2011
        }
    }

}
