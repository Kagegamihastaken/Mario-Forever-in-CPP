// This uniform is provided by SFML automatically. It's the texture of the sprite being drawn.
uniform sampler2D texture;

void main()
{
    // Get the original color of the current pixel from the sprite's texture.
    vec4 originalColor = texture2D(texture, gl_TexCoord[0].xy);

    // Invert the Red, Green, and Blue components.
    // A value of 1.0 means full color, 0.0 means no color.
    // So, 1.0 - color inverts it.
    vec3 invertedColor = vec3(1.0) - originalColor.rgb;

    // The final color for the pixel is the inverted RGB with the original alpha (transparency).
    // This ensures transparent parts of your sprite remain transparent.
    gl_FragColor = vec4(invertedColor, originalColor.a);
}