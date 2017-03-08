#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <Windows.h>//Sleep调用  
#include <ctime>
//中轴
#include<boost/shared_ptr.hpp>
#include <CGAL/Polygon_2.h>
#include<CGAL/Polygon_with_holes_2.h>
#include<CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include "print.h"
#include<boost/shared_ptr.hpp>
#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
//mesh宏
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CDT::Vertex_handle Vertex_handle;
typedef CGAL::Delaunay_mesher_2<CDT, Criteria> Mesher;
typedef CDT::Point Point;
//中轴宏
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2                    Point_2c;
typedef CGAL::Polygon_2<K>            Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes;
typedef CGAL::Straight_skeleton_2<K>  Ss;
typedef boost::shared_ptr<Polygon_2> PolygonPtr;
typedef boost::shared_ptr<Ss> SsPtr;
typedef std::vector<PolygonPtr> PolygonPtrVector;
using namespace std;
class Mesh_Process{
public:
	Mesh_Process();
	~Mesh_Process();
	//读入contours数据
	void Read_Contours_data();
	//文件处理
	int CountLines(string filename);
	//文件处理；
	string Readline(string filename, int line);

	//CDT生成Mesh
	void CDT_Mesh();
	//求中轴；
	void Skeleton();
	//存储countours的x坐标
	vector<Point_2c> vec_x;
	//存储countours的y坐标
	vector<Point_2c> vec_y;
	//存储种子点x的坐标；
	vector<Point_2c> seed_x;
	//存储种子点x的坐标；
	vector<Point_2c> seed_y;
	//存储采样点x的坐标；
	vector<Point_2c> sample_x;
	//存储采样点y的坐标；
	vector<Point_2c> sample_y;
	//存储尺寸信息
	vector<int> size_wh;
	//存储骨架点的信息
	vector <Point_2c> skeleton_edges;

	vector<Point_2c> v;
	//vPoints为存储所有轮廓的容器
	vector<vector<Point_2c>> vPoints;
	//constrain_points用来存储不同contours所构建的不同 限制区域
	vector<vector<Vertex_handle>>constrain_points;
	//constrain_point构建边界区域的四个控制点
	vector<Vertex_handle>constrain_point;
	//关于种子点
	std::list<Point_2c> list_of_seeds;
	vector<Point_2c> v_seed;
	vector<vector<Point_2c>> v_seed_Points;
	//Mesh三角面片的三个顶点
	std::vector<Point_2c> vertices_1;
	std::vector<Point_2c> vertices_2;
	std::vector<Point_2c> vertices_3;
};
