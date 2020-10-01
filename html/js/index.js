
window.userInMoving = false;
window.maxY = 11;
window.maxX = 11;

function lockMoving() {
    window.userInMoving = true;
    setTimeout(function(){
        window.userInMoving = false;
    }, 450)
}

function userMove(newX,newY, direction) {
    if (window.userInMoving) {
        return;
    }
    window.userInMoving = true;
    if (newY < 0) {
        newY = 0;
    }
    if (newY >= window.gMap.countRows) {
        newY = window.gMap.countRows-1
    }

    if (newX < 0) {
        newX = 0;
    }
    if (newX >= window.gMap.countCellsInRow) {
        newX = window.gMap.countCellsInRow-1
    }
    console.log("move, newY = ", newY);
    console.log("move, newX = ", newX);
    console.log("move, direction = ", direction);
    
    var prevX = window.gMap.user["x"];
    var prevY = window.gMap.user["y"];
    
    // renderMap( window.gameMapPlayers, "canvas_game_map_players", window.gMap);

    setTimeout(function() {
        window.gameMapPlayers[prevX][prevY] = { name: "players/player0-" + direction + "-50x50", w: 50, h: 50 };
        renderMap( window.gameMapPlayers, "canvas_game_map_players", window.gMap);

        setTimeout(function() {
            window.gameMapPlayers[newX][newY] = { name: "players/player0-50x50", w: 50, h: 50 };
            window.gameMapPlayers[prevX][prevY] = { name: "objects/empty-50x50", w: 50, h: 50 };
            renderMap( window.gameMapPlayers, "canvas_game_map_players", window.gMap);
            window.gMap.user["x"] = newX;
            window.gMap.user["y"] = newY;
            window.userInMoving = false; 
        }, 250);

    }, 250);
}



function userMoveUp() {
    console.log("move up");
    var x = window.gMap.user["x"];
    var y = window.gMap.user["y"] - 1;
    userMove(x, y, "up");
}

function userMoveDown() {
    console.log("move down");
    var x = window.gMap.user["x"];
    var y = window.gMap.user["y"] + 1;
    userMove(x, y, "down");
}

function userMoveLeft() {
    console.log("move left ");
    var x = window.gMap.user["x"] - 1;
    var y = window.gMap.user["y"];
    userMove(x, y, "left");
}

function userMoveRight() {
    console.log("move right ");
    var x = window.gMap.user["x"] + 1;
    var y = window.gMap.user["y"];
    userMove(x, y, "right");
}

document.addEventListener("keydown", event => {
    console.log(event.keyCode);
    if (event.keyCode == 38) {
        userMoveUp();
    } else if (event.keyCode == 40) {
        userMoveDown();
    } else if (event.keyCode == 37) {
        userMoveLeft()
    } else if (event.keyCode == 39) {
        userMoveRight()
    }
});



window.cacheImages = {};

function loadImageToCache(name, path, callback) {
    var ret = {
        img: new Image(),
        state: 'loading',
    };
    ret.img.addEventListener("load", function() {
        console.log("Loaded: " + name);
        ret.state = 'loaded';
        if (callback) {
            callback(name, ret.img);
        }
    }, false);
    ret.img.addEventListener("error", function() {
        console.log("Error: " + name);
        ret.state = 'error';
        ret.img = null;
        if (callback) {
            callback(name, ret.img);
        }
    }, false);
    ret.img.src = path;
    return ret;
}

// loadImage("ground0")
function loadImage(name, callback) {
    if (!window.cacheImages[name]) {
        window.cacheImages[name] = loadImageToCache(name, "./static/img/" + name + ".png", callback);
        return;
    }
    if (callback) {
        callback(name, window.cacheImages[name].img);
    }
}

function loadImages(names, callback) {
    var counter = 0;
    var imagesLen = names.length;
    var ret = {};
    for (var i in names) {
        loadImage(names[i], function(name, img) {
            counter++;
            ret[name] = img;
            if (counter >= imagesLen) {
                callback(ret);
            }
        })
    }
}

