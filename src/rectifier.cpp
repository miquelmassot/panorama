/*
 * panorama.cpp
 *
 *  Created on: Jun 26, 2012
 *      Author: miquel
 */
 
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <image_geometry/pinhole_camera_model.h>
#include <camera_calibration_parsers/parse_yml.h>
 
int main(int argc, char* argv[])
{
  int num_imgs=0;

  std::cout << "\n[PANORAMA]: creation of a panorama from a two or more images." << std::endl;

  if (argc < 3){
    std::cout << "\tUsage: " << argv[0] << "output_folder calibration.yaml input1.jpg input2.jpg... " << std::endl;
    return -1;
  }
    
  num_imgs = argc - 3;
  std::string output_folder = argv[1];
  std::string camera_info_yaml = argv[2];
  std::string camera_name = "camera";

  // read the calibration file
  sensor_msgs::CameraInfo cam_info;
  std::cout << "[I] Opening calibration file: " << argv[2] << std::endl;
  camera_calibration_parsers::readCalibrationYml(argv[2], camera_name, cam_info);
  image_geometry::PinholeCameraModel camera_model;
  camera_model.fromCameraInfo(cam_info);

  std::cout << "[I] Input: " << num_imgs << " images." << std::endl;

  //try to open the images:
  for(int i=0; i<num_imgs; i++)
  {
    std::cout << "[I] Opening image " << argv[3+i] << std::endl;
    try
    {
      cv::Mat img = cv::imread(argv[3+i]);
      if( !img.data )
         throw "Could not read image";
      cv::Mat img_rect;
      camera_model.rectifyImage(img, img_rect);
      std::cout << "[I] Writing rectified image in " << output_folder << "/" << argv[3+i] << std::endl;
      cv::imwrite(output_folder + "/" + argv[3+i],img_rect);
    }
    catch( char * str )
    {
      std::cout << "Exception raised: " << str  << argv[2+i] << std::endl;
    }
  }

  


  return 0;
}
