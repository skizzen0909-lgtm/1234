#version 150

uniform sampler2D texture_diffuse;
uniform float time;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
    // Базовый цвет текстуры
    vec4 baseColor = texture(texture_diffuse, pass_TexCoord);
    
    // Простой "эффект освещения": увеличиваем яркость по центру
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(pass_TexCoord, center);
    float lightFactor = 1.0 - smoothstep(0.3, 0.6, dist);
    
    // Добавляем мерцание с течением времени
    float flicker = 0.2 * sin(time * 2.0);
    lightFactor += flicker;
    
    // Применяем освещение
    out_Color = baseColor * (1.0 + lightFactor * 0.8);
}