#version 130

// Hardware instanced inputs matching your SpriteBatch layout
in vec4 vertexData;   // xy = Local Quad Position [0..1], zw = Local UV
in vec4 destRect;     // World Destination x, y, width, height
in vec4 uvRect;       // Atlas Target u, v, uWidth, vHeight
in vec4 vertexColor;  // Custom vertex color tint data

out vec2 fragmentUV;
out vec4 fragmentColor;

uniform mat4 cameraMatrix;

void main() {
    // 1. Expand local [0.0, 1.0] vertices into absolute world coordinates
    vec2 worldPosition;
    worldPosition.x = destRect.x + (vertexData.x * destRect.z);
    worldPosition.y = destRect.y + (vertexData.y * destRect.w);

    // 2. Map coordinates cleanly to the light's sub-texture bounds
    fragmentUV.x = uvRect.x + (vertexData.z * uvRect.z);
    fragmentUV.y = uvRect.y + (vertexData.w * uvRect.w);

    // 3. Project world space using the camera matrix
    gl_Position = cameraMatrix * vec4(worldPosition, 0.0, 1.0);
    
    fragmentColor = vertexColor;
}
