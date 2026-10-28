#version 130

in vec2 fragmentUV;
in vec4 fragmentColor; // Captured seamlessly from your vertexColor attribute stream

out vec4 outColor;

uniform sampler2D u_LightMaskTexture;

void main() {
    // 1. Sample ONLY the alpha channel to preserve texture cache lines
    float mask = texture2D(u_LightMaskTexture, fragmentUV).a;
    
    // 2. Early exit optimization: Skip rendering completely transparent fragments
    if (mask < 0.005) {
        discard;
    }
    
    // 3. Output clean, premultiplied light intensity vectors
    outColor = vec4(fragmentColor.rgb, mask);
}
