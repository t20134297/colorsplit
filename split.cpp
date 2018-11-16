#include<iostream>
#include<opencv2/opencv.hpp>
#include<cmath>
#include<string.h>
#include<vector>
#include<dirent.h>

using namespace std;
using namespace cv;

void body_seg(string src);
void calcu_hist(Mat src_image,int dime,int hist[],int start_row,int start_col,int end_row,int end_col,int patch,int small_value,int big_value);
void draw_hist(int hist_array[],string img_name);
double calcu_distance(int hist1[],int hist2[]);
vector<string> get_filename_in_direct(string path);
double distance_between_two_img(string src_img,string test_img);
double accuracy_test(string path_input,string path_test);


int main()
{
  // string src="body/2.jpg";
  // int init_hist[300];
  // int hist1[300],hist2[300];
  //
  // Mat img = imread(src);
  // Mat hsv;
  // int row_num = img.rows;
  // int col_num = img.cols;
  // int start_row,start_col,end_row,end_col;
  // start_row = int(0.19*row_num);
  // start_col = int(0.1*col_num);
  // end_row = int(0.5*row_num);
  // end_col = int(0.9*col_num);
  //
  // cvtColor(img,hsv,CV_BGR2HSV);
  // calcu_hist(hsv,0,init_hist,start_row,start_col,end_row,end_col,10,0,180);
  // memcpy(hist1,init_hist,sizeof(init_hist));
  //
  // img = imread("body/3.jpg");
  // cvtColor(img,hsv,CV_BGR2HSV);
  // calcu_hist(hsv,0,init_hist,start_row,start_col,end_row,end_col,10,0,180);
  // memcpy(hist2,init_hist,sizeof(init_hist));
  //
  // double distance = calcu_distance(hist1,hist2);
  //
  // vector<string> filelist ;
  // cout<<accuracy_test("images/input","images/output")<<endl;
  vector<string> bodyname;
  bodyname = get_filename_in_direct("./mybody");
  int i;
  string name;
  for(i=0;i<bodyname.size();i=i+1)
  {
    name = bodyname[i];
    name="./mybody/"+name;
    body_seg(name);
  }


  return 0;
}

