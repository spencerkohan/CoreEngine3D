/*
	If the current vertex is affected by bones then the vertex position and
	normal will be transformed by the bone matrices. Each vertex wil have up 
	to 4 bone indices (in_boneindex) and bone weights (in_boneweight).
	
	The indices are used to index into the array of bone matrices 
	(BoneMatrixArray) to get the required bone matrix for transformation. The 
	amount of influence a particular bone has on a vertex is determined by the
	weights which should always total 1. So if a vertex is affected by 2 bones 
	the vertex position in world space is given by the following equation:

	position = (BoneMatrixArray[Index0] * in_position) * Weight0 + 
	           (BoneMatrixArray[Index1] * in_position) * Weight1

	The same proceedure is applied to the normals but the translation part of 
	the transformation is ignored.

	After this the position is multiplied by the view and projection matrices 
	only as the bone matrices already contain the model transform for this 
	particular mesh. The two-step transformation is required because lighting 
	will not work properly in clip space.
*/

attribute vec3 in_position;
attribute vec3 in_normal;
attribute vec3 in_tangent;
attribute vec3 in_binormal;
attribute vec2 in_texcoord;
attribute vec4 in_boneindex;
attribute vec4 in_boneweight;

uniform bool	bUseDot3;
uniform mat4 worldViewProjMat;
uniform vec3 lightPos;
uniform int	 BoneCount;
uniform mat4 BoneMatrixArray[8];
uniform mat3 BoneMatrixArrayIT[8];

varying vec3 lightDir;
varying vec2 texcoord;

void main()
{
	if(BoneCount > 0)
	{
		// On PowerVR SGX it is possible to index the components of a vector
		// with the [] operator. However this can cause trouble with PC
		// emulation on some hardware so we "rotate" the vectors instead.
		ivec4 boneIndex = ivec4(in_boneindex);
		vec4 boneWeights = in_boneweight;
	
		mat4 boneMatrix = BoneMatrixArray[boneIndex.x];
		mat3 normalMatrix = BoneMatrixArrayIT[boneIndex.x];
	
		vec4 position = boneMatrix * vec4(in_position, 1.0) * boneWeights.x;
		vec3 worldNormal = normalMatrix * in_normal * boneWeights.x;
		
		vec3 worldTangent;
		vec3 worldBiNormal;
		
		if(bUseDot3)
		{
			worldTangent = normalMatrix * in_tangent * boneWeights.x;
			worldBiNormal = normalMatrix * in_binormal * boneWeights.x;
		}
	
		for (int i = 1; i < 3; ++i)
		{
			if(i < BoneCount)
			{
				// "rotate" the vector components
				boneIndex = boneIndex.yzwx;
				boneWeights = boneWeights.yzwx;
			
				boneMatrix = BoneMatrixArray[boneIndex.x];
				normalMatrix = BoneMatrixArrayIT[boneIndex.x];

				position += boneMatrix * vec4(in_position, 1.0) * boneWeights.x;
				worldNormal += normalMatrix * in_normal * boneWeights.x;
				
				if(bUseDot3)
				{
					worldTangent += normalMatrix * in_tangent * boneWeights.x;
					worldBiNormal += normalMatrix * in_binormal * boneWeights.x;
				}
			}
		}		
		gl_Position = worldViewProjMat * position;
		
		// lighting
		vec3 TmpLightDir = normalize(lightPos - position.xyz);
		
		if(bUseDot3)
		{
			lightDir.x = dot(normalize(worldTangent), TmpLightDir);
			lightDir.y = dot(normalize(worldBiNormal), TmpLightDir);
			lightDir.z = dot(normalize(worldNormal), TmpLightDir);
		}
		else
		{
			lightDir.x = dot(normalize(worldNormal), TmpLightDir);
		}
	}

	
	// Pass through texcoords
	texcoord = in_texcoord;
}
 