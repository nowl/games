uniform sampler2D tex0, tex1;
vec2 TexelKernelh[9];

void main()
{   
    float blurFactor = 0.9;

    vec4 baseColor = vec4(0.0, 0.0, 0.0, 0.0);
    int i;

    TexelKernelh[0] = vec2( -1, -1 );
    TexelKernelh[1] = vec2( -1, 0 );
    TexelKernelh[2] = vec2( -1, 1 );
    TexelKernelh[3] = vec2( 0, -1 );
    TexelKernelh[4] = vec2( 0, 0 );
    TexelKernelh[5] = vec2( 0, 1 );
    TexelKernelh[6] = vec2( 1, -1 );
    TexelKernelh[7] = vec2( 1, 0 );
    TexelKernelh[8] = vec2( 1, 1 );

    for (int i = 0; i < 9; i++)
    {    
        //baseColor += texture2D( tex1, gl_TexCoord[0].xy + TexelKernelh[i].xy);
        baseColor += texture2D( tex0, gl_TexCoord[0].xy + TexelKernelh[i].xy);
    }

    //float newColor = texture2D( tex0, gl_TexCoord[0].xy);

    //gl_FragColor = baseColor / 9.0 * blurFactor + newColor;
    gl_FragColor = baseColor / 9.0 * blurFactor;
}

