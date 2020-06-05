#version 330 core

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
	float opacity;
}; 

struct DirLight {
	vec3 color;
    vec3 direction;
};

struct PointLight {
	vec3 color;
    vec3 position;
	float attenuation;
};

struct SpotLight {
	vec3 color;
	vec3 position;
	vec3 direction;
	float cutoff;
	float exponent;
	float attenuation;
};

vec4 poissonDisk[64] = vec4[] (
	vec4(-0.613392, 0.617481, 0.0, 0.0 ),
	vec4(0.170019, -0.040254, 0.0, 0.0 ),
	vec4(-0.299417, 0.791925, 0.0, 0.0 ),
	vec4(0.645680, 0.493210, 0.0, 0.0 ),
	vec4(-0.651784, 0.717887, 0.0, 0.0 ),
	vec4(0.421003, 0.027070, 0.0, 0.0 ),
	vec4(-0.817194, -0.271096, 0.0, 0.0 ),
	vec4(-0.705374, -0.668203, 0.0, 0.0 ),
	vec4(0.977050, -0.108615, 0.0, 0.0 ),
	vec4(0.063326, 0.142369, 0.0, 0.0 ),
	vec4(0.203528, 0.214331, 0.0, 0.0 ),
	vec4(-0.667531, 0.326090, 0.0, 0.0 ),
	vec4(-0.098422, -0.295755, 0.0, 0.0 ),
	vec4(-0.885922, 0.215369, 0.0, 0.0 ),
	vec4(0.566637, 0.605213, 0.0, 0.0 ),
	vec4(0.039766, -0.396100, 0.0, 0.0 ),
	vec4(0.751946, 0.453352, 0.0, 0.0 ),
	vec4(0.078707, -0.715323, 0.0, 0.0 ),
	vec4(-0.075838, -0.529344, 0.0, 0.0 ),
	vec4(0.724479, -0.580798, 0.0, 0.0 ),
	vec4(0.222999, -0.215125, 0.0, 0.0 ),
	vec4(-0.467574, -0.405438, 0.0, 0.0 ),
	vec4(-0.248268, -0.814753, 0.0, 0.0 ),
	vec4(0.354411, -0.887570, 0.0, 0.0 ),
	vec4(0.175817, 0.382366, 0.0, 0.0 ),
	vec4(0.487472, -0.063082, 0.0, 0.0 ),
	vec4(-0.084078, 0.898312, 0.0, 0.0 ),
	vec4(0.488876, -0.783441, 0.0, 0.0 ),
	vec4(0.470016, 0.217933, 0.0, 0.0 ),
	vec4(-0.696890, -0.549791, 0.0, 0.0 ),
	vec4(-0.149693, 0.605762, 0.0, 0.0 ),
	vec4(0.034211, 0.979980, 0.0, 0.0 ),
	vec4(0.503098, -0.308878, 0.0, 0.0 ),
	vec4(-0.016205, -0.872921, 0.0, 0.0 ),
	vec4(0.385784, -0.393902, 0.0, 0.0 ),
	vec4(-0.146886, -0.859249, 0.0, 0.0 ),
	vec4(0.643361, 0.164098, 0.0, 0.0 ),
	vec4(0.634388, -0.049471, 0.0, 0.0 ),
	vec4(-0.688894, 0.007843, 0.0, 0.0 ),
	vec4(0.464034, -0.188818, 0.0, 0.0 ),
	vec4(-0.440840, 0.137486, 0.0, 0.0 ),
	vec4(0.364483, 0.511704, 0.0, 0.0 ),
	vec4(0.034028, 0.325968, 0.0, 0.0 ),
	vec4(0.099094, -0.308023, 0.0, 0.0 ),
	vec4(0.693960, -0.366253, 0.0, 0.0 ),
	vec4(0.678884, -0.204688, 0.0, 0.0 ),
	vec4(0.001801, 0.780328, 0.0, 0.0 ),
	vec4(0.145177, -0.898984, 0.0, 0.0 ),
	vec4(0.062655, -0.611866, 0.0, 0.0 ),
	vec4(0.315226, -0.604297, 0.0, 0.0 ),
	vec4(-0.780145, 0.486251, 0.0, 0.0 ),
	vec4(-0.371868, 0.882138, 0.0, 0.0 ),
	vec4(0.200476, 0.494430, 0.0, 0.0 ),
	vec4(-0.494552, -0.711051, 0.0, 0.0 ),
	vec4(0.612476, 0.705252, 0.0, 0.0 ),
	vec4(-0.578845, -0.768792, 0.0, 0.0 ),
	vec4(-0.772454, -0.090976, 0.0, 0.0 ),
	vec4(0.504440, 0.372295, 0.0, 0.0 ),
	vec4(0.155736, 0.065157, 0.0, 0.0 ),
	vec4(0.391522, 0.849605, 0.0, 0.0 ),
	vec4(-0.620106, -0.328104, 0.0, 0.0 ),
	vec4(0.789239, -0.419965, 0.0, 0.0 ),
	vec4(-0.545396, 0.538133, 0.0, 0.0 ),
	vec4(-0.178564, -0.596057, 0.0, 0.0 )
);

const int NUM_CASCADES = 3;

in vec3 FragPos;
in vec3 Normal;
in vec2 coord;
in vec4 ShadowCoord[NUM_CASCADES];
in float shadowClipZ;

out vec4 color;

uniform vec3 uColor;

uniform vec3 viewPos;
uniform float ambientCoefficient;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;

uniform bool directionalOn;
uniform bool pointOn;
uniform bool spotOn;

uniform sampler2DShadow shadowTextureID[NUM_CASCADES];
uniform float cascadeEndClip[NUM_CASCADES];
uniform bool shadowsOn;

