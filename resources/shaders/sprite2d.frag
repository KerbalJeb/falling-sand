#version 300 es
in  mediump vec2 uvCords;
out mediump vec4 color;

uniform mediump sampler2D image;
uniform mediump vec3 spriteColor;

void main() {
    color = vec4(spriteColor, 1.0) * texture(image, uvCords);
}
