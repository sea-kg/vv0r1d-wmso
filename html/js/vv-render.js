
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
        this.player_coordinates = {
            x: 20,
            y: 120
        };

        this.player_target_coordinates = {
            x: 20,
            y: 120
        }

        var self = this;
        this.canvas.onmousedown = function(e) {
            self.__render_onclick(e);
        }

        // borders
        this.loadImages(
            [
                "borders/border-bottom",
                "borders/border-bottom-left",
                "borders/border-bottom-right",
                "borders/border-left",
                "borders/border-right",
                "borders/border-top",
                "borders/border-top-left",
                "borders/border-top-right",
            ],
            function() {
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
                    obj.x + this.topLeftRealX,
                    obj.y + this.topLeftRealY
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
                    obj.x + this.topLeftRealX,
                    obj.y + this.topLeftRealY,
                );
            }
        }
    }

    draw_vegetation() {
        for (var i in this.layer_vegetation) {
            var obj = this.layer_vegetation[i];
            // console.log(obj);
            if (this.cacheImages[obj.t] && this.cacheImages[obj.t].state == 'loaded') {
                this.ctx.drawImage(
                    this.cacheImages[obj.t].img,
                    obj.x + this.topLeftRealX,
                    obj.y + this.topLeftRealY,
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
                    obj.x + this.topLeftRealX,
                    obj.y + this.topLeftRealY,
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


        for (var y = 200; y < window.innerHeight - this.bottom_panel_height - 150; y += 50) {
            this.ctx.drawImage(this.cacheImages["borders/border-left"].img, 0, y);
            this.ctx.drawImage(this.cacheImages["borders/border-right"].img, window.innerWidth - this.left_panel_width - 25, y);
        }

        for (var x = 200; x < window.innerWidth - this.left_panel_width - 150; x += 50) {
            this.ctx.drawImage(this.cacheImages["borders/border-top"].img, x, 0);
            this.ctx.drawImage(this.cacheImages["borders/border-bottom"].img, x, window.innerHeight - this.bottom_panel_height - 25);
        }
        
        this.ctx.drawImage(this.cacheImages["borders/border-top-left"].img, 0, 0);
        this.ctx.drawImage(this.cacheImages["borders/border-top-right"].img, window.innerWidth - this.left_panel_width - 200, 0);
        this.ctx.drawImage(this.cacheImages["borders/border-bottom-right"].img, window.innerWidth - this.left_panel_width - 200, window.innerHeight - this.bottom_panel_height - 200);
        this.ctx.drawImage(this.cacheImages["borders/border-bottom-left"].img, 0, window.innerHeight - this.bottom_panel_height - 200);
    }

    draw_player() {
        const texturePlayer = "players/player0-50x50";
        if (!this.cacheImages[texturePlayer]) {
            this.loadImage(texturePlayer);
        }

        if (this.cacheImages[texturePlayer] && this.cacheImages[texturePlayer].state == 'loaded') {
            this.ctx.drawImage(
                this.cacheImages[texturePlayer].img,
                this.player_draw_coordinates.x,
                this.player_draw_coordinates.y
            );
        }
    }

    move_player() {
        if (
            parseInt(this.player_target_coordinates.x) != parseInt(this.player_coordinates.x)
            && parseInt(this.player_target_coordinates.y) != parseInt(this.player_coordinates.y)
        ) {
            var angel = Math.atan2(
                this.player_target_coordinates.x - this.player_coordinates.x,
                this.player_target_coordinates.y - this.player_coordinates.y
            )
            this.player_coordinates.x += parseInt(Math.sin(angel)*5);
            this.player_coordinates.y += parseInt(Math.cos(angel)*5);
        }
    }
    update() {
        this.move_player();

        this.topLeftRealX = this.player_draw_coordinates.x - parseInt(this.player_coordinates.x);
        this.topLeftRealY = this.player_draw_coordinates.y - parseInt(this.player_coordinates.y);
        var _perf_start = performance.now();
        this.ctx.fillRect(0,0,window.innerWidth, window.innerHeight);
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
        
        this.leftRealX = 0;
        this.topRealY = 0;
        
        console.log(this.player_coordinates);

        this.player_target_coordinates = {
            x: this.player_coordinates.x + (x - this.player_draw_coordinates.x),
            y: this.player_coordinates.y + (y - this.player_draw_coordinates.y),
        }
        console.log(this.player_target_coordinates);
        

        // console.log ("x = ", x, " y = ", y);
        // let angel = Math.atan2(x - window.innerWidth/2, y - window.innerHeight/2)
        // console.log ("angel = ", angel);
    }
};
