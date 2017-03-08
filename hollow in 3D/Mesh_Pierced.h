#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <list>
#include <math.h>

//openmesh
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/ArrayKernel.hh>
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Mesh/Status.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
struct MyTraits : OpenMesh::DefaultTraits
{
	// Let Point and Normal be a vector of doubles
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
};
typedef OpenMesh::TriMesh_ArrayKernelT<> OMesh;

//boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

//cgal
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3_to_polyhedron_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel      K;
typedef K::Point_3                                              Point_3;
typedef K::Point_2												Point_2;
typedef CGAL::Delaunay_triangulation_3<K>                       Delaunay;
typedef Delaunay::Vertex_handle                                 Vertex_handle;
typedef CGAL::Polyhedron_3<K>                                   Polyhedron_3;

using namespace std;
class Mesh_Pierced
{
public:
	Mesh_Pierced();
	~Mesh_Pierced();

	//读入子mesh数据 获得三角形数目，每个三角形顶点，等
	void Read_Mesh_data();

	//读入多个独立多边形 获得每个多边形轮廓线顶点序列
	void Read_Contour_data();

	/*删除mesh 内部三角形 获得包围轮廓mesh数据*/
	//求轮廓凸包
	//判断存储内部点
	//删除该点以及相邻边
	void Remove_inContour_triangles();

	/*重建包围轮廓与轮廓线之间的mesh 三角剖分 获得最终mesh*/

	/*for test*/
	//确定一个contour的端点都在哪
	void Read_Contour_points();

	/*连接包围轮廓mesh与轮廓端点*/
	//判断包围轮廓线的端点序列并得到坐标值
	void Get_Boundary_Points();

	/*remesh 三维上连接*/
	void Remesh_Contour();
	
	

	//存储输入mesh数据 
	OMesh meshdata;
	//存储处理mesh数据
	OMesh::VertexHandle vhandle;
	OMesh::FaceHandle fhandle;
	OMesh::HalfedgeHandle ehandle;
	//输入轮廓数据 单独多边形个数以及，每个多边形包含的顶点序列 二维数组

	//存储单独轮廓线端点序列 用于三角剖分计算凸包
	Delaunay T;
	//所有的轮廓端点
	std::list<Point_3> points;
	//二维上 一个轮廓的端点的关联端点
	int num_point;

	//包围轮廓mesh
	OMesh remeshdata;
	//保存二维remesh的拓扑信息
	OMesh remeshbase;
	//保存三维轮廓端点信息
	std::vector<Point_3> points_temp_contour_3;
	//保存三维边界点信息
	std::vector<Point_3> points_temp_boundary_3;
	//保存二维相关点信息
	std::vector<Point_2> point_temp_2;
	//单独轮廓线端点序列加减法向量后 构成的在凸包上的顶点数目
	
};