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
#include <Windows.h>//Sleep����  
#include <ctime>
//����
#include<boost/shared_ptr.hpp>
#include <CGAL/Polygon_2.h>
#include<CGAL/Polygon_with_holes_2.h>
#include<CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include "print.h"
#include<boost/shared_ptr.hpp>
#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
//mesh��
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CDT::Vertex_handle Vertex_handle;
typedef CGAL::Delaunay_mesher_2<CDT, Criteria> Mesher;
typedef CDT::Point Point;
//�����
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
	//����contours����
	void Read_Contours_data();
	//�ļ�����
	int CountLines(string filename);
	//�ļ�����
	string Readline(string filename, int line);

	//CDT����Mesh
	void CDT_Mesh();
	//�����᣻
	void Skeleton();
	//�洢countours��x����
	vector<Point_2c> vec_x;
	//�洢countours��y����
	vector<Point_2c> vec_y;
	//�洢���ӵ�x�����ꣻ
	vector<Point_2c> seed_x;
	//�洢���ӵ�x�����ꣻ
	vector<Point_2c> seed_y;
	//�洢������x�����ꣻ
	vector<Point_2c> sample_x;
	//�洢������y�����ꣻ
	vector<Point_2c> sample_y;
	//�洢�ߴ���Ϣ
	vector<int> size_wh;
	//�洢�Ǽܵ����Ϣ
	vector <Point_2c> skeleton_edges;

	vector<Point_2c> v;
	//vPointsΪ�洢��������������
	vector<vector<Point_2c>> vPoints;
	//constrain_points�����洢��ͬcontours�������Ĳ�ͬ ��������
	vector<vector<Vertex_handle>>constrain_points;
	//constrain_point�����߽�������ĸ����Ƶ�
	vector<Vertex_handle>constrain_point;
	//�������ӵ�
	std::list<Point_2c> list_of_seeds;
	vector<Point_2c> v_seed;
	vector<vector<Point_2c>> v_seed_Points;
	//Mesh������Ƭ����������
	std::vector<Point_2c> vertices_1;
	std::vector<Point_2c> vertices_2;
	std::vector<Point_2c> vertices_3;
};
