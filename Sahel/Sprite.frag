#version 130

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 outColor;

uniform sampler2D ns2TextureSampler;

void main() {
    vec4 textureSample = texture2D(ns2TextureSampler, fragmentUV);
    
    // Base texture color combined with vertex tinting
    vec4 baseColor = textureSample * fragmentColor;
    
    // Alpha discard check handled early before complex calculations
    if (baseColor.a < 0.01) {
        discard;
    }
    
    // Allowing the RGB channels to slightly overshoot standard bounds creates 
    // a vibrant "glowing core" effect when multiple lights overlap in dark areas.
    outColor = vec4(baseColor.rgb, baseColor.a);
}