uniform sampler2D sprite;

// Function prototypes
vec3 calcDiffuse( vec3 lightColor, vec3 lightDir, vec3 normal );
vec3 calcSpecular( vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir );
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

// Returns a random number based on a vec3 and an int.
float random( vec3 seed, int i ) {

	vec4 seed4 = vec4( seed, i );
	float dot_product = dot( seed4, vec4( 12.9898, 78.233, 45.164, 94.673 ) );
	return fract( sin( dot_product ) * 43758.5453 );

}

int getShadowMap( float shadowClipZ ) {

	for ( int i = 0; i < NUM_CASCADES; i++ ) {
	
		if ( shadowClipZ <= cascadeEndClip[i] ) { // In shadow map.
			return i;
		}
	
	}
	return -1; // Not in any shadow map.

}

float calculateShadow( int shadowMapIdx, vec4 shadowCoord ) {

	float bias = max( 0.02 * ( 1.0 - pow( dot( Normal, dirLight.direction ), 2 ) ), 0.005 );
	shadowCoord.z -= bias; // Apply bias to shadow texture depth coordinate.

	float shadowCoeff = 0.5 + 0.5 * textureProj( shadowTextureID[shadowMapIdx], shadowCoord );
	for ( int i=0; i < 16; i++ ) {

        int index = int( 16.0 * random( floor( FragPos.xyz * 1000.0 ), i ) ) % 64; // A random number between 0 and 15, different for each pixel (and each i !)
        shadowCoeff -= 0.2 * ( 1.0 - textureProj( shadowTextureID[shadowMapIdx], shadowCoord + poissonDisk[index] / 1500.0 ) );

    }

	return max( shadowCoeff, 0.0 );

}

void main() {

    // Properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );
	
	vec3 result = vec3( 0.0 );

    // == ======================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == ======================================
    // Phase 1: Directional lighting
    if ( directionalOn ) {
		result += CalcDirLight( dirLight, norm, viewDir );
	}
    // Phase 2: Point light
	//if ( pointOn ) {
	//	result += CalcPointLight( pointLight, norm, FragPos, viewDir );    
	//}
    // Phase 3: Spot light
	//if ( spotOn ) {
	//	result += CalcSpotLight( spotLight, norm, FragPos, viewDir );   
	//}

	vec3 ambient = material.ambient * ambientCoefficient;

	float shadowCoefficient = 1.0;
	//if ( shadowsOn ) { // Render shadows if turned on.
	//	int shadowMap = getShadowMap( shadowClipZ );
	//	if ( shadowMap != -1 ) { // In a shadow map.
	//		shadowCoefficient = calculateShadow( shadowMap, ShadowCoord[shadowMap] );
	//	}
	//}
	result = result * shadowCoefficient + ambient;

	result.x = min( result.x, 1.0 );
	result.y = min( result.y, 1.0 );
	result.z = min( result.z, 1.0 );

	//color = vec4( result * uColor, material.opacity );
	color = texture(sprite, coord) * vec4( result * uColor, material.opacity );

}

// Calculates the diffuse component of the reflected light.
vec3 calcDiffuse( vec3 lightColor, vec3 lightDir, vec3 normal ) {

	float diffFactor = max( dot( normal, lightDir ), 0.0 );
	return diffFactor * material.diffuse * lightColor;

}

// Calculates the specular component of the reflected light.
vec3 calcSpecular( vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir ) {

	vec3 reflectDir = reflect( -lightDir, normal );
    float specFactor = pow( max( dot( viewDir, reflectDir ), 0.0 ), max( material.shininess, 0.000001 ) );
	return specFactor * material.specular * lightColor;

}

// Calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir ) {

    vec3 lightDir = normalize( -light.direction );
	
	if ( dot( normal, lightDir ) <= 0.0f ) {
		return vec3( 0.0f, 0.0f, 0.0f ); // Light not in LOS of point.
	}

    // Calculate components
    vec3 diffuse = calcDiffuse( light.color, lightDir, normal );
    vec3 specular = calcSpecular( light.color, lightDir, normal, viewDir );

	// Combine results
    return ( diffuse + specular );

}

// Calculates the color when using a point light.
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir ) {

    vec3 lightDir = normalize( light.position - fragPos );

	if ( dot( normal, lightDir ) <= 0.0f ) {
		return vec3( 0.0f, 0.0f, 0.0f ); // Light not in LOS of point.
	}

    // Calculate components
    vec3 diffuse = calcDiffuse( light.color, lightDir, normal );
    vec3 specular = calcSpecular( light.color, lightDir, normal, viewDir );

    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.attenuation * distance );
    diffuse *= attenuation;
    specular *= attenuation;
	
	// Combine results
    return ( diffuse + specular );

}

// Calculates the color when using a spot light.
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir ) {

	vec3 lightDir = normalize( light.position - fragPos );
	
	if ( dot( normal, lightDir ) <= 0.0f ) {
		return vec3( 0.0f, 0.0f, 0.0f ); // Light not in LOS of point.
	}

	// Calculate components
    vec3 diffuse = calcDiffuse( light.color, lightDir, normal );
    vec3 specular = calcSpecular( light.color, lightDir, normal, viewDir );

	// Effect of distance from center of cone
	float angleFactor = -dot( lightDir, light.direction );
	if ( angleFactor < light.cutoff ) { // Outside of cone bounds
		return vec3( 0.0f, 0.0f, 0.0f );
	}
	angleFactor = pow( angleFactor, light.exponent );

    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.attenuation * pow( distance, 2 ) );
	attenuation *= angleFactor;
	diffuse *= attenuation;
    specular *= attenuation;
	
    // Combine results
    return ( diffuse + specular );

}