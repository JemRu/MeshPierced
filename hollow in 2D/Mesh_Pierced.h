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

	//������mesh���� �����������Ŀ��ÿ�������ζ��㣬��
	void Read_Mesh_data();

	//��������������� ���ÿ������������߶�������
	void Read_Contour_data();

	/*ɾ��mesh �ڲ������� ��ð�Χ����mesh����*/
	//������͹��
	//�жϴ洢�ڲ���
	//ɾ���õ��Լ����ڱ�
	//�жϰ�Χ�����ߵĶ˵����в��õ�����ֵ
	void Remove_inContour_triangles();
	void GetIntersection();
	void GetBoundary();


	/*�ؽ���Χ������������֮���mesh �����ʷ� �������mesh*/
	/*���Ӱ�Χ����mesh�������˵�*/
	/*remesh ��ά������*/
	void Remesh_Contour();




	//�洢����mesh���� 
	OMesh meshdata;
	//�洢����mesh����
	OMesh::VertexHandle vhandle;
	OMesh::FaceHandle fhandle;
	OMesh::HalfedgeHandle ehandle;

	//�洢���������߶˵����� ���������ʷּ���͹��
	//һ�������˵�
	Points points_one, simplep_one, result_one;
	//��ά�� һ�������Ķ˵�Ĺ����˵�
	int num_point;
	int index_segement;
	Boundary bod;
	std::vector<OMesh::EdgeHandle> edgehandles;

	//��һ����ɾ���ĵ�� �����������Ŀ�ʼ
	//Points point_cstart;
	//��Χ����mesh
	OMesh remeshdata;
	//�����άremesh��������Ϣ
	OMesh remeshbase;
	//������ά�����˵���Ϣ
	std::vector<Point_3> points_temp_contour_3;
	//������ά�߽����Ϣ
	std::vector<Point_3> points_temp_boundary_3;
	//�����ά��ص���Ϣ
	std::vector<Point_2> point_temp_2;
	//���������߶˵����мӼ��������� ���ɵ���͹���ϵĶ�����Ŀ
};