#include <cmath>
#include <iostream>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/LU>
using namespace Eigen;
using namespace std;

void NormalizeImage(int *input_image,  int input_width,  int input_height,
		    unsigned char *output_image, int output_width, int output_height ) {

  double scale;

  if (input_width > input_height) {
    scale = (double)input_width / output_width;
  }else {
    scale = (double)input_height / output_height;
  }
  double input_center_x = input_width/2.0;
  double input_center_y = input_height/2.0;
  double output_center_x = output_width/2.0;
  double output_center_y = output_width/2.0;

  for (int output_y = 0; output_y < output_width; ++output_y) {
    int input_y = scale*(output_y - output_center_y) + input_center_y + 0.5;
    if (input_y < 0) input_y = 0;
    if (input_y > input_height-1) input_y = input_height-1;
    for (int output_x = 0; output_x < output_height; ++output_x) {
      int input_x = scale*(output_x - output_center_x) + input_center_x + 0.5;
      if (input_x < 0) input_x = 0;
      if (input_x > input_width-1) input_x = input_width-1;
      output_image[output_y * output_width + output_x] =
	input_image[input_y * input_width + input_x];
    }
  }
}
void ExtractCC(const unsigned char *input_image,
	       const int input_width, const int input_height,
	       int *output_image, int &output_width, int &output_height ) {
  // ��
  int top = 0;
  for (int y = 0; input_height; ++y) {
    for (int x = 0; x < input_width; ++x) {
      if (input_image[input_width*y + x] != 0) {
	top = y;
	goto BOTTOM;
      }
    }
  }
  // ��
 BOTTOM:
  int bottom = 0;
  for (int y = input_height-1; y >= 0; --y) {
    for (int x = 0; x < input_width; ++x) {
      if (input_image[input_width*y + x] != 0) {
	bottom = y;
	goto LEFT;
      }
    }
  }
 LEFT:
  int left = 0;
  for (int x = 0; x < input_width; ++x) {
    for (int y = 0; y < input_height; ++y) {
      if (input_image[input_width*y + x] != 0) {
	left = x;
	goto RIGHT;
      }
    }
  }
 RIGHT:
  int right = 0;
  for (int x = input_width-1; x >= 0; --x) {
    for (int y = 0; y < input_height; ++y) {
      if (input_image[input_width*y + x] != 0) {
	right = x;
	goto END;
      }
    }
  }
 END:

  output_height = bottom - top + 1 + 2;
  output_width = right - left + 1 + 2;

  int i = 0;
  for (; i < output_width; ++i) output_image[i] = 0;
  for (int y = top; y <= bottom; ++y) {
    output_image[i++] = 0;
    for (int x = left; x <= right; ++x) {
      output_image[i++] = input_image[input_width*y + x];
    }
    output_image[i++] = 0;
  }
  for (; i < output_width * output_height; ++i) output_image[i] = 0;
}

// �������
// ����ԥ塼�����������ʥ�����ҡ�pp126-127 ����
inline int bi_linear(const double u0, const double v0,
		     const int *image, const int w){
  
  // ���Ǻ�ɸ�ʼ¿��ˤ����������Ѵ�
  int u = static_cast<int>(u0);
  int v = static_cast<int>(v0);
 
  double a = u0 - u;
  double b = v0 - v;

  return static_cast<int>( image[v*w + u] * (1 - a) * (1 - b) + 
			   image[v*w + u + 1] * a * (1 - b) + 
			   image[(v + 1)*w + u] * (1 - a) * b + 
			   image[(v + 1)*w + u + 1] * a * b + 0.5);
}

