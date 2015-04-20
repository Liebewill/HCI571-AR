uniform sampler2D glSample0;
uniform sampler2D glSample1;
uniform sampler2D glSample2;
varying vec3 lightDir,normal;

#define textureWidth 512.0
#define textureHeight 512.0	
#define texel_size_x 1.0 / textureWidth
#define texel_size_y 1.0 / textureHeight


vec3 color = vec3(0.0, 0.0, 0.0);

bool greater(in vec3 v1, in vec3 v2)
{
    return all(greaterThan(v1,v2));
}



void main()
{
    vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;

    intensity = max(dot(lightDir,normalize(normal)),0.0);
    
    cf = intensity * (gl_FrontMaterial.diffuse).rgb + gl_FrontMaterial.ambient.rgb;
    af = gl_FrontMaterial.diffuse.a;
    
    
    // Fetch the current pixel in screen coordinates
    vec2 coord = gl_FragCoord.xy;
    coord.x =  coord.x / 1680.0;
    coord.y =  coord.y / 1050.0;
    
    // Map the Video
   
    vec4 maskColor = texture2D(glSample1,coord);

    if( greater(maskColor.xyz,color )  )
    {
        //discard;
        gl_FragColor = gl_Color * texture2D(glSample2, coord);
    }
    else
    {
    
        // Combines the vertex color from lighting operation with the fragment color.
        gl_FragColor = gl_Color * texture2D(glSample0, gl_TexCoord[0].xy);// + texture2D(glSample1,coord);;
    }
    
    

    // Provides a depth value that pushes the video into the background. 
    gl_FragDepth = 0.9999;
}
