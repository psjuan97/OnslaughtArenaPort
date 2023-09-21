(function define_horde_ImageLoader () {

	
horde.ImageLoader = function horde_ImageLoader () {
	this.images = {};
	this.numLoaded = 0;
	this.numImages = 0;
};
	
var proto = horde.ImageLoader.prototype;

proto.load = function horde_Loader_proto_loadImages (images, fn, context) {
	
	var assLo = new AssetLoader();
	this.callback = function horde_Loader_proto_loadImages_anon () {
		fn.call(context);
	};

	for (var key in images) {
		print("loadImage: " + this.numImages + images[key]);

		assLo.loadImage(this.numImages, images[key]);
		//TODO FIX
		var image = new EventTarget();
		image.x =1;
		image.y= 1;

		this.images[key] = image;

		print("lanzamos load y error");

		horde.on("load", this.handleImageLoad, this.images[key], this);
		horde.on("error", this.handleImageError, this.images[key], this);
		this.images[key].src = images[key];
		this.images[key].id =this.numImages;
		this.numImages++;

		print("imagen cargada: " + key);
	}
};

proto.increment = function horde_ImageLoader_proto_increment () {
	this.numLoaded++;
	if (this.numLoaded >= this.numImages) {
		this.callback();
	}
};

proto.handleImageLoad = function horde_ImageLoader_proto_handleImageLoad (e) {
	this.increment();
};

proto.handleImageError = function horde_ImageLoader_proto_handleImageError (e) {
	this.increment();
};

proto.getImage = function horde_ImageLoader_proto_getImage (key) {

	if (this.images[key]) {
		//return this.images[key].src;
		return this.images[key].id;

	}else{
		print("NO EXISTE " + key);
	}
	return false;
};

}());