void Rotate(const int *input_image, 
	    const int input_width, const int input_height,
	    const int angle_x, const int angle_y, const int angle_z,
	    int *output_image, 
	    const int output_width, const int output_height,
	    const double view_z) {
  
  int output_depth = max(output_height,output_width);
  double input_center_x = input_width/2.0;
  double input_center_y = input_height/2.0;
  double output_center_x = output_width/2.0;
  double output_center_y = output_height/2.0;
  double output_center_z = output_depth/2.0;
  
  // ��ž�������롥
  double sin_x = sin(-angle_x*M_PI/180);//x
  double cos_x = cos(-angle_x*M_PI/180);//x
  double sin_y = sin(-angle_y*M_PI/180);//y
  double cos_y = cos(-angle_y*M_PI/180);//y
  double sin_z = sin(-angle_z*M_PI/180);//z
  double cos_z = cos(-angle_z*M_PI/180);//z

  Matrix4d Rx,Ry,Rz,T1,T2; 
  Rx << 
    1,     0,      0, 0,
    0,  cos_x, sin_x, 0,
    0, -sin_x, cos_x, 0,
    0,     0,      0, 1;
  Ry << 
    cos_y, 0, sin_y, 0,
        0, 1,      0, 0,
    -sin_y, 0,  cos_y, 0,
        0, 0,      0, 1;
  Rz << 
     cos_z, sin_z, 0, 0,
    -sin_z, cos_z, 0, 0,
    0,     0,      1, 0,
    0,     0,      0, 1;

  T1 <<
    1,0,0,input_center_x,
    0,1,0,input_center_y,
    0,0,1,0,
    0,0,0,1;
  T2 <<
    1,0,0,-output_center_x,
    0,1,0,-output_center_y,
    0,0,1,-output_center_z,
    0,0,0,1;


  Matrix4d A = T1*Rx*Ry*Rz*T2;

  fill_n(output_image, output_width * output_height, 0);

  Matrix4d T3,T4;
  T3 <<
    1,0,0,-output_center_x,
    0,1,0,-output_center_y,
    0,0,1,0,
    0,0,0,1;

  T4 <<
    1,0,0,output_center_x,
    0,1,0,output_center_y,
    0,0,1,0,
    0,0,0,1;
  // ��������ž����
  if( view_z != 0) {
    for(int z = 0; z < output_depth; z++){
      for(int y = 0; y < output_height; y++){
	for(int x = 0; x < output_width; x++){
	  Vector4d output_xyz(x, y, z, 1);

	  // ��ž����
	  Vector4d input_xyz = A*output_xyz;

	  if(input_xyz.coeff(0) >= 0 && input_xyz.coeff(0) < input_width &&
	     input_xyz.coeff(1) >= 0 && input_xyz.coeff(1) < input_height &&
	     abs(input_xyz.coeff(2)) < 1){
	    if(input_xyz.coeff(0) < input_width-1 &&
	       input_xyz.coeff(1) < input_height-1){
	      Vector4d input_xyz2 = T3*output_xyz;
	      double xv = input_xyz2.coeff(0);
	      double yv = input_xyz2.coeff(1);
	      double zv = input_xyz2.coeff(2);
	      Vector4d B((xv*view_z)/(zv+view_z), (yv*view_z)/(zv+view_z), 0, 1);
	      Vector4d C = T4*B;
	      int xo = C.coeff(0);
	      int yo = C.coeff(1);
	      if(xo >= 0 && yo >= 0 && xo < output_width && yo < output_height){
		output_image[xo + yo*output_width] |= 
		  bi_linear(input_xyz.coeff(0),input_xyz.coeff(1),
			    input_image,input_width);
	      }else{
		output_image[xo + yo*output_width] |=
		  input_image[static_cast<int>(input_xyz.coeff(0)) +
			      static_cast<int>(input_xyz.coeff(1)) * input_width];
	      }
	    }
	  }
	}
      }
    }
  }else{
    for(int z = 0; z < max(output_height,output_width); z++){
      for(int y = 0; y < output_height; y++){
	for(int x = 0; x < output_width; x++){
	  Vector4d output_xyz(x, y, z, 1);
	  // ��ž����
	  Vector4d input_xyz = A*output_xyz;

	  if(input_xyz.coeff(0) >= 0 && input_xyz.coeff(0) < input_width &&
	     input_xyz.coeff(1) >= 0 && input_xyz.coeff(1) < input_height &&
	     abs(input_xyz.coeff(2)) < 1){
	    if(input_xyz.coeff(0) < input_width-1 &&
	       input_xyz.coeff(1) < input_height-1){
	      output_image[x + y*output_width] |=
		bi_linear(input_xyz.coeff(0),input_xyz.coeff(1),
			  input_image,input_width);
	    }else{
	      output_image[x + y*output_width] |=
		input_image[static_cast<int>(input_xyz.coeff(0)) +
			    static_cast<int>(input_xyz.coeff(1)) * input_width];
	    }
	  }
	}
      }
    }
  }
  /*
  for(int z = 0; z < max(output_height,output_width); z++){
    for(int y = 0; y < output_height; y++){
      for(int x = 0; x < output_width; x++){
	Vector4d output_xyz(x, y, z, 1);
	Vector4d input_xyz = T3*output_xyz;


	double xv = input_xyz.coeff(0);
	double yv = input_xyz.coeff(1);
	double zv = input_xyz.coeff(2);


	Vector4d B((xv*view_z)/(zv+view_z), (yv*view_z)/(zv+view_z), 0, 1);
	Vector4d C = T4*B;

	double xo = C.coeff(0);
	double yo = C.coeff(1);
	//cout << x << " " << y << " " << z << endl;
	//cout << xo << " " << yo << " " << zo << endl;
	if(xo >= 0 && yo >= 0 && xo < output_width && yo < output_height){
	  output_image[xo + yo*output_width] |= 
	    three[x + y*output_width + z*output_width*output_height];
	}
	//output_image[(x*z)/(z+d) + (y*z)/(z+d)*output_width] |= 
	// three[xv + yv*output_width + zv*output_width*output_height];
	// cout << three[x + y*output_width + z*output_width*output_height];
      }
      //cout << endl;
    }
    //cout << endl;
  }
  */
}
