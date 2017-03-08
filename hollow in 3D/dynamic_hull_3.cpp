#include "Mesh_Pierced.h"
//#include "Mesh_remesh.h"
int main()
{
  Mesh_Pierced mesh;
  ////mesh.Read_Mesh_data();
  ////mesh.Read_Contour_data();
  ////mesh.Read_Contour_points();
  ////mesh.Remove_inContour_triangles();
  ////mesh.Get_Boundary_Points();
  mesh.Remesh_Contour();

  //Mesh_remesh remesh;
  //remesh.Remesh_onecontour();

  system("pause");
  return 0;
}
