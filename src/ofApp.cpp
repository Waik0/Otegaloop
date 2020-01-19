#include "ofApp.h"
#include <filesystem>
#define ARRAY_SIZE(dArray) (sizeof(dArray)/sizeof(*dArray))
namespace fs = std::filesystem;
//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
	imageLoaded = image.load("image/default.png");
	extension = ".png";
	beginLoadFile.addListener(this, &ofApp::loadFile);
	save.addListener(this, &ofApp::saveFile);
	doProcess.addListener(this, &ofApp::process);
	gui.add(beginLoadFile.setup("load image file"));
	gui.add(doProcess.setup("make loop"));
	gui.add(save.setup("save"));
}
void ofApp::loadFile() {
	auto fileResult = ofSystemLoadDialog("load image", false, "");
	if (fileResult.bSuccess) {
		cout << fileResult.fileName << endl;
		cout << fileResult.getPath()<< endl;
		fs::path fileName = fileResult.fileName;
		cout << fileName.extension();
		extension = fileName.extension().string();
		imageLoaded = image.load(fileResult.getPath());
	}
}
void ofApp::saveFile() {
	fs::path path = "export/result" + extension;
	cout << path;
	image.save(path);
	/*
	OF_IMAGE_FORMAT_BMP     = 0,
    OF_IMAGE_FORMAT_ICO     = 1,
    OF_IMAGE_FORMAT_JPEG    = 2,
    OF_IMAGE_FORMAT_JNG     = 3,
    OF_IMAGE_FORMAT_KOALA   = 4,
    OF_IMAGE_FORMAT_LBM     = 5,
    OF_IMAGE_FORMAT_IFF = OF_IMAGE_FORMAT_LBM,
    OF_IMAGE_FORMAT_MNG     = 6,
    OF_IMAGE_FORMAT_PBM     = 7,
    OF_IMAGE_FORMAT_PBMRAW  = 8,
    OF_IMAGE_FORMAT_PCD     = 9,
    OF_IMAGE_FORMAT_PCX     = 10,
    OF_IMAGE_FORMAT_PGM     = 11,
	OF_IMAGE_FORMAT_PGMRAW  = 12,
    OF_IMAGE_FORMAT_PNG     = 13,
    OF_IMAGE_FORMAT_PPM     = 14,
    OF_IMAGE_FORMAT_PPMRAW  = 15,
    OF_IMAGE_FORMAT_RAS     = 16,
    OF_IMAGE_FORMAT_TARGA   = 17,
    OF_IMAGE_FORMAT_TIFF    = 18,
    OF_IMAGE_FORMAT_WBMP    = 19,
    OF_IMAGE_FORMAT_PSD     = 20,
    OF_IMAGE_FORMAT_CUT     = 21,
    OF_IMAGE_FORMAT_XBM     = 22,
    OF_IMAGE_FORMAT_XPM     = 23,
    OF_IMAGE_FORMAT_DDS     = 24,
    OF_IMAGE_FORMAT_GIF     = 25,
    OF_IMAGE_FORMAT_HDR     = 26,
    OF_IMAGE_FORMAT_FAXG3   = 27,
    OF_IMAGE_FORMAT_SGI     = 28,
    OF_IMAGE_FORMAT_EXR     = 29,
    OF_IMAGE_FORMAT_J2K     = 30,
    OF_IMAGE_FORMAT_JP2     = 31,
    OF_IMAGE_FORMAT_PFM     = 32,
    OF_IMAGE_FORMAT_PICT    = 33,
    OF_IMAGE_FORMAT_RAW     = 34
	*/
}
void ofApp::process() {
	unsigned char * data = image.getPixels().getData();
	cout << ARRAY_SIZE(data);
	
	int width = image.getWidth();
	int height = image.getHeight();
	ofImageType type = image.getImageType();
	//moveleftdown
	switch (type) {
	case OF_IMAGE_GRAYSCALE:
		break;
	case OF_IMAGE_COLOR:
	{
		unsigned char * result = new unsigned char[width * height * 3];
		//ずらし
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int index = y * width + x;
				int index2 = ((y + height / 2) % height) * width + ((x + width / 2) % width);
				result[index * 3] = data[index2 * 3];
				result[index * 3 + 1] = data[index2 * 3 + 1];
				result[index * 3 + 2] = data[index2 * 3 + 2];
			}
		}
		//フィルタ
		//for (int x = 0; x < width; x++) {
		//	for (int y = 0; y < height; y++) {
		//		int index = y * width + x;
		//		int index2 = ((y + height / 2) % height) * width + ((x + width / 2) % width);
		//		result[index * 3] = data[index2 * 3];
		//		result[index * 3 + 1] = data[index2 * 3 + 1];
		//		result[index * 3 + 2] = data[index2 * 3 + 2];
		//	}
		//}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int index = y * width + x;
				float weightx = (-cos(PI * 2 * (x / (float)width)) + 1) / 2;
				float weighty = (-cos(PI * 2 * (y / (float)height)) + 1) / 2;
				float weight = max(weightx, weighty);
				//cout << weightx;
				result[index * 3] = result[index * 3] * (1 - weight ) + data[index * 3] * weight;
				result[index * 3 + 1] = result[index * 3 + 1] * (1 - weight) + data[index * 3 + 1] *weight;
				result[index * 3 + 2] = result[index * 3 + 2] * (1 - weight) + data[index * 3 + 2] * weight;
			}
		}
		//書き込み
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int index = y * width + x;
				data[index * 3] = result[index * 3];
				data[index * 3 + 1] = result[index * 3 + 1];
				data[index * 3 + 2] = result[index * 3 + 2];
			}
		}
		break;
	}
	case OF_IMAGE_COLOR_ALPHA:
	{
		unsigned char * result = new unsigned char[width * height * 4];
		//ずらし
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int index = y * width + x;
				int index2 = ((y + height / 2) % height) * width + ((x + width / 2) % width);
				result[index * 4] = data[index2 * 4];
				result[index * 4 + 1] = data[index2 * 4 + 1];
				result[index * 4 + 2] = data[index2 * 4 + 2];
			}
		}
		//フィルタ
		//for (int x = 0; x < width; x++) {
		//	for (int y = 0; y < height; y++) {
		//		int index = y * width + x;
		//		int index2 = ((y + height / 2) % height) * width + ((x + width / 2) % width);
		//		result[index * 3] = data[index2 * 3];
		//		result[index * 3 + 1] = data[index2 * 3 + 1];
		//		result[index * 3 + 2] = data[index2 * 3 + 2];
		//	}
		//}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int index = y * width + x;
				float weightx = (-cos(PI * 2 * (x / (float)width)) + 1) / 2;
				float weighty = (-cos(PI * 2 * (y / (float)height)) + 1) / 2;
				float weight = max(weightx, weighty);
				//cout << weightx;
				result[index * 4] = result[index * 4] * (1 - weight) + data[index * 4] * weight;
				result[index * 4 + 1] = result[index * 4 + 1] * (1 - weight) + data[index * 4 + 1] * weight;
				result[index * 4 + 2] = result[index * 4 + 2] * (1 - weight) + data[index * 4 + 2] * weight;
			}
		}
		//書き込み
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int index = y * width + x;
				data[index * 4] = result[index * 4];
				data[index * 4 + 1] = result[index * 4 + 1];
				data[index * 4 + 2] = result[index * 4 + 2];
			}
		}
		break;
	}
	}
	
	image.update();

}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	if (imageLoaded) {
		image.draw(ofVec3f(0, 0, 0));
		image.draw(ofVec3f(0, image.getHeight(), 0));
		image.draw(ofVec3f(image.getWidth(), image.getHeight(), 0));
		image.draw(ofVec3f(image.getWidth(), 0, 0));
	}
	else {

		ofDrawBitmapString("image cant load. (maybe path contains 2byte character or not image file.)", ofVec2f(50, 50));
	}
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
