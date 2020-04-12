#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
//layout(location = 1) in vec2 texCoord;

//out vec2 v_TexCoord; // varying variable
out vec4 v_Color;
uniform mat4 u_MVP; // Model view projection matrix

void main()
{
    v_Color = color;
    gl_Position = u_MVP * position;
    //v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
in vec4 v_Color;

void main()
{
    o_Color = v_Color;
};