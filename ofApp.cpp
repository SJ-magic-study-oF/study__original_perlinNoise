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
	b_animation = false;
	
	octaves = 1;
	
	png_id = 0;
	
	State_KeyInput = STATE_KEYINPUT__FADE;
	
	/********************
	********************/
	fp = fopen("../../../data/Log.csv", "w");
	// test_noise_1Dimensional();
	// test_noise_FadeCurve();
}

/******************************
******************************/
void ofApp::test_noise_FadeCurve()
{
	double step = 0.01;
	for(double x = 0; x < 1.0; x += step){
		fprintf(fp, "%f,%f\n", x, PerlinNoise.test_fadeFunction(x));
	}
}

/******************************
******************************/
void ofApp::test_noise_1Dimensional(){
	float t_start, t_end;
	int counter = 0;
	
	double octaves = 2;
	double persistence = 0.5;
	
	double step = 0.1;
	
	t_start = ofGetElapsedTimef();
	for(double x = -10; x < 10; x += step){
		fprintf(fp, "%f,", x);
		// double noise = PerlinNoise.noise(x);
		double noise = PerlinNoise.OctaveNoise(x, octaves, persistence);
		fprintf(fp, "%f,%f,\n", noise, ofNoise(x));
		
		counter++;
	}
	t_end = ofGetElapsedTimef();
	
	printf("%f, %f\n", t_end - t_start, (t_end - t_start) / counter);
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
					if(b_Octave){
						if(b_animation)	noise = 20 * PerlinNoise.OctaveNoise(x, y, now * 0.1, octaves, persistence);
						else			noise = 20 * PerlinNoise.OctaveNoise(x, y, octaves, persistence);
						// else			noise = 20 * PerlinNoise.OctaveNoise(x, y, 0.8, octaves, persistence);
						
					}else{
						if(b_animation)	noise = 20 * PerlinNoise.noise(x, y, now * 0.1);
						else			noise = 20 * PerlinNoise.noise(x, y);
						// else			noise = 20 * PerlinNoise.noise(x, y, 0.8);
					}
					
				}else{
					if(b_animation)		noise = 20 * ofNoise(x, y, now * 0.1);
					else				noise = 20 * ofNoise(x, y, 0.8);
				}
				
				noise = noise - floor(noise);
				
			}else{
				
				if(b_OriginalMethod){
					// const double pos_Gain = 10;
					const double pos_Gain = 12;
					
					if(b_Octave){
						if(b_animation)	noise = PerlinNoise.OctaveNoise(pos_Gain * x, pos_Gain * y, now, octaves, persistence);
						else			noise = PerlinNoise.OctaveNoise(pos_Gain * x, pos_Gain * y, octaves, persistence);	
						// else			noise = PerlinNoise.OctaveNoise(pos_Gain * x, pos_Gain * y, 0.8, octaves, persistence);	
					}else{
						if(b_animation)	noise = PerlinNoise.noise(pos_Gain * x, pos_Gain * y, now);
						else			noise = PerlinNoise.noise(pos_Gain * x, pos_Gain * y);
						// else			noise = PerlinNoise.noise(pos_Gain * x, pos_Gain * y, 0.8);
					}
					
				}else{
					const double pos_Gain = 10;
					
					if(b_animation)		noise = ofNoise(pos_Gain * x, pos_Gain * y, now);
					else				noise = ofNoise(pos_Gain * x, pos_Gain * y, 0.8);
				}
			}
			
			/********************
			********************/
			int index = 4 * (j + WIDTH * i);
			ImageData[index + 0] = // floor(255 * noise);	// R
			ImageData[index + 1] = 0; // floor(255 * noise);	// G
			ImageData[index + 2] = floor(255 * noise);	// B
			ImageData[index + 3] = 255;					// Alpha
			/*
			ImageData[index + 0] = floor(255 * noise);	// R
			ImageData[index + 1] = floor(255 * noise);	// G
			ImageData[index + 2] = floor(255 * noise);	// B
			ImageData[index + 3] = 255;					// Alpha
			*/
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
	PERLIN_NOISE::FADE_TYPE FadeType = PerlinNoise.get_FadeType();
	switch(FadeType){
		case PERLIN_NOISE::FADE_TYPE__DEG_9:
			sprintf(buf_add, "Fade = Deg9\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_7:
			sprintf(buf_add, "Fade = Deg7\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_5:
			sprintf(buf_add, "Fade = Deg5\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_3:
			sprintf(buf_add, "Fade = Deg3\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_1:
			sprintf(buf_add, "Fade = Deg1\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_COS:
			sprintf(buf_add, "Fade = cos");
			break;
			
		default:
			sprintf(buf_add, " ");
			break;

	}
	strcat(buf, buf_add);
	
	/* */
	PERLIN_NOISE::FADE_TYPE SharpenType = PerlinNoise.get_SharpenType();
	switch(SharpenType){
		case PERLIN_NOISE::FADE_TYPE__DEG_9:
			sprintf(buf_add, "Sharpen = Deg9\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_7:
			sprintf(buf_add, "Sharpen = Deg7\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_5:
			sprintf(buf_add, "Sharpen = Deg5\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_3:
			sprintf(buf_add, "Sharpen = Deg3\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_1:
			sprintf(buf_add, "Sharpen = Deg1\n");
			break;
			
		case PERLIN_NOISE::FADE_TYPE__DEG_COS:
			sprintf(buf_add, "Sharpen = cos");
			break;
			
		default:
			sprintf(buf_add, " ");
			break;

	}
	strcat(buf, buf_add);
	
	/* */
	ofDrawBitmapString(buf, 10, 30);
	
	/********************
	********************/
	ofSetColor(0, 255, 255, 255);
	if(State_KeyInput == STATE_KEYINPUT__FADE){
		sprintf(buf,	"0-9    :Fade Type(change mode by f)\n");
	}else{
		sprintf(buf,	"0-9    :Sharpen Type(chage mode by f)\n");
	}
	sprintf(buf_add,	"a      :animation\n"
						"f      :Fade <-> Shrpen\n"
						"m      :OriginalMethod or ofNoise\n"
						"o      :b_Octave\n"
						"s      :b_WoodenStrutcture\n"
						"up/down:octaves\n"
						"space  :save image"
						);
						
	strcat(buf, buf_add);
					
	ofDrawBitmapString(buf, 170, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case '0':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	PerlinNoise.set_FadeType(PERLIN_NOISE::FADE_TYPE__DEG_COS);
			else										PerlinNoise.set_SharpenType(PERLIN_NOISE::FADE_TYPE__DEG_COS);
			break;
			
		case '1':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	PerlinNoise.set_FadeType(PERLIN_NOISE::FADE_TYPE__DEG_1);
			else										PerlinNoise.set_SharpenType(PERLIN_NOISE::FADE_TYPE__DEG_1);
			break;
			
		case '3':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	PerlinNoise.set_FadeType(PERLIN_NOISE::FADE_TYPE__DEG_3);
			else										PerlinNoise.set_SharpenType(PERLIN_NOISE::FADE_TYPE__DEG_3);
			break;
			
		case '5':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	PerlinNoise.set_FadeType(PERLIN_NOISE::FADE_TYPE__DEG_5);
			else										PerlinNoise.set_SharpenType(PERLIN_NOISE::FADE_TYPE__DEG_5);
			break;
			
		case '7':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	PerlinNoise.set_FadeType(PERLIN_NOISE::FADE_TYPE__DEG_7);
			else										PerlinNoise.set_SharpenType(PERLIN_NOISE::FADE_TYPE__DEG_7);
			break;
			
		case '9':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	PerlinNoise.set_FadeType(PERLIN_NOISE::FADE_TYPE__DEG_9);
			else										PerlinNoise.set_SharpenType(PERLIN_NOISE::FADE_TYPE__DEG_9);
			break;
			
		case OF_KEY_UP:
			octaves++;
			break;
			
		case OF_KEY_DOWN:
			octaves--;
			if(octaves < 1) octaves = 1;
			break;
			
		case 'a':
			b_animation = !b_animation;
			break;
			
		case 'f':
			if(State_KeyInput == STATE_KEYINPUT__FADE)	State_KeyInput = STATE_KEYINPUT__SHARP;
			else										State_KeyInput = STATE_KEYINPUT__FADE;
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
