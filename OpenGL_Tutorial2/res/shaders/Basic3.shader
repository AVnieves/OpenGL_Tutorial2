#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float textureIndex;
//layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; // varying variable
out vec4 v_Color;
out float v_TextureIndex;

uniform mat4 u_MVP; // Model view projection matrix

void main()
{
    v_Color = color;
    v_TexCoord = a_TexCoord;
    v_TextureIndex = textureIndex;
    gl_Position = u_MVP * position;


};

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TextureIndex;

uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TextureIndex);
    o_Color = texture(u_Textures[index], v_TexCoord);
    //o_Color = v_Color;
    //o_Color = vec4(1.0, v_TextureIndex/2, v_TextureIndex/2, 1.0);
};