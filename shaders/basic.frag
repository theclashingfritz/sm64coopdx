#version 120

layout (location = 0) in uvec4 cmds;
layout (location = 1) in uvec4 cmds2;
layout (location = 2) in uvec2 modes;
layout (location = 3) in bvec4 options;

out vec4 gl_FragColor;

varying vec2 vTexCoord;
varying vec4 vFog;
varying vec2 vLightMap;

varying vec4d vInput1;
varying vec4d vInput2;
varying vec4d vInput3;
varying vec4d vInput4;
varying vec4d vInput5;
varying vec4d vInput6;
varying vec4d vInput7;
varying vec4d vInput8;

uniform sampler2D uTex0;
uniform vec2 uTex0Size;
uniform bool uTex0Filter;

uniform sampler2D uTex1;
uniform vec2 uTex1Size;
uniform bool uTex1Filter;

uniform int uFilter;
uniform vec3 uLightmapColor;
uniform float uFrameCount;

vec4 texel;
float noise;

#define TEX_OFFSET(off) texture2D(tex, texCoord - (off)/texSize)

#define SHADER_0 0
#define SHADER_INPUT_1 1
#define SHADER_INPUT_2 2
#define SHADER_INPUT_3 3
#define SHADER_INPUT_4 4
#define SHADER_INPUT_5 5
#define SHADER_INPUT_6 6
#define SHADER_INPUT_7 7
#define SHADER_INPUT_8 8
#define SHADER_TEXEL0 9
#define SHADER_TEXEL0A 10
#define SHADER_TEXEL1 11
#define SHADER_TEXEL1A 12
#define SHADER_1 13
#define SHADER_COMBINED 14
#define SHADER_COMBINEDA 15
#define SHADER_NOISE 16

float random(in vec3 value) {
    float rand = dot(sin(value), vec3(12.9898, 78.233, 37.719));
    return fract(sin(rand) * 143758.5453); 
}

vec4 filter3point(in sampler2D tex, in vec2 texCoord, in vec2 texSize) {
    vec2 offset = fract(texCoord * texSize - vec2(0.5));
    offset -= step(1.0, offset.x + offset.y);
    vec4 c0 = TEX_OFFSET(offset);
    vec4 c1 = TEX_OFFSET(vec2(offset.x - sign(offset.x), offset.y));
    vec4 c2 = TEX_OFFSET(vec2(offset.x, offset.y - sign(offset.y)));
    return c0 + abs(offset.x) * (c1-c0) + abs(offset.y) * (c2-c0);
}

vec4 sampleTex(in sampler2D tex, in vec2 uv, in vec2 texSize, in bool dofilter, in int filter) {
    if (dofilter && filter == 2)
        return filter3point(tex, uv, texSize);
    else
        return texture2D(tex, uv);
}


vec4 getShaderItem(in uint id) {
    switch (id) {
        case SHADER_0:
            return vec4(0.0, 0.0, 0.0, 0.0);
        case SHADER_1:
            return vec4(1.0, 1.0, 1.0, 1.0);
        case SHADER_INPUT_1:
            return vInput1;
        case SHADER_INPUT_2:
            return vInput2;
        case SHADER_INPUT_3:
            return vInput3;
        case SHADER_INPUT_4:
            returnvInput4;
        case SHADER_INPUT_5:
            return vInput5;
        case SHADER_INPUT_6:
            return vInput6;
        case SHADER_INPUT_7:
            return vInput7;
        case SHADER_INPUT_8:
            return vInput8;
        case SHADER_TEXEL0:
            vec4 texVal0 = sampleTex(uTex0, vTexCoord, uTex0Size, uTex0Filter, uFilter);
            if (options.z)
                texVal0.rgb *= uLightmapColor.rgb;
            return texVal0;
        case SHADER_TEXEL0A:
            return vec4(texelVal0.a, texelVal0.a, texelVal0.a, texelVal0.a);
        case SHADER_TEXEL1:
            vec4 texVal1 = sampleTex(uTex1, vLightMap, uTex1Size, uTex1Filter, uFilter);
            if (options.z)
                texVal1.rgb = texVal1.rgb * texVal1.rgb + texVal1.rgb;
            return texVal1;
        case SHADER_TEXEL1A:
            return vec4(texelVal1.a, texelVal1.a, texelVal1.a, texelVal1.a);
        case SHADER_COMBINED:
            return texel;
        case SHADER_COMBINEDA:
            return vec4(texel.a, texel.a, texel.a, texel.a);
        case SHADER_NOISE:
            noise = floor(random(floor(vec3(gl_FragCoord.xy, uFrameCount))) + 0.5);
            return vec4(noise);
    }
    
    return vec4(0.0, 0.0, 0.0, 0.0);
}

vec4 generateTexel(in uint mode, in uint cmd1, in uint cmd2, in uint cmd3, in uint cmd4) {
    if (mode == 0) // Single
        return getShaderItem(cmd4);
    else if (mode == 1) // Multiply
        return getShaderItem(cmd1) * getShaderItem(cmd3);
    else if (mode == 3) // Mix
        return mix(getShaderItem(cmd2), getShaderItem(cmd1), getShaderItem(cmd3),);
    else
        return (getShaderItem(cmd1) - getShaderItem(cmd2)) * getShaderItem(cmd3) + getShaderItem(cmd4);
}

void main() {
    // No noise by default.
    noise = vec4(1.0, 1.0, 1.0, 1.0);
    
    texel = generateTexel(modes.x, cmds.x, cmds.y, cmds.z, cmds.w);
    
    // Two Cycle
    if (options.x)
        texel = generateTexel(modes.y, cmds2.x, cmds2.y, cmds2.z, cmds2.w);
      
    // Texture Edges
    if (options.y && texel.a > 0.3) texel.a = 1.0; else discard;
    
    // Fog
    texel = vec4(mix(texel.rgb, vFog.rgb, vFog.a), texel.a);
    
    // Noise
    if (options.w)
        texel.a *= noise;
    
    gl_FragColor = texel;
}