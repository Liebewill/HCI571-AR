uniform sampler2D glSample0;
uniform sampler2D glSample1;
varying vec3 lightDir,normal;

#define textureWidth 1920.0
#define textureHeight 1080.0
#define texel_size_x 1.0 / textureWidth
#define texel_size_y 1.0 / textureHeight


bool greater(in vec4 v1, in vec4 v2)
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
    
    

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Video Mapping
        
    // Fetch the current pixel in screen coordinates
    vec2 coord = gl_FragCoord.xy;
    coord.x =  coord.x / 1920.0;/// 2.0 + 0.225;
    coord.y =  coord.y / 1080.0;// / 2.0;
    
    vec4 prerender_color = texture2D(glSample1,coord);
    
    if(greater(prerender_color, vec4(0.0,0.0,0.0,0.0) ) )
    {
        // Map the Video onto the 3D model
        gl_FragColor = prerender_color;
        //discard;
    }
    else
    {
        gl_FragColor =   texture2D(glSample0,coord) +   + vec4(0.6,0.6,0.6,1.0) ;
    }
    

}
