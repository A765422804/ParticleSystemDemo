#version 330 core

#define TEST_DEFINE

#ifdef TEST_DEFINE
uniform vec4 testColor;
#endif

in vec4 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    //FragColor = ourColor;
#ifdef TEST_DEFINE
    FragColor = testColor;
#endif
    //FragColor = mix(texture(texture1, TexCoord), vec4(1.0, 1.0, 1.0, 1.0), 0.5);
    //FragColor = texture(texture1, TexCoord);
    //FragColor = vec4(1.0,1.0,1.0,1.0);
}


