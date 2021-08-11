
document.addEventListener("keydown", event => {
    // console.log(event.keyCode);
    if (event.keyCode == 38) {
        // userMoveUp();
    } else if (event.keyCode == 40) {
        // userMoveDown();
    } else if (event.keyCode == 37) {
        // userMoveLeft()
    } else if (event.keyCode == 39) {
        // userMoveRight()
    }
});

document.addEventListener('DOMContentLoaded', function(){
    console.log("Content loaded");

    resize_canvas();

    window.vvrender = new VvRender('game_window_render');
    window.vvapi = new VvApi();

    vvapi.load_map().done(function(result) {
        var objects = result["objects"];
        for (var i in objects) {
            var obj = objects[i];
            if (obj.l == VVAPI_LAYER_BACKGROUND) {
                vvrender.add_background_object(obj)
            } else if (obj.l == VVAPI_LAYER_ROADS) {
                vvrender.add_road_object(obj)
            } else if (obj.l == VVAPI_LAYER_VEGETATION) {
                vvrender.add_vegetation_object(obj)
            } else if (obj.l == VVAPI_LAYER_BUILDING) {
                vvrender.add_building_object(obj)
            } else {
                console.error("Unknown layer")
            }
        }
        console.log("game_map = ", result);
        vvrender.resized();
        vvrender.update();
    });
});

function resize_canvas() {

    // console.log(window.innerWidth);
    var el = document.getElementById('game_window_render');
    el.style['width'] = window.innerWidth + 'px';
    el.style['height'] = window.innerHeight + 'px';
    el.width = window.innerWidth;
    el.height = window.innerHeight;
    
    
    if (window.vvrender) {
        window.vvrender.resized();
        window.vvrender.update();
    }
}

window.addEventListener("resize", resize_canvas);

setInterval(() => {
    if (window.vvrender) {
        window.vvrender.update();
    }
}, 40);