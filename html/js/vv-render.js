
class VvRender {
    constructor(canvasid) {
        this.canvas = document.getElementById(canvasid);
       
        this.ctx = this.canvas.getContext("2d");
        this.cacheImages = {};
        this.left_panel_width = 200;
        this.bottom_panel_height = 50;
        this.perf = [];
        this.layer_background = [];
        this.layer_roads = [];
        this.layer_vegetation = [];
        this.layer_buildings = [];
        this.player_draw_coordinates = {};
        this.topLeftRealX = 0;
        this.topLeftRealY = 0;
        this.render_frame_player = {x: 0, y: 0, d: 50, t: 0};
        this.player_coordinates = null;
        this.highlight_road = null;
        this.player_target_coordinates = {
            x: 0,
            y: 0
        }

        var self = this;
        this.canvas.onmousedown = function(e) {
            self.__render_onclick(e);
        }

        this.canvas.onmousemove = function(e) {
            self.__render_onmousemove(e);
        }

        // borders
        this.loadImages([
            "borders0",
            "menu-build",
            "menu-destroy",
        ], function() {
                self.draw_borders();
            }
        );
    }

    resized() {
        this.player_draw_coordinates = {
            x: (window.innerWidth - this.left_panel_width) / 2,
            y: (window.innerHeight - this.bottom_panel_height) / 2
        };
    }

    loadImageToCache(name, path, callback) {
        var ret = {
            img: new Image(),
            state: 'loading',
        };
        ret.img.addEventListener("load", function() {
            // console.log("Loaded: " + name);
            ret.state = 'loaded';
            if (callback) {
                callback(name, ret.img);
            }
        }, false);
        ret.img.addEventListener("error", function() {
            console.error("Error: " + name);
            ret.state = 'error';
            ret.img = null;
            if (callback) {
                callback(name, ret.img);
            }
        }, false);
        ret.img.src = path;
        return ret;
    }

    loadImage(name, callback) {
        if (!this.cacheImages[name]) {
            this.cacheImages[name] = this.loadImageToCache(name, "./img/" + name + ".png", callback);
            return;
        }
        if (callback) {
            callback(name, this.cacheImages[name].img);
        }
    }

    loadImages(names, callback) {
        var counter = 0;
        var imagesLen = names.length;
        var ret = {};
        for (var i in names) {
            this.loadImage(names[i], function(name, img) {
                counter++;
                ret[name] = img;
                if (counter >= imagesLen) {
                    callback(ret);
                }
            })
        }
    }

    add_background_object(obj) {
        this.layer_background.push(obj);
        if (!this.cacheImages[obj.t]) {
            this.loadImage(obj.t)
        }
    }

    add_road_object(obj) {
        this.layer_roads.push(obj);
        if (!this.cacheImages[obj.t]) {
            this.loadImage(obj.t)
        }
    }

    add_vegetation_object(obj) {
        this.layer_vegetation.push(obj);
        if (!this.cacheImages[obj.t]) {
            this.loadImage(obj.t)
        }
    }

    add_building_object(obj) {
        this.layer_buildings.push(obj);
        if (!this.cacheImages[obj.t]) {
            this.loadImage(obj.t)
        }
    }

    draw_background() {
        for (var i in this.layer_background) {
            var obj = this.layer_background[i];
            // console.log(obj);
            if (this.cacheImages[obj.t] && this.cacheImages[obj.t].state == 'loaded') {
                this.ctx.drawImage(this.cacheImages[obj.t].img,
                    obj.x - this.topLeftRealX,
                    obj.y - this.topLeftRealY
                );
            }
        }
    }

    draw_roads() {
        for (var i in this.layer_roads) {
            var obj = this.layer_roads[i];
            // console.log(obj);
            if (this.cacheImages[obj.t] && this.cacheImages[obj.t].state == 'loaded') {
                this.ctx.drawImage(
                    this.cacheImages[obj.t].img,
                    obj.x - this.topLeftRealX,
                    obj.y - this.topLeftRealY,
                );
            }
        }

        if (this.highlight_road) {
            this.ctx.fillStyle = 'rgba(255,0,0,0.2)';
            this.ctx.fillRect(
                this.highlight_road.x - this.topLeftRealX,
                this.highlight_road.y - this.topLeftRealY,
                this.highlight_road.w,
                this.highlight_road.h
            )
            // TODO if radious < 100 so show menu object
        }
    }

    draw_vegetation() {
        for (var i in this.layer_vegetation) {
            var obj = this.layer_vegetation[i];
            // console.log(obj);
            if (this.cacheImages[obj.t] && this.cacheImages[obj.t].state == 'loaded') {
                this.ctx.drawImage(
                    this.cacheImages[obj.t].img,
                    obj.x - this.topLeftRealX,
                    obj.y - this.topLeftRealY,
                );
            }
        }
    }

