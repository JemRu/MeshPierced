#include "Mesh_Process.h"
Mesh_Process::Mesh_Process()
{
}


Mesh_Process::~Mesh_Process()
{
}

void Mesh_Process::Read_Contours_data()
{
	ifstream in("data_info/countours_all.txt");

	string filename;
	string line;

	//vector<Point> vec_x;
	//vector<Point> vec_y;

	if (in) // 有该文件
	{
		int count = 0;
		int contour_id = 0;
		while (getline(in, line)) // line中不包括每行的换行符
		{
			count++;

			if (line != "hello")
			{
				if (count % 2 != 0)
				{
					//out << contour_id << " " << line << endl; // 输入到2.txt中
					vec_x.push_back(Point(contour_id, stoi(line)));
					//vec_x.push_back(Point(5, 4));

				}
				else
				{
					//out2 << contour_id << " " << line << endl; // 输入到3.txt中
					vec_y.push_back(Point(contour_id, stoi(line)));
				}
			}
			else
			{
				contour_id++;
			}
		}
	}


	//for (int i = 0; i < vec_x.size(); i++){
	//	border << vec_x[i][1] << " " << vec_y[i][1] << endl;

	//}

	ifstream seed("data_info/seed_list.txt");
	//ofstream seedx("seed_x.txt");
	//ofstream seedy("seed_y.txt");

	string filename2;
	string line2;
	//vector<Point> seed_x;
	//vector<Point> seed_y;

	if (seed) // 有该文件
	{
		int count_2 = 0;
		int contour_id2 = 0;
		while (getline(seed, line2)) // line中不包括每行的换行符
		{
			count_2++;

			if (line2 != "hello")
			{
				if (count_2 % 2 != 0)
				{
					//seedx << contour_id2 << " " << line2 << endl;
					seed_x.push_back(Point(contour_id2, stoi(line2)));
					//vec_x.push_back(Point(5, 4));

				}
				else
				{
					//seedy << contour_id2 << " " << line2 << endl;
					seed_y.push_back(Point(contour_id2, stoi(line2)));
				}
			}
			else
			{
				contour_id2++;
			}
		}
	}

	ifstream size("data_info/size_info.txt");

	//string filename2;
	string line_id;


	if (size) // 有该文件
	{

		int count_id = 0;
		while (getline(size, line_id)) // line中不包括每行的换行符
		{
			count_id++;
			if (count_id % 2 != 0)
			{
				size_wh.push_back(stoi(line_id));
			}
			else
			{
				size_wh.push_back(stoi(line_id));
			}
		}

	}
	//
	//ifstream sample_pt("data/MAT_Cloud_Points.txt");
	//ofstream sample_pt_x("MAT_Cloud_Points_sample_x.txt");
	//ofstream sample_pt_y("MAT_Cloud_Points_sample_y.txt");

	//string filename3;
	//string line3;
	////vector<Point> seed_x;
	////vector<Point> seed_y;

	//if (sample_pt) // 有该文件
	//{
	//	int count_3 = 0;
	//	int contour_id3= 0;
	//	while (getline(sample_pt, line3)) // line中不包括每行的换行符
	//	{
	//		count_3++;

	//		if (line3 != "hello")
	//		{
	//			if (count_3 % 2 != 0)
	//			{
	//				//sample_pt_x << contour_id3 << " " << line3 << endl;
	//				sample_pt_x  << line3 << endl;

	//				sample_x.push_back(Point(contour_id3, stoi(line3)));
	//				//vec_x.push_back(Point(5, 4));

	//			}
	//			else
	//			{
	//				//sample_pt_y << contour_id3 << " " << line3 << endl;
	//				sample_pt_y<< line3 << endl;
	//				sample_y.push_back(Point(contour_id3, stoi(line3)));
	//			}
	//		}
	//		else
	//		{
	//			contour_id3++;
	//		}
	//	}
	//}
}

