#version 300 es
in mediump vec4 vertex;

out mediump vec2 uvCords;

uniform mediump mat4 model;
uniform mediump mat4 projection;

void main() {
    uvCords = vertex.zw;
    gl_Position = projection*model*vec4(vertex.xy, 0.0, 1.0);
}
