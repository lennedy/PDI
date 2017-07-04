#include "visao.h"

visao::visao():
circulo_centro(MAX_NUM_FIG_GEOMETRICAS), circulo_raio(MAX_NUM_FIG_GEOMETRICAS), num_circulos(0),
mask(3,3,CV_32F)
{
	Mat mask1;
	for(int i=0; i<9; i++){
		media[i] = 1;
	}
	mask = Mat(3, 3, CV_32F, media);
	scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  swap(mask, mask1);
}

void visao::filtro_inicial(){
	
//	cvtColor(img_inicial, img_tratada, CV_BGR2GRAY);

		cvtColor(img_inicial, frame, CV_BGR2GRAY);
		flip(frame, frame, 1);
		frame.convertTo(frame32f, CV_32F);
		filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
		frameFiltered.convertTo(img_tratada, CV_8U);
}

void visao::achar_circulos(){
 
	vector<Vec3f> circles;
	HoughCircles( img_tratada, circles, CV_HOUGH_GRADIENT, 1, img_tratada.rows/8, CALIBRACAO_1_CIRCULO, CALIBRACAO_2_CIRCULO, 0, 0 );
	num_circulos = circles.size();
  for( size_t i = 0; (i < circles.size() && i<MAX_NUM_FIG_GEOMETRICAS); i++ )
  {		
			circulo_centro[i] = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
	    circulo_raio[i] = cvRound(circles[i][2]);
	}

}

void visao::desenhar_circulos(){
	for(int i=0; (i<num_circulos && i<MAX_NUM_FIG_GEOMETRICAS); i++){
		//desenha centro do circulo
		circle( img_tratada, circulo_centro[i], 3, Scalar(0,255,0), -1, 8, 0 );
		//desenha parte externa do circulo
    circle( img_tratada, circulo_centro[i], circulo_raio[i], Scalar(0,0,255), 3, 8, 0 );
	}
	imshow("Circulos", img_tratada);
}

bool visao::iniciar_video(){
	int width,height;
	video.open(0); 
  if(!video.isOpened()) 
    return false;

  width=video.get(CV_CAP_PROP_FRAME_WIDTH);
  height=video.get(CV_CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";;
  std::cout << "altura =" << height<< "\n";;
	
	//namedWindow("video",1);
	return true;
}

void visao::run(){
	char key;
	iniciar_video();
	while(true){
		video >> img_inicial;
		imshow("original", img_inicial);

		filtro_inicial();
		achar_circulos();
		desenhar_circulos();   
    
    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!


	}
		
}

int main(){
	visao v;

	v.run();

	return 0;
}
