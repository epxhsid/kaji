#version 330 core

in vec2 vUV;
in vec4 vColor;

uniform sampler2D uGlyphAtlas;

out vec4 FragColor;

void main() {
    float coverage = texture(uGlyphAtlas, vUV).r;
    FragColor = vec4(vColor.rgb, vColor.a * coverage);
}
