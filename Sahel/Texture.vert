#version 130

// =========================================================================
// 📥 HARDWARE INSTANCED INPUT ATTRIBUTES (From SpriteBatch)
// =========================================================================
in vec4 vertexData;   // Slot 0: xy = Local Quad Position [0..1], zw = Local UV
in vec4 destRect;     // Slot 1: World Destination x, y, width, height
in vec4 uvRect;       // Slot 2: Atlas Target u, v, uWidth, vHeight
in vec4 vertexColor;  // Slot 3: Custom vertex color tint data

// =========================================================================
// 📤 OUTPUT VARIABLES TO FRAGMENT STAGE
// =========================================================================
out vec4 fragmentColor;
out vec2 fragmentUV;
out vec2 v_WorldPos;  // 🟢 CRITICAL: Forwarded absolute world space coordinates

// =========================================================================
// 🎛️ GLOBAL PIPELINE TRANSFORMATION UNIFORM
// =========================================================================
uniform mat4 cameraMatrix;

void main() {
    // 1. Expand local [0.0, 1.0] vertices into absolute world coordinates
    vec2 worldPosition;
    worldPosition.x = destRect.x + (vertexData.x * destRect.z);
    worldPosition.y = destRect.y + (vertexData.y * destRect.w);

    // 🟢 ASSIGN WORLD SPACE: Preserve raw coordinates before clip projections
    v_WorldPos = worldPosition;

    // 2. Map coordinates cleanly to your custom sub-texture atlas sheets
    fragmentUV.x = uvRect.x + (vertexData.z * uvRect.z);
    fragmentUV.y = uvRect.y + (vertexData.w * uvRect.w);

    // 3. Project world space into layout space using the unified camera matrix
    gl_Position = cameraMatrix * vec4(worldPosition, 0.0, 1.0);
    
    fragmentColor = vertexColor;
}
