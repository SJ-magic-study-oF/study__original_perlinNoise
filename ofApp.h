/************************************************************
■パーリンノイズを理解する
	http://postd.cc/understanding-perlin-noise/
	
	■原文
		http://flafla2.github.io/2014/08/09/perlinnoise.html
		
	contents
		非常にわかり易い.
		まず、これを読む。
	
	
■Perlinノイズ(Perlin Noise)
	http://www.technotype.net/hugo.elias/models/m_perlin.html	
	
	contents
		octave noise : 周期を変えたperlin noiseを重ねることで、より複雑で有機的なperlin noiseをつくる
		

■Perlin noise in C++11
	https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
	https://github.com/sol-prog/Perlin_Noise
	
	contents
		sample source.
		"■パーリンノイズを理解する"を読んだ後、これと一緒にsample sourceを読むと理解が深まる。
	

■勾配ベクトルの意味と例題
	https://mathtrain.jp/koubai
	
■ハッシュ関数 (hash function)
	http://wa3.i-3-i.info/word11948.html
	
■hash table
	http://wa3.i-3-i.info/word11947.html
************************************************************/

/************************************************************
考察・検討

■double OctaveNoise(double x, int octaves, double persistence); octaves = 2. の処理時間
How to check.
	test_noise_1Dimensional()で、fprintfをコメントアウトしてtest.
	
Result
	≒ zero
	fprintf()を入れても、1usであった.
	
■1次元noise
Conclusion
	octaves = 2.としてOctaveNoise()を使用することで、十分複雑な結果が得られた(cf: sj_test.xlsx).
	
■2次元模様(2次元 for still image, 3次元 for animation)の処理時間
Conclusion
	sj_test.xlsx - FrameRate比較 より
	ofNoise()と比較して、より高速に処理が完了していることがわかる。
************************************************************/

#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "PerlinNoise.h"
#include "sj_common.h"

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		WIDTH	= 600,
		HEIGHT	= 600,
	};
	enum{
		BUF_SIZE = 512,
	};
	
	enum STATE_KEYINPUT{
		STATE_KEYINPUT__FADE,
		STATE_KEYINPUT__SHARP,
	};
	
	/****************************************
	****************************************/
	ofImage image;
	unsigned char* ImageData;
	
	PERLIN_NOISE PerlinNoise;
	
	STATE_KEYINPUT State_KeyInput;
	
	bool b_OriginalMethod;
	bool b_Octave;
	bool b_WoodenStructure;
	bool b_animation;
	
	int octaves;
	
	int png_id;
	
	/****************************************
	****************************************/
	void makeImageData();
	void test_noise_1Dimensional();
	void test_noise_FadeCurve();
	
public:
	/****************************************
	****************************************/
	ofApp();
	ofApp(int seed);
	~ofApp();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
