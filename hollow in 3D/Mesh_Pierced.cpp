#include "Mesh_Pierced.h"
Mesh_Pierced::Mesh_Pierced(){}
Mesh_Pierced::~Mesh_Pierced(){}

void Mesh_Pierced::Read_Mesh_data()
{
	OpenMesh::IO::Options optt;
	if (!OpenMesh::IO::read_mesh(meshdata, "out_mesh/test-bird-out.obj", optt))
	{
		std::cerr << "Cannot read mesh file" << std::endl;
	}
	else {
		std::cerr << "Get mesh data" << std::endl;
	}
}
//端点坐标序列（x,y,z），法向量（x,,y,z）
void Mesh_Pierced::Read_Contour_data() 
{
	//mesh.point(v_it) + mesh.normal(v_it)
	/*points.push_back(Point_3(1.272022009, 0.2767059803, 0.8843389153));
	points.push_back(Point_3(1.227403998, 0.03198600933, 0.9809429646));
	points.push_back(Point_3(1.464833975, 0.1518640071, 0.9982049465));
	points.push_back(Point_3(1.53057003, 0.02082101256, 1.058725953));*/
	//meshdata.request_vertex_normals();
	// Just make sure that point element type is double

	ifstream infile("in_txt/contour-birds-boundary.txt");
	string line;
	if (infile) 
	{
		while (getline(infile, line))
		{
			vector<string> result;
			boost::split(result, line, boost::is_any_of(" "), boost::token_compress_on);
			double v_x = atof(result[0].c_str());
			double v_y = atof(result[1].c_str());
			double v_z = atof(result[2].c_str());
			points.push_back(Point_3(v_x, v_y, v_z));
		}
	}
	infile.close();

	//if (typeid(OpenMesh::vector_traits<OMesh::Point>::value_type)
	//	!= typeid(double))
	//{
	//	std::cerr << "Ouch! ERROR! Data type is wrong!\n";
	//}
	//// Make sure that normal element type is double
	//if (typeid(OpenMesh::vector_traits<OMesh::Normal>::value_type)
	//	!= typeid(double))
	//{
	//	std::cerr << "Ouch! ERROR! Data type is wrong!\n";
	//}
	//// Add vertex normals as default property (ref. previous tutorial)
	//meshdata.request_vertex_normals();
	//// Add face normals as default property
	//meshdata.request_face_normals();
	//// load a mesh
	//OpenMesh::IO::Options opt;
	//// If the file did not provide vertex normals, then calculate them
	//if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
	//	meshdata.has_face_normals() && meshdata.has_vertex_normals())
	//{
	//	// let the mesh update the normals
	//	meshdata.update_normals();
	//}
	//// move all vertices one unit length along it's normal direction
	//for (OMesh::VertexIter v_it = meshdata.vertices_begin();v_it != meshdata.vertices_end(); ++v_it)
	//{
	//	
	//	if (meshdata.point(*v_it) == OMesh::Point(14.48060131, -15.95770073, 15.98349953) || meshdata.point(*v_it) == OMesh::Point(2.477210045, -18.96610069, 8.707119942) || meshdata.point(*v_it) == OMesh::Point(7.161119938, -19.17410088, 9.303420067) || meshdata.point(*v_it) == OMesh::Point(5.023829937, -19.47730064, 16.73649979))
	//	{
	//		std::cout << "Vertex #" << *v_it << ": " << meshdata.point(*v_it);
	//		double x = meshdata.point(*v_it)[0] + 1 * meshdata.normal(*v_it)[0];
	//		double y = meshdata.point(*v_it)[1] + 1 * meshdata.normal(*v_it)[1];
	//		double z = meshdata.point(*v_it)[2] + 1 * meshdata.normal(*v_it)[2];

	//		double x2 = meshdata.point(*v_it)[0] - 1 * meshdata.normal(*v_it)[0];
	//		double y2 = meshdata.point(*v_it)[1] - 1 * meshdata.normal(*v_it)[1];
	//		double z2 = meshdata.point(*v_it)[2] - 1 * meshdata.normal(*v_it)[2];

	//		std::cout << "normal: " << meshdata.normal(*v_it) << std::endl;
	//		std::cout << "add point x: " << x << "||" << y << "||" << z << std::endl;
	//		std::cout << "add point x2: " << x2 << "||" << y2 << "||" << z2 << std::endl;
	//		points.push_back(Point_3(x, y, z));
	//		points.push_back(Point_3(x2, y2, z2));

	//	}
	//}

	T.insert(points.begin(), points.end());

	std::list<Vertex_handle>  vertices;
	T.incident_vertices(T.infinite_vertex(), std::back_inserter(vertices));
	std::cout << "This convex hull of the n points has " << vertices.size() << " points on it." << std::endl;
	num_point = vertices.size();
}