int Mesh_Process::CountLines(string filename){

	ifstream ReadFile;
	int n = 0;
	string tmp;
	ReadFile.open(filename, ios::in);//ios::in 表示以只读的方式读取文件  
	if (ReadFile.fail())//文件打开失败:返回0  
	{
		return 0;
	}
	else//文件存在  
	{
		while (getline(ReadFile, tmp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
}

string Mesh_Process::Readline(string filename, int line)
{
	int lines, i = 0;
	string temp;
	fstream file;
	file.open(filename, ios::in);
	lines = CountLines(filename);

	if (line <= 0)
	{
		return "Error 1: 行数错误，不能为0或负数。";
	}
	if (file.fail())
	{
		return "Error 2: 文件不存在。";
	}
	if (line>lines)
	{
		return "Error 3: 行数超出文件长度。";
	}
	while (getline(file, temp) && i<line - 1)
	{
		i++;
	}
	file.close();
	return temp;
}

void Mesh_Process::CDT_Mesh(){
	cout << size_wh[0] << " " << size_wh[1] << endl;

	//Read_Contours_data();
	CDT cdt;
	//遍历vec_x，vec_y,取后一维的x,y坐标

	// 记录轮廓的数目	
	int count_contour = (vec_x[vec_x.size() - 1][0]) / 2 + 1;
	//为每条轮廓下的点集创建容器
	cout << count_contour << endl;
	for (int i = 0; i < count_contour; i++)
	{
		vector<Point> temp;
		vPoints.push_back(temp);
	}
	//将指定轮廓下的点集放入指定的容器中
	for (int i = 0; i < vec_x.size(); i++)
	{
		vPoints[vec_x[i][0] / 2].push_back(Point(vec_x[i][1], vec_y[i][1]));
	}
	//为所有限制区域生成容器;
	for (int i = 0; i < count_contour; i++)
	{
		vector<Vertex_handle> temp;
		constrain_points.push_back(temp);
	}

	//cout << constrain_point.size() << endl;

	for (int m = 0; m < count_contour; m++)
	{
		for (int i = 0; i < vPoints[m].size(); i++)
		{

			constrain_points[m].push_back((cdt.insert(Point(vPoints[m][i][0], vPoints[m][i][1]))));
			//cout << vPoints[m][i][0] << "," << vPoints[m][i][1] << endl;
		}
	}

	for (int s = 0; s < count_contour; s++)
	{
		for (int i = 0; i < constrain_points[s].size(); i++)
		{
			//cout << "( " << s << ", " << i << ") " ;
			Vertex_handle p0 = constrain_points[s][i];
			Vertex_handle p1 = constrain_points[s][(i + 1) % constrain_points[s].size()];
			cdt.insert_constraint(p0, p1);
		}
	}

	Vertex_handle  va = cdt.insert(Point(size_wh[0], size_wh[1]));
	Vertex_handle  vb = cdt.insert(Point(0, size_wh[0]));
	Vertex_handle  vc = cdt.insert(Point(0, 0));
	Vertex_handle  vd = cdt.insert(Point(size_wh[1], 0));

	cdt.insert_constraint(va, vb);
	cdt.insert_constraint(vb, vc);
	cdt.insert_constraint(vc, vd);
	cdt.insert_constraint(vd, va);
	// 记录轮廓的数目	
	//cout << "print :"<<seed_x.size() << endl;
	//cout << seed_x[seed_x.size() - 1][0] << endl;
	int count_contour2 = (vec_x[vec_x.size() - 1][0]) / 2 + 1;;
	cout << count_contour2 << endl;

	//为每条轮廓下的点集创建容器
	for (int i = 0; i < count_contour2; i++)
	{
		vector<Point> temp2;
		v_seed_Points.push_back(temp2);
	}

	//将指定轮廓下的点集放入指定的容器中
	for (int i = 0; i < this->seed_x.size(); i++)
	{
		v_seed_Points[seed_x[i][0] / 2].push_back(Point(seed_x[i][1], seed_y[i][1]));
	}

	//這里最容易出错



	for (int s = 0; s < count_contour2; s++)
	{
		//for (int i = 0; i < v_seed_Points[s].size(); i++)
		//{ 

		//list_of_seeds.push_back(Point(seed_x[i], seed_y[i]));
		list_of_seeds.push_back(Point(v_seed_Points[s][3][0], v_seed_Points[s][3][1]));
		//cout << v_seed_Points.size() << " , ";
		//}
	}

	std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
	std::cout << "Meshing the domain..." << std::endl;

	CGAL::refine_Delaunay_mesh_2(cdt, list_of_seeds.begin(), list_of_seeds.end(), Criteria());
	std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
	std::cout << "Number of finite faces: " << cdt.number_of_faces() << std::endl;

	std::ofstream out3("mesh/traingles.txt");
	std::ofstream out("mesh/traingles_data.txt");


	int mesh_faces_counter = 0;


	for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
		fit != cdt.finite_faces_end(); ++fit)
	{
		if (fit->is_in_domain())
		{

			//std::cout << "ver1: " << cdt.triangle(fit).vertex(0).hx() << "," << cdt.triangle(fit).vertex(0).hy();
			//std::cout << "    ver2: " << cdt.triangle(fit).vertex(1).hx() << "," << cdt.triangle(fit).vertex(1).hy();
			//std::cout << "    ver3: " << cdt.triangle(fit).vertex(2).hx() << "," << cdt.triangle(fit).vertex(2).hy() << std::endl;

			out3 << cdt.triangle(fit).vertex(0).hx() << " " << cdt.triangle(fit).vertex(0).hy() << " "
				<< cdt.triangle(fit).vertex(1).hx() << " " << cdt.triangle(fit).vertex(1).hy() << " "
				<< cdt.triangle(fit).vertex(2).hx() << " " << cdt.triangle(fit).vertex(2).hy() << std::endl;


			vertices_1.push_back(Point(cdt.triangle(fit).vertex(0).hx(), cdt.triangle(fit).vertex(0).hy()));
			vertices_2.push_back(Point(cdt.triangle(fit).vertex(1).hx(), cdt.triangle(fit).vertex(1).hy()));
			vertices_3.push_back(Point(cdt.triangle(fit).vertex(2).hx(), cdt.triangle(fit).vertex(2).hy()));


		}

	}
	std::cout << "Number of faces in the mesh domain: " << mesh_faces_counter << std::endl;


	std::map<Vertex_handle, int> vertex_index;
	int idx = 0;
	for (CDT::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit !=
		cdt.finite_vertices_end(); ++vit) {
		vertex_index[vit] = ++idx;
	}

	idx = 0;
	//ofstream out1("E:/paper_cut_complex/render/points");
	ofstream out1("data_info/points.txt");

	for (CDT::Finite_vertices_iterator fit = cdt.finite_vertices_begin();
		fit != cdt.finite_vertices_end(); ++fit)
	{
		//out << "node " << ++idx << " coords 3 " << (*fit) << " 0.0 bc 2 0 0 \n";
		out1 << (*fit) << endl;
	}
	out1.close();
	idx = 1;
	//ofstream out2("E:/paper_cut_complex/render/faces");
	ofstream out2("data_info/faces.txt");

	for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
		fit != cdt.finite_faces_end(); ++fit){
		if (fit->is_in_domain()){
			int i0 = vertex_index[fit->vertex(0)];
			int i1 = vertex_index[fit->vertex(1)];
			int i2 = vertex_index[fit->vertex(2)];
			//	out << "PlaneStress2d " << idx++ << " nodes 3 " << i0 << " " << i1 << " " << i2 << " crossSect 1 mat 1 NIP 1" << "\n";
			out2 << i0 << " " << i1 << " " << i2 << endl;
		}
	}
	out2.close();
	out << "SimpleCS 1 thick 1.0 width 1.0 \n" <<
		"IsoLE 1 d 0. E 15.0 n 0.25 talpha 1.0 \n" <<
		"BoundaryCondition  1 loadTimeFunction 1 prescribedvalue 0.0 \n" <<
		"NodalLoad 2 loadTimeFunction 1 Components 2 2.5 0.0 \n" <<
		"ConstantFunction 1 f(t) 1.0";

	out.close();
	//std::cout << "obj file saved in " << this->fileLoc << std::endl

}

void Mesh_Process::Skeleton()
{
	//定义外边界
	Polygon_2 bortder;

	ofstream skeleton("D:/vsplace/opencv/pg_paper_cut_origin/pg_paper_cut/mesh_out/skeleton_mat.txt");

	cout << size_wh[0] << " " << size_wh[1] << endl;

	//逆时针
	//bortder.push_back(Point(size_wh[0], size_wh[1]));
	//bortder.push_back(Point(0, size_wh[1]));
	//bortder.push_back(Point(0, 0));
	//bortder.push_back(Point(size_wh[0], 0));



	bortder.push_back(Point(564, 564));
	bortder.push_back(Point(0, 564));
	bortder.push_back(Point(0, 0));
	bortder.push_back(Point(564, 0));

	////所有由不同轮廓构成的多边形
	cout << "test" << endl;
	int count_hole = (sample_x[sample_x.size() - 1][0]) / 2 + 1;

	//cout << count_hole << endl;

	vector<Polygon_2> polys;
	for (int i = 0; i < count_hole; i++)
	{
		Polygon_2 temp3;
		polys.push_back(temp3);
	}

	cout << "generating polys for each polygon:" << endl;
	for (int i = 0; i < sample_x.size(); i++)
	{
		polys[sample_x[i][0] / 2].push_back(Point(sample_x[i][1], sample_y[i][1]));

		printf("%.2lf%%\r", i * 100.0 / sample_x.size());
	}

	cout << "Adding holes:" << endl;
	Polygon_with_holes poly2(bortder);

	for (int i = 0; i < polys.size(); i++)
	{
		poly2.add_hole(polys[i]);
		printf("%.2lf%%\r", i * 100.0 / polys.size());
	}


	SsPtr ss = CGAL::create_interior_straight_skeleton_2(poly2);

	int j = 0;

	ofstream  skeleton_edges_output("D:/vsplace/opencv/pg_paper_cut_origin/pg_paper_cut/mesh_out/skeleton_edges.txt");

	//print_straight_skeleton(*ss);
	//输出所有骨架的半边
	for (Ss::Halfedge_const_iterator i = (*ss).halfedges_begin(); i != (*ss).halfedges_end(); ++i)
	{

		if (i->is_bisector())
		{
			skeleton << i->opposite()->vertex()->point().hx() << " " << i->opposite()->vertex()->point().hy() << std::endl;
			skeleton_edges.push_back(Point(i->opposite()->vertex()->point().hx(),
				i->opposite()->vertex()->point().hy()));

		}

	}
	vector<Polygon_2>(polys).swap(polys);

	//输出一定阈值下的半边 

}

