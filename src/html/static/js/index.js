
window.userInMoving = false;
window.maxY = 11;
window.maxX = 11;

function lockMoving() {
    window.userInMoving = true;
    setTimeout(function(){
        window.userInMoving = false;
    }, 450)
}

function userMove(rN,cN, direction) {
    if (rN < 0) {
        rN = 0;
    }
    if (rN >= window.gameMap.countRows) {
        rN = window.gameMap.countRows-1
    }

    if (cN < 0) {
        cN = 0;
    }
    if (cN >= window.gameMap.countCellsInRow) {
        cN = window.gameMap.countCellsInRow-1
    }
    console.log("move, rN = ", rN);
    console.log("move, cN = ", cN);
    console.log("move, direction = ", direction);
    
    if (
        window.gameMap.user["row"] == rN 
        && window.gameMap.user["cell"] == cN
        && window.gameMap.user["direction"] == direction
    ) {
        return;
    }

    if (window.userInMoving) {
        return;
    }
    lockMoving()

    if (window.gameMap.data[rN][cN].canMove) {
        window.gameMap.user["row"] = rN;
        window.gameMap.user["cell"] = cN;
        window.gameMap.user["direction"] = direction;
    }
    renderAll();
}



function userMoveUp() {
    console.log("move up");
    var cN = window.gameMap.user["cell"];
    var rN = window.gameMap.user["row"] - 1;
    userMove(rN, cN, "up");
}

function userMoveDown() {
    console.log("move down");
    var cN = window.gameMap.user["cell"];
    var rN = window.gameMap.user["row"] + 1;
    console.log("move down, rN = ", rN);
    userMove(rN, cN, "down");
}

function userMoveLeft() {
    console.log("move left ");
    var cN = window.gameMap.user["cell"] - 1;
    var rN = window.gameMap.user["row"];
    userMove(rN, cN, "left");
}

function userMoveRight() {
    console.log("move right ");
    var cN = window.gameMap.user["cell"] + 1;
    var rN = window.gameMap.user["row"];
    userMove(rN, cN, "right");
}

window.gameMap = {
    countCellsInRow: 11,
    countRows: 11,
    user: {
        row: 5,
        cell: 5
    },
    data: {}
}

function generateRandomMap() {
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            if (!window.gameMap.data[r]) {
                window.gameMap.data[r] = {}
            }
            if (!window.gameMap.data[r][c]) {
                window.gameMap.data[r][c] = {}
            }
            window.gameMap.data[r][c] = {
                "canMove": Math.random() >= 0.2
            }
        }
    }
}

function convertToElementId(r,c) {
    return "c" + ("" + r).padStart(2, "0") + ("" + c).padStart(2, "0")
}

function renderAll() {
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            var elEd = convertToElementId(r,c)
            var el = document.getElementById(elEd);
            if (!el) {
                console.error("not found elEd: ", elEd)
            }
            var field = window.gameMap.data[r][c]
            if (field.canMove == true) {
                if (el.classList.contains("deny-move")) {
                    el.classList.remove("deny-move")
                }
            } else {
                if (!el.classList.contains("deny-move")) {
                    el.classList.add("deny-move")
                }
            }
        }
    }
    var userR = window.gameMap.user["row"];
    var userC = window.gameMap.user["cell"];
    // var elId = "c" + ("" + userR).padStart(2, "0") + ("" + userC).padStart(2, "0");
    // removeAllPositions("u01")
    var userEl = document.getElementById("u01");
    userEl.style.top = (userR*50) + "px";
    userEl.style.left = (userC*50) + "px";
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

window.gMap = {
    paddingLeft: 0,
    paddingTop: 0,
    countRows: 1,
    countCellsInRow: 1,
}

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

        var cgm_background = document.getElementById("canvas_game_map_background");
        cgm_background.style['left'] = window.gMap.paddingLeft + 'px';
        cgm_background.style['top'] = window.gMap.paddingTop + 'px';
        var ctx = cgm_background.getContext('2d');
        // cgm_background.width = main_w - pad_w;
        // cgm_background.height = main_h - pad_h;
        
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
    
    var cgm_background = document.getElementById("canvas_game_map_background");
    cgm_background.style['left'] = gMap.paddingLeft + 'px';
    cgm_background.style['top'] = gMap.paddingTop + 'px';
    cgm_background.width = main_w - pad_w;
    cgm_background.height = main_h - pad_h;

    // var ctx = cgm_background.getContext('2d');
    renderMap( window.gameMapBackground, "canvas_game_map_background", window.gMap);
    renderMap( window.gameMapObject, "canvas_game_map_objects", window.gMap);
    
});
