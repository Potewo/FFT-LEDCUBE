#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofSetFrameRate(60);
  ofLogToFile("log.log", true);
  std::vector<ofxIO::SerialDeviceInfo> devicesInfo =
      ofxIO::SerialDeviceUtils::listDevices();
  ofLogNotice("ofApp::setup") << "Connected Devices: ";
  for (std::size_t i = 0; i < devicesInfo.size(); ++i) {
    ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
  }

  if (!devicesInfo.empty()) {
    // Connect to the first matching device.
    bool success = device.setup(devicesInfo[0], 115200);

    if (success) {
      device.registerAllEvents(this);
      ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
    } else {
      ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
    }
  } else {
    ofLogNotice("ofApp::setup") << "No devices connected.";
  }

  player.load("sounds/unity.mp3", true);
  /* player.setVolume(1.0); */
  player.play();
  for (int i = 0; i < nBandsToGet; i++) {
    logk[i] = log(i + 2);
  }
}

//--------------------------------------------------------------
void ofApp::update() {
  ofLogNotice("ofApp::setup") << player.getPosition();
  if (player.getPosition() >= 0.47) {
    ofLogNotice("Finish");
    ofApp::exit();
    exit();
  }
  ofBackground(80, 80, 20);
  ofSoundUpdate();
  float *val = ofSoundGetSpectrum(nBandsToGet);
  for (int i = 0; i < nBandsToGet; i++) {
    /* fftSmoothed[i] *= 0.96f; */
    /* if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i] * (i+1); */
    fftSmoothed[i] = val[i] * (i + 1) * 3;
  }
  ofx::IO::ByteBuffer buffer;
  vector<uint8_t> data = generateSpectrumMatrix(fftSmoothed);
  buffer.writeBytes(data);
  device.send(buffer);
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofEnableAlphaBlending();
  ofSetColor(255, 255, 255, 100);
  ofDrawRectangle(100, ofGetHeight() - 300, 5 * 128, 200);
  ofDisableAlphaBlending();

  ofSetColor(255, 255, 255, 255);

  float width = (float)(5 * 128) / nBandsToGet;
  for (int i = 0; i < nBandsToGet; i++) {
    /* ofDrawRectangle(100+i*width, ofGetHeight()-100, width, -(fftSmoothed[i] *
     * 200)); */
    if (fftSmoothed[i] > 0.5) {
      ofDrawRectangle(100 + i * width, ofGetHeight() - 100, width,
                      -30 * min((int)(fftSmoothed[i] * 7), 7));
    }
  }
}

void ofApp::exit() { device.unregisterAllEvents(this); }

void ofApp::onSerialBuffer(const ofx::IO::SerialBufferEventArgs &args) {
  // Decoded serial packets will show up here.
  ofLogError(args.buffer().toString());
}

void ofApp::onSerialError(const ofx::IO::SerialBufferErrorEventArgs &args) {
  // Errors and their corresponding buffer (if any) will show up here.
  ofLogError(args.buffer().toString());
  ofLogError(args.exception().displayText());
}

vector<uint8_t> ofApp::generateSpectrumMatrix(array<float, nBandsToGet> fftData) {
  vector<uint8_t> matrix(64, 0);
  // rows
  for (int i = 0; i < 8; i++) {
    // each bar
    for (int j = 0; j < 8; j++) {
      int height = fftData[8*i+j];
      // each height(layers)
      for (int k = 0; k < 8; k++) {
        if (height > k) {
          matrix[8*(7 - k)+i] |= (0b10000000 >> j);
        }
      }
    }
  }
  return matrix;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
