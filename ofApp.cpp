#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(0.25);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(ofGetFrameNum() * 0.5);

	int half_len = 25;

	ofMesh base_mesh;
	base_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	base_mesh.addVertex(glm::vec3(-half_len, -half_len, half_len));
	base_mesh.addVertex(glm::vec3(half_len, -half_len, half_len));
	base_mesh.addVertex(glm::vec3(half_len, half_len, half_len));
	base_mesh.addVertex(glm::vec3(-half_len, half_len, half_len));
	base_mesh.addVertex(glm::vec3(-half_len, -half_len, -half_len));
	base_mesh.addVertex(glm::vec3(half_len, -half_len, -half_len));
	base_mesh.addVertex(glm::vec3(half_len, half_len, -half_len));
	base_mesh.addVertex(glm::vec3(-half_len, half_len, -half_len));

	for (int i = 0; i < base_mesh.getNumVertices(); i++) {

		base_mesh.setVertex(i, base_mesh.getVertex(i) * 0.65);
	}

	base_mesh.addIndex(0); base_mesh.addIndex(1);
	base_mesh.addIndex(3); base_mesh.addIndex(2);
	base_mesh.addIndex(4); base_mesh.addIndex(5);
	base_mesh.addIndex(7); base_mesh.addIndex(6);

	base_mesh.addIndex(0); base_mesh.addIndex(3);
	base_mesh.addIndex(1); base_mesh.addIndex(2);
	base_mesh.addIndex(4); base_mesh.addIndex(7);
	base_mesh.addIndex(5); base_mesh.addIndex(6);

	base_mesh.addIndex(0); base_mesh.addIndex(4);
	base_mesh.addIndex(1); base_mesh.addIndex(5);
	base_mesh.addIndex(2); base_mesh.addIndex(6);
	base_mesh.addIndex(3); base_mesh.addIndex(7);

	for (int x = -100; x <= 100; x += 50) {

		for (int y = -100; y <= 100; y += 50) {

			for (int z = -100; z <= 100; z += 50) {
				
				int param = (ofGetFrameNum() * 3 + y * 7) % 400;
				if (param > 300) { param = 0; }
				else if (param > 200) { param = ofMap(param, 200, 300, 100, 0); }
				else if (param > 100) { param = 100; }

				auto noise_mesh = base_mesh;
				for (int i = 0; i < base_mesh.getNumVertices(); i++) {

					auto location = glm::vec3(
						ofMap(ofNoise(ofRandom(10000)), 0, 1, -half_len, half_len),
						ofMap(ofNoise(ofRandom(10000)), 0, 1, -half_len, half_len),
						ofMap(ofNoise(ofRandom(10000)), 0, 1, -half_len, half_len));

					noise_mesh.setVertex(i, location);
				}

				for (int i = 0; i < base_mesh.getNumVertices(); i++) {

					auto distance = base_mesh.getVertex(i) - noise_mesh.getVertex(i);
					auto d = (distance / 100) * param;

					noise_mesh.setVertex(i, base_mesh.getVertex(i) - d);
				}

				ofPushMatrix();
				ofTranslate(x, y, z);

				noise_mesh.draw();
				for (auto& vertex : noise_mesh.getVertices()) {

					ofDrawSphere(vertex, 1);
				}

				ofPopMatrix();
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}