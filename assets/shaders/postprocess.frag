#version 150

uniform sampler2D texture_diffuse;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
    // Базовый цвет
    vec4 color = texture(texture_diffuse, pass_TexCoord);
    
    // Простой эффект пост-обработки: слегка повышаем контраст и насыщенность
    vec3 finalColor = color.rgb;
    
    // Повышаем контраст
    finalColor = (finalColor - 0.5) * 1.2 + 0.5;
    
    // Повышаем насыщенность
    float gray = dot(finalColor, vec3(0.299, 0.587, 0.114));
    finalColor = mix(vec3(gray), finalColor, 1.1);
    
    // Легкий эффект виньетки (темные края)
    vec2 uv = pass_TexCoord - vec2(0.5);
    float vignette = 1.0 - dot(uv, uv) * 2.0;
    vignette = clamp(vignette, 0.0, 1.0);
    
    out_Color = vec4(finalColor * vignette, color.a);
}