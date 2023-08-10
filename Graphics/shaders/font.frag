#version 330 core
in vec2 vUv;
out vec4 outColor;

uniform sampler2D text;
uniform vec3 textColor;
// uniform vec3 outlineColor;

void main()
{
    // vec3 fill_col    = vec3(0.0, 0.0, 1.0); // e.g blue
    // vec3 outline_col = vec3(1.0, 0.0, 0.0); // e.g red

    // vec2 tex = texture2D(text, vUv).rg;
    // float fill    = tex.r;
    // float outline = tex.g;

    // float alpha    = max( fill, outline );
    // vec3 mix_color = mix( mix(vec3(0.0), fill_col, fill), outline_col, outline );

    // outColor = vec4(mix_color, alpha);

    // vec2 tex = texture2D(text, vUv).rg;
    // float fill = tex.r;
    // float outline = tex.g;

    // float alpha = max(fill, outline);
    // vec3 mix_color = mix(mix(vec3(0.0), textColor, fill), outlineColor, outline);

    // outColor = vec4(mix_color, alpha);

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vUv).r);
    outColor = vec4(textColor, 1.0) * sampled;
}