#version 130

// =========================================================================
// 📥 INTERPOLATED INPUT INTERFACES
// =========================================================================
in vec4 fragmentColor;
in vec2 fragmentUV;

// =========================================================================
// 📤 FRAGMENT OUTPUT BUFFER TARGETS
// =========================================================================
out vec4 color;

// =========================================================================
// 🗺️ MATERIAL SAMPLER CHANNELS
// =========================================================================
uniform sampler2D ns2TextureSampler;

void main() {
    // 1. Extract raw texture colors using interpolated coordinates
    vec4 textureColor = texture2D(ns2TextureSampler, fragmentUV);
    
    // 2. Blend the texture channels with your vertex tint multiplier color
    color = textureColor * fragmentColor;
}
