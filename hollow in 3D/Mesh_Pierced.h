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

	//������mesh���� �����������Ŀ��ÿ�������ζ��㣬��
	void Read_Mesh_data();

	//��������������� ���ÿ������������߶�������
	void Read_Contour_data();

	/*ɾ��mesh �ڲ������� ��ð�Χ����mesh����*/
	//������͹��
	//�жϴ洢�ڲ���
	//ɾ���õ��Լ����ڱ�
	void Remove_inContour_triangles();

	/*�ؽ���Χ������������֮���mesh �����ʷ� �������mesh*/

	/*for test*/
	//ȷ��һ��contour�Ķ˵㶼����
	void Read_Contour_points();

	/*���Ӱ�Χ����mesh�������˵�*/
	//�жϰ�Χ�����ߵĶ˵����в��õ�����ֵ
	void Get_Boundary_Points();

	/*remesh ��ά������*/
	void Remesh_Contour();
	
	

	//�洢����mesh���� 
	OMesh meshdata;
	//�洢����mesh����
	OMesh::VertexHandle vhandle;
	OMesh::FaceHandle fhandle;
	OMesh::HalfedgeHandle ehandle;
	//������������ ��������θ����Լ���ÿ������ΰ����Ķ������� ��ά����

	//�洢���������߶˵����� ���������ʷּ���͹��
	Delaunay T;
	//���е������˵�
	std::list<Point_3> points;
	//��ά�� һ�������Ķ˵�Ĺ����˵�
	int num_point;

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