    draw_buildings() {
        for (var i in this.layer_buildings) {
            var obj = this.layer_buildings[i];
            // console.log(obj);
            if (this.cacheImages[obj.t] && this.cacheImages[obj.t].state == 'loaded') {
                this.ctx.drawImage(
                    this.cacheImages[obj.t].img,
                    obj.x - this.topLeftRealX,
                    obj.y - this.topLeftRealY,
                );
            }
        }
    }

    draw_borders() {
        this.ctx.fillStyle = '#b04333';
        // bottom panel
        this.ctx.fillRect(0, window.innerHeight - this.bottom_panel_height, window.innerWidth, this.bottom_panel_height);

        // left panel
        this.ctx.fillRect(window.innerWidth - this.left_panel_width, 0, this.left_panel_width, window.innerHeight);

        if (this.cacheImages["borders0"] && this.cacheImages["borders0"].state == 'loaded') {
            var bs = 400;
            var ans = 150;
            var ow = 50;
            var oh = 25;
            for (var y = ans; y < window.innerHeight - this.bottom_panel_height - ans + 2*oh; y += oh) {
                // left
                this.ctx.drawImage(this.cacheImages["borders0"].img, 0, ans, ow, oh, 0, y, ow, oh);
                // right
                this.ctx.drawImage(this.cacheImages["borders0"].img, bs - oh, ans, oh, ow, window.innerWidth - this.left_panel_width - oh, y, oh, ow);
            }
            for (var x = ans; x < window.innerWidth - this.left_panel_width - ans + 2*ow; x += ow) {
                // top
                this.ctx.drawImage(this.cacheImages["borders0"].img, ans, 0, ow, oh, x, 0, ow, oh);
                // bottom
                this.ctx.drawImage(this.cacheImages["borders0"].img, ans, bs - oh, ow, oh, x, window.innerHeight - this.bottom_panel_height - oh, ow, oh);
            }
            // top left
            this.ctx.drawImage(this.cacheImages["borders0"].img, 0, 0, ans, ans, 0, 0, ans, ans);
            // top right
            this.ctx.drawImage(this.cacheImages["borders0"].img, bs - ans, 0, ans, ans, window.innerWidth - this.left_panel_width - ans, 0, ans, ans);
            // bottom right
            this.ctx.drawImage(this.cacheImages["borders0"].img, bs - ans, bs - ans, ans, ans, window.innerWidth - this.left_panel_width - ans, window.innerHeight - this.bottom_panel_height - ans, ans, ans);
            // bottom left
            this.ctx.drawImage(this.cacheImages["borders0"].img, 0, bs - ans, ans, ans, 0, window.innerHeight - this.bottom_panel_height - ans, ans, ans);
        }

        // draw menu
        if (this.cacheImages["menu-destroy"] && this.cacheImages["menu-destroy"].state == 'loaded') {
            this.ctx.drawImage(this.cacheImages["menu-destroy"].img, 
                window.innerWidth - this.left_panel_width0 + 50, 
                50, 50, 50);
        }
    }

    draw_player() {
        const texturePlayer = "players/mafuyu";
        if (!this.cacheImages[texturePlayer]) {
            this.loadImage(texturePlayer);
        }
        if (this.cacheImages[texturePlayer] && this.cacheImages[texturePlayer].state == 'loaded') {

            // line to target
            this.ctx.beginPath(); 
            this.ctx.strokeStyle = 'rgba(255,255,255,0.3)';
            this.ctx.fillStyle = 'rgba(255,255,255,0.1)';
            this.ctx.moveTo(
                this.player_draw_coordinates.x,
                this.player_draw_coordinates.y
            );
            this.ctx.lineTo(
                this.player_target_coordinates.x - this.topLeftRealX,
                this.player_target_coordinates.y - this.topLeftRealY,
            );
            this.ctx.lineWidth = 4;
            
            this.ctx.stroke();
            
            this.ctx.beginPath();
            this.ctx.arc(
                this.player_target_coordinates.x - this.topLeftRealX,
                this.player_target_coordinates.y - this.topLeftRealY,
                25, 0, 2 * Math.PI, false);
            this.ctx.fill();
            
            this.ctx.drawImage(
                this.cacheImages[texturePlayer].img,
                this.render_frame_player.x,
                this.render_frame_player.y,
                50, 50,
                this.player_draw_coordinates.x - 25,
                this.player_draw_coordinates.y - 45,
                50, 50
            );
            this.moveppl += 50;
            if (this.moveppl >= 200) {
                this.moveppl = 0;
            }
        }

        if (this.player_coordinates) {
            this.ctx.fillStyle = "#000";
            this.ctx.fillText(
                "{x,y}={" + this.player_coordinates.x + "," + this.player_coordinates.y + "}",
                20,
                window.innerHeight - 40
            );
        }
        
    }

