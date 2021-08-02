
class VvApi {
    constructor() {
        this.base_url = location.protocol + "//" + location.host;
    }

    promise() {
        var d = {};
        d.completed = false;
        d.failed = false;
        d.successed = false;
        d.done = function(callback){
            d.done_callback = callback;
            if(d.completed && typeof d.done_callback === "function" && d.successed){
                d.done_callback.apply(this, d.result_arguments);
            }
            return d;
        }
        
        d.fail = function(callback){
            d.fail_callback = callback;
            if(d.completed && typeof d.fail_callback === "function" && d.failed){
                d.fail_callback.apply(this,d.error_arguments);
            }
            return d;
        }
        
        d.resolve = function() {
            if(!d.completed){
                d.result_arguments = arguments; // [];
                if(typeof d.done_callback === "function"){
                    d.done_callback.apply(this, d.result_arguments);
                }
            }
            d.successed = true;
            d.completed = true;
        }
        d.reject = function() {
            if(!d.completed){
                d.error_arguments = arguments;
                if(typeof d.fail_callback === "function"){
                    d.fail_callback.apply(this, d.error_arguments);
                }
            }
            d.failed = true;
            d.completed = true;
        }
        return d;
    };

    waitPromises(arr_promise){
        var p = this.promise();
        var max_len = arr_promise.length;
        var result = [];
        function cmpl(r){
            result.push(r);
            if (result.length == max_len){
                p.resolve(result);
            }
        };
        for(var i in arr_promise){
            arr_promise[i].done(cmpl).fail(cmpl);
        }
        return p;
    }

    request(path, d) {
        var xhr = new XMLHttpRequest()
        xhr.onload = function() {
            // alert(`Загружено: ${xhr.status} ${xhr.response}`);
        };
        xhr.onerror = function() { // происходит, только когда запрос совсем не получилось выполнить
            console.error('Ошибка соединения');
            d.reject("error");
        };
        xhr.onreadystatechange = function() {
            if (xhr.readyState != 4) {
                return
            }
            if (xhr.status === 200) {
                d.resolve(JSON.parse(xhr.responseText));
            } else {
                d.reject("error: " + xhr.responseText);
            }
        }
        xhr.open(
            'GET',
            this.base_url + path,
            true
        );
        xhr.send();
    }

    load_map() {
        var d = this.promise();
        this.request("/api/v1/get_map?v=1", d);
        return d;
    }
};
