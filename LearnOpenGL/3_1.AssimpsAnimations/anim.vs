#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIdxes;
layout (location = 4) in vec4 aBoneWeights;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES_SLOTS = 4;
const int MAX_BONES_SUPPORT = 100;

uniform mat4 bones[MAX_BONES_SUPPORT];

void main()
{
	mat4 transform = mat4(1.f);
	for (int i = 0;i < MAX_BONES_SLOTS;i++) {
		transform += bones[aBoneIdxes[i]] * aBoneWeights[i];
	}

    TexCoords = aTexCoords;
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
	//vec4 PosL    = BoneTransform * vec4(aPos, 1.0);
	//gl_Position  = projection * view * model * PosL;
}

