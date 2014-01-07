uniform sampler2D texture;

void main(void)
{
  vec4 color = gl_Color * texture2D(texture, gl_TexCoord[0].st);

  float brightness = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;

  gl_FragColor = vec4( brightness, brightness, brightness, color.a );
} // main()
