#include "Mesh_Pierced.h"
//#include "Mesh_remesh.h"
int main()
{
	Mesh_Pierced mesh;

	mesh.Read_Mesh_data();
	mesh.Read_Contour_data();
	mesh.Remove_inContour_triangles();

	//mesh.Remesh_Contour();

	//Mesh_remesh remesh;
	//remesh.Remesh_onecontour();

    system("pause");
    return 0;
}

//	//ifstream infile("in_file/bunny_uv2.txt");
//	//std::ofstream out("out_file/bunny_uv2.txt");
//	//string line;
//	//if (infile)
//	//{
//	//	while (getline(infile, line))
//	//	{
//	//		vector<string> result;
//	//		boost::split(result, line, boost::is_any_of("	"), boost::token_compress_on);
//	//		double v_x = atof(result[0].c_str());
//	//		double v_y = atof(result[1].c_str());
//	//		out << v_x << " " << v_y << " 0" << std::endl;
//	//	}
//	//}
//	//infile.close();