#version 150
#define TWO_PI 6.283185307179586476925286766559

uniform sampler2DRect tex0;
in vec2 texCoordVarying;
out vec4 outputColor;
uniform vec3 window;
uniform vec2 oscillators;
uniform vec4 color;
uniform vec4 translate;
uniform float feedback;

float powerScale(float signal){
    float scale = pow(window.z, 0.25);
    return pow(signal, 1.0 + pow(scale, 1.0 / pow(scale, 0.5))) * scale;
}

float scalePhase(float phase, float skew){
    float scaledSkew = powerScale(skew);
    return pow(phase, scaledSkew) * TWO_PI;
}

float scaleTension(float tension){
    return pow(tension, 2.0) * 2.0;
}

float unipolar(float signal){
    return signal * 0.5 + 0.5;
}

float bipolar(float signal){
    return (signal - 0.5) * 2.0;
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
    float scaledFrequency = pow(frequency, 0.5) * window.z;
    float sine = sin(scaledPhase * TWO_PI * frequency);
    float scaledTension = scaleTension(tension);
    float scaledDepth = powerScale(depth);
    return pow(unipolar(abs(sine) * sign(sine)), scaledDepth);
}

float averageTwo(float inA, float inB){
    return (inA + inB) / 2.0;
}

float normalizedAdd(float inA, float inB){
    return (inA + inB) / (1.0 + inB);
}

vec3 normalizedAdd(vec3 inA, vec3 inB){
    return (inA + inB) / (1.0 + inB);
}

float process(float signal, float colorPower, float powerPhase){
    return pow(signal, pow(1.0 - colorPower, powerPhase)); 
}

void main()
{
    float a = oscillators.x;
    float b = oscillators.y;
    float c = (a + b) / 2.0;
    float d = abs(a - b) / 2.0;
    float ring = a * b;
    float ringSum = (a / b) + (b / a);
    float ringDifference = abs((a / b) - (b / a));
    vec2 normalized = gl_FragCoord.xy / window.xy;
    vec2 phase = mix(1.0 - normalized, normalized, pow(ring, 0.5)) * 2.0 * abs(0.5 - normalized);
    float powerPhase = abs(sin(phase.x * ringSum * TWO_PI) * sin(phase.y * ringDifference * TWO_PI));
    float xRSkewLFO = lfo(phase.x, a, b, c, d);
    float xRDepthLFO = lfo(phase.x, a, b, d, c);
    float xRFrequencyLFO = lfo(phase.x, a, c, b, d);
    float xRTensionLFO = lfo(phase.x, a, c, d, b);
    float xGSkewLFO = lfo(phase.x, a, d, b, c);
    float xGDepthLFO = lfo(phase.x, a, d, c, b);
    float xGFrequencyLFO = lfo(phase.x, b, a, c, d);
    float xGTensionLFO = lfo(phase.x, b, a, d, c);
    float xBSkewLFO = lfo(phase.x, b, c, a, d);
    float xBDepthLFO = lfo(phase.x, b, c, d, a);
    float xBFrequencyLFO = lfo(phase.x, b, d, a, c);
    float xBTensionLFO = lfo(phase.x, b, d, c, a);
    float yRSkewLFO = lfo(phase.y, c, a, b, d);
    float yRDepthLFO = lfo(phase.y, c, a, d, b);
    float yRFrequencyLFO = lfo(phase.y, c, b, a, d);
    float yRTensionLFO = lfo(phase.y, c, b, d, a);
    float yGSkewLFO = lfo(phase.y, c, d, a, b);
    float yGDepthLFO = lfo(phase.y, c, d, b, a);
    float yGFrequencyLFO = lfo(phase.y, d, a, b, c);
    float yGTensionLFO = lfo(phase.y, d, a, c, b);
    float yBSkewLFO = lfo(phase.y, d, b, a, c);
    float yBDepthLFO = lfo(phase.y, d, b, c, a);
    float yBFrequencyLFO = lfo(phase.y, d, c, a, b);
    float yBTensionLFO = lfo(phase.y, d, c, b, a);
    float xROutput = oscillate(phase.x, xRSkewLFO, xRDepthLFO, xRFrequencyLFO, xRTensionLFO);
    float xGOutput = oscillate(phase.x, xGSkewLFO, xGDepthLFO, xGFrequencyLFO, xGTensionLFO);
    float xBOutput = oscillate(phase.x, xBSkewLFO, xBDepthLFO, xBFrequencyLFO, xBTensionLFO);
    float yROutput = oscillate(phase.y, yRSkewLFO, yRDepthLFO, yRFrequencyLFO, yRTensionLFO);
    float yGOutput = oscillate(phase.y, yGSkewLFO, yGDepthLFO, yGFrequencyLFO, yGTensionLFO);
    float yBOutput = oscillate(phase.y, yBSkewLFO, yBDepthLFO, yBFrequencyLFO, yBTensionLFO);
    float rOutput = xROutput * yROutput;
    float gOutput = xGOutput * yGOutput;
    float bOutput = xBOutput * yBOutput;
    float wOutput = pow(rOutput * gOutput * bOutput, 0.0625) * color.a;
    vec2 reposition = vec2(bipolar(1.0 - c) * translate.x, bipolar(1.0 - d) * translate.y);
    vec2 resize = vec2(powerScale(1.0 - a) * translate.z, powerScale(1.0 - b) * translate.w);
    vec4 feedbackColor = texture2DRect(tex0, texCoordVarying * resize + reposition);
    float rFeedback = feedbackColor.r * pow(color.r * (1.0 - color.r), 0.5);
    float gFeedback = feedbackColor.g * pow(color.g * (1.0 - color.g), 0.5);
    float bFeedback = feedbackColor.b * pow(color.b * (1.0 - color.b), 0.5);
    float wFeedback = pow(feedbackColor.r * feedbackColor.g * feedbackColor.b, 0.0625);
    float white = averageTwo(wOutput, wFeedback);
    float red = normalizedAdd(normalizedAdd(rOutput * color.r, white), rFeedback);
    float green = normalizedAdd(normalizedAdd(gOutput * color.g, white), gFeedback);
    float blue = normalizedAdd(normalizedAdd(bOutput * color.b, white), bFeedback);
    vec3 newColor = vec3(red, green, blue);
    vec3 filterColor = mix(feedbackColor.rgb, 1.0 - feedbackColor.rgb, abs(ringSum - ringDifference));
    vec3 preProcessed = mix(newColor, filterColor, (1.0 - ring) * ring);
    vec3 processed = vec3(process(preProcessed.r, color.r, powerPhase), process(preProcessed.g, color.g, powerPhase), process(preProcessed.b, color.b, powerPhase));
    outputColor = vec4(mix(processed, processed * feedbackColor.rgb, feedback), 1.0);
}