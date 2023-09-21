
var EventTarget = function() {
	this.listeners = {};
};
  
EventTarget.prototype.listeners = null;
EventTarget.prototype.addEventListener = function(type, callback) {
	print("MY addEventListener")
	if(!(type in this.listeners)) {
		this.listeners[type] = [];
	}
	this.listeners[type].push(callback);
	

};

EventTarget.prototype.removeEventListener = function(type, callback) {
	if(!(type in this.listeners)) {
		return;
	}
	var stack = this.listeners[type];
	for(var i = 0, l = stack.length; i < l; i++) {
		if(stack[i] == callback){
		stack.splice(i, 1);
		return this.removeEventListener(type, callback);
		}
	}
};

EventTarget.prototype.dispatchEvent = function(event) {
	if(!(event.type in this.listeners)) {
		return;
	}
	var stack = this.listeners[event.type];
	event.target = this;
	for(var i = 0, l = stack.length; i < l; i++) {
		stack[i].call(this, event);
	}
};
  


var global = new Function('return this')();
global.window = new EventTarget();

var document = new Object();



document.getContext =  function (){
	return this;
}

document.getElementById =  function (){
	return this;
}

document.createElement = function (){
	return this;
};

document.getElementsByTagName =  function (){
	return this;
};

document.appendChild = function(){
	return this;
}
	
document.getElementById =  function (){
	return this;
}


document.body = new EventTarget();


(function () {
	var e = new horde.Engine();
	e.run();
}());




