/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
{
}

/******************************
******************************/
ofApp::ofApp(int seed)
: PerlinNoise(seed)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
	delete[] ImageData;
	
	fclose(fp);
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("Perlin");
	
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	maximize the FrameRate so that I can see the Processing speed.
	...I want to compare the processing speed of this "original perlin noise method" with ofNoise() function.
	********************/
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	
	/********************
	********************/
	ImageData = new unsigned char[WIDTH * HEIGHT * 4];
	
	b_OriginalMethod = true;
	b_Octave = false;
	b_WoodenStructure = false;
	
	octaves = 1;
	
	png_id = 0;
	
	/********************
	********************/
	fp = fopen("../../../data/Log.csv", "w");
	test_noise_1Dimensional();
}

/******************************
******************************/
void ofApp::test_noise_1Dimensional(){
	double octaves = 2;
	double persistence = 0.5;
	
	double step = 0.1;
	
	for(double x = -10; x < 10; x += step){
		fprintf(fp, "%f,", x);
		// double noise = PerlinNoise.noise(x);
		double noise = PerlinNoise.OctaveNoise(x, octaves, persistence);
		fprintf(fp, "%f,%f,\n", noise, ofNoise(x));
	}
}

/******************************
******************************/
void ofApp::makeImageData(){

	/********************
	********************/
	float now = ofGetElapsedTimef();
	
	/********************
	********************/
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			double x = (double)j/WIDTH;
			double y = (double)i/HEIGHT;
			
			/********************
			********************/
			double noise;
			
			double persistence = 0.5;
			
			if(b_WoodenStructure){
				if(b_OriginalMethod){
					// if(b_Octave)	noise = 20 * PerlinNoise.OctaveNoise(x, y, 0.8, octaves, persistence);
					if(b_Octave)	noise = 20 * PerlinNoise.OctaveNoise(x, y, now * 0.1, octaves, persistence);
					
					// else			noise = 20 * PerlinNoise.noise(x, y, 0.8);			// 3次元のnoise()をtest
					// else			noise = 20 * PerlinNoise.noise(x, y);				// 2次元のnoise()をtest
					else			noise = 20 * PerlinNoise.noise(x, y, now * 0.1);	// 3次元のnoise()を使い、2次元模様をanimation
					
				}else{
					noise = 20 * ofNoise(x, y, 0.8);
				}
				
				noise = noise - floor(noise);
				
			}else{
				if(b_OriginalMethod){
					// if(b_Octave)	noise = PerlinNoise.OctaveNoise(10 * x, 10 * y, 0.8, octaves, persistence);
					if(b_Octave)	noise = PerlinNoise.OctaveNoise(10 * x, 10 * y, now, octaves, persistence);
					
					// else			noise = PerlinNoise.noise(10 * x, 10 * y, 0.8);		// 3次元のnoise()をtest
					// else			noise = PerlinNoise.noise(10 * x, 10 * y);			// 2次元のnoise()をtest
					else			noise = PerlinNoise.noise(10 * x, 10 * y, now);		// 3次元のnoise()を使い、2次元模様をanimation
				}else{
					noise = ofNoise(10 * x, 10 * y, 0.8);
				}
			}
			
			/********************
			********************/
			int index = 4 * (j + WIDTH * i);
			ImageData[index + 0] = floor(255 * noise);	// R
			ImageData[index + 1] = floor(255 * noise);	// G
			ImageData[index + 2] = floor(255 * noise);	// B
			ImageData[index + 3] = 255;					// Alpha
		}
	}
	
	/********************
	********************/
	image.setFromPixels(ImageData, WIDTH, HEIGHT, OF_IMAGE_COLOR_ALPHA);
}

//--------------------------------------------------------------
void ofApp::update(){
	makeImageData();
}

//--------------------------------------------------------------
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0, 0, 0, 0);
	ofSetColor(255, 255, 255, 255);
	
	image.draw(0, 0);
	
	/********************
	********************/
	ofSetColor(0, 255, 0, 255);
	
	char buf[BUF_SIZE];
	sprintf(buf, "%5.1f\n", ofGetFrameRate());
	
	char buf_add[BUF_SIZE];
	if(b_OriginalMethod)	sprintf(buf_add, "OriginalMethod\n");
	else					sprintf(buf_add, "ofNoise()\n");
	strcat(buf, buf_add);
	
	if(b_OriginalMethod && b_Octave)	sprintf(buf_add, "Use Octave = %d\n", octaves);
	else								sprintf(buf_add, "no octave\n");
	strcat(buf, buf_add);
	
	/* */
	ofDrawBitmapString(buf, 30, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case OF_KEY_UP:
			octaves++;
			break;
			
		case OF_KEY_DOWN:
			octaves--;
			if(octaves < 1) octaves = 1;
			break;
			
		case 'm':
			b_OriginalMethod = !b_OriginalMethod;
			break;
			
		case 'o':
			b_Octave = !b_Octave;
			break;
			
		case 's':
			b_WoodenStructure = !b_WoodenStructure;
			break;
			
		case ' ':
		{
			char buf[BUF_SIZE];
			
			sprintf(buf, "image_%d.png", png_id);
			ofSaveScreen(buf);
			printf("> %s saved\n", buf);
			
			png_id++;
		}
			break;
	}
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
