
function userMoveUp() {
    console.log("move up");
}

function userMoveDown() {
    console.log("move down");
}

function userMoveLeft() {
    console.log("move left");
}

function userMoveRight() {
    console.log("move right");
}

window.gameMap = {
    countCellsInRow: 16,
    countRows: 9,
    data: {}
}

function generateRandomMap() {
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            if (!window.gameMap[r]) {
                window.gameMap[r] = {}
            }
            if (!window.gameMap[r][c]) {
                window.gameMap[r][c] = {}
            }
            window.gameMap[r][c] = {
                "canMove": Math.random() >= 0.5
            }
        }
    }
}

function selectRandomUserPos() {
    
}

function convertToElementId(r,c) {
    return "c" + ("" + r).padStart(2, "0") + ("" + c).padStart(2, "0")
}

function renderMap() {
    for (var r = 0; r < window.gameMap.countRows; r++) {
        for (var c = 0; c < window.gameMap.countCellsInRow; c++) {
            var elEd = convertToElementId(r,c)
            var el = document.getElementById(elEd);
            var field = window.gameMap[r][c]
            if (field.canMove == true) {
                if (el.classList.contains("deny-move")) {
                    el.classList.remove("deny-move")
                }
            } else {
                if (!el.classList.contains("deny-move")) {
                    el.classList.add("deny-move")
                }
            }

            console.log(elEd)
        }
    }
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
    renderMap();
    selectRandomUserPos();
});
