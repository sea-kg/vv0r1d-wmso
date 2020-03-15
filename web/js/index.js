
window.userInMoving = false;

function lockMoving() {
    window.userInMoving = true;
    setTimeout(function(){
        window.userInMoving = false;
    }, 450)
}

function userMove(rN,cN) {
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
    if (window.gameMap.user["row"] == rN && window.gameMap.user["cell"] == cN) {
        return;
    }

    if (window.userInMoving) {
        return;
    }
    lockMoving()

    if (window.gameMap.data[rN][cN].canMove) {
        window.gameMap.user["row"] = rN;
        window.gameMap.user["cell"] = cN;
    }
    renderAll();
}



function userMoveUp() {
    console.log("move up");
    var cN = window.gameMap.user["cell"];
    var rN = window.gameMap.user["row"] - 1;
    userMove(rN, cN);
}

function userMoveDown() {
    console.log("move down");
    var cN = window.gameMap.user["cell"];
    var rN = window.gameMap.user["row"] + 1;
    console.log("move down, rN = ", rN);
    userMove(rN, cN);
}

function userMoveLeft() {
    console.log("move left ");
    var cN = window.gameMap.user["cell"] - 1;
    var rN = window.gameMap.user["row"];
    userMove(rN, cN);
}

function userMoveRight() {
    console.log("move right ");
    var cN = window.gameMap.user["cell"] + 1;
    var rN = window.gameMap.user["row"];
    userMove(rN, cN);
}

window.gameMap = {
    countCellsInRow: 16,
    countRows: 9,
    user: {},
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

function selectRandomUserPos() {
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            var field = window.gameMap.data[r][c]
            if (field.canMove == true) {
                window.gameMap.user["row"] = r
                window.gameMap.user["cell"] = c
                return;
            }
        }
    }
}

function convertToElementId(r,c) {
    return "c" + ("" + r).padStart(2, "0") + ("" + c).padStart(2, "0")
}

function removeAllPositions(elId) {
    var el = document.getElementById(elId);
    for (var r = 0; r < window.gameMap.countRows; r++) {
        var cnRow = "gcw-r" + ("" + r).padStart(2, "0")
        el.classList.remove(cnRow);
    }
    for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
        var cnCell = "gcw-c" + ("" + c).padStart(2, "0")
        el.classList.remove(cnCell);
    }
}   

function renderAll() {
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            var elEd = convertToElementId(r,c)
            var el = document.getElementById(elEd);
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
    var classNameUserRow = "gcw-r" + ("" + userR).padStart(2, "0")
    var classNameUserCell = "gcw-c" + ("" + userC).padStart(2, "0")
    removeAllPositions("u01")
    var userEl = document.getElementById("u01");
    userEl.classList.add(classNameUserRow)
    userEl.classList.add(classNameUserCell)
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
    generateRandomMap();
    selectRandomUserPos();
    renderAll();
});
