
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

function setVisibleAdd(value) {
    for(var i = 0; i < vkAudioModel.length; i++)
        vkAudioModel[i].visibleImageAdd = value
}

function setVisibleCancel(value) {
    for(var i = 0; i < vkAudioModel.length; i++)
        vkAudioModel[i].visibleImageCancel = value
}
