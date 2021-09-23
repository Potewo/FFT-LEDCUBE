#pragma once

#include "ofMain.h"
#include "ofxSerial.h"

struct SerialMessage {
  std::string message;
  std::string exception;
  int fade = 255;
};

class ofApp : public ofBaseApp {

public:
  void setup();
  void update();
  void draw();
  void exit();
  static constexpr size_t nBandsToGet = 80;
  vector<uint8_t> generateSpectrumMatrix(array<float, nBandsToGet> fftData);

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void onSerialBuffer(const ofxIO::SerialBufferEventArgs &args);
  void onSerialError(const ofxIO::SerialBufferErrorEventArgs &args);
  ofxIO::PacketSerialDevice device;
  std::vector<SerialMessage> serialMessages;

  ofSoundPlayer player;
  std::array<float, nBandsToGet> fftSmoothed{{0}};
  float logk[nBandsToGet];
};
