
class VvRender {
    constructor(canvasid) {
        this.canvas = document.getElementById(canvasid);
        this.ctx = this.canvas.getContext("2d");
        this.cacheImages = {};
        this.left_panel_width = 200;
        this.bottom_panel_height = 50;
        this.perf = [];

        var self = this;
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

    loadImageToCache(name, path, callback) {
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

    update() {
        var _perf_start = performance.now();

        this.draw_borders();
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
        console.log("avarage perf = ", _perf_avarage, "ms, length " + this.perf.length);
    }
};