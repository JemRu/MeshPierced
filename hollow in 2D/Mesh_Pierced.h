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
typedef OMesh::Point Point;
typedef std::vector<Point> Points;
typedef OpenMesh::VPropHandleT<int> Boundary;
//boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

//cgal
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel      K;
typedef K::Point_3                                              Point_3;
typedef K::Point_2												Point_2;

typedef CGAL::Quotient<CGAL::MP_Float>                  NT;
typedef CGAL::Cartesian<NT>                             Kernel;
typedef Kernel::Point_2                                 K_Point_2;
typedef CGAL::Arr_segment_traits_2<Kernel>              Traits_2;
typedef Traits_2::Curve_2                               Segment_2;

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
	//判断包围轮廓线的端点序列并得到坐标值
	void Remove_inContour_triangles();
	void GetIntersection();
	void GetBoundary();


	/*重建包围轮廓与轮廓线之间的mesh 三角剖分 获得最终mesh*/
	/*连接包围轮廓mesh与轮廓端点*/
	/*remesh 三维上连接*/
	void Remesh_Contour();




	//存储输入mesh数据 
	OMesh meshdata;
	//存储处理mesh数据
	OMesh::VertexHandle vhandle;
	OMesh::FaceHandle fhandle;
	OMesh::HalfedgeHandle ehandle;

	//存储单独轮廓线端点序列 用于三角剖分计算凸包
	//一个轮廓端点
	Points points_one, simplep_one, result_one;
	//二维上 一个轮廓的端点的关联端点
	int num_point;
	int index_segement;
	Boundary bod;
	std::vector<OMesh::EdgeHandle> edgehandles;

	//第一个被删除的点的 用于找轮廓的开始
	//Points point_cstart;
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