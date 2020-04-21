
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

document.addEventListener('DOMContentLoaded', function(){
    console.log("Content loaded");
    
    var _content = "";
    console.log(window.gameMap);
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            _content += '<div id="' + convertToElementId(r,c) + '" '
                    + ' style="width: 50px; height: 50px; top: ' + (r * 50) + 'px; left: ' + (c * 50) + 'px;" '
                    + ' class="gcw-field"></div>';
            
            
        }
    }
    _content += '<div id="u01" class="gcw-field-user"></div>';

    var el = document.getElementById("game_fields");
    el.innerHTML = _content;
    el.style.width = (window.gameMap.countRows * 50) + "px";
    el.style.height = (window.gameMap.countCellsInRow * 50) + "px";

    console.log(_content);
    // 
    // el.innerHTML = 
    

    generateRandomMap();
    renderAll();
});