function renderMap(mapInfo, elemId) {
    var imgNames = [] 
    for (var r = 0; r < window.gMap.countRows; r++) {
        for (var c = window.gMap.countCellsInRow - 1; c >= 0; c--) {
            var name = mapInfo[c][r].name;
            if (imgNames.indexOf(name) === -1) {
                imgNames.push(name);
            }
            // console.log("r = ", r);
            // console.log("c = ", c);
        }
    }
    console.log("imgNames: ", imgNames)
    var _mapInfo = mapInfo;
    var _elemId = elemId;
    loadImages(imgNames, function(imgs) {
        console.log("imgs", imgs);

        var cgm_background = document.getElementById(elemId);
        cgm_background.style['left'] = window.gMap.paddingLeft + 'px';
        cgm_background.style['top'] = window.gMap.paddingTop + 'px';
        var ctx = cgm_background.getContext('2d');
        // cgm_background.width = main_w - pad_w;
        // cgm_background.height = main_h - pad_h;
        ctx.clearRect(0, 0, cgm_background.width, cgm_background.height);

        for (var r = 0; r < window.gMap.countRows; r++) {
            for (var c = window.gMap.countCellsInRow - 1; c >= 0; c--) {
                var name = mapInfo[c][r].name;
                var w = mapInfo[c][r].w
                var h = mapInfo[c][r].h
                var img = imgs[name];
                var dx = img.width - w;
                var dy = img.height - h;
                var y = r * h + window.gMap.paddingTop - dy;
                var x = c * h + window.gMap.paddingLeft; // - dx;
                ctx.drawImage(img, x, y);
            }
        }

    })
    // Rendering after load all pictures
}

var userWannaBuildObj = '';

function userWannaBuild(ob) {
    userWannaBuildObj = ob;

    // canvas_game_map_players.style.cursor = "url('" + userWannaBuildObj + ".png'), auto"
}

function userClickOnMap(e) {

    if (userWannaBuildObj != '') {
        var x = e.offsetX - window.gMap.paddingLeft;
        var y = e.offsetY - window.gMap.paddingTop;
        // console.log ("x = ", x, " y = ", y);
    
        var c = (x - (x % 50)) / 50;
        var r = (y - (y % 50)) / 50;
    
        // console.log("c= ", c, " r=", r);
        
        window.gameMapObject[c][r] = { name: userWannaBuildObj, w: 50, h: 50 };
        userWannaBuildObj = '';
        renderMap( window.gameMapObject, "canvas_game_map_objects", window.gMap);
    }
}

document.addEventListener('DOMContentLoaded', function(){
    console.log("Content loaded");
    var main_w = window.innerWidth - 200;
    var main_h = window.innerHeight - 50;
    var pad_w = main_w % 50;
    var pad_h = main_h % 50;
    gMap.paddingLeft = Math.floor(pad_w / 2);
    gMap.paddingTop = Math.floor(pad_h / 2);
    gMap.countRows = (main_h - pad_h) / 50;
    gMap.countCellsInRow = (main_w - pad_w) / 50;
    console.log(gMap);
    
    var canvas_game_map_background = document.getElementById("canvas_game_map_background");
    canvas_game_map_background.style['left'] = gMap.paddingLeft + 'px';
    canvas_game_map_background.style['top'] = gMap.paddingTop + 'px';
    canvas_game_map_background.width = main_w - pad_w;
    canvas_game_map_background.height = main_h - pad_h;

    var canvas_game_map_objects = document.getElementById("canvas_game_map_objects");
    canvas_game_map_objects.style['left'] = gMap.paddingLeft + 'px';
    canvas_game_map_objects.style['top'] = gMap.paddingTop + 'px';
    canvas_game_map_objects.width = main_w - pad_w;
    canvas_game_map_objects.height = main_h - pad_h;
    
    var canvas_game_map_players = document.getElementById("canvas_game_map_players");
    canvas_game_map_players.style['left'] = gMap.paddingLeft + 'px';
    canvas_game_map_players.style['top'] = gMap.paddingTop + 'px';
    canvas_game_map_players.width = main_w - pad_w;
    canvas_game_map_players.height = main_h - pad_h;

    canvas_game_map_players.onmousedown = userClickOnMap;

    // var ctx = cgm_background.getContext('2d');
    renderMap( window.gameMapBackground, "canvas_game_map_background", window.gMap);
    renderMap( window.gameMapObject, "canvas_game_map_objects", window.gMap);
    renderMap( window.gameMapPlayers, "canvas_game_map_players", window.gMap);
    
});
