// the second input from vertex shader
varying vec3 norm;
varying vec2 tc;
uniform sampler2D TEX;	//texture object from application

// input from the rasterizer
varying vec4 epos;

// light properties
uniform vec4 lightPosition, Ia, Id, Is;	

// material properties
uniform vec4	Ka, Kd, Ks;
uniform float	shininess;

uniform mat4 viewMatrix;

void main()
{
	vec4 lPos = viewMatrix*lightPosition;	// light position in the eye-space coordinate

	vec3 n = normalize(norm);	// norm interpolated via rasterizer should be normalized again here
	vec3 p = epos.xyz;			// 3D position of this fragment
	vec3 l = normalize(lPos.xyz-(lPos.a==0.0?vec3(0):p));	// lPos.a==0 means directional light
	vec3 v = normalize(-p);									// eye-ecPos = vec3(0)-ecPos
	vec3 h = normalize(l+v);	// the halfway vector

	vec4 Ira = Ka*Ia;									// ambient reflection
	vec4 Ird = max(Kd*dot(l,n)*Id,0.0);					// diffuse reflection
	vec4 Irs = max(Ks*pow(dot(h,n),shininess)*Is,0.0);	// specular reflection

	//gl_FragColor = Ira + Ird + Irs;
	gl_FragColor = vec4(1.f,1.f,1.f,1.f);//vec4(normalize(norm), 1.0);
	//gl_FragColor = texture2D( TEX, tc ) + Ira + Ird + Irs; // sampling color from texture

}
