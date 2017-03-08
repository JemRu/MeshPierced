#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <list>

//boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;
class Mesh_remesh
{
public:
	Mesh_remesh();
	~Mesh_remesh();

	//remesh Ò»¸öÂÖÀª
	void Remesh_onecontour();
};

