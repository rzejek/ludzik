/*
 * GltfBase.h
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#ifndef GLTFBASE_H_
#define GLTFBASE_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "JsonCommons.h"

struct Accessor
{
	int BufferView;
	int ComponentType;
	int Count;
	std::string Type;

	std::vector<char > Bytes;
	std::vector<short int > ShortInts;
	std::vector<int > Ints;
	std::vector<float > Floats;

	void Calculate(std::vector<char > bytes);
	std::vector<glm::vec3> getVec3();
	std::vector<glm::vec4> getVec4();
};

struct Material
{
	float r,g,b, alpha;
};

struct Primitive
{
	int Position;
	int Normal;
	int Indices;
	int Material;
};

struct Mesh
{
	std::string Name;
	std::vector<Primitive > Primitives;
};

struct Node
{
	std::string Name;
	int MeshId;

    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;

    Node()
    {
    	Scale = glm::vec3(1.0f);
    	Position = glm::vec3(.0f);
    	Rotation = glm::vec3(.0f);
    }
};

struct AnimationNode
{
	std::string Name;
	int NodeId;
	std::string Path;
	int Input;
	int Output;
};

class GltfBase
{
public:
	void init(std::string filename);

	std::vector<Accessor > Accessors;
	std::vector<Mesh > Meshes;
	std::vector<Material > Materials;
	std::vector<Node > Nodes;
	std::vector<AnimationNode > Animations;
};

#endif /* GLTFBASE_H_ */
