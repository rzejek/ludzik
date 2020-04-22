/*
 * GltfBase.cpp
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#include "GltfBase.h"

using namespace std;


void GltfBase::init(std::string filename)
{
	fstream file(filename);

	std::stringstream buffer;
	buffer << file.rdbuf();

	string str = buffer.str();

	int count = 0;
	JsonObject * root = new JsonObject("");
	root->parse(str, count);

	int scene_id = -1;

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonValueType type = var->type;
		if (type == EJsonNumber)
		{
			if (var->name == "scene") scene_id = var->number_value;
		}
	}

	vector<string > filesbin;
	vector<int > sizesbin;
	vector<vector<char> > vbins;

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "buffers")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				JsonObject * obj = table->object_values[i];
				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];
					if (var->name == "uri" && var->type == EJsonString)
					{
						filesbin.push_back("models/"+var->string_value);
					}
					if (var->name == "byteLength" && var->type == EJsonNumber)
					{
						int size = var->number_value;
						sizesbin.push_back(size);
					}
				}
			}
		}
	}

	for (int i = 0; i < filesbin.size(); i++)
	{
		string file_name = filesbin[i];
		ifstream file(file_name, ios::in | ios::binary);
		int size = sizesbin[i];

		vector<char> vbytes;
		vbytes.resize(size);

		file.read(&vbytes[0], size);
		int readed = file.gcount();
		if (readed != size) cout << "read bad count!" << endl;

		vbins.push_back(vbytes);
	}

	vector<vector<char> > views;

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "bufferViews")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				JsonObject * obj = table->object_values[i];
				int bufid = -1;
				int byteLen = 0;
				int byteOffset = 0;
				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];
					if (var->name == "buffer") bufid = var->number_value;
					if (var->name == "byteLength") byteLen = var->number_value;
					if (var->name == "byteOffset") byteOffset = var->number_value;
				}
				if (bufid >= 0)
				{
					vector<char > & buf = vbins[bufid];
					vector<char > values(buf.begin()+byteOffset, buf.begin()+byteOffset+byteLen);
					views.push_back(values);
				}
			}

		}
	}

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "accessors")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				JsonObject * obj = table->object_values[i];
				Accessor accessor;
				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];
					if (var->name == "bufferView") accessor.BufferView = var->number_value;
					if (var->name == "componentType") accessor.ComponentType = var->number_value;
					if (var->name == "count") accessor.Count = var->number_value;
					if (var->name == "type") accessor.Type = var->string_value;
				}
//				cout << "Accessor "<< i <<" : " << " bufferView:" << accessor.BufferView << " componentType:" <<accessor.ComponentType<< " count:"<<accessor.Count << " type:" << accessor.Type << endl;
				Accessors.push_back(accessor);
			}
		}
	}

	for (int i = 0; i < Accessors.size(); i++)
	{
//		Accessor accessor = Accessors[i];
		Accessors[i].Calculate(views[Accessors[i].BufferView]);
//		cout << "Accessor calculate3 "<< accessor.ComponentType<<" size:" << accessor.ShortInts.size() << endl;

	}

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "meshes")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				Mesh mesh;
				JsonObject * obj = table->object_values[i];
				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];
					if (var->name == "name") mesh.Name = var->string_value;
					if (var->name == "primitives" && var->type == EJsonTable)
					{
						JsonTable * table = dynamic_cast<JsonTable *>(var);
						for (int i = 0; i < table->object_values.size(); i++)
						{
							Primitive primitive;

							JsonObject * obj = table->object_values[i];
							for (int i = 0; i < obj->values.size(); i++)
							{
								JsonVariant * var = obj->values[i];

								if (var->name == "attributes" && var->type == EJsonObject)
								{
									JsonObject * obj = dynamic_cast<JsonObject *>(var);
									if (obj)
									{
										for (int i = 0; i < obj->values.size(); i++)
										{
											JsonVariant * var = obj->values[i];
											if (var->name == "POSITION") primitive.Position = var->number_value;
											if (var->name == "NORMAL") primitive.Normal = var->number_value;
										}
									}
								}
								if (var->name == "indices")
								{
									primitive.Indices = var->number_value;
								}
								if (var->name == "material")
								{
									primitive.Material = var->number_value;
								}
							}

							mesh.Primitives.push_back(primitive);
						}
					}
				}

				Meshes.push_back(mesh);
			}
		}
	}

	int mat_count = 0;
	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "materials")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				Material material;

				JsonObject * obj = table->object_values[i];

				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];

					if (var->name == "pbrMetallicRoughness")
					{
						JsonObject * obj = dynamic_cast<JsonObject *>(var);

						for (int i = 0; i < obj->values.size(); i++)
						{
							JsonVariant * var = obj->values[i];
							JsonTable * table = dynamic_cast<JsonTable *>(var);
							if (table && table->name == "baseColorFactor")
							{
								if (table->float_values.size() == 4)
								{
									material.r = table->float_values[0];
									material.g = table->float_values[1];
									material.b = table->float_values[2];
									material.alpha = table->float_values[3];

									mat_count++;
								}
							}
						}
					}
				}
				Materials.push_back(material);
			}
		}
	}

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "nodes")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				JsonObject * obj = table->object_values[i];
				Node node;
				node.MeshId = -1;
				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];

					if (var->name == "mesh")  node.MeshId = var->number_value;
					if (var->name == "name")  node.Name = var->string_value;

					if (var->name == "translation")
					{
						JsonTable * table = dynamic_cast<JsonTable *>(var);
						node.Position.x = table->float_values[0];
						node.Position.y = table->float_values[1];
						node.Position.z = table->float_values[2];
					}
					if (var->name == "scale")
					{
						JsonTable * table = dynamic_cast<JsonTable *>(var);
						node.Scale.x = table->float_values[0];
						node.Scale.y = table->float_values[1];
						node.Scale.z = table->float_values[2];
					}
					if (var->name == "rotation")
					{
						JsonTable * table = dynamic_cast<JsonTable *>(var);
						node.Rotation.x = table->float_values[0];
						node.Rotation.y = table->float_values[1];
						node.Rotation.z = table->float_values[2];
					}
				}
//				cout << "Nodes add: meshid:" << node.MeshId<< " i:" << i << " name:"<<node.Name<< endl;
				Nodes.push_back(node);
			}
		}
	}

	for (int i = 0; i < root->values.size(); i++)
	{
		JsonVariant * var = root->values[i];
		JsonTable * table = dynamic_cast<JsonTable *>(var);
		if (table && table->name == "animations")
		{
			for (int i = 0; i < table->object_values.size(); i++)
			{
				JsonObject * obj = table->object_values[i];
				AnimationNode animation;
				for (int i = 0; i < obj->values.size(); i++)
				{
					JsonVariant * var = obj->values[i];

					if (var->name == "channels")
					{
						JsonTable * table = dynamic_cast<JsonTable * >(var);
						for (int i = 0; i < table->object_values.size(); i++)
						{
							JsonObject * obj = table->object_values[i];
							for (int i = 0; i < obj->values.size(); i++)
							{
								JsonVariant * var = obj->values[i];
								if (var->name == "sampler")
								{
								}
								if (var->name == "target")
								{
									JsonObject * obj = dynamic_cast<JsonObject * >(var);
									for (int i = 0; i < obj->values.size(); i++)
									{
										JsonVariant * var = obj->values[i];
										if (var->name == "node") animation.NodeId = var->number_value;
										if (var->name == "path") animation.Path = var->string_value;
									}
								}
							}
						}
					}
					if (var->name == "name")  animation.Name = var->string_value;
					if (var->name == "samplers") {
						JsonTable * table = dynamic_cast<JsonTable * >(var);
						for (int i = 0; i < table->object_values.size(); i++)
						{
							JsonObject * obj = table->object_values[i];
							for (int i = 0; i < obj->values.size(); i++)
							{
								JsonVariant * var = obj->values[i];
								if (var->name == "input") animation.Input = var->number_value;
								if (var->name == "interpoletion") ;
								if (var->name == "output") animation.Output = var->number_value;
							}
						}
					}
				}
				Animations.push_back(animation);
			}
		}
	}


	cout << "Animations size: " << Animations.size() <<endl;
	cout << "Nodes size: " << Nodes.size() <<endl;
	cout << "Materials size: " << Materials.size() <<endl;
	cout << "Materials count: " << mat_count <<endl;
	cout << "Meshes size: " << Meshes.size()<<endl;
	cout << "views size: " << views.size()<<endl;
	cout << "accessors size: " << Accessors.size()<<endl;

	for (int i = 0; i < filesbin.size(); i++)
	{
		cout << "name: "<<filesbin[i] << endl;
		cout << "size: "<<vbins[i].size() << endl;
	}

	cout << "scene id : " << scene_id << endl;
}

void Accessor::Calculate(vector<char > bytes)
{
	int compsize = 0;
	if (Type == "SCALAR") compsize = 1;
	if (Type == "VEC2") compsize = 2;
	if (Type == "VEC3") compsize = 3;
	if (Type == "VEC4") compsize = 4;

	if (ComponentType == 5120 || ComponentType == 5121) //byte
	{
		int size = Count*compsize*1;
		if (size != bytes.size())
		{
			cout << "! calculate bad count "<<ComponentType<< endl;
			return;
		}

		for (int i = 0; i < size; i++)
		{
			Bytes.push_back(bytes[i]);
		}
	}

	if (ComponentType == 5122 || ComponentType == 5123) //short
	{
		int size = Count*compsize*2;
		if (size != bytes.size())
		{
			cout << "! calculate bad count "<<ComponentType<< endl;
			return;
		}
//		else
//			cout << "calculate "<< ComponentType<<" size:" << size<< endl;

//		cout << "calculate4 " << "short ints : ";
		for (int i = 0; i < size; )
		{
			short int elem;
			unsigned char byte = bytes[i++];
			elem = bytes[i++];
			elem = elem << 8;
			elem += byte;

			ShortInts.push_back(elem);
//			cout << elem << " ";
		}
//		cout << endl;
//		cout << "calculate2 "<< ComponentType<<" size:" << ShortInts.size() << endl;
	}

	if (ComponentType == 5125) //int
	{
		int size = Count*compsize*4;
		if (size != bytes.size())
		{
			cout << "! calculate bad count "<<ComponentType<< endl;
			return;
		}

		for (int i = 0; i < size; )
		{
			int elem;
			unsigned char byte1 = bytes[i++];
			unsigned char byte2 = bytes[i++];
			unsigned char byte3 = bytes[i++];
			unsigned char byte4 = bytes[i++];

			elem = byte4;
			elem = elem << 8;
			elem += byte3;
			elem = elem << 8;
			elem += byte2;
			elem = elem << 8;
			elem += byte1;

			Ints.push_back(elem);
		}
	}

	if (ComponentType == 5126) //float
	{
		int size = Count*compsize*4;
		if (size != bytes.size())
		{
			cout << "! calculate bad count "<<ComponentType<< endl;
			return;
		}

		for (int i = 0; i < size; )
		{
			int elem;
			unsigned char byte1 = bytes[i++];
			unsigned char byte2 = bytes[i++];
			unsigned char byte3 = bytes[i++];
			unsigned char byte4 = bytes[i++];

			elem = byte4;
			elem = elem << 8;
			elem += byte3;
			elem = elem << 8;
			elem += byte2;
			elem = elem << 8;
			elem += byte1;

			float * felem = reinterpret_cast<float * >(&elem);
			Floats.push_back(*felem);
		}
	}

}

std::vector<glm::vec3> Accessor::getVec3()
{
	vector<glm::vec3 > ret_vect;
	for (int i = 0; i < Floats.size();)
	{
		glm::vec3 vect3;

		float fl = Floats[i++];
		vect3.x = fl;
		fl = Floats[i++];
		vect3.y = fl;
		fl = Floats[i++];
		vect3.z = fl;

		ret_vect.push_back(vect3);
	}
	return ret_vect;
}

std::vector<glm::vec4> Accessor::getVec4()
{
	vector<glm::vec4 > ret_vect;
	for (int i = 0; i < Floats.size();)
	{
		glm::vec4 vect4;

		float fl = Floats[i++];
		vect4.x = fl;
		fl = Floats[i++];
		vect4.y = fl;
		fl = Floats[i++];
		vect4.z = fl;
		fl = Floats[i++];
		vect4.w = fl;

		ret_vect.push_back(vect4);
	}
	return ret_vect;
}
