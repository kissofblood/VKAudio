import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtGraphicalEffects 1.0
import "setFunction.js" as Script

Item {
    id: item

    property real widthGroove: 500
    property real sizePixNormal: 60
    property real sizePixIncrease: 100
    property bool isPlay: true
    property bool isLoop: false
    property bool isRandom: false
    property string indefinite: "indefinite"
    property real fastBlurRadius: 5
    property bool addAndCancel: true
    property bool friendAudio: false
    property bool hideListGenre: false
    property string textSearchFriend: ""
    property int indexTextSearchFriend: 0
    property var arrayTextSearchFriend: new Array

    signal selectIdTrack(string id)
    signal positionTrackChange(int value)
    signal volumeTrackChange(int value)
    signal selectPlayTrack()
    signal selectPauseTrack()
    signal selectNextTrack(string id)
    signal selectPrevTrack(string id)
    signal selectLoopTrack(bool value)
    signal selectRandomTrack(bool value)
    signal selectPlaylistFriend(string id)
    signal selectPlaylistRecommended(string id)
    signal selectPlaylistPopular(string id)
    signal selectPlaylistMy()
    signal clickedDownloadTrack(string name)
    signal returnPressedSearchTrack(string search)
    signal returnPressedGlobalSearchTrack(string search)
    signal addTrack(string trackId, string userId)
    signal removeTrack(string trackId, string userId, bool remover)
    signal deleteTrack();
    signal uploadTrack();

    width: 800
    height: 1280
    enabled: false

    Connections {
        target: connectVkAudio
        onProgressDownloadTrack: {
            progressBar.value = value
            if(value == 100)
            {
                var index = listView.currentIndex
                nameTrack.text = vkAudioModel[index].artist + " - " + vkAudioModel[index].title
                imagePlayOrPause.source = "qrc:/icon/icon/player_play.png"
                isPlay = true
            }
        }
        onMediaPositionChanged: {
            positionTrack.value = position
            durationTrack.text = duration
        }
        onMediaDurationChanged: positionTrack.maximumValue = duration
        onIdTrackChanged: {
            if(value == true)
            {
                for(var next = 0; next < vkAudioModel.length; next++)
                    if(vkAudioModel[next].idTrack === id)
                    {
                        listView.currentIndex = next
                        break
                    }
            }
            else for(var prev = vkAudioModel.length - 1; prev >= 0; prev--)
                    if(vkAudioModel[prev].idTrack === id)
                    {
                        listView.currentIndex = prev
                        break
                    }
        }
        onNextTrackDefault: item.selectNextTrack(vkAudioModel[listView.currentIndex].idTrack)
        onEnableTrue: {
            fastBlurRadius = -1
            item.enabled = true
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
                implicitWidth: 100

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

    Component {
        id: scrollViewHandle

        ScrollViewStyle {
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
    }

    Component {
        id: styleTextField

        TextFieldStyle {
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

    Rectangle {
        color: "#212126"
        anchors.fill: parent
    }

    Rectangle {
        id: player
        width: parent.width
        height: 60
        color: "black"

        Rectangle {
            anchors.bottom: player.bottom
            width: parent.width
            height: 5
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#237b9c"; }
                GradientStop { position: 0.7; color: "#33b5e5"; }
            }
        }

        RowLayout {
            anchors.fill: player
            anchors.leftMargin: 40
            anchors.rightMargin: 30
            spacing: 6

            Rectangle {
                id: avatarWindow
                width: 50
                height: 50
                border.color: "#33b5e5"
                border.width: 2

                Image {
                    id: avatarWindowImage
                    width: avatarWindow.width - 2
                    height: avatarWindow.height - 2
                    anchors.centerIn: avatarWindow
                    source: connectVkAudio.getUrlAvatarMy()
                }

                MouseArea {
                    id: clickedFriend
                    anchors.fill: avatarWindow
                    onClicked: {
                        fastBlurRadius = 5
                        listFriend.visible = true
                        rightPanel.enabled = false
                        inputSearchFriend.focus = true
                    }
                }

                FastBlur {
                    anchors.fill: avatarWindow
                    source: avatarWindowImage
                    radius: fastBlurRadius
                    transparentBorder: true
                }
            }

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
                    id: prevTrackImage
                    anchors.verticalCenter: prevTrack.verticalCenter
                    source: "qrc:/icon/icon/player_rew.png"
                }

                MouseArea {
                    id: clickedPrev
                    anchors.fill: prevTrack
                    onReleased: item.selectPrevTrack(vkAudioModel[listView.currentIndex].idTrack)
                }

                FastBlur {
                    anchors.fill: prevTrack
                    source: prevTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
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
                            item.selectPauseTrack()
                        }
                        else
                        {
                            imagePlayOrPause.source = "qrc:/icon/icon/player_play.png"
                            isPlay = true
                            item.selectPlayTrack()
                        }
                    }
                }

                FastBlur {
                    anchors.fill: playOrPauseTrack
                    source: imagePlayOrPause
                    radius: fastBlurRadius
                    transparentBorder: true
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
                    id: nextTrackImage
                    anchors.verticalCenter: nextTrack.verticalCenter
                    source: "qrc:/icon/icon/player_fwd.png"
                }

                MouseArea {
                    id: clickedNext
                    anchors.fill: nextTrack
                    onReleased: item.selectNextTrack(vkAudioModel[listView.currentIndex].idTrack)
                }

                FastBlur {
                    anchors.fill: nextTrack
                    source: nextTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
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
                        text: ""

                        FastBlur {
                            anchors.fill: nameTrack
                            source: nameTrack
                            radius: fastBlurRadius
                            transparentBorder: true
                        }
                    }

                    Text {
                        id: durationTrack
                        font.pixelSize: 20
                        color: "white"
                        text: "0:0"

                        FastBlur {
                            anchors.fill: durationTrack
                            source: durationTrack
                            radius: fastBlurRadius
                            transparentBorder: true
                        }
                    }
                }

                Slider {
                    id: positionTrack
                    minimumValue: 0
                    Layout.fillWidth: true
                    style: styleSlider
                    value: 0
                    onPressedChanged: item.positionTrackChange(value)

                    FastBlur {
                        anchors.fill: positionTrack
                        source: positionTrack
                        radius: fastBlurRadius
                        transparentBorder: true
                    }
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
                color: "transparent"
                antialiasing: true
                radius: 5

                Image {
                    id: loopTrackImage
                    anchors.verticalCenter: loopTrack.verticalCenter
                    source: "qrc:/icon/icon/noatunloopsong.png"
                }

                MouseArea {
                    id: clickedLoop
                    anchors.fill: loopTrack
                    onReleased: {
                        if(isLoop)
                        {
                            loopTrack.color = "transparent"
                            isLoop = false
                        }
                        else
                        {
                            loopTrack.color = "#222"
                            randowTrack.color = "transparent"
                            isLoop = true
                            isRandom = false
                            item.selectRandomTrack(isRandom)
                        }
                        item.selectLoopTrack(isLoop)
                    }
                }

                FastBlur {
                    anchors.fill: loopTrack
                    source: loopTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
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
                color: "transparent"
                antialiasing: true
                radius: 5

                Image {
                    id: randowTrackImage
                    anchors.verticalCenter: randowTrack.verticalCenter
                    source: "qrc:/icon/icon/random.png"
                }

                MouseArea {
                    id: clickedRandow
                    anchors.fill: randowTrack
                    onReleased: {
                        if(isRandom)
                        {
                            randowTrack.color = "transparent"
                            isRandom = false
                        }
                        else
                        {
                            randowTrack.color = "#222"
                            loopTrack.color = "transparent"
                            isRandom = true
                            isLoop = false
                            item.selectLoopTrack(isLoop)
                        }
                        item.selectRandomTrack(isRandom, vkAudioModel[listView.currentIndex].idTrack)
                    }
                }

                FastBlur {
                    anchors.fill: randowTrack
                    source: randowTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
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
                    id: volumeTrackImage
                    anchors.verticalCenter: volumeTrack.verticalCenter
                    source: "qrc:/icon/icon/player_volume.png"
                }

                FastBlur {
                    anchors.fill: volumeTrack
                    source: volumeTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
                }
            }

            Slider {
                id: sliderVolume
                Layout.fillWidth: true
                style: {
                    widthGroove = 50
                    return styleSlider
                }
                minimumValue: 0
                maximumValue: 100
                value: 100
                onPressedChanged: item.volumeTrackChange(value)

                FastBlur {
                    anchors.fill: sliderVolume
                    source: sliderVolume
                    radius: fastBlurRadius
                    transparentBorder: true
                }
            }

            Rectangle {
                id: uploadTrackRect
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
                    id: uploadTrackImage
                    anchors.verticalCenter: uploadTrackRect.verticalCenter
                    source: "qrc:/icon/icon/add.png"
                }

                MouseArea {
                    id: clickedAdd
                    anchors.fill: uploadTrackRect
                    onReleased: item.uploadTrack()
                }

                FastBlur {
                    anchors.fill: uploadTrackRect
                    source: uploadTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
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
                    id: downloadTrackImage
                    anchors.verticalCenter: downloadTrack.verticalCenter
                    source: "qrc:/icon/icon/download.png"
                }

                MouseArea {
                    id: clickedDonwload
                    anchors.fill: downloadTrack
                    onReleased: item.clickedDownloadTrack(nameTrack.text)
                }

                FastBlur {
                    anchors.fill: downloadTrack
                    source: downloadTrackImage
                    radius: fastBlurRadius
                    transparentBorder: true
                }
            }
        }
    }

    TextField {
        id: searchTrack
        anchors.top: player.bottom
        width: parent.width
        height: 40
        font.pixelSize: 25
        placeholderText: "search"
        onAccepted: item.returnPressedSearchTrack(text)
        style: styleTextField

        FastBlur {
            anchors.fill: searchTrack
            source: searchTrack
            radius: fastBlurRadius
            transparentBorder: true
        }
    }

    ScrollView {
        id: scrollViewListTrack
        anchors.top: searchTrack.bottom
        width: parent.width - rightPanel.width
        height: parent.height - searchTrack.height - player.height - progressBar.height
        flickableItem.interactive: true
        style: scrollViewHandle

        ListView {
            id: listView
            anchors.leftMargin: 6
            model: vkAudioModel
            delegate: Item {
                id: itemDelegat
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
                    id: backgroundRect
                    anchors.fill: parent
                    anchors.rightMargin: 8
                    color: "#11ffffff"
                    visible: mouseDelegate.pressed
                }

                MouseArea {
                    id: mouseDelegate
                    anchors.fill: parent
                    enabled: enableMouse
                    onClicked: {
                        listView.currentIndex = model.index
                        item.selectIdTrack(vkAudioModel[listView.currentIndex].idTrack)
                    }
                }

                ColorOverlay {
                    id: colorBackgroundRect
                    anchors.fill: backgroundRect
                    source: backgroundRect
                    color: "#33b5e5"
                    visible: visibleColorItem
                }

                Text {
                    id: textArtist
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    text: artist
                    font.pixelSize: 15

                    FastBlur {
                        anchors.fill: textArtist
                        source: textArtist
                        radius: {
                            if(fastBlurRadius == -1)
                                visible = false
                            else
                                visible = true
                            return fastBlurRadius
                        }
                        transparentBorder: true
                    }
                }

                Text {
                    id: textTitle
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.centerIn: parent
                    color: "white"
                    text: title
                    font.pointSize: 11

                    FastBlur {
                        anchors.fill: textTitle
                        source: textTitle
                        radius: {
                            if(fastBlurRadius == -1)
                                visible = false
                            else
                                visible = true
                            return fastBlurRadius
                        }
                        transparentBorder: true
                    }
                }

                Text {
                    id: textDuration
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    color: "white"
                    text: duration
                    font.pointSize: 11

                    FastBlur {
                        anchors.fill: textDuration
                        source: textDuration
                        radius: {
                            if(fastBlurRadius == -1)
                                visible = false
                            else
                                visible = true
                            return fastBlurRadius
                        }
                        transparentBorder: true
                    }
                }

                Rectangle {
                    id: cancelRemoveTrack
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: textDuration.left
                    width: 40
                    height: 40
                    color: clickedCancelRemoveTrack.pressed ? "#424246" : "transparent"
                    radius: 5
                    visible: visibleImageCancel

                    Image {
                        id: cancelRemoveTrackImage
                        anchors.centerIn: cancelRemoveTrack
                        source: "qrc:/icon/icon/cancel.png"
                    }

                    ColorOverlay {
                        anchors.fill: cancelRemoveTrackImage
                        source: cancelRemoveTrackImage
                        color: "#555555"
                        visible: visibleColorRemove
                    }

                    FastBlur {
                        anchors.fill: cancelRemoveTrack
                        source: cancelRemoveTrack
                        radius: {
                            if(fastBlurRadius == -1)
                                visible = false
                            else
                                visible = true
                            return fastBlurRadius
                        }
                        transparentBorder: true
                    }

                    MouseArea {
                        id: clickedCancelRemoveTrack
                        anchors.fill: cancelRemoveTrack
                        onReleased: {
                            var index = model.index
                            cancelRemoveTrack.enabled = false
                            vkAudioModel[index].enableMouse = false
                            vkAudioModel[index].visibleColorRemove = true
                            vkAudioModel[index].visibleImageAdd = true
                            vkAudioModel[index].visibleColorItem = true
                            item.removeTrack(vkAudioModel[index].idTrack, vkAudioModel[index].idUser, false)
                        }
                    }
                }

                Rectangle {
                    id: okAddTrack
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: cancelRemoveTrack.left
                    width: 40
                    height: 40
                    color: clickedOkAddTrack.pressed ? "#424246" : "transparent"
                    radius: 5
                    visible: visibleImageAdd

                    Image {
                        id: okAddTrackImage
                        anchors.centerIn: okAddTrack
                        source: "qrc:/icon/icon/ok.png"
                    }

                    ColorOverlay {
                        anchors.fill: okAddTrackImage
                        source: okAddTrackImage
                        color: "#555555"
                        visible: visibleColorAdd
                    }

                    FastBlur {
                        anchors.fill: okAddTrack
                        source: okAddTrack
                        radius: {
                            if(fastBlurRadius == -1)
                                visible = false
                            else
                                visible = true
                            return fastBlurRadius
                        }
                        transparentBorder: true
                    }

                    MouseArea {
                        id: clickedOkAddTrack
                        anchors.fill: okAddTrack
                        onReleased: {
                            var index = model.index
                            if(addAndCancel)
                            {
                                vkAudioModel[index].visibleColorRemove = false
                                vkAudioModel[index].visibleImageAdd = false
                                vkAudioModel[index].visibleColorItem = false
                                vkAudioModel[index].enableMouse = true
                                cancelRemoveTrack.enabled = true
                                item.removeTrack(vkAudioModel[index].idTrack, vkAudioModel[index].idUser, true)
                            }
                            else
                            {
                                item.addTrack(vkAudioModel[index].idTrack, vkAudioModel[index].idUser)
                                vkAudioModel[index].visibleColorAdd = true
                                okAddTrack.enabled = false
                            }
                        }
                    }
                }
            }
        }
    }

    ProgressBar {
        id: progressBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 20
        value: 0
        maximumValue: 100
        minimumValue: 0
        style: ProgressBarStyle {
            panel: Rectangle {
                color: "#444"
                opacity: 0.8

                Rectangle {
                    antialiasing: true
                    radius: 1
                    color: "#468bb7"
                    height: parent.height
                    width: parent.width * control.value / control.maximumValue
                }
            }
        }
    }

    Item {
        id: listFriend
        anchors.fill: parent
        visible: false

        PathView {
            id: pathView
            model: vkFriendModel
            anchors.fill: parent
            pathItemCount: 20
            path: Path {
                startX: 0
                startY: 0

                PathAttribute { name: "size"; value: sizePixNormal; }

                PathCurve {
                    y: pathView.height / 2 - 100
                    x: pathView.width / 2 / 2
                }

                PathAttribute { name: "size"; value: sizePixNormal; }
                PathPercent { value: 0.49; }
                PathLine { relativeX: 0; relativeY: 0; }
                PathAttribute { name: "size"; value: sizePixIncrease; }

                PathCurve {
                    y: pathView.height / 2 + 100
                    x: pathView.width / 2 / 2
                }

                PathAttribute { name: "size"; value: sizePixIncrease; }
                PathLine { relativeX: 0; relativeY: 0; }
                PathAttribute { name: "size"; value: sizePixNormal; }
                PathPercent { value: 0.51; }

                PathCurve {
                    x: 0
                    y: pathView.height
                }
            }
            onMovementEnded: {
                var index = Script.currentMidFriend(currentIndex)
                inputSearchFriend.text = vkFriendModel[index].nameFriend
            }
            delegate: Rectangle {
                id: currentAvatarFriend
                width: PathView.size + 2
                height: PathView.size + 2
                border.color: {
                    if(height == sizePixIncrease + 2 && width == sizePixIncrease + 2)
                        return "#33b5e5"
                    return "transparent"
                }
                border.width: 2

                Image {
                    source: {
                        if(idFriend == indefinite)
                            currentAvatarFriend.visible = false
                        return urlAvatar
                    }
                    anchors.centerIn: currentAvatarFriend
                    width: {
                        if(currentAvatarFriend.width == sizePixIncrease + 2)
                            return currentAvatarFriend.width - 2
                        return currentAvatarFriend.width
                    }
                    height: {
                        if(currentAvatarFriend.height == sizePixIncrease + 2)
                            return currentAvatarFriend.height - 2
                        return currentAvatarFriend.height
                    }
                }
            }

            MouseArea {
                anchors.fill: pathView
                onClicked: {
                    var index = Script.currentMidFriend(pathView.currentIndex)
                    inputSearchFriend.text = vkFriendModel[index].nameFriend
                }
            }
        }

        Rectangle {
            id: avatarMy
            y: pathView.height / 2 - 50
            x: pathView.width / 2 / 2 + 80
            width: sizePixIncrease
            height: sizePixIncrease
            border.color: "#33b5e5"
            border.width: 2

            Image {
                anchors.centerIn: avatarMy
                width: avatarMy.width - 2
                height: avatarMy.height - 2
                source: connectVkAudio.getUrlAvatarMy()
            }

            MouseArea {
                anchors.fill: avatarMy
                onClicked: inputSearchFriend.text = connectVkAudio.getNameAvatarMy()
            }
        }

        Rectangle {
            id: searchFriend
            anchors.verticalCenter: listFriend.verticalCenter
            width: pathView.width / 2 / 2 - 80
            height: 50
            color: "black"
            border.width: 1
            border.color: "#33b5e5"
            radius: 6

            RowLayout {
                anchors.fill: searchFriend
                anchors.leftMargin: 6
                anchors.rightMargin: 6
                width: 50
                spacing: 2

                Rectangle {
                    id: okSearchFriend
                    anchors.verticalCenter: searchFriend.verticalCenter
                    Layout.fillWidth: true
                    Layout.minimumWidth: 40
                    Layout.minimumHeight: 40
                    color: clickedOkSearchFriend.pressed ? "#222" : "transparent"
                    radius: 5

                    Image {
                        anchors.centerIn: okSearchFriend
                        source: "qrc:/icon/icon/ok.png"
                    }

                    MouseArea {
                        id: clickedOkSearchFriend
                        anchors.fill: okSearchFriend
                        onReleased: {
                            listFriend.visible = false
                            rightPanel.enabled = true
                            item.enabled = false
                            searchTrack.text = ""
                            globalSearchTrack.text = ""
                            item.deleteTrack()
                            if(inputSearchFriend.text === connectVkAudio.getNameAvatarMy().substring(0, inputSearchFriend.maximumLength))
                            {
                                avatarWindowImage.source = connectVkAudio.getUrlAvatarMy()
                                item.selectPlaylistMy()
                                addAndCancel = true
                                friendAudio = false
                                return
                            }
                            var index = Script.currentMidFriend(pathView.currentIndex)
                            avatarWindowImage.source = vkFriendModel[index].urlAvatar
                            inputSearchFriend.text = ""
                            item.selectPlaylistFriend(vkFriendModel[index].idFriend)
                            addAndCancel = false
                            friendAudio = true
                        }
                    }
                }

                Rectangle {
                    id: cancelSearchFriend
                    anchors.verticalCenter: searchFriend.verticalCenter
                    Layout.fillWidth: true
                    Layout.minimumWidth: 40
                    Layout.minimumHeight: 40
                    color: clickedCancelSearchFriend.pressed ? "#222" : "transparent"
                    radius: 5

                    Image {
                        anchors.centerIn: cancelSearchFriend
                        source: "qrc:/icon/icon/cancel.png"
                    }

                    MouseArea {
                        id: clickedCancelSearchFriend
                        anchors.fill: cancelSearchFriend
                        onReleased: {
                            listFriend.visible = false
                            rightPanel.enabled = true
                            fastBlurRadius = -1
                        }
                    }
                }

                TextField {
                    id: inputSearchFriend
                    Layout.fillWidth: true
                    Layout.minimumHeight: 40
                    font.pixelSize: 20
                    placeholderText: "search"
                    style: styleTextField
                    maximumLength: 11
                    Keys.onPressed: {
                        if(event.key === 16777219)
                            textColor = "white"
                    }
                    onAccepted: {
                        if(text.length == 0)
                            return

                        if(textSearchFriend == text)
                            Script.findFriend()
                        else
                        {
                            textSearchFriend = text
                            indexTextSearchFriend = 0
                            arrayTextSearchFriend = []
                            for(var i = 0; i < vkFriendModel.length; i++)
                            {
                                var friend = vkFriendModel[i].nameFriend.toLowerCase()
                                var input = text.toLowerCase()
                                if(indefinite.search(input) == -1)
                                    if(friend.search(input) !== -1)
                                        arrayTextSearchFriend.push(i)
                            }
                            if(arrayTextSearchFriend.length == 0)
                                textColor = "red"
                            else
                            {
                                textColor = "white"
                                Script.findFriend()
                            }
                        }
                    }
                }
            }
        }
    }

    Item {
        id: rightPanel
        anchors.left: scrollViewListTrack.right
        anchors.top: searchTrack.bottom
        width: 280
        height: parent.height - searchTrack.height - player.height - progressBar.height

        Rectangle {
            anchors.left: rightPanel.left
            width: 2
            height: rightPanel.height
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#237b9c"; }
                GradientStop { position: 0.7; color: "#33b5e5"; }
            }
        }

        ColumnLayout {
            anchors.fill: rightPanel
            anchors.leftMargin: 5

            TextField {
                id: globalSearchTrack
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: rightPanel.width - 5
                Layout.minimumHeight: 40
                font.pixelSize: 25
                placeholderText: "global search"
                onAccepted: {
                    searchTrack.text = ""
                    item.enabled = false
                    if(text.length != 0)
                    {
                        item.returnPressedGlobalSearchTrack(text)
                        item.deleteTrack()
                        addAndCancel = false
                    }
                    else
                    {
                        if(!friendAudio)
                            item.selectPlaylistMy()
                        else
                        {
                            var index = Script.currentMidFriend(pathView.currentIndex)
                            item.selectPlaylistFriend(vkFriendModel[index].idFriend)
                        }
                    }
                }
                style: styleTextField

                FastBlur {
                    anchors.fill: globalSearchTrack
                    source: globalSearchTrack
                    radius: fastBlurRadius
                    transparentBorder: true
                }
            }

            ScrollView {
                id: scrollViewRightPanel
                anchors.top: globalSearchTrack.bottom
                anchors.topMargin: 5
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: rightPanel.width - 8
                Layout.minimumHeight: rightPanel.height - globalSearchTrack.height - 5
                flickableItem.interactive: true
                style: scrollViewHandle

                ColumnLayout {
                    anchors.fill: scrollViewRightPanel

                    Rectangle {
                        id: myAudio
                        color: clickeMyAudio.pressed ? "#222" : "black"
                        width: rightPanel.width - 22
                        height: 40
                        border.width: 2
                        border.color: "#33b5e5"
                        radius: 6

                        FastBlur {
                            anchors.fill: myAudio
                            source: myAudio
                            radius: fastBlurRadius
                            transparentBorder: true
                        }

                        Text {
                            anchors.centerIn: myAudio
                            text: "My Audiorecord"
                            font.pixelSize: 20
                            color: "white"
                        }

                        MouseArea {
                            id: clickeMyAudio
                            anchors.fill: myAudio
                            onReleased: {
                                addAndCancel = false
                                friendAudio = false
                                fastBlurRadius = 5
                                item.enabled = false
                                searchTrack.text = ""
                                globalSearchTrack.text = ""
                                item.selectPlaylistMy()
                                avatarWindowImage.source = connectVkAudio.getUrlAvatarMy()
                            }
                        }
                    }

                    Rectangle {
                        id: currentPlaylist
                        color: clickeCurrentPlaylist.pressed ? "#222" : "black"
                        width: rightPanel.width - 22
                        height: 40
                        border.width: 2
                        border.color: "#33b5e5"
                        radius: 6

                        FastBlur {
                            anchors.fill: currentPlaylist
                            source: currentPlaylist
                            radius: fastBlurRadius
                            transparentBorder: true
                        }

                        Text {
                            anchors.centerIn: currentPlaylist
                            text: "Playlist User"
                            font.pixelSize: 20
                            color: "white"
                        }

                        MouseArea {
                            id: clickeCurrentPlaylist
                            anchors.fill: currentPlaylist
                            onReleased: {
                                addAndCancel = false
                                searchTrack.text = ""
                                fastBlurRadius = 5
                                item.enabled = false
                                globalSearchTrack.text = ""
                                if(!friendAudio)
                                    item.selectPlaylistMy()
                                else
                                {
                                    var index = Script.currentMidFriend(pathView.currentIndex)
                                    item.selectPlaylistFriend(vkFriendModel[index].idFriend)
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: recommendedRec
                        color: clickedRecommended.pressed ? "#222" : "black"
                        width: rightPanel.width - 22
                        height: 40
                        border.width: 2
                        border.color: "#33b5e5"
                        radius: 6

                        FastBlur {
                            anchors.fill: recommendedRec
                            source: recommendedRec
                            radius: fastBlurRadius
                            transparentBorder: true
                        }

                        Text {
                            anchors.centerIn: recommendedRec
                            text: "Recommendation"
                            font.pixelSize: 20
                            color: "white"
                        }

                        MouseArea {
                            id: clickedRecommended
                            anchors.fill: recommendedRec
                            onReleased: {
                                searchTrack.text = ""
                                globalSearchTrack.text = ""
                                fastBlurRadius = 5
                                item.enabled = false
                                addAndCancel = false
                                if(!friendAudio)
                                    item.selectPlaylistRecommended(connectVkAudio.getIdMy())
                                else
                                {
                                    var index = Script.currentMidFriend(pathView.currentIndex)
                                    item.selectPlaylistRecommended(vkFriendModel[index].idFriend)
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: popularRect
                        color: clickedPopular.pressed ? "#222" : "black"
                        width: rightPanel.width - 22
                        height: 40
                        border.width: 2
                        border.color: "#33b5e5"
                        radius: 6

                        FastBlur {
                            anchors.fill: popularRect
                            source: popularRect
                            radius: fastBlurRadius
                            transparentBorder: true
                        }

                        Text {
                            anchors.centerIn: popularRect
                            text: "Popular"
                            font.pixelSize: 20
                            color: "white"
                        }

                        MouseArea {
                            id: clickedPopular
                            anchors.fill: popularRect
                            onReleased: {
                                if(hideListGenre)
                                {
                                    listGenre.visible = false
                                    hideListGenre = false
                                }
                                else
                                {
                                    listGenre.visible = true
                                    hideListGenre = true
                                }
                            }
                        }
                    }

                    ListView {
                        id: listGenre
                        width: rightPanel.width - 22
                        height: 575
                        spacing: 2
                        interactive: false
                        visible: false
                        model: ListModel {
                            ListElement { genre: "Rock"; genreId: "1"; }
                            ListElement { genre: "Pop"; genreId: "2"; }
                            ListElement { genre: "Rap & Hip-Hop"; genreId: "3"; }
                            ListElement { genre: "Easy Listening"; genreId: "4"; }
                            ListElement { genre: "Dance & House"; genreId: "5"; }
                            ListElement { genre: "Instrumental"; genreId: "6"; }
                            ListElement { genre: "Metal"; genreId: "7"; }
                            ListElement { genre: "Alternative"; genreId: "21"; }
                            ListElement { genre: "Dubstep"; genreId: "8"; }
                            ListElement { genre: "Jazz & Blues"; genreId: "9"; }
                            ListElement { genre: "Drum & Bass"; genreId: "10"; }
                            ListElement { genre: "Trance"; genreId: "11"; }
                            ListElement { genre: "Chanson"; genreId: "12"; }
                            ListElement { genre: "Ethnic"; genreId: "13"; }
                            ListElement { genre: "Acoustic & Vocal"; genreId: "14"; }
                            ListElement { genre: "Reggae"; genreId: "15"; }
                            ListElement { genre: "Classical"; genreId: "16"; }
                            ListElement { genre: "Indie Pop"; genreId: "17"; }
                            ListElement { genre: "Speech"; genreId: "19"; }
                            ListElement { genre: "Electropop & Disco"; genreId: "22"; }
                            ListElement { genre: "Other"; genreId: "18"; }
                        }
                        delegate: Rectangle {
                            id: genreDelegat
                            color: clikedGenreDelegat.pressed ? "#222" : "black"
                            width: rightPanel.width - 22
                            height: 25
                            border.width: 1
                            border.color: "#33b5e5"
                            radius: 6

                            FastBlur {
                                anchors.fill: genreDelegat
                                source: genreDelegat
                                radius: fastBlurRadius
                                transparentBorder: true
                            }

                            Text {
                                id: txtGenre
                                anchors.centerIn: genreDelegat
                                text: genre
                                font.pixelSize: 20
                                color: "white"
                            }

                            MouseArea {
                                id: clikedGenreDelegat
                                anchors.fill: genreDelegat
                                onReleased: {
                                    addAndCancel = false
                                    searchTrack.text = ""
                                    fastBlurRadius = 5
                                    item.enabled = false
                                    globalSearchTrack.text = ""
                                    item.selectPlaylistPopular(genreId)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
