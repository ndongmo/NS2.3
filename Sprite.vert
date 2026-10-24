#version 130

// =========================================================================
// 📥 INPUT ATTRIBUTES 
// =========================================================================
// Slot 0: Static Local Quad Template Data (xy = Position, zw = UV Bounds)
in vec4 vertexData; 

// Slots 1-3: Streamed Dynamic Instanced Fields per Sprite via SpriteInstanceData
in vec4 destRect;    // Slot 1: x, y, width, height
in vec4 uvRect;      // Slot 2: u, v, uWidth, vHeight
in vec4 vertexColor; // Slot 3: Normalized explicitly on upload (GL_TRUE)

// =========================================================================
// 📤 OUTPUT VARIABLES TO FRAGMENT STAGE
// =========================================================================
out vec4 fragmentColor;
out vec2 fragmentUV;

// =========================================================================
// 🎛️ GLOBAL CAMERA SYSTEM UNIFORM
// =========================================================================
uniform mat4 cameraMatrix;

void main() {
    // 1. GPU GEOMETRY PROCESSING: Expand [0.0, 1.0] local bounds to world coordinates
    vec2 worldPosition;
    worldPosition.x = destRect.x + (vertexData.x * destRect.z);
    worldPosition.y = destRect.y + (vertexData.y * destRect.w);

    // 2. TEXTURE SHEET CORRECTION: Calculate custom cropped texture sheet coordinates
    fragmentUV.x = uvRect.x + (vertexData.z * uvRect.z);
    fragmentUV.y = uvRect.y + (vertexData.w * uvRect.w);

    // 3. TRANSFORM COORDS: Map world coordinates directly with a flat Z value (0.0)
    // Painter's algorithm handled on the CPU sets the absolute presentation layer order
    gl_Position = cameraMatrix * vec4(worldPosition, 0.0, 1.0);
    
    // Pass color down to fragment interpolation steps
    fragmentColor = vertexColor;
}
