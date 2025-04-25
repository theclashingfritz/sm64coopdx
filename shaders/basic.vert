#version 330

attribute vec4 aVtxPos;
attribute vec2 aTexCoord;
varying vec2 vTexCoord;
attribute vec4 aFog;
varying vec4 vFog;
attribute vec2 aLightMap;
varying vec2 vLightMap;

attribute vec4d aInput1;
varying vec4d vInput1;
attribute vec4d aInput2;
varying vec4d vInput2;
attribute vec4d aInput3;
varying vec4d vInput3;

void main() {
    vTexCoord = aTexCoord;
    vFog = aFog;
    vLightMap = aLightMap;
    
    vInput1 = aInput1;
    vInput2 = aInput2;
    vInput3 = aInput3;
    
    gl_Position = aVtxPos;
}