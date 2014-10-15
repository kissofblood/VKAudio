
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

function findFriend() {
    if(indexTextSearchFriend === arrayTextSearchFriend.length)
        indexTextSearchFriend = 0
    pathView.positionViewAtIndex(arrayTextSearchFriend[indexTextSearchFriend], PathView.Center)
    indexTextSearchFriend += 1
}