void Mesh_Pierced::Remove_inContour_triangles()
{
	meshdata.request_face_status();
	meshdata.request_edge_status();
	meshdata.request_vertex_status();
	
	for (OMesh::VertexIter v_i = meshdata.vertices_sbegin(); v_i != meshdata.vertices_end(); ++v_i)
	{
		//std::cerr << "Get all vertex data" << meshdata.point(*v_i) << std::endl;
		float x = meshdata.point(*v_i)[0];
		float y = meshdata.point(*v_i)[1];
		float z = meshdata.point(*v_i)[2];
		points.push_back(Point_3(x, y, z));
		//std::cerr << "::::::::::points has: " << points.size() << std::endl;

		Delaunay T2;
		T2.insert(points.begin(), points.end());

		std::list<Vertex_handle>  vertice;
		T2.incident_vertices(T2.infinite_vertex(), std::back_inserter(vertice));
		//std::cerr << "convex has vexter: " << vertice.size() << std::endl;

		Polyhedron_3 convex;
		CGAL::convex_hull_3_to_polyhedron_3(T2,convex);
		if (convex.size_of_vertices() == num_point)
		{
			//std::cerr << "Get maybe in vertex data" << meshdata.point(*v_i) << std::endl;
			bool in = true;

			//remove point on convex
			for (Polyhedron_3::Point_iterator temp = convex.points_begin(); temp != convex.points_end(); temp++)
			{
				//std::cerr << "convex points is: " << *temp << std::endl;
				if (*temp == Point_3(x, y, z))
					in = false;
			}

			// delete points in convex
			if (in) 
			{
				std::cerr << "Get vertex data" << meshdata.point(*v_i) << std::endl;
				vhandle = (*v_i);
				meshdata.delete_vertex(vhandle, false);
			}
		}
		points.pop_back();
	}
	meshdata.garbage_collection();
	if (!OpenMesh::IO::write_mesh(meshdata,"test-bird-out.obj"))
	{
		std::cerr << "write error\n";
		exit(1);
	}
	meshdata.release_face_status();
	meshdata.release_edge_status();
	meshdata.release_vertex_status();
}

void Mesh_Pierced::Read_Contour_points()
{
	meshdata.request_face_status();
	meshdata.request_edge_status();
	meshdata.request_vertex_status();
	for (OMesh::VertexIter v_it = meshdata.vertices_begin(); v_it != meshdata.vertices_end(); ++v_it)
	{
		if (meshdata.point(*v_it) == OMesh::Point(14.48060131, -15.95770073, 15.98349953) || meshdata.point(*v_it) == OMesh::Point(2.477210045, -18.96610069, 8.707119942) || meshdata.point(*v_it) == OMesh::Point(7.161119938, - 19.17410088, 9.303420067) || meshdata.point(*v_it) == OMesh::Point(5.023829937, -19.47730064, 16.73649979))
		{
			std::cerr << "Get maybe in vertex data" << meshdata.point(*v_it) << std::endl;
			// delete point
			vhandle = (*v_it);
			meshdata.delete_vertex(vhandle, false);
			meshdata.garbage_collection();
		}
	}
	if (!OpenMesh::IO::write_mesh(meshdata, "bunnyr-point.obj"))
	{
		std::cerr << "write error\n";
		exit(1);
	}
	meshdata.release_face_status();
	meshdata.release_edge_status();
	meshdata.release_vertex_status();
	
}

void Mesh_Pierced::Get_Boundary_Points()
{
	//for (OMesh::VertexIter v_it = meshdata.vertices_begin(); v_it != meshdata.vertices_end(); ++v_it)
	//{
	//	vhandle = (*v_it);
	//	if (meshdata.is_boundary(vhandle))
	//	{
	//		/*double x = meshdata.point(*v_it)[0];
	//		double y = meshdata.point(*v_it)[1];
	//		double z = meshdata.point(*v_it)[2];
	//		points_temp.push_back(Point_3(x, y, z));*/
	//		//std::cerr << "Get vertex dataqqqqq:" << vhandle.idx() << std::endl;
	//	}
	//}
	for (OMesh::HalfedgeIter hv_it = meshdata.halfedges_begin(); hv_it != meshdata.halfedges_end(); ++hv_it)
	{
		
		ehandle = (*hv_it);
		if (meshdata.is_boundary(ehandle))
		{
			std::ofstream out_boundary("out_txt/boundary-test-bird.txt");
			for (OMesh::HalfedgeLoopIter hel_it = meshdata.hl_begin(*hv_it); hel_it.is_valid(); ++hel_it)
			{
				std::cerr << "Get boundary vertex in older:" << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << std::endl;
				out_boundary << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << endl;
			}
			out_boundary.close();
			break;
		}
	}
}

