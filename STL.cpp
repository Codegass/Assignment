/* 
	Author: Chenhao Wei
	I get some help from my friend Tianchen Liu about some STL Cylinder structure.
    	It is a really hard work.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;
const long double pi = 3.14159265359;


class Shape {
public:
		virtual string print() = 0;
};
// First to deal with the Cube
class Cube: public Shape {
private:
		double size;
		double vectorset[3];
public:
		Cube(double x, double y, double z, double size) : size(size){
				vectorset[0]=x;
				vectorset[1]=y;
				vectorset[2]=z;
		};
		string facet_vector(double vectorD, int vector2){  //vectorD is the direction, vector2 is the location.
				ostringstream output_result;
				for (int i=0;i<3;i++)
						if (i==vector2)
								output_result<<" "<<vectorD;
						else
								output_result<<" 0";
				output_result<<"\n";
				return output_result.str();
		}

		string vertex_base(double vectorD, int vector2,double else_value){
				ostringstream output_result;
				for (int i=0;i<3;i++)
						if (i==vector2){
								output_result<<" "<< ((vectorD+1)/2)*size+vectorset[i];
                        }
                        else
								output_result<<" "<<else_value*size+vectorset[i];
				output_result<<"\n";
				return output_result.str();
		}

		string vertex_third_point(double vectorD, int vector2,double else_value){
				ostringstream output_result;
				for (int i=0;i<3;i++)
						if (i==vector2){
								output_result<<" "<< ((vectorD+1)/2)*size+vectorset[i];
                        }
                        else{
								output_result<<" "<<abs(else_value)*size+vectorset[i];
								else_value++;
						}
				output_result<<"\n";
				return output_result.str();
		}

		string triangle_print(double vectorD, int vector2) {
				string output_result = "";
				output_result = "  facet normal" + output_result + facet_vector(vectorD, vector2) +"    outer loop\n" +"      vertex" + vertex_base(vectorD, vector2, 0) +"      vertex" + vertex_base(vectorD, vector2, 1) +"      vertex" + vertex_third_point(vectorD, vector2, 0) +"    endloop\n  endfacet\n" +"  facet normal" + facet_vector(vectorD, vector2) + "    outer loop\n" +"      vertex" + vertex_base(vectorD, vector2, 1) +"      vertex" + vertex_base(vectorD, vector2, 0) +"      vertex" + vertex_third_point(vectorD, vector2, -1) +"    endloop\n  endfacet\n";
				return output_result;
		}

		string triangle_print_reverse(double vectorD, int vector2){
				string output_result = "";
				output_result = "  facet normal"+output_result+facet_vector(vectorD,vector2)+"    outer loop\n"+"      vertex" +vertex_base(vectorD,vector2,1)+"      vertex" +vertex_base(vectorD,vector2,0)+"      vertex" +vertex_third_point(vectorD,vector2,0)+"    endloop\n  endfacet\n"+"  facet normal"+facet_vector(vectorD,vector2)+"    outer loop\n"+"      vertex" +vertex_base(vectorD,vector2,0)+"      vertex" +vertex_base(vectorD,vector2,1)+"      vertex" +vertex_third_point(vectorD,vector2,-1)+"    endloop\n  endfacet\n";
				return output_result;
		}

		string print(){
				string output_result="";
				output_result=triangle_print(1, 0)+triangle_print_reverse(-1, 0)+triangle_print_reverse(1, 1)+triangle_print(-1, 1)+triangle_print(1, 2)+triangle_print_reverse(-1, 2);
				return output_result;
		}
};

// next is the Cylinder
class Cylinder : public Shape{
private:
		double r, h;// for clinder we need x,y,z and r & h to build.
		int facets;
		double vectorset[3];
        
public:
		Cylinder(double x, double y, double z, double r, double h, int facets):r(r),h(h),facets(facets) {
				vectorset[0]=x;
				vectorset[1]=y;
				vectorset[2]=z;
		}
		string vector(double x1,double y1,double z1){  
				ostringstream output_result;
				output_result<<" "<< vectorset[0]+x1<<" "<<vectorset[1]+y1<<" "<<vectorset[2]+z1<<"\n";
				return output_result.str();
		}

		string angle_print(double n){
				double angle1=(n*360*pi)/(180*facets);
		        double angle2=((n+1)*360*pi)/(180*facets);
                string output_result="";
				output_result="  facet normal"+output_result+vector(cos(angle1+360/facets),sin(angle1+360/facets),0)+"    outer loop\n"+"      vertex" +vector(r*cos(angle2),r*sin(angle2),h)+"      vertex" +vector(r*cos(angle1),r*sin(angle1),h)+"      vertex" +vector(r*cos(angle1),r*sin(angle1),0)+"    endloop\n  endfacet\n"+"  facet normal"+vector(cos(angle1+360/facets),sin(angle1+360/facets),0)+"    outer loop\n"+"      vertex" +vector(r*cos(angle2),r*sin(angle2),h)+"      vertex" +vector(r*cos(angle1),r*sin(angle1),0)+"      vertex" +vector(r*cos(angle2),r*sin(angle2),0)+"    endloop\n  endfacet\n"+"  facet normal"+vector(0,0,1)+"    outer loop\n"+"      vertex" +vector(0,0,h)+"      vertex" +vector(r*cos(angle1),r*sin(angle1),h)+"      vertex" +vector(r*cos(angle2),r*sin(angle2),h)+"    endloop\n  endfacet\n"+"  facet normal"+vector(0,0,-1)+"    outer loop\n"+"      vertex" +vector(0,0,0)+"      vertex" +vector(r*cos(angle2),r*sin(angle2),0)+"      vertex" +vector(r*cos(angle1),r*sin(angle1),0)+"    endloop\n  endfacet\n";
				return output_result;
		};
		string print(){
				string output_result="";
				for (int i=0;i<facets;i++){
							output_result=output_result+angle_print(i);
                    };
				return output_result;
		}       //out put final result
};

class CAD {
private:
		vector<Shape*> shapes;
public:
		void add(Shape *p){
				shapes.push_back(p);
		}
		void write(string filename){
				fstream fin(filename, fstream::out);
				fin<< "solid OpenSCAD_Model\n";
				for(int i=0;i<shapes.size();i++)
						fin << shapes[i]->print();
				fin<<"endsolid OpenSCAD_Model\n";
				cout<<"Mission complete\n";
		}
};

//https://www.stratasysdirect.com/resources/how-to-prepare-stl-files/
//https://www.viewstl.com/
int main() {
		CAD c;
		c.add(new Cube(0,0,0,   5));
		c.add(new Cylinder(100,0,0,    3, 10, 10));
		c.write("test.stl");
}
