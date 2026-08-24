#version 150

uniform sampler2D texture_diffuse;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main()
{
    
    out_Color = texture(texture_diffuse, pass_TexCoord);
}