void Mesh_Pierced::Remesh_Contour()
{
	OpenMesh::IO::Options optt;
	if (!OpenMesh::IO::read_mesh(remeshdata, "out_mesh/test-bird-out.obj", optt))
	{
		std::cerr << "Cannot read mesh file" << std::endl;
	}
	else {
		std::cerr << "Get mesh data" << std::endl;
	}

	//put poins on boundary in vertexhandle
	ifstream infile("in_txt/contour-birds-boundary.txt");
	string line;
	OMesh::VertexHandle points_boundary[100];
	if (infile)
	{
		int i = 0;
		while (getline(infile, line))
		{
			vector<string> result;
			boost::split(result, line, boost::is_any_of(" "), boost::token_compress_on);
			double v_x = atof(result[0].c_str());
			double v_y = atof(result[1].c_str());
			double v_z = atof(result[2].c_str());
			//points_boundary[i] = remeshdata.add_vertex(OMesh::Point(v_x, v_y, v_z));
			points_temp_boundary_3.push_back(Point_3(v_x, v_y, v_z));
			i++;
		}
		std::cerr << "Get boundary data" << std::endl;
	}
	infile.close();

	//put poins on contour in vertexhandle
	ifstream in("in_txt/contour-birds-origin.txt");
	string line2;
	OMesh::VertexHandle points_contour[100];
	if (in)
	{
		int i = 0;
		while (getline(in,line2))
		{
			vector<string> pointscontour;
			boost::split(pointscontour, line2, boost::is_any_of(" "), boost::token_compress_on);
			double v_x = atof(pointscontour[0].c_str());
			double v_y = atof(pointscontour[1].c_str());
			double v_z = atof(pointscontour[2].c_str());
			//vertexhandle for add
			//points_contour[i] = remeshdata.add_vertex(OMesh::Point(v_x, v_y, v_z));
			//vector for search
			points_temp_contour_3.push_back(Point_3(v_x, v_y, v_z));
			i++;
		}
		std::cerr << "Get contour data" << std::endl;
	}
	in.close();

	ifstream infile4("out_mesh/traingles.txt");
	string line4;
	if (infile4)
	{
		while (getline(infile4, line4))
		{
			vector<string> result;
			boost::split(result, line4, boost::is_any_of(" "), boost::token_compress_on);
			string temp = "";
			double v_x = atof(result[0].c_str());
			double v_y = atof(result[1].c_str());

			double v2_x = atof(result[2].c_str());
			double v2_y = atof(result[3].c_str());

			double v33_x = atof(result[4].c_str());
			double v33_y = atof(result[5].c_str());

			OMesh::VertexHandle remesh_vhandle[3];

			std::cerr << "traverse vertex in contour 2D:" << v_x << " " << v_y << " " << std::endl;

			bool mayinboundary1 = true;
			//if on countour_3
			for (std::vector<Point_3>::iterator p3_i = points_temp_contour_3.begin(); p3_i != points_temp_contour_3.end(); ++p3_i)
			{
				double v3_x = (*p3_i).hx();
				double v3_y = (*p3_i).hy();
				double v3_z = (*p3_i).hz();

				//std::cerr << "traverse corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;/
				if (v3_x * v_x >= 0 && fabs(fabs(v3_x) - fabs(v_x)) < 0.00001 && v3_y * v_y >= 0 && fabs(fabs(v3_y) - fabs(v_y)) < 0.00001)
				{
					std::cerr << "add corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
					mayinboundary1 = false;
					remesh_vhandle[0] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, v3_z));
					break;
				}
			}

			std::cerr << "find second triangle points" << std::endl;
			//if on boundary
			if (mayinboundary1)
			{
				for (std::vector<Point_3>::iterator p3_i = points_temp_boundary_3.begin(); p3_i != points_temp_boundary_3.end(); ++p3_i)
				{
					double v3_x = (*p3_i).hx();
					double v3_y = (*p3_i).hy();
					double v3_z = (*p3_i).hz();
					//std::cerr << "traverse corresponding adjacent vertex in boundary 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
					if (v3_x * v_x >= 0 && fabs(fabs(v3_x) - fabs(v_x)) < 0.00001 && v3_y * v_y >= 0 && fabs(fabs(v3_y) - fabs(v_y)) < 0.00001)
					{
						std::cerr << "add corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
						remesh_vhandle[0] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, v3_z));
						break;
					}

				}
			}

			bool mayinboundary2 = true;
			//if on countour_3
			for (std::vector<Point_3>::iterator p3_i = points_temp_contour_3.begin(); p3_i != points_temp_contour_3.end(); ++p3_i)
			{
				double v3_x = (*p3_i).hx();
				double v3_y = (*p3_i).hy();
				double v3_z = (*p3_i).hz();
				//std::cerr << "traverse corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
				if (v3_x * v2_x >= 0 && fabs(fabs(v3_x) - fabs(v2_x)) < 0.00001 && v3_y * v2_y >= 0 && fabs(fabs(v3_y) - fabs(v2_y)) < 0.00001)
				{
					std::cerr << "add corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
					mayinboundary2 = false;
					remesh_vhandle[1] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, v3_z));
					break;
				}
			}

			//if on boundary
			if (mayinboundary2)
			{
				for (std::vector<Point_3>::iterator p3_i = points_temp_boundary_3.begin(); p3_i != points_temp_boundary_3.end(); ++p3_i)
				{
					double v3_x = (*p3_i).hx();
					double v3_y = (*p3_i).hy();
					double v3_z = (*p3_i).hz();
					//std::cerr << "traverse corresponding adjacent vertex in boundary 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
					if (v3_x * v2_x >= 0 && fabs(fabs(v3_x) - fabs(v2_x)) < 0.00001 && v3_y * v2_y >= 0 && fabs(fabs(v3_y) - fabs(v2_y)) < 0.00001)
					{
						std::cerr << "add corresponding adjacent vertex in boundary 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
						remesh_vhandle[1] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, v3_z));
						break;
					}

				}
			}

			std::cerr << "find third triangle points" << std::endl;
			bool mayinboundary3 = true;
			//if on countour_3
			for (std::vector<Point_3>::iterator p3_i = points_temp_contour_3.begin(); p3_i != points_temp_contour_3.end(); ++p3_i)
			{
				double v3_x = (*p3_i).hx();
				double v3_y = (*p3_i).hy();
				double v3_z = (*p3_i).hz();
				//std::cerr << "traverse corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;

				if (v3_x * v33_x >= 0 && fabs(fabs(v3_x) - fabs(v33_x)) < 0.00001 && v3_y * v33_y >= 0 && fabs(fabs(v3_y) - fabs(v33_y)) < 0.00001)
				{
					std::cerr << "add corresponding adjacent vertex in contour 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
					mayinboundary3 = false;
					remesh_vhandle[1] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, v3_z));
					break;
				}
			}

			//if on boundary
			if (mayinboundary3)
			{
				for (std::vector<Point_3>::iterator p3_i = points_temp_boundary_3.begin(); p3_i != points_temp_boundary_3.end(); ++p3_i)
				{
					double v3_x = (*p3_i).hx();
					double v3_y = (*p3_i).hy();
					double v3_z = (*p3_i).hz();
					//std::cerr << "traverse corresponding adjacent vertex in boundary 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
					if (v3_x * v33_x >= 0 && fabs(fabs(v3_x) - fabs(v33_x)) < 0.00001 && v3_y * v33_y >= 0 && fabs(fabs(v3_y) - fabs(v33_y)) < 0.00001)
					{
						std::cerr << "add corresponding adjacent vertex in boundary 3D:" << v3_x << " " << v3_y << " " << v3_z << std::endl;
						remesh_vhandle[2] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, v3_z));
						break;
					}

				}
			}

			std::vector<OMesh::VertexHandle>  face_vhandles;
			face_vhandles.clear();
			face_vhandles.push_back(remesh_vhandle[0]);
			face_vhandles.push_back(remesh_vhandle[1]);
			face_vhandles.push_back(remesh_vhandle[2]);
			remeshdata.add_face(face_vhandles);
		}
	}
	infile4.close();

	//输出remesh
	if (!OpenMesh::IO::write_mesh(remeshdata, "remesh_test_bird_3.obj"))
	{
		std::cerr << "write error\n";
		exit(1);
	}

}