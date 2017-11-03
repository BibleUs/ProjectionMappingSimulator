//
//  Projector.cpp
//  ProjectorSimulator
//
//  Created by Nabi Lab on 2017. 7. 17..
//
//

#define DEG2RAD 3.141592f/180.0;

#include "Projector.h"

Projector::Projector() {
    this->xPos = 0;
    this->yPos = 0;
    this->zPos = 0;
    this->xRotation = 0;
    this->yRotation = 0;
    this->zRotation = 0;
    isSelected = false;
    isTrsMode = false;
}

Projector::Projector(float xPos, float yPos, float zPos) {
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->xRotation = 0;
    this->yRotation = 0;
    this->zRotation = 0;
    isSelected = false;
    isTrsMode = false;
}

Projector::Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation) {
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->xRotation = xRotation;
    this->yRotation = yRotation;
    this->zRotation = zRotation;
    isSelected = false;
    isTrsMode = false;
}

Projector::Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation, int projectorNum) {
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->xRotation = xRotation;
    this->yRotation = yRotation;
    this->zRotation = zRotation;
    this->projectorNum = projectorNum = projectorNum;
    isSelected = false;
    isTrsMode = false;
}

void Projector::setup() {
    projectorParameters.setName("No." + ofToString(projectorNum) + " Projector");
    //projectorParameters.add(isPlaying.set("Video Play",false));
    projectorParameters.add(xPos.set("X Pos",xPos,-3000,3000));
    projectorParameters.add(yPos.set("Y Pos",yPos,-3000,3000));
    projectorParameters.add(zPos.set("Z Pos",zPos,-3000,3000));
    projectorParameters.add(xRotation.set("X Rotate",xRotation,-360,360));
    projectorParameters.add(yRotation.set("Y Rotate",yRotation,-360,360));
    projectorParameters.add(zRotation.set("Z Rotate",zRotation,-360,360));
    
    allocateShadowFbo();
    
    isSetted = false;
    isSelected = false;
    
}

void Projector::update() {
    if(!videoPlayer.isLoaded())
        return ;
    
    videoPlayer.update();
    copiedTexture.allocate(videoPlayer.getPixels());
}

