#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D sequence_frame_texture;
//uniform vec3 spriteColor;

void main()
{
    color = texture(sequence_frame_texture, TexCoords);
    //color = vec4(vec3(texture(image, TexCoords)), ParticleColor.a);
}
