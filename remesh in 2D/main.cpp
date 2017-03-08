#include <igl/cotmatrix.h>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <igl/viewer/Viewer.h>
#include <igl/triangle/triangulate.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
//boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

// Input polygon
Eigen::MatrixXd V;
Eigen::MatrixXi E;
Eigen::MatrixXd H;

// Triangulated interior
Eigen::MatrixXd V2;
Eigen::MatrixXi F2;

int main(int argc, char *argv[])
{
  using namespace Eigen;
  using namespace std;
  vector<double> points_x, points_y, seed_x, seed_y;

  //  ‰»Îcountour
  ifstream infile2("in_file/countour-pane.txt");
  string line2;
  if (infile2)
  {
	  while (getline(infile2, line2))
	  {
		  vector<string> result;
		  boost::split(result, line2, boost::is_any_of(" "), boost::token_compress_on);
		  double v_x = (atof(result[0].c_str()) - 282) / 60;
		  double v_y = (atof(result[1].c_str()) - 282) / 60;
		  points_x.push_back(v_x);
		  points_y.push_back(v_y);
	  }
  }
  infile2.close();
  unsigned countour_size = points_x.size();

  // ‰»Î±ﬂΩÁ
  ifstream infile("in_file/panel-boundary.txt");
  string line;
  if (infile)
  {
	  while (getline(infile, line))
	  {
		  vector<string> result;
		  boost::split(result, line, boost::is_any_of(" "), boost::token_compress_on);
		  if (result[0].compare("seed") == 0)
		  {
			  seed_x.push_back(atof(result[1].c_str()));
			  seed_y.push_back(atof(result[2].c_str()));
		  }
		  else
		  {
			  points_x.push_back(atof(result[0].c_str()));
			  points_y.push_back(atof(result[1].c_str()));
		  }
	  }
  }
  infile.close();

  unsigned seed_size = seed_x.size();
  unsigned boundary_size = points_x.size()-countour_size;
  unsigned size = points_x.size();
  std::cout << "countour_size: " << countour_size << "  ||boundary_size: " << boundary_size << "  ||seed_size: " << seed_size << endl;
  
  V.resize(size, 2);
  E.resize(size, 2);
  H.resize(1, 2);

  unsigned index = 0;
  for (vector<double>::iterator it = points_x.begin(); it != points_x.end(); ++it) {
	  V(index, 0) = (*it);
	  index++;
  }
  unsigned index2 = 0;
  for (vector<double>::iterator itt = points_y.begin(); itt != points_y.end(); ++itt) {
	  V(index2, 1) = (*itt);
	  index2++;
  }
  //std::cout << V << endl;

  for (unsigned e = 0; e < countour_size; e++)
  {
	  E(e, 0) = e;
	  if ((e + 1) == countour_size) {
		  E(e, 1) = 0;
	  }
	  else
	  {
		  E(e, 1) = e + 1;
	  }
  }

  for (unsigned ed = countour_size; ed < size; ed++) {
	  E(ed, 0) = ed;
	  if ((ed + 1) == size) {
		  E(ed, 1) = countour_size;
	  }
	  else
	  {
		  E(ed, 1) = ed + 1;
	  }
  }
  //std::cout << E << endl;
 
  vector<double>::iterator it_t = seed_x.begin();
  H(0, 0) = (*it_t);
  vector<double>::iterator i_t = seed_y.begin(); 
  H(0, 1) = (*i_t);

  //std::cout << H << endl;
  

  //Triangulate the interior
  igl::triangle::triangulate(V,E,H,"0.5",V2,F2);
  
  std::cout << F2 << endl;
  //std::cout << F2(0,1) << " " << F2(0,2) << " " << F2(0, 3) << endl;

  //Plot the generated mesh
  igl::viewer::Viewer viewer;
  viewer.data.set_mesh(V2,F2);
  viewer.launch();
}