    move_player() {
        if (!this.player_coordinates) {
            return;
        }
        if (
            Math.hypot(
                this.player_target_coordinates.x - this.player_coordinates.x,
                this.player_target_coordinates.y - this.player_coordinates.y
            ) > 5
        ) {
            var angel = Math.atan2(
                this.player_target_coordinates.x - this.player_coordinates.x,
                this.player_target_coordinates.y - this.player_coordinates.y
            )
            
            const leftDown  = -1 * Math.PI / 4;
            const leftTop  = -3 * Math.PI / 4;
            const rightTop =  3 * Math.PI / 4;
            const rightDown =  Math.PI / 4;
            
            if (angel >= rightDown && angel <= rightTop) {
                this.render_frame_player.y = 100;
            } else if (angel > leftTop && angel <= leftDown) {
                this.render_frame_player.y = 50;
            } else if (angel > leftDown && angel <= rightDown) {
                this.render_frame_player.y = 0;
            } else if (angel > rightTop || angel <= leftTop) {
                this.render_frame_player.y = 150;
            } else {
                this.render_frame_player.y = 0;
            }
            const t = new Date().getTime();
            if (t - this.render_frame_player.t > 100) { // every 10 frames
                this.render_frame_player.x += this.render_frame_player.d;
                if (this.render_frame_player.x >= 200) {
                    this.render_frame_player.x = 0;
                }
                this.render_frame_player.t = t;
            }
            
            // console.log(angel);
            this.player_coordinates.x += parseInt(Math.sin(angel)*5);
            this.player_coordinates.y += parseInt(Math.cos(angel)*5);
        } else {
            this.render_frame_player = {
                x: 0,
                y: 0,
                d: 50,
                t: 0
            }
        }
    }

    update() {
        var _perf_start = performance.now();
        this.ctx.fillStyle = 'black';
        this.ctx.fillRect(0,0,window.innerWidth, window.innerHeight);

        if (!this.player_coordinates) {
            this.draw_borders();
            return;
        }

        this.move_player();
        this.topLeftRealX = this.player_coordinates.x - this.player_draw_coordinates.x;
        this.topLeftRealY = this.player_coordinates.y - this.player_draw_coordinates.y;
        
        this.draw_background();
        this.draw_roads();
        this.draw_vegetation();
        this.draw_buildings();
        this.draw_borders();
        this.draw_player();
        this.update_perf(performance.now() - _perf_start);
    }

    update_perf(new_val_perf) {
        this.perf.push(new_val_perf);
        while (this.perf.length > 50) {
            this.perf.splice(0,1);
        }
        var _perf_avarage = 0;
        for (var i = 0; i < this.perf.length; i++) {
            _perf_avarage += this.perf[i];
        }
        _perf_avarage = _perf_avarage / this.perf.length;
        // console.log("avarage perf = ", _perf_avarage, "ms, length " + this.perf.length);
        this.ctx.fillStyle = "#000";
        this.ctx.fillText(
            _perf_avarage.toFixed(3) + "ms",
            20,
            window.innerHeight - 20
        );
    }

    __render_onclick(e) {
        var x = e.offsetX;
        var y = e.offsetY;
        
        if (
            y > window.innerHeight - this.bottom_panel_height
            || x > window.innerWidth - this.left_panel_width) {
            return;
        }

        // console.log(this.player_coordinates);

        if (this.player_coordinates) {
            this.player_target_coordinates = {
                x: this.player_coordinates.x + (x - this.player_draw_coordinates.x),
                y: this.player_coordinates.y + (y - this.player_draw_coordinates.y),
            }
            console.log(this.player_target_coordinates);
            vvapi.ws_player_move_to(this.player_target_coordinates.x, this.player_target_coordinates.y);
        }
    }

    __render_onmousemove(e) {
        var x = e.offsetX;
        var y = e.offsetY;
        x = this.topLeftRealX + x;
        y = this.topLeftRealY + y;

        console.log("x,y = ", x, y);
        // TODO highlight objects
        var road_highlighted = false;
        for (var i in this.layer_roads) {
            var r = this.layer_roads[i];
            if (
                x >= r.x && x <= r.x + r.w
                && y >= r.y && y <= r.y + r.h
            ) {
                road_highlighted = true;
                this.highlight_road = r;
            }
        }
        if (!road_highlighted) {
            this.highlight_road = null;
        }
    }

};
