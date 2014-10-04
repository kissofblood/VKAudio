import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtGraphicalEffects 1.0

Item {
    id: item

    property int widthGroove: 500
    property int sizePixNormal: 60
    property int sizePixIncrease: 100
    property bool isPlay: true
    property bool isLoop: false
    property bool isRandom: false
    property string indefinite: "indefinite"
    property real fastBlurRadius: -1

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
    signal selectPlaylistMy()
    signal clickedDownloadTrack(string name)
    signal returnPressedSearch(string search)

    function currentMidFriend(currentIndex) {
        var pathCount
        if(pathView.count < pathView.pathItemCount)
            pathCount = pathView.count / 2
        else
            pathCount = pathView.pathItemCount / 2

        var currentIndexMid = currentIndex + pathCount
        if(currentIndexMid < pathView.count)
            return currentIndexMid
        return currentIndexMid - pathView.count
    }

    width: 800
    height: 1280

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
                        listView.model.index = next
                        break
                    }
            }
            else for(var prev = vkAudioModel.length - 1; prev >= 0; prev--)
                if(vkAudioModel[prev].idTrack === id)
                {
                    listView.currentIndex = prev
                    listView.model.index = prev
                    break
                }
        }
        onNextTrackDefault: item.selectNextTrack(vkAudioModel[listView.currentIndex].idTrack)
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
                    source: "image://avatarMy/" + connectVkAudio.getIdAvatarMy()
                }

                MouseArea {
                    id: clickedFriend
                    anchors.fill: avatarWindow
                    onClicked: {
                        listFriend.visible = true
                        inputSearchFriend.focus = true
                        fastBlurRadius = 5
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
                    id: addTrackImage
                    anchors.verticalCenter: addTrack.verticalCenter
                    source: "qrc:/icon/icon/add.png"
                }

                MouseArea {
                    id: clickedAdd
                    anchors.fill: addTrack
                }

                FastBlur {
                    anchors.fill: addTrack
                    source: addTrackImage
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
        onEditingFinished: item.returnPressedSearch(text)
        style: styleTextField

        FastBlur {
            anchors.fill: searchTrack
            source: searchTrack
            radius: fastBlurRadius
            transparentBorder: true
        }
    }

    ScrollView {
        id: scrollView
        anchors.top: searchTrack.bottom
        width: parent.width
        height: parent.height - searchTrack.height - player.height - progressBar.height
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
            id: listView
            anchors.leftMargin: 6
            model: vkAudioModel
            delegate: Item {
                width: parent.width
                height: 70

                Rectangle {
                    id: qwer
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
                    onClicked: {
                        listView.currentIndex = model.index
                        item.selectIdTrack(vkAudioModel[listView.currentIndex].idTrack)
                    }
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
                var index = currentMidFriend(currentIndex)
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
                        return "image://avatarFriend/" + idFriend
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
                    var index = currentMidFriend(pathView.currentIndex)
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
                id: qwe
                anchors.centerIn: avatarMy
                width: avatarMy.width - 2
                height: avatarMy.height - 2
                source: "image://avatarMy/" + connectVkAudio.getIdAvatarMy()
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
                            fastBlurRadius = -1
                            if(inputSearchFriend.text === connectVkAudio.getNameAvatarMy()
                                    .substring(0, inputSearchFriend.maximumLength))
                            {
                                avatarWindowImage.source = "image://avatarMy/" + connectVkAudio.getIdAvatarMy()
                                item.selectPlaylistMy()
                                return
                            }
                            var index = currentMidFriend(pathView.currentIndex)
                            avatarWindowImage.source = "image://avatarFriend/" + vkFriendModel[index].idFriend
                            inputSearchFriend.text = ""
                            item.selectPlaylistFriend(vkFriendModel[index].idFriend)                        }
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
                    onEditingFinished: {
                        for(var i = 0; i < vkFriendModel.length; i++)
                        {
                            var friend = vkFriendModel[i].nameFriend.toLowerCase()
                            var input = text.toLowerCase()
                            if(indefinite.search(input.toLowerCase()) == -1)
                                if(friend.toLowerCase().search(input.toLowerCase()) !== -1)
                                    pathView.currentIndex = currentMidFriend(i);
                        }
                    }
                }
            }
        }
    }
}
