#include "Mesh_remesh.h"

//cgal triangulation 
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>

struct FaceInfo2
{
	FaceInfo2(){}
	int nesting_level;

	bool in_domain(){
		return nesting_level % 2 == 1;
	}
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel       K;
typedef CGAL::Triangulation_vertex_base_2<K>                      Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, K>    Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<K, Fbb>        Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>               TDS;
typedef CGAL::Exact_predicates_tag                                Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag>  CDT;
typedef CDT::Point                                                Point;
typedef CGAL::Polygon_2<K>                                        Polygon_2;

void mark_domains(CDT& ct,
	CDT::Face_handle start,
	int index,
	std::list<CDT::Edge>& border)
{
	if (start->info().nesting_level != -1){
		return;
	}
	std::list<CDT::Face_handle> queue;
	queue.push_back(start);

	while (!queue.empty()){
		CDT::Face_handle fh = queue.front();
		queue.pop_front();
		if (fh->info().nesting_level == -1){
			fh->info().nesting_level = index;
			for (int i = 0; i < 2; i++){
				CDT::Edge e(fh, i);
				CDT::Face_handle n = fh->neighbor(i);
				if (n->info().nesting_level == -1){
					if (ct.is_constrained(e)) border.push_back(e);
					else queue.push_back(n);
				}
			}
		}
	}
}

//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident 
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
void mark_domains(CDT& cdt)
{
	for (CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
		it->info().nesting_level = -1;
	}

	std::list<CDT::Edge> border;
	mark_domains(cdt, cdt.infinite_face(), 0, border);
	while (!border.empty()){
		CDT::Edge e = border.front();
		border.pop_front();
		CDT::Face_handle n = e.first->neighbor(e.second);
		if (n->info().nesting_level == -1){
			mark_domains(cdt, n, e.first->info().nesting_level + 1, border);
		}
	}
}

Mesh_remesh::Mesh_remesh(){}
Mesh_remesh::~Mesh_remesh(){}

void Mesh_remesh::Remesh_onecontour()
{

	Polygon_2 polygon1;
	ifstream infile("in_txt/boundary-test-bird.txt");
	string line;
	if (infile)
	{
		while (getline(infile, line))
		{
			vector<string> result;
			boost::split(result, line, boost::is_any_of(" "), boost::token_compress_on);
			double v_x = atof(result[0].c_str());
			double v_y = atof(result[1].c_str());
			polygon1.push_back(Point(v_x, v_y));

		}
	}
	infile.close();

	Polygon_2 polygon2;
	ifstream infile2("in_txt/contour-birds-origin.txt");
	string line2;
	if (infile2)
	{
		while (getline(infile2, line2))
		{
			vector<string> result2;
			boost::split(result2, line2, boost::is_any_of(" "), boost::token_compress_on);
			double v_x2 = atof(result2[0].c_str());
			double v_y2 = atof(result2[1].c_str());
			polygon2.push_back(Point(v_x2, v_y2));

		}
	}
	infile2.close();

	//Insert the polygons into a constrained triangulation
	CDT cdt;
	cdt.insert_constraint(polygon1.vertices_begin(), polygon1.vertices_end(), true);
	cdt.insert_constraint(polygon2.vertices_begin(), polygon2.vertices_end(), true);

	//Mark facets that are inside the domain bounded by the polygon
	mark_domains(cdt);
	std::ofstream out("out_mesh/traingles.txt");

	for (CDT::Finite_faces_iterator f_it = cdt.finite_faces_begin(); f_it != cdt.finite_faces_end(); ++f_it)
	{
		if (f_it->info().in_domain())
		{
			out << cdt.triangle(f_it).vertex(0).hx() << " " << cdt.triangle(f_it).vertex(0).hy() << " "
				<< cdt.triangle(f_it).vertex(1).hx() << " " << cdt.triangle(f_it).vertex(1).hy() << " "
				<< cdt.triangle(f_it).vertex(2).hx() << " " << cdt.triangle(f_it).vertex(2).hy() << std::endl;
		}
	}
}