void Projector::draw() {
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(255,255,255);
    //ofDrawBitmapString("No." + ofToString(projectorNum) + " Projector (" + ofToString(xPos) + ", " + ofToString(yPos) + ", " + ofToString(zPos) + ")", xPos, yPos + 20, zPos);
    
    ofTranslate(xPos,yPos,zPos);
    ofRotate(xRotation,1,0,0);
    ofRotate(yRotation,0,1,0);
    ofRotate(zRotation,0,0,1);
    ofTranslate(-xPos,-yPos,-zPos);
    
    //Depth limit. if vertex's z value is far more than left operand('-4000' in this case) then, texturing won't come up
    float distance = abs(-4000 - zPos); //calculate between -2000 and zPos.
    
    xRadVal = ceil(580 * distance / 1039 / 2);
    yRadVal = ceil(363 * distance / 1039 / 2);
    zRadVal = 1;
    
    //Draw light radiation line for about 50 pixels ahead
    if(isSetted) {
        if(isSelected)
            ofSetColor(255, 165, 0);
        else
            ofSetColor(255,0,0);
        
        ofDrawLine(xPos - 1.6 * 10, yPos + 1.0 * 10, zPos - zRadVal * 10, xPos - 1.6 * 50, yPos + 1.0 * 50, zPos - zRadVal * 50);
        ofDrawLine(xPos + 1.6 * 10, yPos + 1.0 * 10, zPos - zRadVal * 10, xPos + 1.6 * 50, yPos + 1.0 * 50, zPos - zRadVal * 50);
        ofDrawLine(xPos - 1.6 * 10, yPos - 1.0 * 10, zPos - zRadVal * 10, xPos - 1.6 * 50, yPos - 1.0 * 50, zPos - zRadVal * 50);
        ofDrawLine(xPos + 1.6 * 10, yPos - 1.0 * 10, zPos - zRadVal * 10, xPos + 1.6 * 50, yPos - 1.0 * 50, zPos - zRadVal * 50);
        
        ofDrawLine(xPos - 1.6 * 10, yPos + 1.0 * 10, zPos - zRadVal * 10, xPos + 1.6 * 10, yPos + 1.0 * 10, zPos - zRadVal * 10);
        ofDrawLine(xPos + 1.6 * 10, yPos + 1.0 * 10, zPos - zRadVal * 10, xPos + 1.6 * 10, yPos - 1.0 * 10, zPos - zRadVal * 10);
        ofDrawLine(xPos - 1.6 * 10, yPos - 1.0 * 10, zPos - zRadVal * 10, xPos - 1.6 * 10, yPos + 1.0 * 10, zPos - zRadVal * 10);
        ofDrawLine(xPos + 1.6 * 10, yPos - 1.0 * 10, zPos - zRadVal * 10, xPos - 1.6 * 10, yPos - 1.0 * 10, zPos - zRadVal * 10);
        
        ofDrawLine(xPos - 1.6 * 50, yPos + 1.0 * 50, zPos - zRadVal * 50, xPos + 1.6 * 50, yPos + 1.0 * 50, zPos - zRadVal * 50);
        ofDrawLine(xPos + 1.6 * 50, yPos + 1.0 * 50, zPos - zRadVal * 50, xPos + 1.6 * 50, yPos - 1.0 * 50, zPos - zRadVal * 50);
        ofDrawLine(xPos + 1.6 * 50, yPos - 1.0 * 50, zPos - zRadVal * 50, xPos - 1.6 * 50, yPos - 1.0 * 50, zPos - zRadVal * 50);
        ofDrawLine(xPos - 1.6 * 50, yPos - 1.0 * 50, zPos - zRadVal * 50, xPos - 1.6 * 50, yPos + 1.0 * 50, zPos - zRadVal * 50);
    }
    
    ofVec3f topLeft = ofVec3f(xPos - (xRadVal), yPos + (yRadVal), zPos - (zRadVal * distance));
    ofVec3f topRight = ofVec3f(xPos + (xRadVal), yPos + (yRadVal), zPos - (zRadVal * distance));
    ofVec3f bottomLeft = ofVec3f(xPos - (xRadVal), yPos - (yRadVal), zPos - (zRadVal * distance));
    ofVec3f bottomRight = ofVec3f(xPos + (xRadVal), yPos - (yRadVal), zPos - (zRadVal * distance));
    
    
    width = abs(topLeft.distance(topRight));
    height = abs(topLeft.distance(bottomLeft));
    
    //ofLog() << "Distance : " << abs(-4000 - zPos) << ", Width : " << width << ", Height : " << height << ", Aspect Ratio : " << width/height << ", Throw Ratio : " << distance / width;
    
    allocateTextureFbo(width, height);
    textureFbo.begin();
    clearTextureFbo();
    if(videoPlayer.isLoaded()) {
        if(isSelected) {
            /*ofPushStyle();
            ofColor(255,0,0);
            ofDrawRectangle(pt[0].x - 50, pt[0].y - 50, 50, 50);
            ofDrawRectangle(pt[1].x, pt[1].y - 50, 50, 50);
            ofDrawRectangle(pt[2].x, pt[2].y, 50, 50);
            ofDrawRectangle(pt[3].x - 50, pt[3].y, 50, 50);
            ofPopStyle();*/
            copiedTexture.draw(pt[3], pt[2], pt[1], pt[0]);
        }
        else
            videoPlayer.draw(0, 0, width, height);
    }
    
    ofPoint points[4];
    ofPoint centerPt;
    centerPt.x = width/2; centerPt.y = height/2;
    textureFbo.end();
    
    //Make matrices for Projective Texturing
    //Projector's view matrix
    ofVec3f projectorPos = ofVec3f(xPos, yPos, zPos);
    float sx, sy, sz, cx, cy, cz, theta;
    theta = xRotation * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);
    
    theta = yRotation * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);
    
    theta = zRotation * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);
    
    ofVec3f projectorTarget = ofVec3f(xPos - (sy * cz + cy * sx * sz), yPos - (sy * sz - cy * sx * cz), zPos - (cy * cx));
    ofVec3f projectorUp = ofVec3f(-1 * cx * sz, cx * cz, sx);
    //ofVec3f projectorUp = ofVec3f(0,1,0);
    
    projectorView.makeLookAtViewMatrix(projectorPos, projectorTarget, projectorUp);
    
    //Projector's projection matrix
    float aspect = float(width/height);
    projectorProjection = ofMatrix4x4::newPerspectiveMatrix(90, aspect, distance/1000, distance); //need to find a way to calculate proper fov value
    
    //0-1 bias matrix
    projectorBias = ofMatrix4x4::newIdentityMatrix();
    projectorBias.scale(ofVec3f(0.5, -0.5, 0.5)); //coordinate origin starts inversed.
    projectorBias.translate(ofVec3f(0.5, 0.5, 0.5));
    projectorBias.scale(ofVec3f(width, height, 1));
    
    //Projector matrix
    projectorMatrix = projectorView * projectorProjection * projectorBias;
    
    ofPopStyle();
    ofPopMatrix();
}

void Projector::allocateShadowFbo() {
    ofFbo::Settings settings;
    settings.width = 1024;
    settings.height = 768;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.internalformat = GL_RGBA32F_ARB;
    settings.useDepth = true;
    settings.depthStencilAsTexture = true;
    settings.useStencil = true;
    
    shadowFbo.allocate(settings);
}

void Projector::allocateTextureFbo(int width, int height) {
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.wrapModeVertical = GL_CLAMP_TO_BORDER_ARB;
    settings.wrapModeHorizontal = GL_CLAMP_TO_BORDER_ARB;
    textureFbo.allocate(settings);
}

void Projector::clearShadowFbo() {
    ofClear(0);
    ofSetColor(255);
    shadowFbo.getDepthTexture().draw(0,0);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Projector::clearTextureFbo() {
    ofClear(0);
    ofSetColor(255);
    textureFbo.getTexture().draw(0,0);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Projector::deactivate() {
    isSetted = false;
    isSelected = false;
    videoPlayer.close();
}
