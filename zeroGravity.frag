#version 150
#define TWO_PI 6.283185307179586476925286766559

uniform sampler2DRect tex0;
in vec2 texCoordVarying;
out vec4 outputColor;
uniform float feedback;
uniform vec2 window;
uniform vec2 oscillators;
uniform float translate;
float pixels;

float powerScale(float input){
    float scale = pow(pixels, 0.25);
    return pow(input, 1.0 + pow(scale, 1.0 / pow(scale, 0.5))) * scale;
}

float scalePhase(float phase, float skew){
    float scaledSkew = powerScale(skew);
    return pow(phase, scaledSkew) * TWO_PI;
}

float scaleTension(float tension){
    return pow(tension, 2.0) * 2.0;
}

float unipolar(float input){
    return input * 0.5 + 0.5;
}

float lfo(float phase, float skew, float depth, float offset, float tension){
    float scaledOffset = offset * TWO_PI;
    float scaledPhase = scalePhase(phase, skew);
    float sine = sin(scaledPhase + scaledOffset);
    float scaledTension = scaleTension(tension);
    float scaledDepth = powerScale(depth);
    return pow(unipolar(abs(sine) * sign(sine)), scaledDepth);
}

float oscillate(float phase, float skew, float depth, float frequency, float tension){
    float scaledPhase = scalePhase(phase, skew);
    float scaledFrequency = pow(frequency, 0.5) * pixels;
    float sine = sin(scaledPhase * TWO_PI * frequency);
    float scaledTension = scaleTension(tension);
    float scaledDepth = powerScale(depth);
    return pow(unipolar(abs(sine) * sign(sine)), scaledDepth);
}

float averageTwo(float a, float b){
    return (a + b) / 2.0;
}

void main()
{
    pixels = window.x * window.y;
    vec2 normalized = gl_FragCoord.xy / window;
    float a = oscillators.x;
    float b = oscillators.y;
    float c = (a + b) / 2.0;
    float d = abs(a - b) / 2.0;
    float xRSkewLFO = lfo(normalized.x, a, b, c, d);
    float xRDepthLFO = lfo(normalized.x, a, b, d, c);
    float xRFrequencyLFO = lfo(normalized.x, a, c, b, d);
    float xRTensionLFO = lfo(normalized.x, a, c, d, b);
    float xGSkewLFO = lfo(normalized.x, a, d, b, c);
    float xGDepthLFO = lfo(normalized.x, a, d, c, b);
    float xGFrequencyLFO = lfo(normalized.x, b, a, c, d);
    float xGTensionLFO = lfo(normalized.x, b, a, d, c);
    float xBSkewLFO = lfo(normalized.x, b, c, a, d);
    float xBDepthLFO = lfo(normalized.x, b, c, d, a);
    float xBFrequencyLFO = lfo(normalized.x, b, d, a, c);
    float xBTensionLFO = lfo(normalized.x, b, d, c, a);
    float yRSkewLFO = lfo(normalized.y, c, a, b, d);
    float yRDepthLFO = lfo(normalized.y, c, a, d, b);
    float yRFrequencyLFO = lfo(normalized.y, c, b, a, d);
    float yRTensionLFO = lfo(normalized.y, c, b, d, a);
    float yGSkewLFO = lfo(normalized.y, c, d, a, b);
    float yGDepthLFO = lfo(normalized.y, c, d, b, a);
    float yGFrequencyLFO = lfo(normalized.y, d, a, b, c);
    float yGTensionLFO = lfo(normalized.y, d, a, c, b);
    float yBSkewLFO = lfo(normalized.y, d, b, a, c);
    float yBDepthLFO = lfo(normalized.y, d, b, c, a);
    float yBFrequencyLFO = lfo(normalized.y, d, c, a, b);
    float yBTensionLFO = lfo(normalized.y, d, c, b, a);
    float xROutput = oscillate(normalized.x, xRSkewLFO, xRDepthLFO, xRFrequencyLFO, xRTensionLFO);
    float xGOutput = oscillate(normalized.x, xGSkewLFO, xGDepthLFO, xGFrequencyLFO, xGTensionLFO);
    float xBOutput = oscillate(normalized.x, xBSkewLFO, xBDepthLFO, xBFrequencyLFO, xBTensionLFO);
    float yROutput = oscillate(normalized.y, yRSkewLFO, yRDepthLFO, yRFrequencyLFO, yRTensionLFO);
    float yGOutput = oscillate(normalized.y, yGSkewLFO, yGDepthLFO, yGFrequencyLFO, yGTensionLFO);
    float yBOutput = oscillate(normalized.y, yBSkewLFO, yBDepthLFO, yBFrequencyLFO, yBTensionLFO);
    float rOutput = xROutput * yROutput;
    float gOutput = xGOutput * yGOutput;
    float bOutput = xBOutput * yBOutput;
    vec4 feedbackColor = texture2DRect(tex0, texCoordVarying);
    float rFeedback = (1.0 - rOutput) * feedbackColor.r;
    float gFeedback = (1.0 - gOutput) * feedbackColor.g;
    float bFeedback = (1.0 - bOutput) * feedbackColor.b;
    float red = averageTwo(rOutput, rFeedback);
    float green = averageTwo(gOutput, gFeedback);
    float blue = averageTwo(bOutput, bFeedback);
    /*
    float red = 0.0;
    float green = 1.0;
    float blue = 0.0;
    */
    outputColor = vec4(red, green, blue, 1.0);
}