void body_seg(string src)
{
  Mat img = imread(src);
  int width,height;
  width = img.rows;
  height = img.cols;
  int i,j,k;
  int row_1,row_2,row_3;
  row_1 = int(0.19*width);
  row_2 = int(0.5*width);
  row_3 = int(0.7*width);

  int body_1,body_2,leg_1,leg_2;
  body_1 = int(height*0.1);
  body_2 = int(height*0.9);

  leg_1 = int(height*0.15);
  leg_2 = int(height*0.85);


  for(i=row_1;i<row_1+4;i=i+1)
  {
    for(j=body_1;j<body_2;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }

  for(i=row_2;i<row_2+4;i=i+1)
  {
    for(j=body_1;j<body_2;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }

  for(i=row_3;i<row_3+4;i=i+1)
  {
    for(j=leg_1;j<leg_2;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }

  for(i=row_1;i<row_2;i=i+1)
  {
    for(j=body_1;j<body_1+4;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }

  for(i=row_1;i<row_2;i=i+1)
  {
    for(j=body_2;j<body_2+4;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }

  for(i=row_2;i<row_3;i=i+1)
  {
    for(j=leg_1;j<leg_1+4;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }

  for(i=row_2;i<row_3;i=i+1)
  {
    for(j=leg_2;j<leg_2+4;j=j+1)
    {
      img.at<cv::Vec3b>(i,j)[0]=255;
      img.at<cv::Vec3b>(i,j)[1]=0;
      img.at<cv::Vec3b>(i,j)[2]=0;
    }
  }
  imshow("boundary",img);
  waitKey(0);
}

void calcu_hist(Mat src_image,int dime,int hist[],int start_row,int start_col,int end_row,int end_col,int patch,int small_value,int big_value)
{
  memset(hist, 0, sizeof(int)*300);
  int allnumber = (big_value-small_value)/patch + 1;
  int value;
  hist[0] = allnumber;
  int i,j;
  for(i = start_row;i<end_row;i=i+1)
  {
    for(j=start_col;j<end_col;j=j+1)
    {
      value = int(src_image.at<Vec3b>(i,j)[dime]);
      if(value > (small_value-1) && value <(big_value+1) )
      {
        hist[(value-small_value)/patch+1] = hist[(value-small_value)/patch+1] + 1;
      }
    }
  }

}

void draw_hist(int hist_array[],string img_name)
{
  int color_num,index;
  color_num = hist_array[0];

  Mat hist_diagram = Mat::zeros(Size(color_num*5+100,600),CV_8UC3);
  cout<<hist_diagram.channels()<<endl;
  cout<<"start"<<endl;
  int i,j,k;
  for(i=1;i<(color_num+1);i=i+1)
  {
    cout<<"1for"<<endl;
    for(j=400-hist_array[i];j<400;j=j+1)
    {
      cout<<"2for"<<endl;
      for(k=50+i*5;k<50+(i+1)*5;k=k+1)
      {
        cout<<"3for"<<endl;
        if(0==i%2)
        {
          cout<<"ifstart"<<endl;
          hist_diagram.at<Vec3b>(j,k)[0]=255;
          cout<<"ddddd"<<endl;
        }
        else
        {
          cout<<"elsestart"<<endl;
          hist_diagram.at<Vec3b>(j,k)[1]=255;
          cout<<"else end"<<endl;
        }
      }
    }
  }
  cout<<img_name<<endl;
  imwrite(img_name,hist_diagram);
  imshow("empty",hist_diagram);
  waitKey(0);
}

double calcu_distance(int hist1[],int hist2[])
{
  double distance=0.0;
  int color_num = hist1[0];
  int sum_hist1=0,sum_hist2=0;
  int i;
  for(i=1;i<(color_num+1);i=i+1)
  {
    sum_hist1 = sum_hist1 + hist1[i];
    sum_hist2 = sum_hist2 + hist2[i];
  }

  double sqrt_sum = 0.0;
  for(i=1;i<(color_num+1);i=i+1)
  {
    sqrt_sum = sqrt_sum + sqrt(hist1[i]*hist2[i]);
  }
  distance = sqrt(abs(1-(sqrt_sum/sqrt((sum_hist1*sum_hist2)))));
  return distance;

}

vector<string> get_filename_in_direct(string path)
{
  struct dirent *ptr;
  DIR *dir;
  const char* PATH = path.c_str();
  dir = opendir(PATH);
  vector<string> files;
  while((ptr=readdir(dir))!=NULL)
  {
    if(ptr->d_name[0] == '.')
      continue;
    files.push_back(ptr->d_name);
  }
  return files;
}

double distance_between_two_img(string img_path1,string img_path2)
{
  double distance = 0.0;
  int init_hist[300];
  int hist1[300];
  int hist2[300];
  Mat img1,img2,hsv1,hsv2;
  img1 = imread(img_path1);
  img2 = imread(img_path2);
  cvtColor(img1,hsv1,CV_BGR2HSV);
  cvtColor(img2,hsv2,CV_BGR2HSV);
  int width,height;
  width = img1.cols;
  height = img1.rows;
  int col_start_body,col_end_body,col_start_leg,col_end_leg;
  col_start_body = int(0.1*width);
  col_end_body = int(0.9*width);
  col_start_leg = int(0.15*width);
  col_end_leg = int(0.85*width);
  int row_list[15];
  int i=0;
  for(i=0;i<5;i=i+1)
  {
    row_list[i] = int((0.19+i*0.06)*height);
  }
  for(i=0;i<6;i=i+1)
  {
    row_list[i+5] = int((0.5+i*0.04)*height );
  }
  for(i=0;i<5;i=i+1)
  {
    calcu_hist(hsv1,0,init_hist,row_list[i],col_start_body,row_list[i+1],col_end_body,10,0,180);
    memcpy(hist1,init_hist,sizeof(init_hist));
    calcu_hist(hsv2,0,init_hist,row_list[i],col_start_body,row_list[i+1],col_end_body,10,0,180);
    memcpy(hist2,init_hist,sizeof(init_hist));
    distance = distance + 0.5 * calcu_distance(hist1,hist2);

    calcu_hist(hsv1,1,init_hist,row_list[i],col_start_body,row_list[i+1],col_end_body,10,0,255);
    memcpy(hist1,init_hist,sizeof(init_hist));
    calcu_hist(hsv2,1,init_hist,row_list[i],col_start_body,row_list[i+1],col_end_body,10,0,255);
    memcpy(hist2,init_hist,sizeof(init_hist));
    distance = distance + 0.3 * calcu_distance(hist1,hist2);

    calcu_hist(hsv1,2,init_hist,row_list[i],col_start_body,row_list[i+1],col_end_body,10,0,255);
    memcpy(hist1,init_hist,sizeof(init_hist));
    calcu_hist(hsv2,2,init_hist,row_list[i],col_start_body,row_list[i+1],col_end_body,10,0,255);
    memcpy(hist2,init_hist,sizeof(init_hist));
    distance = distance + 0.2 * calcu_distance(hist1,hist2);
  }

  for(i=0;i<5;i=i+1)
  {
    calcu_hist(hsv1,0,init_hist,row_list[i+5],col_start_leg,row_list[i+6],col_end_leg,10,0,180);
    memcpy(hist1,init_hist,sizeof(init_hist));
    calcu_hist(hsv2,0,init_hist,row_list[i+5],col_start_leg,row_list[i+6],col_end_leg,10,0,180);
    memcpy(hist2,init_hist,sizeof(init_hist));
    distance = distance + 0.5 * calcu_distance(hist1,hist2);

    calcu_hist(hsv1,1,init_hist,row_list[i+5],col_start_leg,row_list[i+6],col_end_leg,10,0,255);
    memcpy(hist1,init_hist,sizeof(init_hist));
    calcu_hist(hsv2,1,init_hist,row_list[i+5],col_start_leg,row_list[i+6],col_end_leg,10,0,255);
    memcpy(hist2,init_hist,sizeof(init_hist));
    distance = distance + 0.3 * calcu_distance(hist1,hist2);

    calcu_hist(hsv1,2,init_hist,row_list[i+5],col_start_leg,row_list[i+6],col_end_leg,10,0,255);
    memcpy(hist1,init_hist,sizeof(init_hist));
    calcu_hist(hsv2,2,init_hist,row_list[i+5],col_start_leg,row_list[i+6],col_end_leg,10,0,255);
    memcpy(hist2,init_hist,sizeof(init_hist));
    distance = distance + 0.3 * calcu_distance(hist1,hist2);
  }
  return distance;
}

double accuracy_test(string path_input,string path_test)
{
  double accuracy = 0.0;
  vector<string> input_list;
  vector<string> test_list;

  input_list = get_filename_in_direct(path_input);
  test_list = get_filename_in_direct(path_test);
  int input_length = input_list.size();
  int test_length = test_list.size();
  int accuracy_number=0, test_number=input_length;
  int index_input,index_test;
  double distance_smallest,distance_temp;
  string img_test_temp,img_result;
  string person_input,person_result;
  string img_input;
  for(index_input = 0;index_input<input_length;index_input = index_input+1)
  {
    distance_smallest = 10.0;
    img_input = input_list[index_input];
    person_input = img_input.substr(0,img_input.find("_"));
    img_input = path_input + "/"+img_input;
    for(index_test=0;index_test<test_length;index_test = index_test+1)
    {
      img_test_temp = path_test +"/"+ test_list[index_test];
      distance_temp = distance_between_two_img(img_input,img_test_temp);
      if(distance_temp<distance_smallest)
      {
        distance_smallest = distance_temp;
        person_result = test_list[index_test];
      }
    }
    person_result = person_result.substr(0,person_result.find("_"));
    if(person_result == person_input)
    {
      accuracy_number = accuracy_number+1;
    }
    cout<<"intput:"<<person_input<<endl;
    cout<<"result:"<<person_result<<endl;
  }

  return (accuracy_number/(test_number*1.0));
}



//dsadsa
