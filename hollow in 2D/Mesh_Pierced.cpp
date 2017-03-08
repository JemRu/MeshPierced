#include "Mesh_Pierced.h"
Mesh_Pierced::Mesh_Pierced()
{
}

Mesh_Pierced::~Mesh_Pierced()
{
}

void Mesh_Pierced::Read_Mesh_data()
{
	OpenMesh::IO::Options optt;
	if (!OpenMesh::IO::read_mesh(meshdata, "in_file/panel.obj", optt))
	{
		std::cerr << "Cannot read mesh file" << std::endl;
	}
	else {
		std::cerr << "Get mesh data" << std::endl;
	}
	meshdata.add_property(bod);
	for (OMesh::VertexIter v_ii = meshdata.vertices_sbegin(); v_ii != meshdata.vertices_end(); ++v_ii)
	{
		meshdata.property(bod, *v_ii) = 0;
	}
}
void Mesh_Pierced::Read_Contour_data()
{
	ifstream infile("in_file/countour-pane.txt");
	string line;
	if (infile)
	{
		while (getline(infile, line))
		{
			if (line.compare("POLYGON") == 0)
			{
				break;
			}
			vector<string> result;
			boost::split(result, line, boost::is_any_of(" "), boost::token_compress_on);
			string temp = "";
			double v_x = (atof(result[0].c_str())-282)/60;
			double v_y = (atof(result[1].c_str())-282)/60;
			points_one.push_back(Point(v_x, v_y, 0));
		}
	}
	infile.close();

	//CGAL::convex_hull_2(points_one.begin(), points_one.end(), std::back_inserter(simplep_one));
	//std::cout << points_one.size() << "points on the convex hull" << std::endl;
	//std::cout << simplep_one.size() << "vertex on the convex hull" << std::endl;
	//num_point = simplep_one.size();
}
void Mesh_Pierced::Remove_inContour_triangles()
{
	meshdata.request_face_status();
	meshdata.request_edge_status();
	meshdata.request_vertex_status();

	/*use convex*/
	//bool first = true;
	//for (OMesh::VertexIter v_i = meshdata.vertices_sbegin(); v_i != meshdata.vertices_end(); ++v_i)
	//{
	//	//std::cerr << "Get all vertex data" << meshdata.point(*v_i) << std::endl;
	//	float x = meshdata.point(*v_i)[0];
	//	float y = meshdata.point(*v_i)[1];
	//	float z = meshdata.point(*v_i)[2];

	//	simplep_one.push_back(Point_2(x, y));

	//	CGAL::convex_hull_2(simplep_one.begin(), simplep_one.end(), std::back_inserter(result_one));
	//	//std::cout << result_one.size() << " vertex on the convex hull" << std::endl;
	//	//std::cout << simplep_one.size() << " points on the convex hull" << std::endl;

	//	if (result_one.size() == num_point)
	//	{
	//		//std::cerr << "Get maybe in vertex data" << meshdata.point(*v_i) << std::endl;
	//		bool in = true;

	//		//remove point on convex
	//		for (Points::iterator iter = result_one.begin(); iter != result_one.end(); ++iter)
	//		{
	//			//std::cerr << "convex points is: " << *iter << std::endl;
	//			if (*iter == Point_2(x, y))
	//				in = false;
	//		}

	//		// delete points in convex
	//		if (in)
	//		{
	//			std::cerr << "Get vertex data" << meshdata.point(*v_i) << std::endl;
	//			vhandle = (*v_i);
	//			if (first)
	//			{
	//				for (OMesh::VertexVertexIter vv_i = meshdata.cvv_iter(*v_i); vv_i.is_valid(); ++vv_i)
	//				{
	//					std::cerr << "|||||||||||||||||||||||||||||||||||||||" << meshdata.point(*vv_i) << std::endl;
	//					point_cstart.push_back(Point_2(meshdata.point(*vv_i)[0], meshdata.point(*vv_i)[1]));
	//				}
	//			}
	//			first = false;
	//			meshdata.delete_vertex(vhandle, false);
	//		}
	//	}
	//	simplep_one.pop_back();
	//	result_one.clear();
	//}

	/*use polygon*/
	//int poly_sides = points_one.size();
	//int i = 0;
	//float poly_X[1000];  // 多边形各顶点的X轴坐标  
	//float poly_Y[1000]; // 多边形各顶点的Y轴坐标  
	//for (Points::iterator index = points_one.begin(); index != points_one.end();++index)
	//{
	//	poly_X[i] = (*index).hx();
	//	poly_Y[i] = (*index).hy();
	//	i++;
	//}

	//for (OMesh::VertexIter v_i = meshdata.vertices_sbegin(); v_i != meshdata.vertices_end(); ++v_i)
	//{
	//	float x = meshdata.point(*v_i)[0];
	//	float y = meshdata.point(*v_i)[1];

	//	//in or not
	//	int i, j;
	//	j = poly_sides - 1;
	//	int res = 0;
	//	for (i = 0; i<poly_sides; i++)
	//	{
	//		if ((poly_Y[i]<y && poly_Y[j] >= y || poly_Y[j]<y && poly_Y[i] >= y) && (poly_X[i] <= x || poly_X[j] <= x))
	//		{
	//			res ^= ((poly_X[i] + (y - poly_Y[i]) / (poly_Y[j] - poly_Y[i])*(poly_X[j] - poly_X[i])) < x);
	//		}
	//		j = i;
	//	}
	//	if (res == 1)
	//	{
	//		std::cerr << "Get vertex data" << meshdata.point(*v_i) << std::endl;
	//		vhandle = (*v_i);
	//		meshdata.delete_vertex(vhandle, false);
	//	}
	//}

	int poly_sides = points_one.size();
	int i = 0;
	double poly_X[1000];
	double poly_Y[1000];
	Segment_2 segments[1000];

	for (Points::iterator index = points_one.begin(); index != points_one.end(); ++index)
	{
		poly_X[i] = (*index)[0];
		poly_Y[i] = (*index)[1];
		
		if ((i+1) == poly_sides) 
		{
			segments[i] = { Segment_2(K_Point_2((*index)[0], (*index)[1]), K_Point_2(poly_X[0], poly_Y[0])) };
		}
		else
		{
			Points::iterator next = index;
			++next;
			segments[i] = { Segment_2(K_Point_2((*index)[0], (*index)[1]), K_Point_2((*next)[0], (*next)[1])) };
		}
		i++;
		index_segement = i;
	}

	//删除mesh上  在多边形内部或端点或边界上的点
	std::ofstream out_boundary("in_file/panel-boundary.txt");

	for (OMesh::VertexIter v_i = meshdata.vertices_sbegin(); v_i != meshdata.vertices_end(); ++v_i)
	{
		double px = meshdata.point(*v_i)[0];
		double py = meshdata.point(*v_i)[1];
		bool flag = false;
		int res = -1;

		for (int i = 0, l = poly_sides, j = l - 1; i < l; j = i, i++) {
			double sx = poly_X[i];
			double sy = poly_Y[i];
			double tx = poly_X[j];
			double ty = poly_Y[j];


			if ((sx*px >= 0 && fabs(fabs(sx) - fabs(px))<0.00001 && sy*py >= 0 && fabs(fabs(sy) - fabs(py))< 0.00001) || (tx*px >= 0 && fabs(fabs(tx) - fabs(px)) < 0.00001 && ty*py >= 0 && fabs(fabs(ty) - fabs(py)) < 0.00001)) {
				res = 0; //at point
			}
			if ((sy < py && ty >= py) || (sy >= py && ty < py)) {
				double x = sx + (py - sy) * (tx - sx) / (ty - sy);
				if (x*px >= 0 && fabs(fabs(x) - fabs(px))<0.00001) { //on egde
					res = 1;
				}
				if (x > px) {
					flag = !flag;  //in
				}
			}
		}
		if (flag)
		{
			out_boundary << "seed " << px << " " << py << endl;
		}

		if (res == 0 || res == 1 || flag)
		{
			std::cerr << "Get vertex data" << meshdata.point(*v_i) << std::endl;
			vhandle = (*v_i);
			for (OMesh::VertexVertexIter pp_it = meshdata.vv_iter(*v_i); pp_it.is_valid(); ++pp_it)
			{
				meshdata.property(bod, *pp_it) = 1;
			}
			meshdata.delete_vertex(vhandle, false);
		}
	}
	meshdata.garbage_collection();

	//求交点 删相交的边
	std::list<K_Point_2> pts;
	//for (OMesh::EdgeIter e_itr = meshdata.edges_begin(); e_itr != meshdata.edges_end(); ++e_itr)
	//{

	//}
	/*for (OMesh::HalfedgeIter h_itr = meshdata.halfedges_begin(); h_itr != meshdata.halfedges_end(); ++h_itr)
	{
		float fv_x = meshdata.point(meshdata.from_vertex_handle(*h_itr))[0];
		float fv_y = meshdata.point(meshdata.from_vertex_handle(*h_itr))[1];
		float tv_x = meshdata.point(meshdata.to_vertex_handle(*h_itr))[0];
		float tv_y = meshdata.point(meshdata.to_vertex_handle(*h_itr))[1];*/

	    //segments[index_segement] = { Segment_2(K_Point_2(1.87034, 3.58482), K_Point_2(2.0262, 3.58482)) };
		//std::cout << "V_X " << fv_x << " V_Y" << fv_y << "V2_X " << tv_x << " V2_Y" << tv_y << std::endl;

		//CGAL::compute_intersection_points(segments, segments + (index_segement + 1), std::back_inserter(pts));
		//std::cout << "Found " << pts.size() << " intersection points: " << std::endl;

		//if (pts.size() > 0)
		//{
		//	std::cout << "Found " << pts.size() << " intersection points: " << std::endl;
		//	OMesh::EdgeHandle edge = (meshdata.edge_handle(*h_itr));
		//	meshdata.delete_edge(edge);

		//	//for (OMesh::VertexVertexIter ppp_it = meshdata.vv_iter(meshdata.from_vertex_handle(*h_itr)); ppp_it.is_valid(); ++ppp_it)
		//	//{
		//	//	meshdata.property(bod, *ppp_it) = 1;
		//	//}
		//}

	//}

	meshdata.garbage_collection();
	bool go_on = false;
	do
	{
		for (OMesh::HalfedgeIter hv_it = meshdata.halfedges_begin(); hv_it != meshdata.halfedges_end(); ++hv_it)
		{
			ehandle = (*hv_it);
			if (meshdata.is_boundary(ehandle))
			{
				//std::cerr << "boundary vertex" << meshdata.point(meshdata.from_vertex_handle(*hv_it)) << std::endl;
				int ifbod = meshdata.property(bod, (meshdata.from_vertex_handle(*hv_it)));
				if (ifbod == 1)
				{
					go_on = false;
					//std::cerr << "Get Right boundary vertex" << meshdata.point(meshdata.from_vertex_handle(*hv_it)) << std::endl;
					for (OMesh::HalfedgeLoopIter hel_it = meshdata.hl_begin(*hv_it); hel_it.is_valid(); ++hel_it)
					{
						//std::cerr << "Get boundary vertex in older:" << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << std::endl;
						//std::cerr << "vertex " << meshdata.property(bod, (meshdata.from_vertex_handle(*hv_it))) << std::endl;
						//out_boundary << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << endl;

						double fv_x = meshdata.point(meshdata.from_vertex_handle(*hel_it))[0];
						double fv_y = meshdata.point(meshdata.from_vertex_handle(*hel_it))[1];
						double tv_x = meshdata.point(meshdata.to_vertex_handle(*hel_it))[0];
						double tv_y = meshdata.point(meshdata.to_vertex_handle(*hel_it))[1];

						segments[index_segement] = { Segment_2(K_Point_2(fv_x, fv_y), K_Point_2(tv_x, tv_y)) };
						CGAL::compute_intersection_points(segments, segments + index_segement + 1, std::back_inserter(pts));
						//std::cout << "index_segement: " << index_segement << " |||points: " << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << " |||" << meshdata.point(meshdata.to_vertex_handle(*hel_it)) << std::endl;

						if (pts.size() > 0)
						{
							std::cout << "Found " << pts.size() << " intersection points: " << std::endl;
							OMesh::EdgeHandle edge = (meshdata.edge_handle(*hel_it));
							edgehandles.push_back(edge);
							go_on = true;
							
							for (OMesh::VertexVertexIter ppp_it = meshdata.vv_iter(meshdata.from_vertex_handle(*hel_it)); ppp_it.is_valid(); ++ppp_it)
							{
								meshdata.property(bod, *ppp_it) = 1;
							}
						}
						pts.clear();
					}
					for (std::vector<OMesh::EdgeHandle>::iterator temp_de = edgehandles.begin(); temp_de != edgehandles.end(); ++temp_de)
					{
						meshdata.delete_edge(*temp_de);
					}
					meshdata.garbage_collection();
					edgehandles.clear();
					std::cout << "delete edges:::::::::::::::::::::::::::::::::::::::::::::::::::::: " << endl;
					break;
				}
			}
		}
	} while (go_on);
	
	for (OMesh::HalfedgeIter hv_it = meshdata.halfedges_begin(); hv_it != meshdata.halfedges_end(); ++hv_it)
	{
		ehandle = (*hv_it);
		if (meshdata.is_boundary(ehandle))
		{
			//std::cerr << "boundary vertex" << meshdata.point(meshdata.from_vertex_handle(*hv_it)) << std::endl;
			int ifbod = meshdata.property(bod, (meshdata.from_vertex_handle(*hv_it)));
			if (ifbod == 1)
			{
				std::cerr << "Get Right boundary vertex" << meshdata.point(meshdata.from_vertex_handle(*hv_it)) << std::endl;
				for (OMesh::HalfedgeLoopIter hel_it = meshdata.hl_begin(*hv_it); hel_it.is_valid(); ++hel_it)
				{
					//std::cerr << "Get boundary vertex in older:" << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << std::endl;
					//std::cerr << "vertex " << meshdata.property(bod, (meshdata.from_vertex_handle(*hv_it))) << std::endl;
					out_boundary << meshdata.point(meshdata.from_vertex_handle(*hel_it)) << endl;
				}
				break;
			}
		}
	}
	out_boundary.close();

	if (!OpenMesh::IO::write_mesh(meshdata, "in_file/panel_out.obj"))
	{
		std::cerr << "write error\n";
		exit(1);
	}
	meshdata.release_face_status();
	meshdata.release_edge_status();
	meshdata.release_vertex_status();
}
void Mesh_Pierced::Remesh_Contour()
{
	OpenMesh::IO::Options optt;
	if (!OpenMesh::IO::read_mesh(remeshdata, "in_file/panel_out.obj", optt))
	{
		std::cerr << "Cannot read mesh file" << std::endl;
	}
	else {
		std::cerr << "Get mesh data" << std::endl;
	}
	remeshdata.add_property(bod);
	for (OMesh::VertexIter v_ii = remeshdata.vertices_sbegin(); v_ii != remeshdata.vertices_end(); ++v_ii)
	{
		remeshdata.property(bod, *v_ii) = 0;
	}

	ifstream infile("in_file/traingles.txt");
	string line;
	if (infile)
	{
		while (getline(infile, line))
		{
			vector<string> result;
			boost::split(result, line, boost::is_any_of(" "), boost::token_compress_on);
			string temp = "";
			double v_x = atof(result[0].c_str());
			double v_y = atof(result[1].c_str());
			double v2_x = atof(result[2].c_str());
			double v2_y = atof(result[3].c_str());
			double v3_x = atof(result[4].c_str());
			double v3_y = atof(result[5].c_str());

			OMesh::VertexHandle remesh_vhandle[3];
			remesh_vhandle[0] = remeshdata.add_vertex(OMesh::Point(v_x, v_y, 0));
			remesh_vhandle[1] = remeshdata.add_vertex(OMesh::Point(v2_x, v2_y, 0));
			remesh_vhandle[2] = remeshdata.add_vertex(OMesh::Point(v3_x, v3_y, 0));

			std::vector<OMesh::VertexHandle>  face_vhandles;
			face_vhandles.clear();
			face_vhandles.push_back(remesh_vhandle[0]);
			face_vhandles.push_back(remesh_vhandle[1]);
			face_vhandles.push_back(remesh_vhandle[2]);
			remeshdata.add_face(face_vhandles);
		}
	}
	infile.close();
	try
	{
		if (!OpenMesh::IO::write_mesh(remeshdata, "in_file/panel.obj"))
		{
			std::cerr << "Cannot write mesh to file" << std::endl;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
	}

}