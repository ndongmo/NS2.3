#version 130

// =========================================================================
// 🎛️ FEATURE TOGGLES: Comment out the line to disable the feature instantly
// =========================================================================
//#define USE_EMBOSS_NORMALS      // Toggle for brick cracks, stone tile edges, and grooves
//#define USE_BEVEL_SPHERE        // Toggle for turning items/entities into 3D curved volumes

// =========================================================================
// 📥 INTERPOLATED VARIABLES FROM VERTEX STAGE
// =========================================================================
in vec4 fragmentColor;
in vec2 fragmentUV;
in vec2 v_WorldPos;

// =========================================================================
// 📤 SHADER CHANNEL OUTPUTS
// =========================================================================
out vec4 color;

// =========================================================================
// 🗺️ MATERIAL & SCENE UNIFORMS
// =========================================================================
#define LIGHT_TYPE_POINT       0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_CONE        2
#define MAX_LIGHTS             32

struct Light {
    int   type;
    vec2  position;
    vec2  direction;
    vec3  color;
    float height;
    float intensity;
    vec2  cutoffs; // x = innerAngle, y = outerAngle
    float radius;
};

uniform Light     u_Lights[MAX_LIGHTS];
uniform int       u_ActiveLightCount;
uniform vec3      u_AmbientColor;      
uniform sampler2D ns2TextureSampler;   

// Required texture coordinates boundary layout block to isolate sprite cells
uniform vec4      uvRect; // Passed into shader to read current instance UV bounds (x, y, w, h)

void main() {
    // 1. Initialize total light tracking with your background baseline
    vec3 totalLight = u_AmbientColor;

    // 2. COMPUTE SURFACE NORMAL VECTOR ON THE FLY BASED ON CONFIGURATIONS
    vec3 normal = vec3(0.0, 0.0, 1.0); // Default flat normal vector facing the camera

    #if defined(USE_EMBOSS_NORMALS)
        // Hardcoded asset resolution factor (adjust based on texture atlas dimensions, e.g., 2048)
        vec2 texelSize = vec2(1.0 / 2048.0); 

        // Grayscale slope profile reading adjacent pixels on the active channel
        float left  = texture2D(ns2TextureSampler, fragmentUV - vec2(texelSize.x, 0.0)).r;
        float right = texture2D(ns2TextureSampler, fragmentUV + vec2(texelSize.x, 0.0)).r;
        float up    = texture2D(ns2TextureSampler, fragmentUV + vec2(0.0, texelSize.y)).r;
        float down  = texture2D(ns2TextureSampler, fragmentUV - vec2(0.0, texelSize.y)).r;

        // Extract a slope vector. 0.2 represents depth; lower values make details stand out more.
        normal = normalize(vec3(left - right, down - up, 0.2));
    #endif

    #if defined(USE_BEVEL_SPHERE)
        // Convert the current fragment UV relative to its bounding sprite card layout box
        vec2 localCoord = (fragmentUV - uvRect.xy) / uvRect.zw;
        vec2 sphereXY = (localCoord - vec2(0.5)) * 2.0; // Map range to [-1.0..1.0] centering on center points

        float distSq = dot(sphereXY, sphereXY);
        if (distSq <= 1.0) {
            float sphereZ = sqrt(1.0 - distSq);
            // Blend or overwrite the current normal distribution maps
            normal = normalize(vec3(sphereXY, sphereZ));
        }
    #endif

    // 3. Single-pass loop accumulating all scene lights over our final normal vector direction
    for (int i = 0; i < u_ActiveLightCount; ++i) {
        Light light = u_Lights[i];

        if (light.type == LIGHT_TYPE_DIRECTIONAL) {
            totalLight += light.color * light.intensity;
            continue;
        }

        float distance = length(light.position - v_WorldPos);
        if (distance > light.radius) continue;

        float attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);

        // Calculate a 3D light vector. 45.0 represents the virtual elevation height of your torches.
        vec3 lightDir3D = normalize(vec3(light.position - v_WorldPos, light.height));
        
        // standard diffuse mapping rule formulation
        float diffuse = max(dot(normal, lightDir3D), 0.0);
        vec3 finalColor = light.color * light.intensity * attenuation * diffuse;

        if (light.type == LIGHT_TYPE_POINT) {
            totalLight += finalColor;
        }
        else if (light.type == LIGHT_TYPE_CONE) {
            vec2 fragDir  = normalize(v_WorldPos - light.position);
            vec2 lightDir = normalize(light.direction);
            
            float cosAngle     = dot(fragDir, lightDir);
            float currentAngle = acos(cosAngle);

            float coneMask = smoothstep(light.cutoffs.y, light.cutoffs.x, currentAngle);
            totalLight += finalColor * coneMask;
        }
    }

    // 4. Sample and mix lighting layers
    vec4 textureColor = texture2D(ns2TextureSampler, fragmentUV);
    color = vec4(textureColor.rgb * totalLight, textureColor.a) * fragmentColor;
}
